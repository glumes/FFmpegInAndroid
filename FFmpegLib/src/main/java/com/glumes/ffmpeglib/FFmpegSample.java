package com.glumes.ffmpeglib;

import timber.log.Timber;

/**
 * Created by zhaoying on 2017/6/20.
 */

public class FFmpegSample {

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
        System.loadLibrary("x264-148");
    }

    private static class FFmpegSampleHolder {
        private static FFmpegSample mInstance = new FFmpegSample();
    }

    public static FFmpegSample getInstance() {
        return FFmpegSampleHolder.mInstance;
    }


    private FFmpegSample() {
        Timber.d("Init FFmpegSample");

    }


    public native void onDecodeAudio(String inFileName, String outFileName);

    public native void onDecodeVideo(String inFileName, String outFileName);

    public native void onEncodeAudio(String outFileName);

    public native void onEncodeVideo(String outFileName);

    public void test() {
        SimpleDecoder decoder = new SimpleDecoder();
        decoder.test();
    }
}
