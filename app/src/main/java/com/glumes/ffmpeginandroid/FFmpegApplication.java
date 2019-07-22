package com.glumes.ffmpeginandroid;

import android.app.Application;
import android.os.Environment;

import com.glumes.ffmpeginandroid.util.Log;

import java.io.File;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;

import io.reactivex.Observable;
import io.reactivex.android.schedulers.AndroidSchedulers;
import io.reactivex.schedulers.Schedulers;
import timber.log.Timber;


/**
 * Created by zhaoying on 2017/5/20.
 */

public class FFmpegApplication extends Application {

    @Override
    public void onCreate() {
        super.onCreate();
        initTimber();

        Observable.fromCallable(() -> {
            copyAssertFileToSDCard();
            return true;
        }).subscribeOn(Schedulers.io())
                .observeOn(AndroidSchedulers.mainThread())
                .subscribe(
                        aBoolean -> Log.Companion.d("copy Assert File To SDCard done.."),
                        throwable -> Log.Companion.d("copy Assert File To SDCard failed.."));

    }

    private void initTimber() {
        Timber.plant(new Timber.DebugTree());
    }

    private void copyAssertFileToSDCard() {

        Log.Companion.d("copy Assert File To SDCard start");
        String pathPrefix = Environment.getExternalStorageDirectory() + File.separator;
        try {
            String[] fileNames = getApplicationContext().getAssets().list("files");
            for (String name : fileNames) {
                String path = pathPrefix + name;
                File file = new File(path);
                if (file.exists()) {
                    Log.Companion.d("file is exist " + name);
                    continue;
                }
                InputStream is = getApplicationContext().getAssets().open("files/" + name);
                FileOutputStream fos = new FileOutputStream(file);
                byte[] buffer = new byte[1024];
                int byteCount = 0;
                while ((byteCount = is.read(buffer)) != -1) {
                    fos.write(buffer, 0, byteCount);
                }
                fos.flush();
                is.close();
                fos.close();
                Log.Companion.d("move file : " + name + " to " + path);
            }
        } catch (IOException e) {
            Log.Companion.e(e.getMessage());
        }
    }
}
