package com.glumes.ffmpeglib.muxer;

public class FFmpegMuxerOperation {

    static {
        System.loadLibrary("ffmpegmuxer");
    }

    public native void demuxerSample();

    public native void demuxerStandard();
}
