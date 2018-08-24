package com.liux.ffmpeg;

public class JNIThread {

    static {
        System.loadLibrary("native");
    }

    public static native void runThread();
}
