#include <jni.h>
#include <pthread.h>

JavaVM *jvm;
pthread_t decode;

void *callbackPrepare(void *data) {

    pthread_exit(&decode);
}

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM *vm, void *reserved) {
    jvm = vm;
    JNIEnv *env;
    if(vm->GetEnv((void **) &env, JNI_VERSION_1_6) != JNI_OK) return JNI_ERR;
    return JNI_VERSION_1_6;
}

extern "C"
JNIEXPORT void JNICALL
Java_com_liux_ffmpeg_media_MediaPlayer_n_1setDataSource(JNIEnv *env, jobject instance,
                                                  jstring dataSource_) {
    const char *dataSource = env->GetStringUTFChars(dataSource_, 0);

    env->ReleaseStringUTFChars(dataSource_, dataSource);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_liux_ffmpeg_media_MediaPlayer_n_1prepare(JNIEnv *env, jobject instance) {
    pthread_create(&decode, NULL, &callbackPrepare, instance);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_liux_ffmpeg_media_MediaPlayer_n_1prepareAsync(JNIEnv *env, jobject instance) {

}
