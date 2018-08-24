package com.liux.ffmpeg.demo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.liux.ffmpeg.FFMpeg;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        FFMpeg.getVersion();
    }
}
