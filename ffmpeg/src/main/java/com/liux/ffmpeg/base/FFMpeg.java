package com.liux.ffmpeg.base;

/**
 * Created by Liux on 2017/7/1.
 */

public class FFMpeg {

    static {
        System.loadLibrary("base");
    }

    public static native String getVersion();
}
