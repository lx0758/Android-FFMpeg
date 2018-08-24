package com.liux.ffmpeg;

/**
 * Created by Liux on 2017/7/1.
 */

public class FFMpeg {

    static {
        System.loadLibrary("native");
    }

    public static native String getVersion();
}
