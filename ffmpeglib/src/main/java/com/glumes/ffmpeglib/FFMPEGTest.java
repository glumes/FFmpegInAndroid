package com.glumes.ffmpeglib;

/**
 * Created by zhaoying on 2017/5/16.
 */

public class FFMPEGTest {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("avcodec-57");
    }

    public static native String stringFromJNI(String name);
}
