//
// Created by glumes on 2018/4/19.
//


#include <jni.h>

#include "logutil.h"

#ifdef  __cplusplus

#endif

extern "C" {
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/opt.h"
#include "libavutil/imgutils.h"

#ifdef  __cplusplus
};


bool check_sample_fmt(const AVCodec *pCodec, AVSampleFormat format);

int select_sample_rate(const AVCodec *pCodec);

uint64_t select_channel_layout(const AVCodec *pCodec);


extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_codec_EncodeUtils_encode_1audio(JNIEnv *env, jclass type,
                                                          jstring outputFile_) {
    const char *outputFile = env->GetStringUTFChars(outputFile_, 0);

    // TODO

    const AVCodec *avCodec;
    AVCodecContext *avCodecContext = NULL;
    AVFrame *avFrame;
    AVPacket *avPacket;
    int i, j, k, ret;

    FILE *file;

    uint16_t *samples;
    float t, tincr;

    avCodec = avcodec_find_encoder(AV_CODEC_ID_MP2);
    if (!avCodec) {
        LOGI("codec not found\n");
        return;
    }

    avCodecContext = avcodec_alloc_context3(avCodec);

    if (!avCodecContext) {
        LOGI("Could not allocate audio codec context\n");
        return;
    }

    avCodecContext->bit_rate = 64000;

    avCodecContext->sample_fmt = AV_SAMPLE_FMT_S16;

    if (!check_sample_fmt(avCodec, avCodecContext->sample_fmt)) {
        LOGI("encoder does not support sample format %s",
             av_get_sample_fmt_name(avCodecContext->sample_fmt));
        return;
    }

    avCodecContext->sample_rate = select_sample_rate(avCodec);
    avCodecContext->channel_layout = select_channel_layout(avCodec);
    avCodecContext->channels = av_get_channel_layout_nb_channels(avCodecContext->channel_layout);

    if (avcodec_open2(avCodecContext, avCodec, NULL) < 0) {
        LOGI("could not open codec\n");
        return;
    }

    file = fopen(outputFile, "wb");
    if (!file) {
        LOGI("could not open file\n");
        return;
    }

    avPacket = av_packet_alloc();
    if (!avPacket) {
        LOGI("could not allocate the packet\n");
        return;
    }

    avFrame = av_frame_alloc();
    if (!avFrame) {
        LOGI("Could not allocate the frame\n");
        return;
    }

    avFrame->nb_samples = avCodecContext->frame_size;
    avFrame->format = avCodecContext->sample_fmt;
    avFrame->channel_layout = avCodecContext->channel_layout;

    ret = av_frame_get_buffer(avFrame, 0);

    if (ret < 0) {
        LOGI("Could not allocate audio data buffers\n");
        return;
    }

    t = 0;
    tincr = 2 * M_PI * 440.0 / avCodecContext->sample_rate;
    for (i = 0; i < 200; i++) {
        ret = av_frame_make_writable(avFrame);
        if (ret < 0) {
            return;
        }
        samples = (uint16_t *) avFrame->data[0];
        for (j = 0; j < avCodecContext->frame_size; j++) {
            samples[2*j]
        }
    }


    env->ReleaseStringUTFChars(outputFile_, outputFile);
}

uint64_t select_channel_layout(const AVCodec *pCodec) {
    return 0;
}


bool check_sample_fmt(const AVCodec *pCodec, AVSampleFormat format) {
    return false;
}


int select_sample_rate(const AVCodec *pCodec) {
    return 0;
}

