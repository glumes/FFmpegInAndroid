package com.glumes.ffmpeglearn;

import android.app.Application;

import timber.log.Timber;

/**
 * Created by zhaoying on 2017/5/20.
 */

public class MyApplication extends Application {

    @Override
    public void onCreate() {
        super.onCreate();
        initTimber();
    }

    private void initTimber() {
        Timber.plant(new Timber.DebugTree());
    }
}
