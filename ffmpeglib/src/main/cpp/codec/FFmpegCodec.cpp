//
// Created by glumes on 2019/3/14.
//

#include "FFmpegCodec.h"


int flush_encoder(AVFormatContext *formatContext, unsigned int stream_index) {
    int ret;
    int got_frame;

    AVPacket packet;
    if (!(formatContext->streams[stream_index]->codec->codec->capabilities & CODEC_CAP_DELAY)) {
        return 0;
    }
    while (1) {
        packet.data = nullptr;
        packet.size = 0;
        av_init_packet(&packet);

        ret = avcodec_encode_video2(formatContext->streams[stream_index]->codec, &packet, nullptr,
                                    &got_frame);

        av_frame_free(nullptr);

        if (ret < 0) {
            break;
        }
        if (!got_frame) {
            ret = 0;
            break;
        }
        LOGD("Flush Encoder:Success to encode 1 frame!\tsize:%5d\n", packet.size);

        ret = av_write_frame(formatContext, &packet);
        if (ret < 0) {
            break;
        }
    }
    return ret;
}

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


/**
 * 解码后的 yuv 文件通过 ffplay -f rawvideo -video_size 640x360 yuv_file_path 来播放，注意分辨率大小
 * @param input_path
 * @param output_path
 */
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
//
//    char input_str[500] = {0};
//    char output_str[500] = {0};

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


    sprintf(info, "[Input     ]%s\n", input_path);
    sprintf(info, "%s[Output    ]%s\n", info, output_path);
    sprintf(info, "%s[Format    ]%s\n", info, formatContext->iformat->name);
    sprintf(info, "%s[Codec     ]%s\n", info, codecContext->codec->name);
    sprintf(info, "%s[Resolution]%dx%d\n", info, codecContext->width, codecContext->height);

    file_yuv = fopen(output_path, "wb+");

    if (file_yuv == nullptr) {
        LogClient::LogD("Cannot open output file \n");
        return;
    }

    frame_cnt = 0;

    time_start = clock();

    while (av_read_frame(formatContext, packet) >= 0) {
        if (packet->stream_index == videoindex) {
            ret = avcodec_decode_video2(codecContext, frame, &got_picture, packet);
            if (ret < 0) {
                LogClient::LogD("decode error");
                return;
            }
            if (got_picture) {
                sws_scale(img_convert_ctx, frame->data, frame->linesize, 0, codecContext->height,
                          frameYUV->data, frameYUV->linesize);
                y_size = codecContext->width * codecContext->height;
                fwrite(frameYUV->data[0], 1, y_size, file_yuv);
                fwrite(frameYUV->data[1], 1, y_size / 4, file_yuv);
                fwrite(frameYUV->data[2], 1, y_size / 4, file_yuv);

                char picType_str[10] = {0};
                switch (frame->pict_type) {
                    case AV_PICTURE_TYPE_I:
                        sprintf(picType_str, "I");
                        break;
                    case AV_PICTURE_TYPE_B:
                        sprintf(picType_str, "B");
                        break;
                    case AV_PICTURE_TYPE_P:
                        sprintf(picType_str, "P");
                    default:
                        sprintf(picType_str, "Other");
                        break;
                }
                LOGD("Frame Index:%5d.Type:%s", frame_cnt, picType_str);
                frame_cnt++;
            }
        }
        av_free_packet(packet);
    }

    while (1) {
        ret = avcodec_decode_video2(codecContext, frame, &got_picture, packet);
        if (ret < 0) {
            break;
        }
        if (!got_picture) {
            break;
        }
        sws_scale(img_convert_ctx, frame->data, frame->linesize, 0, codecContext->height,
                  frameYUV->data, frameYUV->linesize);

        int y_size = codecContext->width * codecContext->height;
        fwrite(frameYUV->data[0], 1, y_size, file_yuv);
        fwrite(frameYUV->data[1], 1, y_size / 4, file_yuv);
        fwrite(frameYUV->data[2], 1, y_size / 4, file_yuv);

        char picType_str[10] = {0};
        switch (frame->pict_type) {
            case AV_PICTURE_TYPE_I:
                sprintf(picType_str, "I");
                break;
            case AV_PICTURE_TYPE_B:
                sprintf(picType_str, "B");
                break;
            case AV_PICTURE_TYPE_P:
                sprintf(picType_str, "P");
            default:
                sprintf(picType_str, "Other");
                break;
        }
        LOGD("Frame Index:%5d.Type:%s", frame_cnt, picType_str);
        frame_cnt++;
    }

    time_finish = clock();
    time_duration = time_finish - time_start;

    sprintf(info, "%s[Time      ]%fms\n", info, time_duration);
    sprintf(info, "%s[Count     ]%d\n", info, frame_cnt);

    sws_freeContext(img_convert_ctx);

    fclose(file_yuv);

    av_frame_free(&frameYUV);
    av_frame_free(&frame);
    avcodec_close(codecContext);
    avformat_close_input(&formatContext);


    LogClient::LogD(info);
}


void FFmpegCodec::decode_video_to_h264(const char *input_path, const char *output_path) {
    AVFormatContext *formatContext = nullptr;
    int videoIndex;

    AVCodecContext *codecContext = nullptr;
    AVCodec *codec = nullptr;
    AVPacket *packet = nullptr;
    FILE *file_h264 = nullptr;

    file_h264 = fopen(output_path, "wb+");

    if (file_h264 == nullptr) {
        LogClient::LogD("could not open file");
        return;
    }
    av_register_all();

    formatContext = avformat_alloc_context();
    if (avformat_open_input(&formatContext, input_path, nullptr, nullptr)) {
        LogClient::LogD("could not open input stream");
        return;
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        LogClient::LogD("could not find stream information");
        return;
    }

    videoIndex = -1;
    for (int i = 0; i < formatContext->nb_streams; ++i) {
        if (formatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoIndex = i;
            break;
        }
    }

    if (videoIndex == -1) {
        LogClient::LogD("could not find video stream");
        return;
    }

    codecContext = formatContext->streams[videoIndex]->codec;


    codec = avcodec_find_decoder(codecContext->codec_id);

    if (codec == nullptr) {
        LogClient::LogD("could not find codec");
        return;
    }


    packet = static_cast<AVPacket *>(av_malloc(sizeof(AVPacket)));

    while (av_read_frame(formatContext, packet) >= 0) {
        if (packet->stream_index == videoIndex) {
            fwrite(packet->data, 1, packet->size, file_h264);
        }
        av_free_packet(packet);
    }

    fclose(file_h264);
    avcodec_close(codecContext);
    avformat_close_input(&formatContext);

}


void FFmpegCodec::encode_yuv_to_video(const char *input_path, const char *output_path) {

}

/**
 * 编码后的 h264 文件通过 ffplay -stats -f h264 h264_file_path 来播放
 * @param input_path
 * @param output_path
 */
void FFmpegCodec::encode_yuv_to_h264(const char *input_path, const char *output_path) {

    AVFormatContext *formatContext;

    AVOutputFormat *outputFormat;

    AVStream *video_stream;

    AVCodecContext *codecContext;

    AVCodec *codec;

    AVPacket packet;

    uint8_t *picture_buf;

    AVFrame *frame;

    int picture_size;

    int y_size;

    int framecnt = 0;

    FILE *in_file = fopen(input_path, "rb");

    if (!in_file) {
        LOGD("file not exist\n");
        return;
    }

    int in_w = 480, in_h = 272;

    int framenum = 100;

    av_register_all();

    formatContext = avformat_alloc_context();

    outputFormat = av_guess_format(nullptr, output_path, nullptr);

    formatContext->oformat = outputFormat;

    // method2
//    avformat_alloc_output_context2(&formatContext, nullptr, nullptr, output_path);
//    outputFormat = formatContext->oformat;

    if (avio_open(&formatContext->pb, output_path, AVIO_FLAG_READ_WRITE) < 0) {
        LogClient::LogD("failed to open output file\n");
        return;
    }

    video_stream = avformat_new_stream(formatContext, 0);

    if (video_stream == nullptr) {
        LogClient::LogD("new stream failed\n");
        return;
    }

    codecContext = video_stream->codec;
    codecContext->codec_id = outputFormat->video_codec;
    codecContext->codec_type = AVMEDIA_TYPE_VIDEO;
    codecContext->pix_fmt = AV_PIX_FMT_YUV420P;
    codecContext->width = in_w;
    codecContext->height = in_h;

    codecContext->bit_rate = 400000;
    codecContext->gop_size = 250;

    codecContext->time_base.num = 1;
    codecContext->time_base.den = 25;

    codecContext->qmin = 10;
    codecContext->qmax = 51;

    codecContext->max_b_frames = 3;

    AVDictionary *param = 0;

    if (codecContext->codec_id == AV_CODEC_ID_H264) {
        av_dict_set(&param, "preset", "slow", 0);
        av_dict_set(&param, "tune", "zerolatency", 0);
    }

    if (codecContext->codec_id == AV_CODEC_ID_H265) {
        av_dict_set(&param, "preset", "ultrafast", 0);
        av_dict_set(&param, "tune", "zero-latency", 0);
    }

    av_dump_format(formatContext, 0, output_path, 1);

    codec = avcodec_find_encoder(codecContext->codec_id);

    if (!codec) {
        LogClient::LogD("can not find encoder\n");
        return;
    }

    if (avcodec_open2(codecContext, codec, &param) < 0) {
        LogClient::LogD("failed to open encoder\n");
        return;
    }

    frame = av_frame_alloc();
    picture_size = avpicture_get_size(codecContext->pix_fmt, codecContext->width,
                                      codecContext->height);
    picture_buf = static_cast<uint8_t *>(av_malloc(picture_size));

    avpicture_fill(reinterpret_cast<AVPicture *>(frame), picture_buf, codecContext->pix_fmt,
                   codecContext->width,
                   codecContext->height);

    avformat_write_header(formatContext, nullptr);

    av_new_packet(&packet, picture_size);

    y_size = codecContext->width * codecContext->height;

    for (int i = 0; i < framenum; ++i) {
        if (fread(picture_buf, 1, y_size * 3 / 2, in_file) <= 0) {
            LogClient::LogD("failed to read raw data\n");
        } else if (feof(in_file)) {
            break;
        }
        frame->data[0] = picture_buf;
        frame->data[1] = picture_buf + y_size;
        frame->data[2] = picture_buf + y_size * 5 / 4;
        frame->pts = i * (video_stream->time_base.den) / ((video_stream->time_base.num) * 25);

        int got_picture = 0;

        int ret = avcodec_encode_video2(codecContext, &packet, frame, &got_picture);
        if (ret < 0) {
            LogClient::LogD("failed to encode\n");
            return;
        }

        if (got_picture == 1) {
            LOGD("success to encode frame : %5d\tsize:%5d\n", framecnt, packet.size);
            framecnt++;
            packet.stream_index = video_stream->index;
            ret = av_write_frame(formatContext, &packet);
            av_free_packet(&packet);
        }
    }

    int ret = flush_encoder(formatContext, 0);

    if (ret < 0) {
        LogClient::LogD("flush encoder failed\n");
        return;
    }

    av_write_trailer(formatContext);

    if (video_stream) {
        avcodec_close(video_stream->codec);
        av_free(frame);
        av_free(picture_buf);
    }

    avio_close(formatContext->pb);
    avformat_free_context(formatContext);

    fclose(in_file);

    LogClient::LogD("encode yuv to h264 success\n");
}

