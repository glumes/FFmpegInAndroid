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
    LogClient::LogD("output video path is %s and audio path is %s", output_path_video,output_path_audio);

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
            int ret = av_bitstream_filter_filter(h264bsfc, formatContext->streams[videoindex]->codec, nullptr,
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

    AVFormatContext *iformatContext = nullptr, *oformatContext_a = nullptr, *oformatContext_v = nullptr;

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

    avformat_alloc_output_context2(&oformatContext_v, nullptr, nullptr, output_path_video);

    if (!oformatContext_v) {
        LogClient::LogD("could not create output context\n");
        return;
    }

    outputFormat_v = oformatContext_v->oformat;

    avformat_alloc_output_context2(&oformatContext_a, nullptr, nullptr, output_path_audio);

    if (!oformatContext_a) {
        LogClient::LogD("could not create output context\n");
        return;
    }

    outputFormat_a = oformatContext_a->oformat;

    for (i = 0; i < iformatContext->nb_streams; i++) {
        AVFormatContext *oformatContext;
        AVStream *in_stream = iformatContext->streams[i];
        AVStream *out_stream = nullptr;
        if (iformatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO){
            videoindex = i;
            out_stream = avformat_new_stream(oformatContext_v,in_stream->codec->codec);
            oformatContext = oformatContext_v;
        } else if (iformatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_AUDIO){
            audioindex = i;
            out_stream = avformat_new_stream(oformatContext_a,in_stream->codec->codec);
            oformatContext = oformatContext_a;
        } else{
            break;
        }
        if (!out_stream){
            LogClient::LogD("failed allocating output stream");
            return;
        }

        if (avcodec_copy_context(out_stream->codec,in_stream->codec) <0){
            LogClient::LogD("avcodec_copy_context failed");
            return;
        }
        out_stream->codec->codec_tag = 0;
        if (oformatContext->oformat->flags & AVFMT_GLOBALHEADER){
            out_stream->codec->flags |= CODEC_FLAG_GLOBAL_HEADER;
        }
    }

    if (!(oformatContext_v->flags & AVFMT_NOFILE)){
        if (avio_open(&oformatContext_v->pb,output_path_video,AVIO_FLAG_WRITE) < 0){
            LogClient::LogD("could not open output file %s",output_path_video);
            return;
        }
    }

    if (!(oformatContext_a->flags & AVFMT_NOFILE)){
        if (avio_open(&oformatContext_a->pb,output_path_audio,AVIO_FLAG_WRITE) < 0){
            LogClient::LogD("could not open output file %s",output_path_audio);
            return;
        }
    }

    if (avformat_write_header(oformatContext_v, nullptr) < 0){
        LogClient::LogD("avformat_write_header failed");
        return;
    }

    if (avformat_write_header(oformatContext_a, nullptr) < 0){
        LogClient::LogD("avformat_write_header failed");
        return;
    }


#if USE_H264BSF
    AVBitStreamFilterContext* h264bsfc =  av_bitstream_filter_init("h264_mp4toannexb");
#endif

    while (1){
        AVFormatContext *ofmt_ctx;
        AVStream *in_stream, *out_stream;

        if (av_read_frame(iformatContext,&packet) < 0){
            LogClient::LogD("read frame failed");
            break;
        }

        in_stream = iformatContext->streams[packet.stream_index];

        if (packet.stream_index == videoindex){
            out_stream = oformatContext_v->streams[0];
            ofmt_ctx = oformatContext_v;
            LogClient::LogD("Write Video Packet. size:%d\\tpts:%lld\\n ",packet.size,packet.pts);
#if USE_H264BSF
            av_bitstream_filter_filter(h264bsfc,in_stream->codec, nullptr,
                    &packet.data,&packet.size,packet.data,packet.size,0);
#endif
        } else if (packet.stream_index == audioindex){
            out_stream = oformatContext_a->streams[0];
            ofmt_ctx = oformatContext_a;
            LogClient::LogD("Write Audio Packet. size:%d\\tpts:%lld\\n ",packet.size,packet.pts);
        } else{
            continue;
        }

        packet.pts = av_rescale_q_rnd(packet.pts,in_stream->time_base,out_stream->time_base, (AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
        packet.dts = av_rescale_q_rnd(packet.dts,in_stream->time_base,out_stream->time_base,(AVRounding)(AV_ROUND_NEAR_INF|AV_ROUND_PASS_MINMAX));
        packet.duration = av_rescale_q(packet.duration,in_stream->time_base,out_stream->time_base);
        packet.pos = -1;
        packet.stream_index= 0;

        if (av_interleaved_write_frame(ofmt_ctx,&packet)<0){
            LogClient::LogD("Error muxing packet\\n");
            break;
        }
        av_free_packet(&packet);
        frame_index++;
    }
#if USE_H264BSF
    av_bitstream_filter_close(h264bsfc);
#endif

    av_write_trailer(oformatContext_a);
    av_write_trailer(oformatContext_v);

    avformat_close_input(&iformatContext);

    if (outputFormat_v && !(outputFormat_v->flags & AVFMT_NOFILE)){
        avio_close(oformatContext_v->pb);
    }
    if (outputFormat_a && !(outputFormat_a->flags & AVFMT_NOFILE)){
        avio_close(oformatContext_a->pb);
    }

    avformat_free_context(oformatContext_a);
    avformat_free_context(oformatContext_v);

    LogClient::LogD("end...");
}
