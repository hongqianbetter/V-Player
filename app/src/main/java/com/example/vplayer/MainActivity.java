package com.example.vplayer;

import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.os.Bundle;
import android.os.Environment;
import android.util.Log;
import android.view.SurfaceView;
import android.view.View;
import android.widget.TextView;
import android.widget.Toast;

import java.io.File;

public class MainActivity extends AppCompatActivity {
    SurfaceView surfaceView;

        private final String PATH = "rtmp://58.200.131.2:1935/livetv/hunantv";
//    private final String PATH = Environment.getExternalStorageDirectory() + File.separator + "a.mkv";
    MyPlayer myPlayer;

    // 要申请的权限
    private String[] permissions = {Manifest.permission.WRITE_EXTERNAL_STORAGE
            , Manifest.permission.READ_EXTERNAL_STORAGE
            , Manifest.permission.INTERNET};

    int i = 1;
    boolean isFlag = true;
    int j = 1;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        ActivityCompat.requestPermissions(this, permissions, 321);

        surfaceView = findViewById(R.id.surfaceView);
//
//        myPlayer = new MyPlayer();
//
//        myPlayer.setDataSource(PATH);
//        myPlayer.setSurfaceView(surfaceView);
//
//
//        myPlayer.setPreparedListener(new MyPlayer.OnPreparedListener() {
//
//            @Override
//            public void onPrepared() {
//                runOnUiThread(new Runnable() {
//                    @Override
//                    public void run() {
//                        myPlayer.start();
//                        Toast.makeText(MainActivity.this, "onPrepared()", Toast.LENGTH_SHORT).show();
//
//                    }
//                });
//            }
//
//            @Override
//            public void onError(String errorText) {
//                runOnUiThread(new Runnable() {
//                    @Override
//                    public void run() {
//                        Toast.makeText(MainActivity.this, errorText, Toast.LENGTH_SHORT).show();
//                    }
//                });
//            }
//        });

        findViewById(R.id.prepare).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View v) {
//                myPlayer.prepare();
                myPlayer2.prepare();
            }
        });

        test();
    }

    SurfaceView surfaceView2;
    MyPlayer myPlayer2;
    private void test() {
        surfaceView2 = findViewById(R.id.surfaceView2);
        myPlayer2 = new MyPlayer();

        myPlayer2.setDataSource(PATH);
        myPlayer2.setSurfaceView(surfaceView2);

        myPlayer2.setPreparedListener(new MyPlayer.OnPreparedListener() {

            @Override
            public void onPrepared() {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        myPlayer2.start();
                        Toast.makeText(MainActivity.this, "onPrepared()", Toast.LENGTH_SHORT).show();

                    }
                });
            }

            @Override
            public void onError(String errorText) {
                runOnUiThread(new Runnable() {
                    @Override
                    public void run() {
                        Toast.makeText(MainActivity.this, errorText, Toast.LENGTH_SHORT).show();
                    }
                });
            }
        });
    }


    @Override
    protected void onResume() {
        super.onResume();

        //        myPlayer.prepare();
    }

    @Override
    protected void onPause() {
        super.onPause();
        //        myPlayer.stop();
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        //        myPlayer.release();
    }
}