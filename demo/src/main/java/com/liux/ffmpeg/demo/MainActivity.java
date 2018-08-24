package com.liux.ffmpeg.demo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.view.View;

import com.liux.ffmpeg.FFMpeg;
import com.liux.ffmpeg.JNIThread;

public class MainActivity extends AppCompatActivity {

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
}
