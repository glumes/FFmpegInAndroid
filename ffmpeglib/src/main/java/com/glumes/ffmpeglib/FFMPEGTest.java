package com.glumes.ffmpeglib;

/**
 * Created by zhaoying on 2017/5/16.
 */

public class FFMPEGTest {
    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
        System.loadLibrary("avcodec-57");
        System.loadLibrary("avdevice-57");
        System.loadLibrary("avfilter-6");
        System.loadLibrary("avformat-57");
        System.loadLibrary("avresample-3");
        System.loadLibrary("avutil-55");
        System.loadLibrary("postproc-54");
        System.loadLibrary("swresample-2");
        System.loadLibrary("swscale-4");
        System.loadLibrary("simple-mediadata-lib");
        System.loadLibrary("media-lib");
    }

    public static native void printFFmpegConfigInfo();

    public static native void yuv420Split(String filename, String output_y, String output_u, String output_v);
}
