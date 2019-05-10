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

class FFmpegCodec {

private:

public:

    void encode_video(const char *filename, const char *codec_name);

    void codec_mp4_to_yuv(const char *input_path, const char *output_path);

    void codec_mp4_to_h264(const char *input_path, const char *output_path);

    void encode_yuv_to_video(const char *input_path, const char *output_path);

    void codec_yuv_to_h264(const char *input_path, const char *output_path);

    void codec_h264_to_mp4(const char *input_path, const char *output_path);

};


#endif //FFMPEGINANDROID_FFMPEGCODEC_H
