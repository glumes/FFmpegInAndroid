//
// Created by glumes on 2017/6/2.
//

#include <jni.h>
#include <string>
#include <stdlib.h>
#include <iostream>

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


#ifdef  ANDROID

#include <jni.h>
#include <android/log.h>

#define TAG "CONFIG"
#define LOGE(format, ...)  __android_log_print(ANDROID_LOG_ERROR, TAG, format, ##__VA_ARGS__)
#define LOGD(format, ...)  __android_log_print(ANDROID_LOG_DEBUG, TAG, format, ##__VA_ARGS__)

#endif

char *urlprotocolinfo();

char *avformatInfo();

char *avcodecInfo();

char *avfilterInfo();

char *configInfo();

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_FFMPEGTest_printFFmpegConfigInfo(JNIEnv *env, jclass type) {

    LOGD("print ffmpeg config info");
    char *infostr = NULL;
    infostr = configInfo();
    LOGD("\n<<Configuration>>\n%s",infostr);
    free(infostr);

    infostr = urlprotocolinfo();
    LOGD("\n<<URLProtocol>>\n%s",infostr);
    free(infostr);

    infostr = avformatInfo();
    LOGD("\n<<AVFormat>>\n%s",infostr);
    free(infostr);

    infostr = avcodecInfo();
    LOGD("\n<<AVCodec>>\n%s",infostr);
    free(infostr);

    infostr = avfilterInfo();
    LOGD("\n<<AVFilter>>\n%s",infostr);
    free(infostr);



}


/**
 * protocol Support Infomation
 * @return
 */
char *urlprotocolinfo() {
    LOGD("funtciomn");
    char *info = (char *) malloc(40000);
    memset(info, 0, 40000);
    av_register_all();
    struct URLProtocol *pup = NULL;
    struct URLProtocol **p_temp = &pup;
    avio_enum_protocols((void **) p_temp, 0);
    while ((*p_temp) != NULL) {
        sprintf(info, "%s[In ][%10s]\n", info, avio_enum_protocols((void **) p_temp, 0));
    }
    pup = NULL;
    avio_enum_protocols((void **) p_temp, 1);
    while ((*p_temp) != NULL) {
        sprintf(info, "%s[Out][%10s]\n", info, avio_enum_protocols((void **) p_temp, 1));
    }

    return info;
}


/**
 * AVFormat Support Information
 * @return
 */
char *avformatInfo() {
    char *info = (char *) malloc(40000);
    memset(info, 0, 40000);
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

/**
 * AVCodec Support Infomation
 * @return
 */
char *avcodecInfo() {
    char *info = (char *) malloc(40000);
    memset(info, 0, 40000);

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

/**
 * AVFilter Support Information
 * @return
 */
char *avfilterInfo() {
    char *info = (char *) malloc(40000);
    memset(info, 0, 40000);
    av_register_all();

    AVFilter *f_temp = (AVFilter *) avfilter_next(NULL);
    while (f_temp != NULL) {
        sprintf(info, "%s[%15s]\n", info, f_temp->name);
        f_temp = f_temp->next;
    }
    return info;
}

/**
 * Configuration Information
 * @return
 */
char *configInfo() {
    char *info = (char *) malloc(40000);
    memset(info, 0, 40000);
    av_register_all();
    sprintf(info, "%s\n", avcodec_configuration());
    return info;
}