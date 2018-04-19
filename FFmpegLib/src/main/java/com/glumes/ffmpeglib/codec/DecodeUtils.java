package com.glumes.ffmpeglib.codec;

/**
 * Created by glumes on 17/04/2018
 */
public class DecodeUtils {


    static {
        System.loadLibrary("codec-lib");
    }


    public static native void decode_MPEG_to_PGM(String inputFile, String outputFile);

}
