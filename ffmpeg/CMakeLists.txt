# http://www.jianshu.com/p/5f29fd671750

# CMake最低版本
cmake_minimum_required(VERSION 3.4.1)

# 第一步:引入头文件
include_directories( src/main/cpp/include )

# 第二步:引入FFmpeg
add_library( ffmpeg
             SHARED
             IMPORTED )
set_target_properties( ffmpeg
                       PROPERTIES IMPORTED_LOCATION
                       ${CMAKE_SOURCE_DIR}/src/main/jniLibs/${ANDROID_ABI}/libffmpeg.so )

# 第三步:引入原生代码
add_library( android_ffmpeg
             SHARED
             src/main/cpp/com_liux_ffmpeg_FFMPEG.cpp )

# 第四步:添加系统日志库
find_library( log-lib
              log )

# 第五步:连接库文件
# 警告!!!注意连接顺序
target_link_libraries( android_ffmpeg

                       ffmpeg

                       ${log-lib} )
