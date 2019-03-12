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


