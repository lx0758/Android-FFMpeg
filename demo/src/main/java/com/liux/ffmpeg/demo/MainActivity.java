package com.liux.ffmpeg.demo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.util.Log;
import android.view.View;

import com.liux.ffmpeg.FFMpeg;
import com.liux.ffmpeg.JNIThread;
import com.liux.ffmpeg.NativeCallJava;

public class MainActivity extends AppCompatActivity {

    private NativeCallJava.Callback callback = new NativeCallJava.Callback() {
        @Override
        public String onCallback(int code, String msg) {
            Log.d("Activity", "code:" + code + " mas:" + msg);
            return "RESULT";
        }
    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
    }

    public void onFFMpeg(View view) {
        FFMpeg.getVersion();
    }

    public void onRunThread(View view) {
        JNIThread.runThread();
    }

    NativeCallJava nativeCallJava;
    public void onNativeCallJava(View view) {
        nativeCallJava = new NativeCallJava(callback);
        nativeCallJava.callJavaOnMainThread("main thread");
        nativeCallJava.callJavaOnChildThread("child thread");
    }
}
