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
    // 检测 JNI 协议版本
    JNIEnv *env;
    if(vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) return JNI_ERR;

    // 获取全局 JVM
    env->GetJavaVM(&jvm);

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

    return &variable;
}

/**
 * 主线程中调用 Java 方法
 */
extern "C"
JNIEXPORT jstring JNICALL
Java_com_liux_ffmpeg_NativeCallJava_callJavaOnMainThread(JNIEnv *env, jobject instance, jstring data) {
    LOGD("[call]:main thread call java method");
    variable = data;
    jstring *result = callMethod(env, instance, 100, "msg from main thread");
    return *result;
}

/**
 * 子线程回调
 * @param data
 * @return
 */
void *callMethodChild(void *data) {
    jobject instance = (jobject)(data);

    JNIEnv *env;
    jvm->AttachCurrentThread(&env, NULL);
    LOGD("[call]:child thread call java method");
    callMethod(env, instance, 200, "msg from child thread");
    jvm->DetachCurrentThread();
    pthread_exit(&pthread);
}

/**
 * 子线程中调用 Java 方法
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_liux_ffmpeg_NativeCallJava_callJavaOnChildThread(JNIEnv *env, jobject instance, jstring data) {
    variable = data;
    // 新建一个可以全局引用的对象
    instance = env->NewGlobalRef(instance);
    pthread_create(&pthread, NULL, callMethodChild, instance);
}

