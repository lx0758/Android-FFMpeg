# http://www.jianshu.com/p/5f29fd671750

# CMake最低版本
cmake_minimum_required(
    VERSION 3.4.1
)

# 第一步:引入头文件
include_directories(
    src/main/cpp/include
)

# 第二步:引入FFmpeg
# 声明库类
add_library(
    ffmpeg
    SHARED
    IMPORTED
)
# 设置库类属性
set_target_properties(
    ffmpeg
    PROPERTIES IMPORTED_LOCATION
    ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libffmpeg.so
)

# 第三步:引入原生代码
add_library(
    base
    SHARED

    # 源码相对路径
    src/main/cpp/base/FFMpeg.cpp
    src/main/cpp/base/JNIThread.cpp
    src/main/cpp/base/NativeCallJava.cpp
)
add_library(
    media
    SHARED

    src/main/cpp/media/MediaPlayer.cpp
)

# 第四步:连接库文件
# 警告!!!注意连接顺序
target_link_libraries(
    # 代码编译库
    base

    # 依赖原生库
    ffmpeg

    # 系统自带库
    log
)
target_link_libraries(
    # 代码编译库
    media

    # 依赖原生库
    ffmpeg

    # 系统自带库
    log
)
