//
// Created by glumes on 2018/4/18.
//
#ifndef FFMPEGINANDROID_DECODEUTIL_H
#define FFMPEGINANDROID_DECODEUTIL_H


#include <jni.h>
#include "logutil.h"

#ifdef  __cplusplus
extern "C" {
#endif
#include "libavcodec/avcodec.h"

#ifdef  __cplusplus
};
#endif


void pgm_save(unsigned char *string, int i, int width, int height, const char *filename);

void decode(AVCodecContext *pContext, AVFrame *pFrame, AVPacket *pPacket, const char *name);


#endif //FFMPEGINANDROID_DECODEUTIL_H
