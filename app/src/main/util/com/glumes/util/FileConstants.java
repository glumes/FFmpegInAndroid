package com.glumes.util;

import android.os.Environment;

import java.io.File;

/**
 * Created by glumes on 2017/6/2.
 */

public class FileConstants {

    // 旧版，基本不用了
    public static final String EXTERNAL_PATH = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator;


    public static final String YUV420 = EXTERNAL_PATH + "lena_256x256_yuv420p.yuv";
    public static final String YUV420_Y = EXTERNAL_PATH + "output_420_y.y";
    public static final String YUV420_U = EXTERNAL_PATH + "output_420_u.y";
    public static final String YUV420_V = EXTERNAL_PATH + "output_420_v.y";


    public static final String FILE_MPEG = EXTERNAL_PATH + "mpeg_file.mpeg";

    public static final String FILE_M2V = EXTERNAL_PATH + "bigbuckbunny_480x272.m2v";

    public static final String FILE_YUV = EXTERNAL_PATH + "files/ds_480x272.yuv";

    public static final String PGM_DRI = EXTERNAL_PATH + "PGM";

    public static final String OUT_PUT_FILE = PGM_DRI + File.separator + "test";

    public static final String FILE_H264 = EXTERNAL_PATH + "test.h264";

    public static final String TAG = "FFmpegInAndroid";

}
