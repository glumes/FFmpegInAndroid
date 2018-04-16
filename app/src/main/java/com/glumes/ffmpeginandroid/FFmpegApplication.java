package com.glumes.ffmpeginandroid;

import android.app.Application;


/**
 * Created by zhaoying on 2017/5/20.
 */

public class FFmpegApplication extends Application {

    @Override
    public void onCreate() {
        super.onCreate();
        initTimber();
    }

    private void initTimber() {
    }
}
