//
// Created by glumes on 2019/3/17.
//

#ifndef FFMPEGINANDROID_FFMPEGMUXER_H
#define FFMPEGINANDROID_FFMPEGMUXER_H


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

class FFmpegMuxer {

public:
    void
    muxer_simple(const char *input_path, const char *output_path, const char *output_path_audio);

    void
    muxer_standard(const char *input_path, const char *output_path, const char *output_path_audio);
};


#endif //FFMPEGINANDROID_FFMPEGMUXER_H
