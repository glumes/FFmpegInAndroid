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
#ifdef  __cplusplus
};
#endif

#include <LogClient.h>

class FFmpegCodec {

public:

    void encode_video(const char *filename, const char *codec_name);

    void decode_video_to_yuv(const char *input_path, const char *output_path);
};


#endif //FFMPEGINANDROID_FFMPEGCODEC_H
