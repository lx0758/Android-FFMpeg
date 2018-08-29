package com.liux.ffmpeg.demo;

import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.EditText;
import android.widget.Toast;

import com.liux.ffmpeg.base.FFMpeg;
import com.liux.ffmpeg.base.JNIThread;
import com.liux.ffmpeg.media.MediaPlayer;
import com.liux.ffmpeg.base.NativeCallJava;

public class MainActivity extends AppCompatActivity {

    private EditText audioText;

    private NativeCallJava.Callback callback = new NativeCallJava.Callback() {
        @Override
        public String onCallback(int code, String msg) {
            Log.d("Activity", "code:" + code + " mas:" + msg);
            return "this is java return String";
        }
    };
    NativeCallJava nativeCallJava = new NativeCallJava(callback);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        audioText = findViewById(R.id.et_audio);
    }



    public void onFFMpeg(View view) {
        FFMpeg.getVersion();
    }

    public void onRunThread(View view) {
        JNIThread.runThread();
    }

    public void onNativeCallJava(View view) {
        nativeCallJava.callJavaOnMainThread("main thread");
        nativeCallJava.callJavaOnChildThread("child thread");
    }


    MediaPlayer mediaPlayer = new MediaPlayer();

    public void onPlayAudio(View view) {
        String uriText = audioText.getText().toString();
        if (TextUtils.isEmpty(uriText)) {
            Toast.makeText(this, "源资源不能为空", Toast.LENGTH_SHORT).show();
            return;
        }

        mediaPlayer.setDataSource(uriText);
        mediaPlayer.setOnPreparedListener(mp -> {
            Toast.makeText(this, "onPrepared()", Toast.LENGTH_SHORT).show();
        });
        mediaPlayer.prepare();
    }
}
