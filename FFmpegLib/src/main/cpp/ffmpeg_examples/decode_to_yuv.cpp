//
// Created by glumes on 2017/6/28.
//

#include <jni.h>
#include <android/log.h>

#ifdef  __cplusplus
extern "C" {
#endif
#include "libavcodec/avcodec.h"
#include "libavutil/frame.h"
#include "libavutil/mem.h"

#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libavutil/avutil.h"
#include "libavfilter/avfilter.h"

#ifdef  __cplusplus
};
#endif


#ifdef  ANDROID
#define LOGI(format, ...)  __android_log_print(ANDROID_LOG_INFO,  "FFmpegExamples", format, ##__VA_ARGS__)
#endif


extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_FFmpegSample_onHevcToYuv(JNIEnv *env, jobject instance, jstring s_,
                                                   jstring s1_) {
    const char *inFileName = env->GetStringUTFChars(s_, 0);
    const char *outFileName = env->GetStringUTFChars(s1_, 0);

    FILE *inFile;
    FILE *outFile;
    AVCodecContext *avCodecContext = nullptr;
    AVCodec *avCodec = nullptr;
    AVCodecParserContext *parserContext = nullptr;
    AVPacket *packet;
    AVFrame *frame;

    uint8_t *data;
    size_t data_size;

    inFile = fopen(inFileName, "rb");
    if (!inFile) {
        LOGI("open in file failed");
        return;
    }

    outFile = fopen(outFileName, "wb");
    if (!outFile) {
        LOGI("open out file failed");
        return;
    }

    avcodec_register_all();

    // 由 AVCodec 才能初始化 AVParserContext 和 AVCodecContext
    avCodec = avcodec_find_decoder(AV_CODEC_ID_HEVC);
    if (!avCodec) {
        LOGI("find codec failed");
        return;
    }

    parserContext = av_parser_init(avCodec->id);
    if (!parserContext) {
        LOGI("parserContext failed");
        return;
    }

    avCodecContext = avcodec_alloc_context3(avCodec);
    if (!avCodecContext) {
        LOGI("avCodecContext failed");
        return;
    }

    if (avcodec_open2(avCodecContext, avCodec, nullptr) < 0) {
        LOGI("open codec failed");
        return;
    };

    frame = av_frame_alloc();
    packet = av_packet_alloc();
    if (!frame) {
        LOGI("frame failed");
        return;
    }
    if (!packet) {
        LOGI("packet failed");
        return;
    }


    env->ReleaseStringUTFChars(s_, inFileName);
    env->ReleaseStringUTFChars(s1_, outFileName);
}


extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_FFmpegSample_onH264ToYuv(JNIEnv *env, jobject instance, jstring s_,
                                                   jstring s1_) {
    const char *inFileName = env->GetStringUTFChars(s_, 0);
    const char *outFileName = env->GetStringUTFChars(s1_, 0);

    // TODO

    env->ReleaseStringUTFChars(s_, inFileName);
    env->ReleaseStringUTFChars(s1_, outFileName);
}