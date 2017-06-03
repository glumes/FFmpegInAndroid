//
// Created by glumes on 2017/6/2.
//

#include <stdio.h>
#include <malloc.h>
#include "simple_mediadata.h"

#include <iostream>
#include <jni.h>
#include <android/log.h>

#define TAG "NativeCodec-looper"
#define LOGV(...) __android_log_print(ANDROID_LOG_VERBOSE, TAG, __VA_ARGS__)
using namespace std;

extern "C"
int simple_mediadata::media_yuv420_split(char *url, int w, int h, int num) {
    FILE *fp = fopen(url, "rb+");
    const char *y_file = "output_420_y.txt";
    FILE *fp1 = fopen(y_file, "wb+");
    FILE *fp2 = fopen("output_420_u.y", "wb+");
    FILE *fp3 = fopen("output_420_v.y", "wb+");

//    LOGD("yuv address is %s,w is %d,h is %d,num is %d", url,h,w,num);

//    LOGV("yuv address is %s,w is %d,h is %d,num is %d", url,h,w,num);

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
