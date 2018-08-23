package com.liux.ffmpegdemo;

import android.app.Activity;
import android.os.Bundle;
import android.view.View;
import android.widget.EditText;

import com.liux.ffmpeg.FFMpeg;
import com.liux.ffmpeg.R;

public class MainActivity extends Activity {

    private EditText mEditText;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initView();
        initListener();
    }

    private void initView() {
        mEditText = (EditText) findViewById(R.id.activity_main_edittext);
    }

    private void initListener() {

    }

    public void onClick(View view) {
        String version = FFMpeg.getVersion();

        mEditText.getText().append(version).append("\n");
    }
}
