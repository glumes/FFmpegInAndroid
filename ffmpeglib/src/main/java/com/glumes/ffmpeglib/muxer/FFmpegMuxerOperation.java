package com.glumes.ffmpeglib.muxer;

public class FFmpegMuxerOperation {

    static {
        System.loadLibrary("ffmpegmuxer");
    }

    public native void demuxerSample(String input_file,String output_audio_file,String output_video_file);


    public native void demuxerStandard();
}
