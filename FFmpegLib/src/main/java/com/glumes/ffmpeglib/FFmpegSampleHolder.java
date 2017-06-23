package com.glumes.ffmpeglib;

/**
 * Created by zhaoying on 2017/6/20.
 */

public class FFmpegSampleHolder {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

}
