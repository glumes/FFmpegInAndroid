//
// Created by glumes on 2019/3/14.
//

#include "FFmpegCodec.h"

void FFmpegCodec::encode_video(const char *filename, const char *codec_name) {

    const AVCodec *codec;

    AVCodecContext *codecContext = nullptr;
    int i, ret, x, y, got_output;
    FILE *f;
    AVFrame *frame;
    AVPacket packet;

    u_int8_t encode[] = {0, 0, 1, 0xb7};

    avcodec_register_all();

    codec = avcodec_find_encoder_by_name(codec_name);

    if (!codec) {
        LogClient::LogE("codec not found\n");
        return;
    }

    codecContext = avcodec_alloc_context3(codec);

    if (!codecContext) {
        LogClient::LogE("could not allocate video codec context\n");
        return;
    }
    /* put sample parameters */
    codecContext->bit_rate = 40000;
    /* resolution must be a multiple of two */
    codecContext->width = 352;
    codecContext->height = 288;
    /* frames per second */
    codecContext->time_base = (AVRational) {1, 25};
    codecContext->framerate = (AVRational) {25, 1};

    /* emit one intra frame every ten frames
     * check frame pict_type before passing frame
     * to encoder, if frame->pict_type is AV_PICTURE_TYPE_I
     * then gop_size is ignored and the output of encoder
     * will always be I frame irrespective to gop_size
     */
    codecContext->gop_size = 10;
    codecContext->max_b_frames = 1;
    codecContext->pix_fmt = AV_PIX_FMT_YUV420P;

    if (codec->id == AV_CODEC_ID_H264) {
        av_opt_set(codecContext->priv_data, "preset", "slow", 0);
    }

    /* open codec */
    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        LogClient::LogE("could not open codec\n");
        return;
    }

    f = fopen(filename, "wb");
    if (!f) {
        LogClient::LogE("could not open file");
        return;
    }

    frame = av_frame_alloc();
    if (!frame) {
        LogClient::LogE("could not allocate video frame");
        return;
    }

    frame->format = codecContext->pix_fmt;
    frame->width = codecContext->width;
    frame->height = codecContext->height;

    ret = av_frame_get_buffer(frame, 32);
    if (ret < 0) {
        LogClient::LogE("could not allocate the video frame data\n");
        return;
    }

    /* encode 1 second of video */
    for (i = 0; i < 25; ++i) {
        av_init_packet(&packet);
        packet.data = nullptr;
        packet.size = 0;

//        fflush(stdout);

        /* make sure the frame data is writable */
        ret = av_frame_make_writable(frame);
        if (ret < 0) {
            LogClient::LogE("frame can not write");
            return;
        }

        /* prepare a dummy image */
        /* Y */
        for (y = 0; y < codecContext->height; y++) {
            for (x = 0; x < codecContext->width; ++x) {
                frame->data[0][y * frame->linesize[0] + x] = x + y + i * 3;
            }
        }

        /* Cb and Cr */
        for (y = 0; y < codecContext->height / 2; ++y) {
            for (int x = 0; x < codecContext->width / 2; ++x) {
                frame->data[1][y * frame->linesize[1] + x] = 128 + y + i * 2;
                frame->data[2][y * frame->linesize[2] + x] = 64 + x + i * 5;
            }
        }

        frame->pts = i;
        ret = avcodec_encode_video2(codecContext, &packet, frame, &got_output);
        if (ret < 0) {
            LogClient::LogE("error encoding frame\n");
            return;
        }

        if (got_output) {
            LOGD("write frame %3d (size = %5d)\n", i, packet.size);
            fwrite(packet.data, 1, packet.size, f);
            av_packet_unref(&packet);
        }
    }

    for (got_output = 1; got_output; i++) {

        ret = avcodec_encode_video2(codecContext, &packet, nullptr, &got_output);

        if (ret < 0) {
            LogClient::LogD("error encodeing frame");
        }

        if (got_output) {
            LOGD("write frame %3d (size = %5d)\n", i, packet.size);
            fwrite(packet.data, 1, packet.size, f);
            av_packet_unref(&packet);
        }
    }

    fwrite(encode, 1, sizeof(encode), f);
    fclose(f);

    avcodec_free_context(&codecContext);
    av_frame_free(&frame);


}

void FFmpegCodec::decode_video_to_yuv(const char *input_path, const char *output_path) {

    AVFormatContext *formatContext;

    int i, videoindex;

    AVCodecContext *codecContext;

    AVCodec *codec;

    AVFrame *frame, *frameYUV;

    uint8_t *out_buffer;

    AVPacket *packet;

    int y_size;
    int ret, got_picture;

    struct SwsContext *img_convert_ctx;

    FILE *file_yuv;

    int frame_cnt;

    clock_t time_start, time_finish;

    double time_duration = 0.0;

    char input_str[500] = {0};
    char output_str[500] = {0};
    char info[1000] = {0};

//    sprintf(input_str,"%s",)

    av_register_all();
    avformat_network_init();

    formatContext = avformat_alloc_context();

    if (avformat_open_input(&formatContext, input_path, nullptr, nullptr) != 0) {
        LogClient::LogD("could not open input stream");
        return;
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        LogClient::LogD("could not find stream information\n");
        return;
    }

    videoindex = -1;

    for (i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoindex = i;
            break;
        }
    }

    if (videoindex == -1) {
        LogClient::LogD("could find a video stream");
        return;;
    }

    codecContext = formatContext->streams[videoindex]->codec;

    codec = avcodec_find_decoder(codecContext->codec_id);

    if (codec == nullptr) {
        LogClient::LogD("could find codec\n");
        return;
    }

    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        LogClient::LogD("could not open codec");
        return;
    }

    frame = av_frame_alloc();
    frameYUV = av_frame_alloc();

    out_buffer = static_cast<uint8_t *>(av_malloc(
            av_image_get_buffer_size(AV_PIX_FMT_YUV420P, codecContext->width, codecContext->height,
                                     1)));

    av_image_fill_arrays(frameYUV->data, frameYUV->linesize, out_buffer, AV_PIX_FMT_YUV420P,
                         codecContext->width, codecContext->height, 1);

    packet = static_cast<AVPacket *>(av_malloc(sizeof(AVPacket)));

    img_convert_ctx = sws_getContext(
            codecContext->width,
            codecContext->height,
            codecContext->pix_fmt,
            codecContext->width,
            codecContext->height,
            AV_PIX_FMT_YUV420P,
            SWS_BICUBIC,
            nullptr, nullptr, nullptr);



}
