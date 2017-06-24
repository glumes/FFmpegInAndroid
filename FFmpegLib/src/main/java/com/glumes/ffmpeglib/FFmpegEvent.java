package com.glumes.ffmpeglib;

import android.view.View;


import timber.log.Timber;

/**
 * Created by zhaoying on 2017/6/23.
 */

public class FFmpegEvent {

    public void onDecodeAudio(View view) {
        Timber.d("onDecodeAudio");
//        FFmpegSample.getInstance().onDecodeAudio("", "");
        FFmpegSample.getInstance().test();
    }

    public void onDecodeVideo(View view) {
        Timber.d("onDecodeVideo");
        FFmpegSample.getInstance().onDecodeVideo("", "");
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
