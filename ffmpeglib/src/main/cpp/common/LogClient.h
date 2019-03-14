//
// Created by glumes on 2019/3/12.
//

#ifndef FFMPEGINANDROID_LOGCLIENT_H
#define FFMPEGINANDROID_LOGCLIENT_H

#include <android/log.h>

#include <string>

#define LOG_TAG "NativeMethod"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

#define LOG_LENGTH 1024

using namespace std;

class LogClient {

public:
    static void LogD(char *msg);

    static void LogD(const char *tag, const char *fmt, ...);

    static void LogD(std::string msg);

    static void LogE(char *msg);

    static void LogE(std::string msd);
};


#endif //FFMPEGINANDROID_LOGCLIENT_H
