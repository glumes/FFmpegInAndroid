//
// Created by glumes on 2019/3/14.
//

#ifndef FFMPEGINANDROID_FFMPEGCODEC_H
#define FFMPEGINANDROID_FFMPEGCODEC_H

#ifdef  __cplusplus
extern "C" {
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libavutil/avutil.h"
#include "libavfilter/avfilter.h"
#include "libavutil/opt.h"
#include "libavutil/mathematics.h"
#ifdef  __cplusplus
};

#endif

#define USE_H264BSF 0

#define USE_AACBSF 0
#define __STDC_CONSTANT_MACROS

#include <LogClient.h>

#define RET_OK 1
#define RET_FAIL  0


struct FileContext {
    AVFormatContext *s_pFormatCtx;
    AVCodecContext *s_pCodecCtx;
    AVCodec *s_pVideoCodec;
    AVCodec *s_pAudioCodec;

    int s_VideoIndex = -1;
    int s_AudioIndex = -1;

};


class FFmpegCodec {

private:

//    FileContext inputFileContext;

//    AVFormatContext *m_pFormatCtx;
//
//    int m_iVideoIndex = -1;
//
//    int m_iAudioIndex = -1;
//
//    AVCodecContext *m_pCodecCtx;
//    AVCodec *m_pVideoCodec;

    AVOutputFormat *m_pOutputFmt;

public:

    int parseFile(const char *input_path, FileContext &fileContext);

    void encode_video(const char *filename, const char *codec_name);

    void codec_mp4_to_yuv(const char *input_path, const char *output_path);

    void codec_mp4_to_h264(const char *input_path, const char *output_path);

    void codec_yuv_to_mp4(const char *input_path, const char *output_path);

    void codec_yuv_to_h264(const char *input_path, const char *output_path);

    void codec_h264_to_mp4(const char *input_path, const char *output_path);


};


#endif //FFMPEGINANDROID_FFMPEGCODEC_H
