//
// Created by glumes on 2019/3/11.
//

#include "ffmpeg_basic_operations.h"

static FFmpegBasic *fFmpegBasic;
extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_basic_FFmpegBasicOperation_printFileMetaData(JNIEnv *env,
                                                                       jobject instance) {

    LogClient::LogD("print av format info");
    char *format_infostr = FFmpegBasic::getAvFormatInfo();
    LogClient::LogD(format_infostr);
    free(format_infostr);

    LogClient::LogD("print av codec info");
    char *codec_infostr = FFmpegBasic::getAvCodecInfo();
    LogClient::LogD(codec_infostr);
    free(codec_infostr);

    LogClient::LogD("print av filter info");
    char *avfilter_infostr = FFmpegBasic::getAvFilterInfo();
    LogClient::LogD(avfilter_infostr);
    free(avfilter_infostr);

}

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_basic_FFmpegBasicOperation_printFFmpegInfo(JNIEnv *env, jobject instance,
                                                                     jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);

    LOGD("%s and data is  %d", __FUNCTION__, __LINE__);

    fFmpegBasic = new FFmpegBasic();

    fFmpegBasic->printVideoInfo(path);

    env->ReleaseStringUTFChars(path_, path);
}

