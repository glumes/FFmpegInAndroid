package com.glumes.ffmpeglib.codec;

/**
 * @Author glumes
 */
public class FFmpegCodecOperation {

    static {
        System.loadLibrary("ffmpegcodec");
    }


    /**
     * yuv h264 mp4 三者之间的转换操作
     */
    // mp4 到 yuv
    public native void codecMp4ToYUV(String inname, String outname);

    // yuv 到 h264
    public native void codecYUVToH264(String inname, String outname);

    // h264 数据到 mp4
    public native void codecH264ToMp4(String inname, String outname);

    public native void codecMp4ToH264(String input_mp4_file_path, String mp4_to_yuv_output_path);

    public native void codecH264ToYUV(String input_yuv_file_path, String yuv_to_h264_output_path);

    public native void codecYUVToMp4(String input_yuv_file_path, String yuv_to_mp4_output_path);


//    public native void decodeVideo2H264(String inname, String outname);

//    public native void decodeVideo2YUV(String inname, String outname);
}
