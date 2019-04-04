package com.glumes.ffmpeglib.basic;

public class FFmpegBasicOperation {


    static {
        System.loadLibrary("basicoperation");
    }

    public native void printFileMetaData();

    public native void printFFmpegInfo(String path);
}
