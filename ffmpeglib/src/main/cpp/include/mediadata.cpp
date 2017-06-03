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




int media_yuv420_split(char *url, int w, int h, int num) {
    FILE *fp = fopen(url, "rb+");
    const char *y_file = "output_420_y.txt";
    FILE *fp1 = fopen(y_file, "wb+");
    FILE *fp2 = fopen("output_420_u.y", "wb+");
    FILE *fp3 = fopen("output_420_v.y", "wb+");

    LOGD("yuv address is %s,w is %d,h is %d,num is %d", url,h,w,num);

    unsigned char *pic = (unsigned char *) malloc((size_t) (w * h * 3 / 2));

    if (fp1 == NULL) {
        return -1;
    } else {
        return 1;
    }
//    for (int i = 0; i < num; i++) {
//
//        fread(pic, 1, (size_t) (w * h * 3 / 2), fp);
//        //Y
//        fwrite(pic, 1, (size_t) (w * h), fp1);
//        //U
//        fwrite(pic + w * h, 1, (size_t) (w * h / 4), fp2);
//        //V
//        fwrite(pic + w * h * 5 / 4, 1, (size_t) (w * h / 4), fp3);
//    }
//
//    free(pic);
//    fclose(fp);
//    fclose(fp1);
//    fclose(fp2);
//    fclose(fp3);

    return 0;
}


extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_FFMPEGTest_yuv420Split(JNIEnv *env, jclass type, jstring filename_) {
    const char *filename = env->GetStringUTFChars(filename_, 0);
//    media_yuv420_split((char *) filename, 256, 256, 1);

    FILE *fp = fopen(filename, "rb+");
    const char *y_file = "output_420_y.txt";
    FILE *fp1 = fopen(y_file, "wb+");
    FILE *fp2 = fopen("output_420_u.y", "wb+");
    FILE *fp3 = fopen("output_420_v.y", "wb+");

    int w = 256;
    int h = 256;
    int num = 1;

    LOGD("yuv address is %s,w is %d,h is %d,num is %d", filename,w,h,num);

    unsigned char *pic = (unsigned char *) malloc((size_t) (w * h * 3 / 2));

    if (fp1 == NULL) {
        LOGD("FAILED");
    } else {
        LOGD("SUCCESS");
    }
    env->ReleaseStringUTFChars(filename_, filename);
}