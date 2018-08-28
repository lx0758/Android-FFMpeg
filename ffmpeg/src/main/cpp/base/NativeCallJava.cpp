#include <jni.h>
#include <pthread.h>
#include "android_log.h"

JavaVM *jvm;
jstring variable;
pthread_t pthread;

/**
 * 原生库被载入时会被调用
 * @param vm
 * @param reserved
 * @return
 */
JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    // 存储 jvm, 不使用 GetJavaVM() 因为该方法不能调用多次
    jvm = vm;

    // 校验 JNI 版本, 获取 JNIEnv 实例
    JNIEnv *env;
    if(vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) return JNI_ERR;

    return JNI_VERSION_1_6;
}

/**
 * 获取 JVM 中 Java 实例对象
 * 获取方法对象 ID 并调用
 * @param env
 * @param instance
 * @param code
 * @param msg
 * @return
 */
jstring *callMethod(JNIEnv *env, jobject instance, int code, char *msg) {
    jclass jclazz = env->GetObjectClass(instance);

    // 签名获取方式 javap -s *.class
    jmethodID jmethodid = env->GetMethodID(jclazz, "onCallback", "(ILjava/lang/String;)Ljava/lang/String;");

    jstring jmsg = env->NewStringUTF(msg);
    jobject result = env->CallObjectMethod(instance, jmethodid, code, jmsg);
    const char *ret = env->GetStringUTFChars((jstring) result, 0);
    LOGD("[call]:result=%s", ret);
    env->ReleaseStringUTFChars((jstring) result, ret);

    return &variable;
}

/**
 * 主线程中调用 Java 方法
 */
extern "C"
JNIEXPORT jstring JNICALL
Java_com_liux_ffmpeg_base_NativeCallJava_callJavaOnMainThread(JNIEnv *env, jobject instance, jstring data) {
    LOGD("[call]:native main thread call java method");
    variable = data;
    jstring *result = callMethod(env, instance, 100, "msg from native main thread");
    return *result;
}

/**
 * 子线程回调
 * @param data
 * @return
 */
void *callMethodChild(void *data) {
    LOGD("[call]:native child thread call java method");
    jobject instance = (jobject)(data);

    JNIEnv *env;
    jvm->AttachCurrentThread(&env, NULL);

    callMethod(env, instance, 200, "msg from native child thread");

    env->DeleteGlobalRef(instance);
    jvm->DetachCurrentThread();

    pthread_exit(&pthread);
}

/**
 * 子线程中调用 Java 方法
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_liux_ffmpeg_base_NativeCallJava_callJavaOnChildThread(JNIEnv *env, jobject instance, jstring data) {
    variable = data;
    // 新建一个可以全局引用的对象
    instance = env->NewGlobalRef(instance);
    pthread_create(&pthread, NULL, callMethodChild, instance);
}

