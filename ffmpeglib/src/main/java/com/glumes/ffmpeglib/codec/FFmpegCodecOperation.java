package com.glumes.ffmpeglib.codec;

/**
 * @Author glumes
 */
public class FFmpegCodecOperation {

    static {
        System.loadLibrary("ffmpegcodec");
    }

    // mp4 到 yuv
    public native void decodeVideo(String inname, String outname);

    // yuv 到 h264
    public native void encodeVideo(String inname, String outname);

    // h264 数据到 mp4
    public native void encodeH264ToMp4(String inname, String outname);

    public native void decodeVideo2H264(String inname, String outname);

    public native void decodeVideo2YUV(String inname, String outname);


}
