package com.liux.ffmpeg;

/**
 * Created by Liux on 2017/7/1.
 */

public class FFMpeg {

    static {
        System.loadLibrary("android_ffmpeg");
    }

    public static native String getVersion();
}
