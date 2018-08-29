#include <jni.h>
#include <pthread.h>
#include "android_log.h"

extern "C" {
#include "libavformat/avformat.h"
}

const char *url = NULL;
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
    url = dataSource;
    //env->ReleaseStringUTFChars(dataSource_, url);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_liux_ffmpeg_media_MediaPlayer_n_1prepare(JNIEnv *env, jobject instance) {
    av_register_all();
    avformat_network_init();

    AVFormatContext *pFormatCtx = avformat_alloc_context();

    if(avformat_open_input(&pFormatCtx, url, NULL, NULL) != JNI_OK) {
        LOGE("can not open url :%s", url);
        return;
    }

    if(avformat_find_stream_info(pFormatCtx, NULL) < JNI_OK) {
        LOGE("can not find streams from %s", url);
        return;
    }

    int avStreamIndex = JNI_ERR;
    AVCodecParameters *avCodecParameters = NULL;
    for(int i = 0; i < pFormatCtx->nb_streams; i++) {
        // 得到音频流
        if(pFormatCtx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            avStreamIndex = i;
            avCodecParameters = pFormatCtx->streams[i]->codecpar;
            break;
        }
    }
    if (avStreamIndex == JNI_ERR) {
        LOGE("can not find audio streams from %s", url);
        return;
    }

    AVCodec *avCodec = avcodec_find_decoder(avCodecParameters->codec_id);
    if(!avCodec) {
        LOGE("can not find decoder");
        return;
    }

    AVCodecContext *avCodecContext = avcodec_alloc_context3(avCodec);
    if(!avCodecContext) {
        LOGE("can not alloc new decodecctx");
        return;
    }

    if(avcodec_parameters_to_context(avCodecContext, avCodecParameters) < 0) {
        LOGE("can not fill decodecctx");
        return;
    }

    int count = 0;
    while (true) {
        count ++;

        AVPacket *avPacket = av_packet_alloc();
        int result = av_read_frame(pFormatCtx, avPacket);

        if(avPacket->stream_index == avStreamIndex) {
            LOGE("decode %d frame, audio", count);
        } else{
            LOGE("decode %d frame, other", count);
        }

        // 解码操作
        av_packet_free(&avPacket);
        av_free(avPacket);

        if (result != JNI_OK) {
            LOGE("decode finished");
            break;
        }
    }
}

extern "C"
JNIEXPORT void JNICALL
Java_com_liux_ffmpeg_media_MediaPlayer_n_1prepareAsync(JNIEnv *env, jobject instance) {
    pthread_create(&decode, NULL, &callbackPrepare, instance);
}
