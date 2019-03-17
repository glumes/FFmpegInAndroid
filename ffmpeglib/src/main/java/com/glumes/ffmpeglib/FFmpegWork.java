package com.glumes.ffmpeglib;


import android.os.Environment;

import com.glumes.ffmpeglib.basic.FFmpegBasicOperation;
import com.glumes.ffmpeglib.codec.FFmpegCodecOperation;
import com.glumes.ffmpeglib.muxer.FFmpegMuxerOperation;

import java.io.File;

public class FFmpegWork {

    private static final String video_input_path = Environment.getExternalStorageDirectory() + File.separator + "sintel.mp4";
    private static final String video_output_path = Environment.getExternalStorageDirectory() + File.separator + "sintel.yuv";

    private static final String yuv_input_path = Environment.getExternalStorageDirectory() + File.separator + "ds_480x272.yuv";
    private static final String h264_output_path = Environment.getExternalStorageDirectory() + File.separator + "ds_480x272.h264";

    private static FFmpegBasicOperation basicOperation = new FFmpegBasicOperation();
    private static FFmpegCodecOperation codecOperation = new FFmpegCodecOperation();
    private static FFmpegMuxerOperation muxerOperation = new FFmpegMuxerOperation();

    public static void printFileMetaData() {
        basicOperation.printFileMetaData();
    }

    public static void printFFmpegInfo() {
        basicOperation.printFFmpegInfo();
    }

    public static void codecDecodeVideo() {
        codecOperation.decodeVideo(video_input_path, video_output_path);
    }

    public static void codecEncodeYUV2H264() {
        codecOperation.encodeVideo(yuv_input_path, h264_output_path);
    }

    public static void muxerDemuxerSample() {
        muxerOperation.demuxerSample();
    }

    public static void muxerDemuxerStandard() {
        muxerOperation.demuxerStandard();
    }
}
