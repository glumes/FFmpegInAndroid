package com.glumes.commonlibrary;

import timber.log.Timber;

/**
 * Created by glumes on 2017/6/23.
 */

public class BaseApplication extends android.app.Application {

    @Override
    public void onCreate() {
        super.onCreate();
        initTimber();
    }

    private void initTimber() {
        Timber.plant(new Timber.DebugTree());
    }
}
