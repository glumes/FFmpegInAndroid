//
// Created by glumes on 2017/6/24.
//

#ifndef ENCODE_VIDEO_CPP
#define ENCODE_VIDEO_CPP

#include <jni.h>
#include <string>
#include <android/log.h>
#ifdef  __cplusplus
extern "C" {
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libavutil/imgutils.h"
#include "libavutil/opt.h"
#ifdef  __cplusplus
};
#endif

#define LOGI(format, ...)  __android_log_print(ANDROID_LOG_INFO,  "FFmpegExamples", format, ##__VA_ARGS__)

void encodeVideo(AVCodecContext *pContext, AVFrame *pFrame, AVPacket *pPacket, FILE *pFILE);


extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_FFmpegSample_onEncodeVideo(JNIEnv *env, jobject instance,
                                                     jstring outFileName_) {
    const char *outFileName = env->GetStringUTFChars(outFileName_, 0);

    // TODO

    LOGI("%s", "onEncodeVideo");

    const AVCodec *codec;
    AVCodecContext *avCodecContext = nullptr;
    FILE *file;
    AVFrame *frame;
    AVPacket *packet;
    uint8_t endcode[] = {0, 0, 1, 0xb7};
    int ret;
    avcodec_register_all();

    // find the mepg1video encoder
    codec = avcodec_find_decoder_by_name("");

    if (!codec) {
        LOGI("codec not found");
        return;
    }

    avCodecContext = avcodec_alloc_context3(codec);
    if (!avCodecContext) {
        LOGI("could not allocate video codec context");
        return;
    }

    packet = av_packet_alloc();
    if (!packet) {
        return;
    }

    // put sample parameters
    avCodecContext->bit_rate = 400000;
    avCodecContext->width = 352;
    avCodecContext->height = 288;
    avCodecContext->time_base = (AVRational) {1, 25};
    avCodecContext->framerate = (AVRational) {25, 1};

    /** emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */

    avCodecContext->gop_size = 10;
    avCodecContext->max_b_frames = 1;
    avCodecContext->pix_fmt = AV_PIX_FMT_YUV420P;

    if (codec->id == AV_CODEC_ID_H264) {
        av_opt_set(avCodecContext->priv_data, "preset", "slow", 0);
    }

    // open it
    ret = avcodec_open2(avCodecContext, codec, nullptr);
    if (ret < 0) {
        LOGI("could not open codec %s", av_err2str(ret));
        return;
    }

    file = fopen(outFileName, "wb");
    if (!file) {
        LOGI("could not open file");
        return;
    }

    frame = av_frame_alloc();
    if (!frame) {
        LOGI("could not allocate video frame");
        return;
    }

    frame->format = avCodecContext->pix_fmt;
    frame->width = avCodecContext->width;
    frame->height = avCodecContext->height;

    ret = av_frame_get_buffer(frame, 32);
    if (ret < 0) {
        LOGI("could not allocate the video frame data");
        return;
    }

    // encode 1 second of video
    for (int i = 0; i < 25; ++i) {
        fflush(stdout);

        // make sure the frame data is writable
        ret = av_frame_make_writable(frame);
        if (ret < 0) {
            return;
        }

        for (int y = 0; y < avCodecContext->height; ++y) {
            for (int x = 0; x < avCodecContext->width; ++x) {
                frame->data[0][y * frame->linesize[0] + x] = (uint8_t) (x + y + i * 3);
            }
        }

        // cb and cr
        for (int y = 0; y < avCodecContext->height / 2; ++y) {
            for (int x = 0; x < avCodecContext->width / 2; ++x) {
                frame->data[1][y * frame->linesize[1] + x] = (uint8_t) (128 + y + i * 2);
                frame->data[2][y * frame->linesize[2] + x] = (uint8_t) (64 + x + i * 5);
            }
        }

        frame->pts = i;
        //encode the image
        encodeVideo(avCodecContext, frame, packet, file);
    }

    //flush the encoder
    encodeVideo(avCodecContext, nullptr, packet, file);

    //add sequence end code to have a real MPEG file
    fwrite(endcode, 1, sizeof(endcode), file);
    fclose(file);

    avcodec_free_context(&avCodecContext);
    av_frame_free(&frame);
    av_packet_free(&packet);

    env->ReleaseStringUTFChars(outFileName_, outFileName);
}



void encodeVideo(AVCodecContext *pContext, AVFrame *pFrame, AVPacket *pPacket, FILE *pFILE) {
    int ret;
    if (pFrame) {
        LOGI("send frame %3"
                     PRId64
                     "\n", pFrame->pts);
    }

    ret = avcodec_send_frame(pContext, pFrame);
    if (ret < 0) {
        LOGI("Error sending a frame for encoding");
        return;
    }

    while (ret >= 0) {
        ret = avcodec_receive_packet(pContext, pPacket);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return;
        } else if (ret < 0) {
            LOGI("Error during encoding\n");
            return;
        }

        LOGI("Write packet %3"
                     PRId64
                     " (size=%5d)\n", pPacket->pts, pPacket->size);
        fwrite(pPacket->data, 1, (size_t) pPacket->size, pFILE);
        av_packet_unref(pPacket);
    }
}

#endif