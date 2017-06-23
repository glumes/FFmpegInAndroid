package com.glumes.ffmpeglib;

import android.util.Log;
import android.view.View;

import com.glumes.commonlibrary.LogUtil;

import timber.log.Timber;

/**
 * Created by zhaoying on 2017/6/23.
 */

public class FFmpegEvent {

    public void onDecodeAudio(View view) {
        Timber.d("onDecodeAudio");
    }

    public void onDecodeVideo(View view) {
        Timber.d("onDecodeVideo");
    }

    public void onEncodeAudio(View view) {
        Timber.d("onEncodeAudio");
    }

    public void onEncodeVideo(View view) {
        Timber.d("onEncodeVideo");
    }



}
