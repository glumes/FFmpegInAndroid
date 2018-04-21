//
// Created by glumes on 2018/4/19.
//



#include <jni.h>

#include "logutil.h"

#ifdef  __cplusplus
extern "C" {
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"

#ifdef  __cplusplus
};
#endif


void encode(AVCodecContext *avCodecContext, AVFrame *avFrame, AVPacket *avPacket, FILE *outFile);

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_codec_EncodeUtils_encode_1YUV_1to_1H264(JNIEnv *env, jclass type,
                                                                  jstring inputFile_,
                                                                  jstring outputFile_) {

    LOGI("start encode");

    const char *inputFile = env->GetStringUTFChars(inputFile_, 0);
    const char *outputFile = env->GetStringUTFChars(outputFile_, 0);

    AVCodec *avCodec;
    AVCodecContext *avCodecContext = nullptr;
    FILE *inFile;
    FILE *outFile;

    int ret, i, x, y;

    AVFrame *avFrame;
    AVPacket *avPacket;

    AVCodecID codecID = AV_CODEC_ID_H264;

    int in_w = 480;
    int in_h = 272;
    int framenum = 100;


    uint8_t endcode[] = {0, 0, 1, 0xb7};

    avcodec_register_all();

    avCodec = avcodec_find_encoder(codecID);

    if (!avCodec) {
        LOGI("Codec not found\n");
        return;
    }

    avCodecContext = avcodec_alloc_context3(avCodec);

    if (!avCodecContext) {
        LOGI("Could not allocate video codec context\n");
        return;
    }


    avCodecContext->bit_rate = 400000;
    avCodecContext->width = in_w;
    avCodecContext->height = in_h;

//    avCodecContext->time_base.num = 1;
//    avCodecContext->time_base.den = 25;

    avCodecContext->time_base = (AVRational) {1, 25};
    avCodecContext->framerate = (AVRational) {25, 1};


    avCodecContext->gop_size = 10;
    avCodecContext->max_b_frames = 1;

    avCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;

    // if code_id is av_codec_id_h264,do this
    av_opt_set(avCodecContext->priv_data, "preset", "slow", 0);


    if (avcodec_open2(avCodecContext, avCodec, NULL) < 0) {
        LOGI("Could not open codec\n");
        return;
    }


    avPacket = av_packet_alloc();

    if (!avPacket) {
        LOGI("Could not allocate video packet\n");
        return;
    }

    avFrame = av_frame_alloc();

    if (!avFrame) {
        LOGI("Could not allocate video frame\n");
        return;
    }

    avFrame->format = avCodecContext->pix_fmt;
    avFrame->width = avCodecContext->width;
    avFrame->height = avCodecContext->height;


    ret = av_frame_get_buffer(avFrame, 32);

    if (ret < 0) {
        LOGI("Could not allocate the video frame data\n");
        return;
    }


//    inFile = fopen(inputFile, "rb");
//    if (!inFile) {
//        LOGI("Could not open file\n");
//        return;
//    }

    outFile = fopen(outputFile, "wb");
    if (!outFile) {
        LOGI("Could not open file\n");
        return;
    }


    for (i = 0; i < 250; ++i) {
        ret = av_frame_make_writable(avFrame);
        if (ret < 0) {
            return;
        }

        for (y = 0; y < avCodecContext->height; ++y) {
            for (x = 0; x < avCodecContext->width; ++x) {
                avFrame->data[0][y * avFrame->linesize[0] + x] = x + y + i * 3;
            }
        }

        for (y = 0; y < avCodecContext->height / 2; y++) {
            for (x = 0; x < avCodecContext->width / 2; x++) {
                avFrame->data[1][y * avFrame->linesize[1] + x] = 128 + y + i * 2;
                avFrame->data[2][y * avFrame->linesize[2] + x] = 64 + x + i * 5;
            }
        }

        avFrame->pts = i;
        encode(avCodecContext, avFrame, avPacket, outFile);
    }

    encode(avCodecContext, NULL, avPacket, outFile);

    fwrite(endcode, 1, sizeof(endcode), outFile);

    fclose(outFile);


    av_free(avCodecContext);

    av_frame_free(&avFrame);

    av_packet_free(&avPacket);

    env->ReleaseStringUTFChars(inputFile_, inputFile);
    env->ReleaseStringUTFChars(outputFile_, outputFile);
}


void encode(AVCodecContext *avCodecContext, AVFrame *avFrame, AVPacket *avPacket, FILE *outFile) {
    int ret;
    if (avFrame) {
        LOGI("Send frame %3lld PRId64\n", avFrame->pts);
    }

    ret = avcodec_send_frame(avCodecContext, avFrame);


    while (ret >= 0) {
        ret = avcodec_receive_packet(avCodecContext, avPacket);

        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return;
        } else if (ret < 0) {
            LOGI("Error during encoding\n");
        }

        LOGI("Write packet %3lld PRId64 (size=%5d)\n", avPacket->pts, avPacket->size);

        fwrite(avPacket->data, 1, avPacket->size, outFile);

        av_packet_unref(avPacket);

    }
}
