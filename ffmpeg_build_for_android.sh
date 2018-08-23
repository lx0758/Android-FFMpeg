#!/bin/bash

# NDK 是旧版本兼容新版本,尽量使用低版本
# FFMpeg 使用 3.3.8, 因为 3.4.x 需要 NDK platform 21 或以上支持,否则会报库类找不到
# Android 平台 64位 架构是从 NDK platform 21 开始支持的

# 下载FFmpeg源代码之后，首先需要对源代码中的configure文件进行修改。
# 由于编译出来的动态库文件名的版本号在.so之后（例如“libavcodec.so.5.100.1”），
# 而android平台不能识别这样文件名，所以需要修改这种文件名。在configure文件中找到下面几行代码：
# SLIBNAME_WITH_MAJOR='$(SLIBNAME).$(LIBMAJOR)'
# LIB_INSTALL_EXTRA_CMD='$$(RANLIB)"$(LIBDIR)/$(LIBNAME)"'
# SLIB_INSTALL_NAME='$(SLIBNAME_WITH_VERSION)'
# SLIB_INSTALL_LINKS='$(SLIBNAME_WITH_MAJOR)$(SLIBNAME)'
# 替换为下面内容：
# SLIBNAME_WITH_MAJOR='$(SLIBPREF)$(FULLNAME)-$(LIBMAJOR)$(SLIBSUF)'
# LIB_INSTALL_EXTRA_CMD='$$(RANLIB)"$(LIBDIR)/$(LIBNAME)"'
# SLIB_INSTALL_NAME='$(SLIBNAME_WITH_MAJOR)'
# SLIB_INSTALL_LINKS='$(SLIBNAME)'

NDK_HOME=~/android-ndk-r10e
FFMPEG_HOME=~/ffmpeg-3.3.8
OUTPUT=~/jniLibs

set -o errexit
chmod 777 ${NDK_HOME} -R
chmod 777 ${FFMPEG_HOME} -R

function build {
    echo
    echo "start build ffmpeg for ${ABI}"

    ROOT=$(pwd)
    PREFIX=${OUTPUT}/${ABI}

    cd ${FFMPEG_HOME}

    ./configure \
    --prefix=${PREFIX}/ \
    \
    --disable-gpl \
    --enable-version3 \
    --disable-nonfree \
    \
    --enable-static \
    --disable-shared \
    --enable-small \
    --enable-runtime-cpudetect \
    --disable-gray \
    --disable-swscale-alpha \
    \
    --disable-programs \
    --disable-ffmpeg \
    --disable-ffplay \
    --disable-ffprobe \
    --disable-ffserver \
    \
    --disable-doc \
    --disable-htmlpages \
    --disable-manpages \
    --disable-podpages \
    --disable-txtpages \
    \
    --enable-avdevice \
    --enable-avcodec \
    --enable-avformat \
    --enable-swresample \
    --enable-swscale \
    --enable-postproc \
    --enable-avfilter \
    \
    --arch=${ARCH} \
    --target-os=linux \
    --cross-prefix=${CROSS_COMPILE} \
    --enable-cross-compile \
    --sysroot=${SYSROOT} \
    --extra-cflags="-Os -fpic ${ADDI_CFLAGS}" \
    --extra-ldflags="${ADDI_LDFLAGS}" \
    \
    --disable-pic \
    --disable-thumb \
    --disable-lto \
    --disable-symver \
    \
    --disable-asm \
    --disable-yasm \
    \
    --disable-debug \
    \
    ${ADDITIONAL_CONFIGURE_FLAG}

    make clean
    make -j4
    make install

    cd ${ROOT}

    echo
    echo "marge ffmpeg for ${ABI}"
    ${CROSS_COMPILE}ld \
    -rpath-link=${SYSROOT_LIB} \
    -L${SYSROOT_LIB} \
    -L${PREFIX}/lib \
    -soname libffmpeg.so \
    -shared -nostdlib -Bsymbolic --whole-archive --no-undefined \
    -o ${PREFIX}/libffmpeg.so \
    ${PREFIX}/lib/*.a \
    -lc -lm -lz -ldl -llog \
    --dynamic-linker=/system/bin/linker \
    ${LIBGCC}

    echo
    echo "optimize ffmpeg for ${ABI}"
    ${CROSS_COMPILE}strip \
    --strip-unneeded \
    ${PREFIX}/libffmpeg.so

    echo
    echo "build ffmpeg for ${ABI} finished"
}

function armeabi {
    PLATFORM_VERSION=android-14
    ARCH=arm
    ABI=armeabi
    SYSROOT=${NDK_HOME}/platforms/${PLATFORM_VERSION}/arch-arm
    SYSROOT_LIB=${SYSROOT}/usr/lib
    TOOLCHAIN=${NDK_HOME}/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64
    CROSS_COMPILE=${TOOLCHAIN}/bin/arm-linux-androideabi-
    LIBGCC=${TOOLCHAIN}/lib/gcc/arm-linux-androideabi/4.9/libgcc.a
    ADDI_CFLAGS="-marm"
    build
}

function armeabi-v7a {
    PLATFORM_VERSION=android-14
    ARCH=arm
    ABI=armeabi-v7a
    SYSROOT=${NDK_HOME}/platforms/${PLATFORM_VERSION}/arch-arm
    SYSROOT_LIB=${SYSROOT}/usr/lib
    TOOLCHAIN=${NDK_HOME}/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64
    CROSS_COMPILE=${TOOLCHAIN}/bin/arm-linux-androideabi-
    LIBGCC=${TOOLCHAIN}/lib/gcc/arm-linux-androideabi/4.9/libgcc.a
    ADDI_CFLAGS="-marm -march=armv7-a"
    build
}

function x86 {
    PLATFORM_VERSION=android-14
    ARCH=x86
    ABI=x86
    SYSROOT=${NDK_HOME}/platforms/${PLATFORM_VERSION}/arch-x86
    SYSROOT_LIB=${SYSROOT}/usr/lib
    TOOLCHAIN=${NDK_HOME}/toolchains/x86-4.9/prebuilt/linux-x86_64
    CROSS_COMPILE=${TOOLCHAIN}/bin/i686-linux-android-
    LIBGCC=${TOOLCHAIN}/lib/gcc/i686-linux-android/4.9/libgcc.a
    ADDI_CFLAGS="-march=i686 -mtune=intel -mssse3 -mfpmath=sse -m32"
    build
}

function arm64-v8a {
    PLATFORM_VERSION=android-21
    ARCH=aarch64
    ABI=arm64-v8a
    SYSROOT=${NDK_HOME}/platforms/${PLATFORM_VERSION}/arch-arm64
    SYSROOT_LIB=${SYSROOT}/usr/lib
    TOOLCHAIN=${NDK_HOME}/toolchains/aarch64-linux-android-4.9/prebuilt/linux-x86_64
    CROSS_COMPILE=${TOOLCHAIN}/bin/aarch64-linux-android-
    LIBGCC=${TOOLCHAIN}/lib/gcc/aarch64-linux-android/4.9/libgcc.a
    ADDI_CFLAGS=""
    build
}

function x86_64 {
    PLATFORM_VERSION=android-21
    ARCH=x86_64
    ABI=x86_64
    SYSROOT=${NDK_HOME}/platforms/${PLATFORM_VERSION}/arch-x86_64
    SYSROOT_LIB=${SYSROOT}/usr/lib64
    TOOLCHAIN=${NDK_HOME}/toolchains/x86_64-4.9/prebuilt/linux-x86_64
    CROSS_COMPILE=${TOOLCHAIN}/bin/x86_64-linux-android-
    LIBGCC=${TOOLCHAIN}/lib/gcc/x86_64-linux-android/4.9/libgcc.a
    ADDI_CFLAGS="-march=x86-64 -msse4.2 -mpopcnt -m64 -mtune=intel"
    ADDI_LDFLAGS="-Wl,-rpath-link=${SYSROOT}/usr/lib64 -L${SYSROOT}/usr/lib64 -nostdlib -lc -lm -ldl -llog"
    build
}

armeabi
armeabi-v7a
x86
arm64-v8a
x86_64
