package com.glumes.ffmpeglearn;

import android.os.Bundle;
import android.support.v7.app.AppCompatActivity;
import android.view.View;


import com.glumes.ffmpeglearn.fileaddress.FileContants;
import com.glumes.ffmpeglib.FFMPEGTest;

import timber.log.Timber;

public class MainActivity extends AppCompatActivity implements View.OnClickListener {

    String fileName = "sintel.mp4";

    String targetFileName = "sintel.yuv";


    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
//        setContentView(R.layout.activity_main);
//        setContentView;
        setContentView(R.layout.activity_app_main);
        findViewById(R.id.button).setOnClickListener(this);
    }

//    @OnClick(R.id.button)
    public void onViewClicked() {
//        String folderurl = Environment.getExternalStorageDirectory().getAbsolutePath()
//                + File.separator + fileName;
//
//        Timber.d(folderurl);
//
//        String outputUrl = Environment.getExternalStorageDirectory().getAbsolutePath()
//                + File.separator + targetFileName;
//
//        Timber.d(outputUrl);
//
//        Log.d("FFMEPG", folderurl + outputUrl);
//
//        SimpleDecoder simpleDecoder = new SimpleDecoder();
//
//        simpleDecoder.decode(folderurl,outputUrl);

        Timber.d("testtest");

        FFMPEGTest.printFFmpegConfigInfo();

        FFMPEGTest.yuv420Split(FileContants.YUV420, FileContants.YUV420_Y, FileContants.YUV420_U, FileContants.YUV420_V);

    }

    @Override
    public void onClick(View v) {
        Timber.d("testtest");

        FFMPEGTest.printFFmpegConfigInfo();
    }
}
