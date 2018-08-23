#include <jni.h>
#include <string>

extern "C" {

#include "libavutil/ffversion.h"
#include "libavcodec/avcodec.h"

jstring Java_com_liux_ffmpeg_FFMpeg_getVersion(JNIEnv *env, jclass) {
    std::string version = FFMPEG_VERSION;
    version += '\n';
    version += avcodec_configuration();
    return env->NewStringUTF(version.c_str());
}

}
