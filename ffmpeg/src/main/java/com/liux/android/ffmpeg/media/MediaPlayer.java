package com.liux.android.ffmpeg.media;

public class MediaPlayer {

    static {
        System.loadLibrary("media");
    }

    public void setDataSource(String path) {
        n_setDataSource(path);
    }

    private OnPreparedListener mOnPreparedListener;

    public void setOnPreparedListener(OnPreparedListener listener) {
        mOnPreparedListener = listener;
    }

    public interface OnPreparedListener {

        void onPrepared(MediaPlayer mp);
    }

    public void prepare() {
        n_prepare();
    }

    public void prepareAsync() {
        n_prepareAsync();
    }

    private native void n_setDataSource(String dataSource);

    private native void n_prepare();

    private native void n_prepareAsync();
}
