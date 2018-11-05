#include <jni.h>
#include <string>
#include "android_log.h"

extern "C" {
#include <libavutil/ffversion.h>
#include <libavcodec/avcodec.h>
}

extern "C"
JNIEXPORT jstring JNICALL
Java_com_liux_android_ffmpeg_base_FFMpeg_getVersion(JNIEnv *env, jclass type) {
    std::string version = FFMPEG_VERSION;
    version += '\n';
    version += avcodec_configuration();

    LOGD("[version]:%s", version.c_str());

    return env->NewStringUTF(version.c_str());
}