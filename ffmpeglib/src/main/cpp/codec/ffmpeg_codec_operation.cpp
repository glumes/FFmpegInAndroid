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


    LOGD("inname is %s", inname);
    LOGD("outname is %s", outname);

    ffmpegCodec.decode_video_to_yuv(inname, outname);

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

    LOGD("inname is %s", inname);
    LOGD("outname is %s", outname);

    ffmpegCodec.encode_yuv_to_h264(inname, outname);

    env->ReleaseStringUTFChars(inname_, inname);
    env->ReleaseStringUTFChars(outname_, outname);
}