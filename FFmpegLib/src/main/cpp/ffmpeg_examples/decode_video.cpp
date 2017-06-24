//
// Created by glumes on 2017/6/24.
//

#include <jni.h>
#include <android/log.h>
#include <fstream>
#include <iostream>

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


#define LOGI(format, ...)  __android_log_print(ANDROID_LOG_INFO,  "FFmpegExamples", format, ##__VA_ARGS__)
#define INBUF_SIZE 4096


using namespace std;

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_FFmpegSample_onDecodeVideo(JNIEnv *env, jobject instance,
                                                     jstring inFileName_, jstring outFileName_) {
    const char *inFileName = env->GetStringUTFChars(inFileName_, 0);
    const char *outFileName = env->GetStringUTFChars(outFileName_, 0);

// TODO
    LOGI("%s", "onDecodeVideo on FFmpegSample");

    const AVCodec *codec;
    AVCodecContext *avCodecContext = nullptr;
    AVCodecParserContext *parserContext;
    ofstream outFile;
    ifstream inFile;
    AVFrame *frame;
    AVPacket *packet;
    uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    uint8_t *data;
    size_t data_size;
    int ret;

    avcodec_register_all();

    packet = av_packet_alloc();

    if (!packet) {
        LOGI("%s", "malloc for packet failed");
        return;
    }

    memset(inbuf + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);

    codec = avcodec_find_decoder(AV_CODEC_ID_MPEG1VIDEO);
    if (!codec) {
        LOGI("%s", "Codec not found\n");
        return;
    }

    parserContext = av_parser_init(codec->id);
    if (!parserContext) {
        LOGI("%s", "parser not found");
        return;
    }

    avCodecContext = avcodec_alloc_context3(codec);
    if (!avCodecContext) {
        LOGI("%s", "Could not allocate video codec context\n");
        return;
    }

    if (avcodec_open2(avCodecContext, codec, nullptr) < 0) {
        LOGI("%s", "Could not open codec\n");
        return;
    }

//    inFile.open(inFileName)



    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&avCodecContext);
    env->ReleaseStringUTFChars(inFileName_, inFileName);
    env->ReleaseStringUTFChars(outFileName_, outFileName);
}


