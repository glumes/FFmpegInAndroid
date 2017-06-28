package com.glumes.ffmpeglib;

import android.os.Environment;
import android.view.View;


import com.glumes.ffmpeglib.utils.Constants;

import junit.framework.TestSuite;

import java.io.File;

import timber.log.Timber;

/**
 * Created by zhaoying on 2017/6/23.
 */

public class FFmpegEvent {


    String mpegFilePath = Environment.getExternalStorageDirectory() + File.separator + Constants.MEGP_1_FILE_NAME;

    String pgmFilePath = Environment.getExternalStorageDirectory() + File.separator + "PGM" + File.separator + "test";

    String mp2FilePath = Environment.getExternalStorageDirectory() + File.separator + "output.mp2";

    String pcmFilePath = Environment.getExternalStorageDirectory() + File.separator + Constants.PCM_FILE_NAME;

    String outpcmFilePath = Environment.getExternalStorageDirectory() + File.separator + Constants.OUT_PCM_FILE_NAME;


    String hevcFilePath = Environment.getExternalStorageDirectory() + File.separator + Constants.HEVC_FILE_NAME;

    String outYuvFilePath = Environment.getExternalStorageDirectory() + File.separator + "testOutput.yuv";

    /**
     * decode Failed
     *
     * @param view
     */
    public void onDecodeAudio(View view) {
        Timber.d("onDecodeAudio");
        FFmpegSample.getInstance().onDecodeAudio(mp2FilePath, outpcmFilePath);
    }


    /**
     * decode video mpeg_1 to pgm success
     *
     * @param view
     */
    public void onDecodeVideo(View view) {
        Timber.d("onDecodeVideo");
        FFmpegSample.getInstance().onDecodeVideo(mpegFilePath, pgmFilePath);
    }

    /**
     * encode audio pcm to mp2 success
     *
     * @param view
     */
    public void onEncodeAudio(View view) {
        Timber.d("onEncodeAudio");
        FFmpegSample.getInstance().onEncodeAudio(pcmFilePath, mp2FilePath);
    }

    public void onEncodeVideo(View view) {
        Timber.d("onEncodeVideo");
        FFmpegSample.getInstance().onEncodeVideo("");
    }

    /**
     * print FFmpeg Config success
     *
     * @param view
     */
    public void onFFmpegConfig(View view) {
        Timber.d("onFFmepgConfig");
        FFMPEGTest.printFFmpegConfigInfo();
    }

    public void onHevcToYuv(View view) {
        Timber.d("decode HEVC TO YUV");
        FFmpegSample.getInstance().onHevcToYuv(hevcFilePath, outYuvFilePath);
    }

    public void onH264ToYuv(View view) {
        Timber.d("decode H264 TO YUV");
        FFmpegSample.getInstance().onH264ToYuv("", "");
    }

}
