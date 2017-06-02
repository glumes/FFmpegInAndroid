package com.glumes.ffmpeglearn;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.util.Log;
import android.widget.Button;

import com.glumes.ffmpeglib.FFMPEGTest;
import com.glumes.ffmpeglib.SimpleDecoder;

import java.io.File;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;
import timber.log.Timber;

public class MainActivity extends AppCompatActivity {


    @BindView(R.id.button)
    Button button;

    String fileName = "sintel.mp4";

    String targetFileName = "sintel.yuv";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);
    }

    @OnClick(R.id.button)
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
    }
}
