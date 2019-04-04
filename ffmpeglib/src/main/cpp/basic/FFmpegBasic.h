//
// Created by glumes on 2019/3/12.
//

#ifndef FFMPEGINANDROID_FFMPEGBACIC_H
#define FFMPEGINANDROID_FFMPEGBACIC_H

#define BUFFER_SIZE 40000

#include <cstdlib>
#include <stdlib.h>
#include <string>

#ifdef  __cplusplus
extern "C" {
#endif
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libavutil/avutil.h"
#include "libavfilter/avfilter.h"
#ifdef  __cplusplus
};
#endif

#include <LogClient.h>

class FFmpegBasic {
public:
    FFmpegBasic();

    ~FFmpegBasic();

    static char *getAvFormatInfo();

    static char *getAvCodecInfo();

    static char *getAvFilterInfo();

    void printVideoInfo(const char *file_path);
};


#endif //FFMPEGINANDROID_FFMPEGBACIC_H
