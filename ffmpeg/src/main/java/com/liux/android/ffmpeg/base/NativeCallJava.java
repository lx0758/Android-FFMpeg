package com.liux.android.ffmpeg.base;

public class NativeCallJava {

    static {
        System.loadLibrary("base");
    }

    private Callback callback;

    public NativeCallJava(Callback callback) {
        this.callback = callback;
    }

    public String onCallback(int code, String msg) {
        if (callback != null) return callback.onCallback(code, msg);
        return null;
    }

    public native String callJavaOnMainThread(String data);

    public native void callJavaOnChildThread(String data);

    public interface Callback {

        String onCallback(int code, String msg);
    }
}
