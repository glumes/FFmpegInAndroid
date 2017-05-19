package com.glumes.ffmpeglib;

/**
 * Created by glumes on 2017/5/19.
 */

public class SimpleDecoder {

    public native int decode(String inputurl, String outputurl);

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
    }
}
