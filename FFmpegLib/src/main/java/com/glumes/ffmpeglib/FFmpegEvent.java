package com.glumes.ffmpeglib;

import android.view.View;

import timber.log.Timber;

/**
 * Created by zhaoying on 2017/6/23.
 */

public class FFmpegEvent {

    public void onDecodeAudio(View view) {
        Timber.d("onDecodeAudio");
    }
}
