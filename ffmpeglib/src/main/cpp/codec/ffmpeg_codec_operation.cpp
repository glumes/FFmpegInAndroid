//
// Created by glumes on 2019/3/16.
//

#include "ffmpeg_codec_operation.h"

static FFmpegCodec ffmpegCodec;

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_codec_FFmpegCodecOperation_decodeVideo(JNIEnv *env, jobject instance,
                                                                 jstring inname_,
                                                                 jstring outname_) {
    const char *inname = env->GetStringUTFChars(inname_, 0);
    const char *outname = env->GetStringUTFChars(outname_, 0);

    LOGD("convert %s to %s", inname, outname);

    ffmpegCodec.codec_mp4_to_yuv(inname, outname);

    env->ReleaseStringUTFChars(inname_, inname);
    env->ReleaseStringUTFChars(outname_, outname);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_codec_FFmpegCodecOperation_encodeVideo(JNIEnv *env, jobject instance,
                                                                 jstring inname_,
                                                                 jstring outname_) {
    const char *inname = env->GetStringUTFChars(inname_, 0);
    const char *outname = env->GetStringUTFChars(outname_, 0);

    LOGD("convert %s to %s", inname, outname);

    ffmpegCodec.codec_yuv_to_h264(inname, outname);

    env->ReleaseStringUTFChars(inname_, inname);
    env->ReleaseStringUTFChars(outname_, outname);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_codec_FFmpegCodecOperation_codecMp4To264(JNIEnv *env, jobject instance,
                                                                      jstring inname_,
                                                                      jstring outname_) {
    const char *inname = env->GetStringUTFChars(inname_, 0);
    const char *outname = env->GetStringUTFChars(outname_, 0);

    LOGD("convert %s to %s", inname, outname);

    ffmpegCodec.codec_mp4_to_h264(inname, outname);

    env->ReleaseStringUTFChars(inname_, inname);
    env->ReleaseStringUTFChars(outname_, outname);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_codec_FFmpegCodecOperation_decodeVideo2YUV(JNIEnv *env, jobject instance,
                                                                     jstring inname_,
                                                                     jstring outname_) {
    const char *inname = env->GetStringUTFChars(inname_, 0);
    const char *outname = env->GetStringUTFChars(outname_, 0);

    LOGD("convert %s to %s", inname, outname);

    ffmpegCodec.codec_mp4_to_yuv(inname, outname);

    env->ReleaseStringUTFChars(inname_, inname);
    env->ReleaseStringUTFChars(outname_, outname);
}


/**
 * 将 H264 数据 编码成 Mp4 文件
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_codec_FFmpegCodecOperation_encodeH264ToMp4(JNIEnv *env, jobject instance,
                                                                     jstring inname_,
                                                                     jstring outname_) {
    const char *inname = env->GetStringUTFChars(inname_, 0);
    const char *outname = env->GetStringUTFChars(outname_, 0);

    LOGD("convert %s to %s", inname, outname);

    ffmpegCodec.codec_h264_to_mp4(inname, outname);

    env->ReleaseStringUTFChars(inname_, inname);
    env->ReleaseStringUTFChars(outname_, outname);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_codec_FFmpegCodecOperation_codecMp4ToH264(JNIEnv *env, jobject instance,
                                                                    jstring input_mp4_file_path_,
                                                                    jstring mp4_to_yuv_output_path_) {
    const char *input_mp4_file_path = env->GetStringUTFChars(input_mp4_file_path_, 0);
    const char *mp4_to_yuv_output_path = env->GetStringUTFChars(mp4_to_yuv_output_path_, 0);


    env->ReleaseStringUTFChars(input_mp4_file_path_, input_mp4_file_path);
    env->ReleaseStringUTFChars(mp4_to_yuv_output_path_, mp4_to_yuv_output_path);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_codec_FFmpegCodecOperation_codecH264ToYUV(JNIEnv *env, jobject instance,
                                                                    jstring input_yuv_file_path_,
                                                                    jstring yuv_to_h264_output_path_) {
    const char *input_yuv_file_path = env->GetStringUTFChars(input_yuv_file_path_, 0);
    const char *yuv_to_h264_output_path = env->GetStringUTFChars(yuv_to_h264_output_path_, 0);


    env->ReleaseStringUTFChars(input_yuv_file_path_, input_yuv_file_path);
    env->ReleaseStringUTFChars(yuv_to_h264_output_path_, yuv_to_h264_output_path);
}

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_codec_FFmpegCodecOperation_codecYUVToMp4(JNIEnv *env, jobject instance,
                                                                   jstring input_h264_file_path_,
                                                                   jstring h264_to_mp4_output_path_) {
    const char *input_h264_file_path = env->GetStringUTFChars(input_h264_file_path_, 0);
    const char *h264_to_mp4_output_path = env->GetStringUTFChars(h264_to_mp4_output_path_, 0);


    env->ReleaseStringUTFChars(input_h264_file_path_, input_h264_file_path);
    env->ReleaseStringUTFChars(h264_to_mp4_output_path_, h264_to_mp4_output_path);
}