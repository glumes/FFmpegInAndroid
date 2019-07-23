//
// Created by glumes on 2019/3/17.
//

#include "FFmpegMuxer.h"

#define USE_H264BSF 1

/**
 *
 * @param input_path 输入的文件
 * @param output_path_video 输出的视频文件
 * @param output_path_audio 输出的音频文件
 */
void FFmpegMuxer::muxer_simple(const char *input_path, const char *output_path_video,
                               const char *output_path_audio) {


    av_register_all();

    avformat_network_init();

    avcodec_register_all();

    AVFormatContext *formatContext = nullptr;

    AVPacket packet;

    int ret, i;
    int videoindex = -1, audioindex = -1;


    LogClient::LogD("start muxer simple and input file path is %s", input_path);

    ret = avformat_open_input(&formatContext, input_path, 0, 0);

    if (ret < 0) {
        LogClient::LogD("could not open input file");
        return;
    }

    ret = avformat_find_stream_info(formatContext, 0);
    if ( ret < 0) {
        LogClient::LogD("failed to retrieve input stream information");
        return;
    }

    LogClient::LogD("duration is %lld",formatContext->duration);

    for (i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoindex = i;
        } else if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioindex = i;
        }
    }

    FILE *fp_audio = fopen(output_path_audio, "wb+");
    FILE *fp_video = fopen(output_path_video, "wb+");

#if USE_H264BSF
    AVBitStreamFilterContext *h264bsfc = av_bitstream_filter_init("h264_mp4toannexb");
#endif

    while (av_read_frame(formatContext, &packet) >= 0) {

        if (packet.stream_index == videoindex) {
#if USE_H264BSF
            av_bitstream_filter_filter(h264bsfc, formatContext->streams[videoindex]->codec, nullptr,
                                       &packet.data, &packet.size, packet.data, packet.size, 0);
#endif
            LOGD("write video packet.size:%d\tpts:%lld\n", packet.size, packet.pts);
            fwrite(packet.data, 1, packet.size, fp_video);
        } else if (packet.stream_index == audioindex) {
            LOGD("write audio packet.size:%d\tpts:%lld\n", packet.size, packet.pts);
            fwrite(packet.data, 1, packet.size, fp_audio);
        }
        av_free_packet(&packet);
    }
#if USE_H264BSF
    av_bitstream_filter_close(h264bsfc);
#endif
    fclose(fp_video);
    fclose(fp_audio);

    avformat_close_input(&formatContext);

    LogClient::LogD("end...");
}

void FFmpegMuxer::muxer_standard(const char *input_path, const char *output_path_video,
                                 const char *output_path_audio) {

    AVOutputFormat *outputFormat_a = nullptr, *outputFormat_v = nullptr;

    AVFormatContext *iformatContext = nullptr, *iformatContext_a = nullptr, *iformatContext_v = nullptr;

    AVPacket packet;

    int ret, i;

    int videoindex = -1, audioindex = -1;

    int frame_index = 0;

    av_register_all();

    if ((ret = avformat_open_input(&iformatContext, input_path, 0, 0)) < 0) {
        LogClient::LogD("could not open input file");
        return;
    }

    if ((ret = avformat_find_stream_info(iformatContext, 0)) < 0) {
        LogClient::LogD("failed to retrieve input stream information");
        return;
    }

    avformat_alloc_output_context2(&iformatContext_v, nullptr, nullptr, output_path_video);

    if (!iformatContext_v) {
        LogClient::LogD("could not create output context\n");
        return;
    }

    outputFormat_v = iformatContext_v->oformat;

    avformat_alloc_output_context2(&iformatContext_a, nullptr, nullptr, output_path_audio);

    if (!iformatContext_a) {
        LogClient::LogD("could not create output context\n");
        return;
    }

    outputFormat_a = iformatContext_a->oformat;

//    for (i = 0; i < iformatContext->nb_streams; i++) {
//        AVFormatContext *
//    }
}
