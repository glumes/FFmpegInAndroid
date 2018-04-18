//
// Created by glumes on 2017/6/3.
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


#include <jni.h>
#include <android/log.h>

#define TAG "CONFIGTEST"
#define LOGE(format, ...)  __android_log_print(ANDROID_LOG_ERROR, TAG, format, ##__VA_ARGS__)
#define LOGD(format, ...)  __android_log_print(ANDROID_LOG_DEBUG, TAG, format, ##__VA_ARGS__)




extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_FFMPEGTest_yuv420Split(JNIEnv *env, jclass type, jstring filename_,
                                                 jstring output_y_, jstring output_u_,
                                                 jstring output_v_) {
    const char *filename = env->GetStringUTFChars(filename_, 0);
    const char *output_y = env->GetStringUTFChars(output_y_, 0);
    const char *output_u = env->GetStringUTFChars(output_u_, 0);
    const char *output_v = env->GetStringUTFChars(output_v_, 0);

    // TODO

    FILE *fp = fopen(filename, "rb+");
    FILE *fp1 = fopen(output_y, "wb+");
    FILE *fp2 = fopen(output_u, "wb+");
    FILE *fp3 = fopen(output_v, "wb+");

    int w = 256;
    int h = 256;
    int num = 1;
    LOGD("yuv address is %s,w is %d,h is %d,num is %d", filename, h, w, num);

    unsigned char *pic = (unsigned char *) malloc((size_t) (w * h * 3 / 2));

    if (fp1 == NULL) {
        LOGD("open failed");
    } else {
        LOGD("open success");
    }

    for (int i = 0; i < num; i++) {

        fread(pic, 1, (size_t) (w * h * 3 / 2), fp);
        //Y
        fwrite(pic, 1, (size_t) (w * h), fp1);
        //U
        fwrite(pic + w * h, 1, (size_t) (w * h / 4), fp2);
        //V
        fwrite(pic + w * h * 5 / 4, 1, (size_t) (w * h / 4), fp3);
    }

    free(pic);
    fclose(fp);
    fclose(fp1);
    fclose(fp2);
    fclose(fp3);

    env->ReleaseStringUTFChars(filename_, filename);
    env->ReleaseStringUTFChars(output_y_, output_y);
    env->ReleaseStringUTFChars(output_u_, output_u);
    env->ReleaseStringUTFChars(output_v_, output_v);
}

