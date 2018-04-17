//
// Created by glumes on 2018/4/17.
//

#include <jni.h>
#include <string>
#include <stdlib.h>
#include <iostream>
#include <android/log.h>

#ifdef  __cplusplus
extern "C" {
#endif
#include "libavcodec/avcodec.h"
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


#define INBUF_SIZE 4096

void decode(AVCodecContext *pContext, AVFrame *pFrame, AVPacket *pPacket, FILE *pFILE);


extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_codec_DecodeUtils_decodeMP4TOYUV(JNIEnv *env, jclass type,
                                                           jstring inputFile_,
                                                           jstring outputFile_) {
    const char *inputFile = env->GetStringUTFChars(inputFile_, 0);
    const char *outputFile = env->GetStringUTFChars(outputFile_, 0);

    FILE *inFile;
    FILE *outFile;
    AVCodecContext *avCodecContext = nullptr;
    AVCodec *avCodec = nullptr;
    AVCodecParserContext *parserContext = nullptr;
    AVPacket *packet;
    AVFrame *frame;

    uint8_t inbuffer[INBUF_SIZE + FF_INPUT_BUFFER_PADDING_SIZE] = {0};
    uint8_t *data;
    size_t data_size;


    memset(inbuffer + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);

    inFile = fopen(inputFile, "rb");
    if (!inFile) {
        LOGI("open in file failed");
        return;
    }

    outFile = fopen(outputFile, "wb");
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

    while (!feof(inFile)) {
        data_size = fread(inbuffer, 1, INBUF_SIZE, inFile);
        if (!data_size) {
            LOGI("read data failed");
            return;
        }
        LOGI("data_size is %d", data_size);
        // 读取的缓冲数据，赋值给指针
        data = inbuffer;
        while (data_size > 0) {
            // 完成解析，到 packet
            int ret = av_parser_parse2(parserContext, avCodecContext,
                                       &packet->data, &packet->size,
                                       data, data_size,
                                       AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
            // 返回值小于 0 则解析失败
            if (ret < 0) {
                LOGI("parser data failed");
                return;
            }
            // 解析了多少数据，相当于指针就往前移动多少
            data += ret;
            // 解析了多少数据，则减少多少数据，
            // 全部完成时，退出当前 while 循环，继续读取下一个 INBUF_SIZE 大小数据
            data_size -= ret;

            if (packet->size) {
//                LOGI("[Packet]Size:%6d\n", packet->size);
                switch (parserContext->pict_type) {
                    case AV_PICTURE_TYPE_I:
//                        LOGI("Type:I");
                        break;
                    case AV_PICTURE_TYPE_P:
//                        LOGI("Type:P");
                        break;
                    case AV_PICTURE_TYPE_B:
//                        LOGI("Type:B");
                        break;
                    default:
                        break;
                }
//                LOGI("Number:%4d\n", parserContext->output_picture_number);

                decode(avCodecContext, frame, packet, outFile);
            }

        }

    }
    decode(avCodecContext, frame, NULL, outFile);

    fclose(inFile);
    fclose(outFile);
    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&avCodecContext);
    av_parser_close(parserContext);

    env->ReleaseStringUTFChars(inputFile_, inputFile);
    env->ReleaseStringUTFChars(outputFile_, outputFile);
}


void decode(AVCodecContext *pContext, AVFrame *pFrame, AVPacket *pPacket, FILE *pFILE) {
    char buf[1024];
    int ret;

    if (pContext == nullptr) {
        LOGI("avCodecContext is null");
        return;
    }

    ret = avcodec_send_packet(pContext, pPacket);
    if (ret < 0) {
        LOGI("Error sending a packet for decoding\n");
        LOGI("ret error value is %d", ret);
        return;
    }
    while (ret > 0) {
        ret = avcodec_receive_frame(pContext, pFrame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            LOGI("receive error");
            return;
        } else if (ret < 0) {
            LOGI("Error while decoding\n");
            return;
        }

//        avcodec_decode_video2()

        for (int i = 0; i < pFrame->height; ++i) {
            fwrite(pFrame->data[0] + pFrame->linesize[0] * i, 1, (size_t) pFrame->width, pFILE);
        }
        for (int i = 0; i < pFrame->height / 2; ++i) {
            fwrite(pFrame->data[1] + pFrame->linesize[1] * i, 1, (size_t) (pFrame->width / 2),
                   pFILE);
        }
        for (int i = 0; i < pFrame->height / 2; ++i) {
            fwrite(pFrame->data[2] + pFrame->linesize[2] * i, 1, (size_t) (pFrame->width / 2),
                   pFILE);
        }

    }

}