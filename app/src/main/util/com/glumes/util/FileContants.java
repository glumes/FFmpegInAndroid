package com.glumes.util;

import android.os.Environment;

import java.io.File;

/**
 * Created by glumes on 2017/6/2.
 */

public class FileContants {

    public static final String EXTERNAL_PATH = Environment.getExternalStorageDirectory().getAbsolutePath() + File.separator;
    public static final String YUV420 = EXTERNAL_PATH + "lena_256x256_yuv420p.yuv";
    public static final String YUV420_Y = EXTERNAL_PATH + "output_420_y.y";
    public static final String YUV420_U = EXTERNAL_PATH + "output_420_u.y";
    public static final String YUV420_V = EXTERNAL_PATH + "output_420_v.y";
}
