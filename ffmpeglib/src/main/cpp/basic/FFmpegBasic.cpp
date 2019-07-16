//
// Created by glumes on 2019/3/12.
//

#include "FFmpegBasic.h"

FFmpegBasic::FFmpegBasic() {

}

FFmpegBasic::~FFmpegBasic() {

}

char *FFmpegBasic::getAvFormatInfo() {
    char *info = static_cast<char *>(malloc(BUFFER_SIZE));

    memset(info, 0, BUFFER_SIZE);

    av_register_all();

    AVInputFormat *if_temp = av_iformat_next(NULL);
    AVOutputFormat *of_temp = av_oformat_next(NULL);

    while (if_temp != NULL) {
        sprintf(info, "%s[In ] %10s\n", info, if_temp->name);
        if_temp = if_temp->next;
    }

    while (of_temp != NULL) {
        sprintf(info, "%s[Out] %10s\n", info, of_temp->name);
        of_temp = of_temp->next;
    }
    return info;
}

char *FFmpegBasic::getAvCodecInfo() {
    char *info = static_cast<char *>(malloc(BUFFER_SIZE));
    memset(info, 0, BUFFER_SIZE);

    av_register_all();

    AVCodec *c_temp = av_codec_next(NULL);
    while (c_temp != NULL) {
        if (c_temp->decode != NULL) {
            sprintf(info, "%s[Dec]", info);
        } else {
            sprintf(info, "%s[Enc]", info);
        }

        switch (c_temp->type) {
            case AVMEDIA_TYPE_VIDEO:
                sprintf(info, "%s[Video]", info);
                break;
            case AVMEDIA_TYPE_AUDIO:
                sprintf(info, "%s[Audio]", info);
                break;
            default:
                sprintf(info, "%s[Other]", info);
                break;
        }
        sprintf(info, "%s %10s\n", info, c_temp->name);
        c_temp = c_temp->next;
    }

    return info;
}

char *FFmpegBasic::getAvFilterInfo() {
    char *info = static_cast<char *>(malloc(BUFFER_SIZE));
    memset(info, 0, BUFFER_SIZE);

    av_register_all();
    avfilter_register_all();

    auto *f_temp = const_cast<AVFilter *>(avfilter_next(NULL));
    while (f_temp != NULL) {
        sprintf(info, "%s[%15s]\n", info, f_temp->name);
        f_temp = f_temp->next;
    }

    return info;
}

void FFmpegBasic::printVideoInfo(const char *file_path) {

    //
    av_register_all();

    avformat_network_init();

    avcodec_register_all();

    AVFormatContext *avFormatContext = nullptr;

    int ret = avformat_open_input(&avFormatContext, file_path, 0, 0);

    if (ret != 0) {
        LogClient::LogD("open failed\n");
        return;
    }

    ret = avformat_find_stream_info(avFormatContext, 0);

    if (ret < 0) {
        LogClient::LogD("find stream info failed\n");
        return;
    }

    int nb_streams = avFormatContext->nb_streams;
    int64_t duration = avFormatContext->duration / AV_TIME_BASE;
    char *filename = avFormatContext->filename;
    int64_t start_time = avFormatContext->start_time / AV_TIME_BASE;
    int64_t bit_rate = avFormatContext->bit_rate / 1000;

    LogClient::LogD("nb_streams is %d", nb_streams);
    LogClient::LogD("duration is %lld", duration);
    LogClient::LogD("filename is %s", filename);
    LogClient::LogD("start_time is %lld", start_time);
    LogClient::LogD("bit_rate is %lld", bit_rate);

    int audioIndex = 0;
    int videoIndex = 0;

    LogClient::LogD("----------------------");

    for (int i = 0; i < avFormatContext->nb_streams; ++i) {
        AVStream *stream = avFormatContext->streams[i];
        AVCodecParameters *avCodecParameters = stream->codecpar;

        if (avCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoIndex = i;
            LogClient::LogD("video index is %d", videoIndex);
            LogClient::LogD("video frame rate is %d", stream->avg_frame_rate);
            LogClient::LogD("width is %d", avCodecParameters->width);
            LogClient::LogD("height is %d", avCodecParameters->height);
            LogClient::LogD("pixel_format is %d", avCodecParameters->format);
            LogClient::LogD("codec_id is %s", avcodec_get_name(avCodecParameters->codec_id));
            LogClient::LogD("bitrate is %lld kb/s", avCodecParameters->bit_rate / 1000);
            LogClient::LogD("----------------------");
        } else if (avCodecParameters->codec_type == AVMEDIA_TYPE_AUDIO) {
            audioIndex = i;
            LogClient::LogD("audio index is %d", audioIndex);
            LogClient::LogD("audio codec name is %s", avcodec_get_name(avCodecParameters->codec_id));
            LogClient::LogD("audio sample_rate is %d", avCodecParameters->sample_rate);
            LogClient::LogD("audio channels is %d", avCodecParameters->channels);
            LogClient::LogD("audio sample_fmt is %d", stream->codec->sample_fmt);
            LogClient::LogD("audio frame size is %d", stream->codec->frame_size);
            LogClient::LogD("audio bit_rate %lld kb/s", stream->codec->bit_rate / 1000);
            LogClient::LogD("----------------------");
        }
    }

    AVDictionaryEntry *m = nullptr;
    while ((m = av_dict_get(avFormatContext->metadata, "", m, AV_DICT_IGNORE_SUFFIX))) {
        LogClient::LogD("key is %s", m->key);
        LogClient::LogD("value is %s", m->value);
    }
}


