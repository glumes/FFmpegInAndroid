//
// Created by glumes on 2017/6/24.
//


#include <jni.h>
#include <string>
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

#define LOGI(format, ...)  __android_log_print(ANDROID_LOG_INFO,  "FFmpegExamples", format, ##__VA_ARGS__)


int check_sample_fmt(const AVCodec *pCodec, enum AVSampleFormat format);

int select_sample_rate(const AVCodec *pCodec);

uint64_t select_channel_layout(const AVCodec *pCodec);

void encode(AVCodecContext *pContext, AVFrame *pFrame, AVPacket *pPacket, FILE *pFILE);

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_FFmpegSample_onEncodeAudio(JNIEnv *env, jobject instance,
                                                     jstring inFileName_,
                                                     jstring outFileName_) {
    const char *outFileName = env->GetStringUTFChars(outFileName_, 0);
    const char *inFileName = env->GetStringUTFChars(inFileName_, 0);

    LOGI("%s", "onEncodeAudio");

    const AVCodec *codec;
    AVCodecContext *avCodecContext = nullptr;
    AVFrame *frame;
    AVPacket *packet;
    int ret;
    FILE *file;
    uint16_t *samples;
    float t, tincr;

    avcodec_register_all();
    codec = avcodec_find_encoder(AV_CODEC_ID_MP2);
    if (!codec) {
        LOGI("codec not found");
        return;
    }

    avCodecContext = avcodec_alloc_context3(codec);
    if (!avCodecContext) {
        LOGI("could not allocate audio codec context");
        return;
    }

    avCodecContext->bit_rate = 64000;
    avCodecContext->sample_fmt = AV_SAMPLE_FMT_S16;
    if (!check_sample_fmt(codec, avCodecContext->sample_fmt)) {
        LOGI("Encoder does not support sample format %s",
             av_get_sample_fmt_name(avCodecContext->sample_fmt));
        return;
    }

    avCodecContext->sample_rate = select_sample_rate(codec);
    avCodecContext->channel_layout = select_channel_layout(codec);
    avCodecContext->channels = av_get_channel_layout_nb_channels(avCodecContext->channel_layout);

    if (avcodec_open2(avCodecContext, codec, nullptr) < 0) {
        LOGI("could not open codec");
        return;
    }

    file = fopen(outFileName, "wb");
    if (!file) {
        LOGI("could not allocate the packet");
        return;
    }

    // packet for holding encoded output
    packet = av_packet_alloc();
    if (!packet) {
        LOGI("could not allocate the packet");
        return;
    }

    // frame containing input raw audio
    frame = av_frame_alloc();
    if (!frame) {
        LOGI("could not allocate audio frame");
        return;
    }

    frame->nb_samples = avCodecContext->frame_size;
    frame->format = avCodecContext->sample_fmt;
    frame->channel_layout = avCodecContext->channel_layout;

    // allocate the data buffers
    ret = av_frame_get_buffer(frame, 0);
    if (ret < 0) {
        LOGI("could not allocate audio data buffers");
        return;
    }

    t = 0;
    tincr = (float) (2 * M_PI * 440.0 / avCodecContext->sample_rate);
    for (int i = 0; i < 200; ++i) {
        // make sure the frame is writable
        // makes a copy if the encoder kept a reference internally
        ret = av_frame_make_writable(frame);
        if (ret < 0) {
            return;
        }
        samples = (uint16_t *) frame->data[0];
        for (int j = 0; j < avCodecContext->frame_size; ++j) {
            samples[2 * j] = (uint16_t) (sin(t) * 10000);

            for (int k = 1; k < avCodecContext->channels; ++k) {
                samples[2 * j + k] = samples[2 * j];
            }
            t += tincr;
        }
        encode(avCodecContext, frame, packet, file);
    }

    encode(avCodecContext, nullptr, packet, file);

    fclose(file);

    av_frame_free(&frame);
    av_packet_free(&packet);
    avcodec_free_context(&avCodecContext);

    env->ReleaseStringUTFChars(outFileName_, outFileName);
}


void encode(AVCodecContext *pContext, AVFrame *pFrame, AVPacket *pPacket, FILE *pFILE) {
    int ret;

    ret = avcodec_send_frame(pContext, pFrame);
    if (ret < 0) {
        LOGI("Error send the frame to the encoder");
        return;
    }
    while (ret >= 0) {
        ret = avcodec_receive_packet(pContext, pPacket);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return;
        } else if (ret < 0) {
            LOGI("error encoding audio frame");
            return;
        }
        fwrite(pPacket->data, 1, (size_t) pPacket->size, pFILE);
        av_packet_unref(pPacket);
    }
}


/**
 * select layout with the highest channel count
 * @param pCodec
 * @return
 */
uint64_t select_channel_layout(const AVCodec *pCodec) {
    const uint64_t *p;
    uint64_t best_ch_layout = 0;
    int best_nb_channels = 0;
    if (!pCodec->channel_layouts) {
        return AV_CH_LAYOUT_STEREO;
    }
    p = pCodec->channel_layouts;
    while (*p) {
        int nb_channels = av_get_channel_layout_nb_channels(*p);
        if (nb_channels > best_nb_channels) {
            best_ch_layout = *p;
            best_nb_channels = nb_channels;
        }
        p++;
    }
    return best_ch_layout;
}

/**
 * just pick the highest supported samplerate
 * @param pCodec
 * @return
 */
int select_sample_rate(const AVCodec *pCodec) {
    const int *p;
    int best_samplerate = 0;
    if (!pCodec->supported_samplerates) {
        return 44100;
    }
    p = pCodec->supported_samplerates;
    while (*p) {
        if (!best_samplerate || abs(44100 - *p) < abs(44100 - best_samplerate)) {
            best_samplerate = *p;
        }
        p++;
    }
    return best_samplerate;
}

/**
 * check that a given sample format is supported by the encoder
 * @param pCodec
 * @param format
 * @return
 */
int check_sample_fmt(const AVCodec *pCodec, enum AVSampleFormat format) {
    const enum AVSampleFormat *p = pCodec->sample_fmts;
    while (*p != AV_SAMPLE_FMT_NONE) {
        if (*p == format) {
            return 1;
        }
    }
    return 0;
}

