package com.glumes.ffmpeglib;


import com.glumes.ffmpeglib.basic.FFmpegBasicOperation;

public class FFmpegWork {


    private static FFmpegBasicOperation basicOperation = new FFmpegBasicOperation();

    public static void printFileMetaData() {
        basicOperation.printFileMetaData();
    }

    public static void printFFmpegInfo() {
        basicOperation.printFFmpegInfo();
    }

}
