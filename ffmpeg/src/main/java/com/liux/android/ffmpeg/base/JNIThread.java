package com.liux.android.ffmpeg.base;

public class JNIThread {

    static {
        System.loadLibrary("base");
    }

    public static native void runThread();
}
