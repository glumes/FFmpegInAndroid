package com.glumes.ffmpeglib.codec;

/**
 * Created by glumes on 19/04/2018
 */
public class EncodeUtils {

    static {
        System.loadLibrary("codec-lib");
    }


    public static native void encode_YUV_to_H264(String inputFile, String outputFile);
}
