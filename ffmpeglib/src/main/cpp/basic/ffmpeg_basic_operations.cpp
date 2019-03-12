//
// Created by glumes on 2019/3/11.
//

#include "ffmpeg_basic_operations.h"

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_basic_FFmpegBasicOperation_printFileMetaData(JNIEnv *env,
                                                                       jobject instance) {

//    std::string tag = "spdlog";
//    auto android_logger = spdlog::android_logger_mt("android", tag);
//    android_logger->critical("Use \"adb shell logcat\" to view this message.");
    LOGD("print meta data");
}


extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_basic_FFmpegBasicOperation_printFFmpegInfo(JNIEnv *env,
                                                                     jobject instance) {


}