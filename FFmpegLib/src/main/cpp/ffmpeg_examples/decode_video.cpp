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


void decode(AVCodecContext *pContext, AVFrame *pFrame, AVPacket *pPacket, const char *name);

using namespace std;


void decode(AVCodecContext *pContext, AVFrame *pFrame, AVPacket *pPacket, const char *name);

void pgm_save(uint8_t *string, int i, int width, int height, const char *filename);


/**
 * 实现 MEPG 文件到 PCM 图片格式的转换
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_FFmpegSample_onDecodeVideo(JNIEnv *env, jobject instance,
                                                     jstring inFileName_, jstring outFileName_) {
    const char *inFileName = env->GetStringUTFChars(inFileName_, 0);
    const char *outFileName = env->GetStringUTFChars(outFileName_, 0);

// TODO
    LOGI("onDecodeVideo on FFmpegSample");

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

    FILE *file;

    avcodec_register_all();

    packet = av_packet_alloc();

    if (!packet) {
        LOGI("malloc for packet failed");
        return;
    }

    memset(inbuf + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);

    codec = avcodec_find_decoder(AV_CODEC_ID_MPEG1VIDEO);
    if (!codec) {
        LOGI("Codec not found\n");
        return;
    }

    parserContext = av_parser_init(codec->id);
    if (!parserContext) {
        LOGI("parser not found");
        return;
    }

    avCodecContext = avcodec_alloc_context3(codec);
    if (!avCodecContext) {
        LOGI("Could not allocate video codec context\n");
        return;
    }

    if (avcodec_open2(avCodecContext, codec, nullptr) < 0) {
        LOGI("Could not open codec\n");
        return;
    }

    // open file
//    inFile.open(inFileName, ios_base::in | ios_base::binary);
//    if (!inFile.is_open()) {
//        LOGI("%s", "open file failed\n");
//        return;
//    }

    file = fopen(inFileName, "rb");
    LOGI("inFileName is %s", inFileName);
    LOGI("outFileName is %s", outFileName);
    if (!file) {
        LOGI("open file failed");
        return;
    }

    frame = av_frame_alloc();
    if (!frame) {
        LOGI("Could not allocate video frame\n");
        return;
    }



    while (!feof(file)) {

//        data_size = (size_t) inFile.read((char *) inbuf, INBUF_SIZE);
        data_size = fread(inbuf, 1, INBUF_SIZE, file);
        if (!data_size) {
            LOGI("read data from file failed");
        }
        data = inbuf;
        while (data_size > 0) {
            ret = av_parser_parse2(parserContext, avCodecContext,
                                   &packet->data, &packet->size,
                                   data, data_size,
                                   AV_NOPTS_VALUE, AV_NOPTS_VALUE, 0);
            if (ret < 0) {
                LOGI("Error while parsing\n");
                return;
            }
            data += ret;
            data_size -= ret;
            if (packet->size) {
                decode(avCodecContext, frame, packet, outFileName);
            }
        }
    }

    /**
     * flush the decoder
     */

    decode(avCodecContext, frame, NULL, outFileName);

    fclose(file);

    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&avCodecContext);
    av_parser_close(parserContext);
    env->ReleaseStringUTFChars(inFileName_, inFileName);
    env->ReleaseStringUTFChars(outFileName_, outFileName);
}


void decode(AVCodecContext *pContext, AVFrame *pFrame, AVPacket *pPacket, const char *name) {
    char buf[1024];
    int ret;
    ret = avcodec_send_packet(pContext, pPacket);
    if (ret < 0) {
//        LOGI("Error sending a packet for decoding\n");
        return;
    }

    while (ret >= 0) {
        ret = avcodec_receive_frame(pContext, pFrame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return;
        } else if (ret < 0) {
            LOGI("Error while decoding\n");
            return;
        }

        if (pContext->frame_number == 200) {
            break;
        }
        LOGI("Saving frame %3d\n", pContext->frame_number);
        fflush(stdout);
        snprintf(buf, sizeof(buf), "%s-%d", name, pContext->frame_number);
        LOGI("buf name is %s", buf);
        pgm_save(pFrame->data[0], pFrame->linesize[0], pFrame->width, pFrame->height, buf);
        LOGI("save success");
    }
}

void pgm_save(unsigned char *buf, int wrap, int width, int height, const char *name) {
    FILE *file;
    char *filename = strcat((char *) name, ".pgm");

    file = fopen(filename, "w");
    if (!file) {
        LOGI("could not open file");
    }
    fprintf(file, "P5\n%d %d\n%d\n", width, height, 255);
    for (int i = 0; i < height; ++i) {
        fwrite(buf + i * wrap, 1, width, file);
    }
    fclose(file);
}



