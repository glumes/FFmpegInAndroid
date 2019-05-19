//
// Created by glumes on 2019/3/14.
//

#include "FFmpegCodec.h"

#include <libavutil/mathematics.h>

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


int FFmpegCodec::parseFile(const char *input_path, FileContext &fileContext) {

    int ret;

    fileContext.s_pFormatCtx = avformat_alloc_context();

    ret = avformat_open_input(&fileContext.s_pFormatCtx, input_path, nullptr, nullptr);

    if (ret < 0) {
        LogClient::LogE("could not open input stream failed and ret is %d");
        return RET_FAIL;
    }

    if (avformat_find_stream_info(fileContext.s_pFormatCtx, nullptr) < 0) {
        LogClient::LogE("avformat_find_stream_info failed\n");
    }

    fileContext.s_VideoIndex = fileContext.s_AudioIndex = -1;

    for (int i = 0; i < fileContext.s_pFormatCtx->nb_streams; i++) {
        if (fileContext.s_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            fileContext.s_VideoIndex = i;
            break;
        }
    }

    if (fileContext.s_VideoIndex == -1) {
        LogClient::LogE("could find a video stream\n");
    }

    for (int i = 0; i < fileContext.s_pFormatCtx->nb_streams; ++i) {
        if (fileContext.s_pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO) {
            fileContext.s_AudioIndex = i;
            break;
        }
    }

    if (fileContext.s_AudioIndex == -1) {
        LogClient::LogE("could find a audio stream\n");
    }

    fileContext.s_pCodecCtx = fileContext.s_pFormatCtx->streams[fileContext.s_VideoIndex]->codec;

    fileContext.s_pVideoCodec = avcodec_find_decoder(fileContext.s_pCodecCtx->codec_id);

    if (fileContext.s_pVideoCodec == nullptr) {
        LogClient::LogE("could find video codec\n");
    }

    if (avcodec_open2(fileContext.s_pCodecCtx, fileContext.s_pVideoCodec, nullptr) < 0) {
        LogClient::LogE("could open video codec\n");
    }

    return RET_OK;
}

/**
 * 解码后的 yuv 文件通过 ffplay -f rawvideo -video_size 640x360 yuv_file_path 来播放，注意分辨率大小
 * @param input_path
 * @param output_path
 */
void FFmpegCodec::codec_mp4_to_yuv(const char *input_path, const char *output_path) {


    av_register_all();

    FileContext fileContext;

    if (parseFile(input_path, fileContext) != RET_OK) {
        return;
    }

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
    char info[1000] = {0};

    frame = av_frame_alloc();
    frameYUV = av_frame_alloc();

    out_buffer = static_cast<uint8_t *>(av_malloc(
            av_image_get_buffer_size(AV_PIX_FMT_YUV420P, fileContext.s_pCodecCtx->width,
                                     fileContext.s_pCodecCtx->height,
                                     1)));

    av_image_fill_arrays(frameYUV->data, frameYUV->linesize, out_buffer, AV_PIX_FMT_YUV420P,
                         fileContext.s_pCodecCtx->width, fileContext.s_pCodecCtx->height, 1);

    packet = static_cast<AVPacket *>(av_malloc(sizeof(AVPacket)));

    img_convert_ctx = sws_getContext(
            fileContext.s_pCodecCtx->width,
            fileContext.s_pCodecCtx->height,
            fileContext.s_pCodecCtx->pix_fmt,
            fileContext.s_pCodecCtx->width,
            fileContext.s_pCodecCtx->height,
            AV_PIX_FMT_YUV420P,
            SWS_BICUBIC,
            nullptr, nullptr, nullptr);


    sprintf(info, "[Input     ]%s\n", input_path);
    sprintf(info, "%s[Output    ]%s\n", info, output_path);
    sprintf(info, "%s[Format    ]%s\n", info, fileContext.s_pFormatCtx->iformat->name);
    sprintf(info, "%s[Codec     ]%s\n", info, fileContext.s_pCodecCtx->codec->name);
    sprintf(info, "%s[Resolution]%dx%d\n", info, fileContext.s_pCodecCtx->width,
            fileContext.s_pCodecCtx->height);

    file_yuv = fopen(output_path, "wb+");

    if (file_yuv == nullptr) {
        LogClient::LogD("Cannot open output file \n");
        return;
    }

    frame_cnt = 0;

    time_start = clock();

    while (av_read_frame(fileContext.s_pFormatCtx, packet) >= 0) {

        LogClient::LogD("av_read_frame > 0");

        if (packet->stream_index == fileContext.s_VideoIndex) {
            ret = avcodec_decode_video2(fileContext.s_pCodecCtx, frame, &got_picture, packet);
            if (ret < 0) {
                LogClient::LogD("decode error");
                return;
            }
            if (got_picture) {
                sws_scale(img_convert_ctx, frame->data, frame->linesize, 0,
                          fileContext.s_pCodecCtx->height,
                          frameYUV->data, frameYUV->linesize);
                y_size = fileContext.s_pCodecCtx->width * fileContext.s_pCodecCtx->height;
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
            } else {
                LogClient::LogE("not get picture");
            }
        }
        av_free_packet(packet);
    }

    while (1) {
        ret = avcodec_decode_video2(fileContext.s_pCodecCtx, frame, &got_picture, packet);
        if (ret < 0) {
            break;
        }
        if (!got_picture) {
            break;
        }
        sws_scale(img_convert_ctx, frame->data, frame->linesize, 0, fileContext.s_pCodecCtx->height,
                  frameYUV->data, frameYUV->linesize);

        int y_size = fileContext.s_pCodecCtx->width * fileContext.s_pCodecCtx->height;
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
    avcodec_close(fileContext.s_pCodecCtx);
    avformat_close_input(&fileContext.s_pFormatCtx);

    LogClient::LogD(info);
}


void FFmpegCodec::codec_mp4_to_h264(const char *input_path, const char *output_path) {

    AVPacket *packet = nullptr;

    FILE *file_h264 = nullptr;

    file_h264 = fopen(output_path, "wb+");

    if (file_h264 == nullptr) {
        LogClient::LogD("could not open file");
        return;
    }
    av_register_all();

    FileContext fileContext;

    if (parseFile(input_path, fileContext) != RET_OK) {
        return;
    }

    packet = static_cast<AVPacket *>(av_malloc(sizeof(AVPacket)));

    while (av_read_frame(fileContext.s_pFormatCtx, packet) >= 0) {
        if (packet->stream_index == fileContext.s_VideoIndex) {
            fwrite(packet->data, 1, packet->size, file_h264);
        }
        av_free_packet(packet);
    }

    fclose(file_h264);
    avcodec_close(fileContext.s_pCodecCtx);
    avformat_close_input(&fileContext.s_pFormatCtx);

}


void FFmpegCodec::codec_yuv_to_mp4(const char *input_path, const char *output_path) {

    codec_yuv_to_h264(input_path, output_path);

}

/**
 * 编码后的 h264 文件通过 ffplay -stats -f h264 h264_file_path 来播放
 * @param input_path
 * @param output_path
 */
void FFmpegCodec::codec_yuv_to_h264(const char *input_path, const char *output_path) {

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

    FileContext fileContext;

//    if (parseFile(input_path,fileContext) != RET_OK){
//        return;
//    }


    // 输出文件的信息配置 AVFormatContext
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

    // 编码成 H264 需要的配置信息
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

//    for (int i = 0; i < framenum; ++i) {


    int i = 0;
    while (fread(picture_buf, 1, y_size * 3 / 2, in_file) > 0) {


//            if (fread(picture_buf, 1, y_size * 3 / 2, in_file) <= 0) {
//                LogClient::LogD("failed to read raw data\n");
//            } else if (feof(in_file)) {
//                break;
//            }

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
        i++;
    }
//    }

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

/**
 * 将 h264 数据编码成 mp4 文件
 * @param input_path
 * @param output_path
 */
void FFmpegCodec::codec_h264_to_mp4(const char *input_path, const char *output_path) {

    AVFormatContext *ifmt_ctx = nullptr, *ofmt_ctx = nullptr;
    AVOutputFormat *ofmt;

    AVPacket packet;

    int ret, i;
    int videoindex_v = 0, videoindex_out = 0;

    int frame_index = 0;
    int64_t cur_pts_v = 0, cur_pts_a = 0;

    FILE *in_file = fopen(input_path, "rb");

    if (!in_file) {
        LOGD("file %s not exist\n", input_path);
        return;
    }

    av_register_all();

    if ((ret = avformat_open_input(&ifmt_ctx, input_path, nullptr, nullptr)) < 0) {
        LogClient::LogD("could not open input stream");
        return;
    }

    if ((ret = avformat_find_stream_info(ifmt_ctx, nullptr)) < 0) {
        LogClient::LogD("could not find stream information");
        return;
    }

    avformat_alloc_output_context2(&ofmt_ctx, nullptr, nullptr, output_path);
    if (!ofmt_ctx) {
        LogClient::LogD("could not create output context");
        return;
    }

    ofmt = ofmt_ctx->oformat;

    LOGD("nb_streams is %d", ifmt_ctx->nb_streams);

    for (int i = 0; i < ifmt_ctx->nb_streams; ++i) {
        AVStream *in_stream = ifmt_ctx->streams[i];
        AVStream *out_stream = avformat_new_stream(ofmt_ctx, in_stream->codec->codec);

        videoindex_v = i;

        if (!out_stream) {
            return;
        }

        videoindex_out = out_stream->index;

        if (avcodec_copy_context(out_stream->codec, in_stream->codec) < 0) {
            LogClient::LogD("copy context failed");
            return;
        }

        out_stream->codec->codec_tag = 0;
        if (ofmt_ctx->oformat->flags & AVFMT_GLOBALHEADER) {
            out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
        }

    }

    if (!(ofmt->flags & AVFMT_NOFILE)) {
        if (avio_open(&ofmt_ctx->pb, output_path, AVIO_FLAG_WRITE) < 0) {
            LOGE("could not open file %s", output_path);
        }
    }

    if (avformat_write_header(ofmt_ctx, nullptr) < 0) {
        LOGE("write header failed");
    }

#if USE_H264BSF
    AVBitStreamFilterContext* h264bsfc =  av_bitstream_filter_init("h264_mp4toannexb");
#endif
#if USE_AACBSF
    AVBitStreamFilterContext* aacbsfc =  av_bitstream_filter_init("aac_adtstoasc");
#endif
    while (1) {
        int stream_index = videoindex_out;
        AVStream *in_stream, *out_stream;

        if (av_read_frame(ifmt_ctx, &packet) >= 0) {
            do {
                in_stream = ifmt_ctx->streams[packet.stream_index];
                out_stream = ofmt_ctx->streams[stream_index];

                if (packet.stream_index == videoindex_v) {
                    if (packet.pts == AV_NOPTS_VALUE) {
                        AVRational time_base1 = in_stream->time_base;
                        int64_t calc_duration =
                                (double) AV_TIME_BASE / av_q2d(in_stream->r_frame_rate);

                        packet.pts = (double) (frame_index * calc_duration) /
                                     (double) (av_q2d(time_base1) * AV_TIME_BASE);

                        packet.dts = packet.pts;

                        packet.duration = (double) calc_duration /
                                          (double) (av_q2d(time_base1) * AV_TIME_BASE);
                        frame_index++;
                    }
                    cur_pts_v = packet.pts;
                    break;
                }
            } while (av_read_frame(ifmt_ctx, &packet) >= 0);
        } else {
            break;
        }


#if USE_H264BSF
        av_bitstream_filter_filter(h264bsfc, in_stream->codec, NULL, &pkt.data, &pkt.size, pkt.data, pkt.size, 0);
#endif
#if USE_AACBSF
        av_bitstream_filter_filter(aacbsfc, out_stream->codec, NULL, &pkt.data, &pkt.size, pkt.data, pkt.size, 0);
#endif

        packet.pts = av_rescale_q_rnd(packet.pts, in_stream->time_base, out_stream->time_base,
                                      AV_ROUND_PASS_MINMAX);
        packet.dts = av_rescale_q_rnd(packet.dts, in_stream->time_base, out_stream->time_base,
                                      AV_ROUND_PASS_MINMAX);
        packet.duration = av_rescale_q(packet.duration, in_stream->time_base,
                                       out_stream->time_base);
        packet.pos = -1;
        packet.stream_index = stream_index;

        if (av_interleaved_write_frame(ofmt_ctx, &packet) < 0) {
            return;
        }
        av_free_packet(&packet);
    }
    av_write_trailer(ofmt_ctx);

#if USE_H264BSF
    av_bitstream_filter_close(h264bsfc);
#endif
#if USE_AACBSF
    av_bitstream_filter_close(aacbsfc);
#endif

    avformat_close_input(&ifmt_ctx);
    if (ofmt_ctx && !(ofmt->flags & AVFMT_NOFILE)) {
        avio_close(ofmt_ctx->pb);
    }
    avformat_free_context(ofmt_ctx);

    if (ret < 0) {
        LOGD("convert %s to %s failed", input_path, output_path);
    } else {
        LOGD("convert %s to %s success", input_path, output_path);
    }
}



