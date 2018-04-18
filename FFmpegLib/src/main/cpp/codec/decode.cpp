//
// Created by glumes on 2018/4/17.
//

#include <jni.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include "logutil.h"

#ifdef  __cplusplus
extern "C" {
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"

#ifdef  __cplusplus
};
#endif


#define INBUF_SIZE 4096

#include "decodeUtil.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_FFmpegSample_decode_1mpeg_1to_1pgm(JNIEnv *env, jobject instance,
                                                             jstring inputFilePath_,
                                                             jstring outputFilePath_) {
    const char *inputFilePath = env->GetStringUTFChars(inputFilePath_, 0);
    const char *outputFilePath = env->GetStringUTFChars(outputFilePath_, 0);
    // 解码器
    const AVCodec *codec;
    //
    AVCodecParserContext *parserContext;
    //
    AVCodecContext *avCodecContext;
    //
    AVFrame *frame;
    AVPacket *packet;

    FILE *inFile;

    // 读取的缓存内容
    uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    uint8_t *data;

    // 读取的缓存大小
    size_t data_size;

    int ret;

    // 注册了所有的文件格式和编解码器的库
    av_register_all();


    // 找到对应的解码器
    codec = avcodec_find_decoder(AV_CODEC_ID_MPEG1VIDEO);
    if (!codec) {
        LOGI("Codec not found\n");
        return;
    }

    // 初始化 AVCodecParserContext 结构体
    parserContext = av_parser_init(codec->id);
    if (!parserContext) {
        LOGI("parser not found\n");
        return;
    }

    // 初始化 AVCodecContext 结构体
    avCodecContext = avcodec_alloc_context3(codec);
    if (!avCodecContext) {
        LOGI("Could not allocate video codec context\n");
    }

    // 打开解码器
    if (avcodec_open2(avCodecContext, codec, nullptr) < 0) {
        LOGI("Could not open codec\n");
        return;
    }

    // 打开输入文件
    inFile = fopen(inputFilePath, "rb");
    if (!inFile) {
        LOGI("Could not open input file\n");
        return;
    }

    // 创建编码前的数据结构
    packet = av_packet_alloc();
    if (!packet) {
        LOGI("Could not allocate video packet\n");
        return;
    }

    // 创建解码后的数据结构
    frame = av_frame_alloc();
    if (!frame) {
        LOGI("Could not allocate video frame\n");
        return;
    }


    while (!feof(inFile)) {
        data_size = fread(inbuf, 1, INBUF_SIZE, inFile);
        if (!data_size) {
            LOGI("read data from input file failed");
        }
        data = inbuf;
        while (data_size > 0) {
            ret = av_parser_parse2(parserContext, avCodecContext, &packet->data, &packet->size,
                                   data, data_size, AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
            if (ret < 0) {
                LOGI("Error while parsing\n");
                return;
            }
            data += ret;
            data_size -= ret;
            if (packet->size) {
                decode(avCodecContext, frame, packet, outputFilePath);
            }
        }
    }

    fclose(inFile);
    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&avCodecContext);
    av_parser_close(parserContext);

    env->ReleaseStringUTFChars(inputFilePath_, inputFilePath);
    env->ReleaseStringUTFChars(outputFilePath_, outputFilePath);
}