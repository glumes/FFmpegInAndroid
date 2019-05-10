package com.glumes.ffmpeglib;


import android.os.Environment;

import com.glumes.ffmpeglib.basic.FFmpegBasicOperation;
import com.glumes.ffmpeglib.codec.FFmpegCodecOperation;
import com.glumes.ffmpeglib.muxer.FFmpegMuxerOperation;

import java.io.File;

public class FFmpegWork {

    // 原始的输入文件，拷贝到 SDCard 上
    private static final String input_mp4_file_path = Environment.getExternalStorageDirectory() + File.separator + "sintel.mp4";
    private static final String input_yuv_file_path = Environment.getExternalStorageDirectory() + File.separator + "ds_480x272.yuv";
    private static final String input_h264_file_path = Environment.getExternalStorageDirectory() + File.separator + "ds_480x272.h264";


    private static final String yuv_to_h264_output_path = Environment.getExternalStorageDirectory() + File.separator + "yuv_to_h264_output.h264";
    private static final String mp4_to_yuv_output_path = Environment.getExternalStorageDirectory() + File.separator + "mp4_to_yuv_output.yuv";
    private static final String h264_to_mp4_output_path = Environment.getExternalStorageDirectory() + File.separator + "h264_to_mp4_output.mp4";

    private static FFmpegBasicOperation basicOperation = new FFmpegBasicOperation();
    private static FFmpegCodecOperation codecOperation = new FFmpegCodecOperation();
    private static FFmpegMuxerOperation muxerOperation = new FFmpegMuxerOperation();

    /**
     * 打印 FFmpeg 相关信息
     */
    public static void printFFmpegConfig() {
        basicOperation.printFileMetaData();
    }

    /**
     * 输出多媒体文件信息
     */
    public static void printFFmpegInfo() {
        basicOperation.printFFmpegInfo(input_mp4_file_path);
    }

    /**
     * mp4 转换成 yuv 文件
     */
    public static void codecMp4ToYUV() {
        codecOperation.decodeVideo(input_mp4_file_path, mp4_to_yuv_output_path);
    }

    public static void codecMp4ToH264() {
        codecOperation.codecMp4ToH264(input_mp4_file_path, mp4_to_yuv_output_path);
    }

    /**
     * yuv 转换成 h264 文件
     */
    public static void codecYUVToH264() {
        codecOperation.encodeVideo(input_yuv_file_path, yuv_to_h264_output_path);
    }

    public static void codecH264ToYUV(){
        codecOperation.codecH264ToYUV(input_yuv_file_path, yuv_to_h264_output_path);

    }

    /**
     * h264 转换到 mp4 文件
     */
    public static void codecH264ToMp4() {
        codecOperation.encodeH264ToMp4(input_h264_file_path, h264_to_mp4_output_path);
    }

    /**
     * yuv 转换到 mp4 文件
     */
    public static void codecYUVToMp4() {
        codecOperation.codecYUVToMp4(input_h264_file_path, h264_to_mp4_output_path);
    }


    public static void muxerDemuxerSample() {
        muxerOperation.demuxerSample();
    }

    public static void muxerDemuxerStandard() {
        muxerOperation.demuxerStandard();
    }
}
