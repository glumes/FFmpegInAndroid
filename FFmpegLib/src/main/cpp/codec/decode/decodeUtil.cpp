//
// Created by glumes on 2018/4/18.
//

#include "decodeUtil.h"


void pgm_save(unsigned char *buf, int wrap, int width, int height, const char *name) {
    FILE *file;

//    char *filename = strcat((char *) name, ".pgm");

    file = fopen(name, "w");

    LOGI("file path is %s", name);

    if (!file) {
        LOGI("could not open file and return\n");
        return;
    }

    fprintf(file, "P5\n%d %d\n%d\n", width, height, 255);
    for (int i = 0; i < height; ++i) {
        fwrite(buf + i * wrap, 1, width, file);
    }

    fclose(file);
}


void decode(AVCodecContext *pContext, AVFrame *pFrame, AVPacket *pPacket, const char *name) {

    char buf[1024];
    int ret;
    ret = avcodec_send_packet(pContext, pPacket);

    if (ret < 0) {
        LOGI("Error sending a packet for decoding\n");
        return;
    }

    while (ret >= 0) {
        ret = avcodec_receive_frame(pContext, pFrame);
        if (ret == AVERROR(EAGAIN) || ret == AVERROR_EOF) {
            return;
        } else if (ret < 0) {
            LOGI("Error while decoding\n");
            return;
        }

        LOGI("Saving frame %3d\n", pContext->frame_number);
        fflush(stdout);
        snprintf(buf, sizeof(buf), "%s-%d", name, pContext->frame_number);
        LOGI("buf name is %s", buf);
        pgm_save(pFrame->data[0], pFrame->linesize[0], pFrame->width, pFrame->height, buf);
        LOGI("save success");
    }

}

