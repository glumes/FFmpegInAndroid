package com.glumes.ffmpeglib.codec;

/**
 * @Author glumes
 */
public class FFmpegCodecOperation {

    static {
        System.loadLibrary("ffmpegcodec");
    }

    public native void decodeVideo(String inname, String outname);

    public native void decodeVideo2H264(String inname, String outname);

    public native void decodeVideo2YUV(String inname, String outname);

    public native void encodeVideo(String inname, String outname);


}
