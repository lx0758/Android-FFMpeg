#include "android/log.h"

#ifndef FFMPEG_ANDROID_LOG_H
#define FFMPEG_ANDROID_LOG_H

#endif //FFMPEG_ANDROID_LOG_H

#define LOGV(FORMAT, ...) __android_log_print(ANDROID_LOG_VERBOSE, "FFMpeg", FORMAT, ##__VA_ARGS__);
#define LOGD(FORMAT, ...) __android_log_print(ANDROID_LOG_DEBUG, "FFMpeg", FORMAT, ##__VA_ARGS__);
#define LOGI(FORMAT, ...) __android_log_print(ANDROID_LOG_INFO, "FFMpeg", FORMAT, ##__VA_ARGS__);
#define LOGW(FORMAT, ...) __android_log_print(ANDROID_LOG_WARN, "FFMpeg", FORMAT, ##__VA_ARGS__);
#define LOGE(FORMAT, ...) __android_log_print(ANDROID_LOG_ERROR, "FFMpeg", FORMAT, ##__VA_ARGS__);
