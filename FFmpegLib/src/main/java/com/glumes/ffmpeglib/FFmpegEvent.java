package com.glumes.ffmpeglib;

import android.os.Environment;
import android.view.View;


import com.glumes.ffmpeglib.utils.Constants;

import java.io.File;

import timber.log.Timber;

/**
 * Created by zhaoying on 2017/6/23.
 */

public class FFmpegEvent {


    String mpegFilePath = Environment.getExternalStorageDirectory() + File.separator + Constants.MEGP_1_FILE_NAME;

    String targFilePath = Environment.getExternalStorageDirectory() + File.separator + "PGM" + File.separator + "test";


    String mp2FilePath = Environment.getExternalStorageDirectory() + File.separator + "output.mp2";

    public void onDecodeAudio(View view) {
        Timber.d("onDecodeAudio");
        FFmpegSample.getInstance().onDecodeAudio(mpegFilePath, mp2FilePath);
    }

    public void onDecodeVideo(View view) {
        Timber.d("onDecodeVideo");
        FFmpegSample.getInstance().onDecodeVideo(mpegFilePath, targFilePath);
    }

    public void onEncodeAudio(View view) {
        Timber.d("onEncodeAudio");
        FFmpegSample.getInstance().onEncodeAudio("");
    }

    public void onEncodeVideo(View view) {
        Timber.d("onEncodeVideo");
        FFmpegSample.getInstance().onEncodeVideo("");
    }

    public void onFFmpegConfig(View view) {
        Timber.d("onFFmepgConfig");
        FFMPEGTest.printFFmpegConfigInfo();
    }

}
