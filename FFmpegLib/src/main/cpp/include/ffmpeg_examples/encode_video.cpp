//
// Created by glumes on 2017/6/24.
//

#include <jni.h>
#include <string>
#include <android/log.h>

#define LOGI(format, ...)  __android_log_print(ANDROID_LOG_INFO,  "FFmpegExamples", format, ##__VA_ARGS__)

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_FFmpegSample_onEncodeVideo(JNIEnv *env, jobject instance,
                                                     jstring outFileName_) {
    const char *outFileName = env->GetStringUTFChars(outFileName_, 0);

    // TODO

    LOGI("%s","onEncodeVideo");

    env->ReleaseStringUTFChars(outFileName_, outFileName);
}
