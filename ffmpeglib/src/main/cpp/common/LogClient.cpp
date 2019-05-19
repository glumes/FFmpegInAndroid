//
// Created by glumes on 2019/3/12.
//

#include "LogClient.h"

void LogClient::LogD(char *msg) {
    string str_msg(msg);
    LogD(str_msg);
}

void LogClient::LogD(std::string msg) {
    int len = msg.length();
    if (len > LOG_LENGTH) {
        for (int i = 0; i < len; i += LOG_LENGTH) {
            if (i + LOG_LENGTH < len) {
                LogD("%s", msg.substr(i, i + LOG_LENGTH).c_str());
            } else {
                LogD("%s", msg.substr(i, msg.length()).c_str());
            }
        }
    } else {
        LogD("%s", msg.c_str());
    }
}

void LogClient::LogE(char *msg) {
    string str_msg(msg);
    LogE(str_msg);
}

void LogClient::LogE(std::string msg) {
    int len = msg.length();
    if (len > LOG_LENGTH) {
        for (int i = 0; i < len; i += LOG_LENGTH) {
            if (i + LOG_LENGTH < len) {
                LogE(msg.substr(i, i + LOG_LENGTH).c_str());
            } else {
                LogE(msg.substr(i, msg.length()).c_str());
            }
        }
    } else {
        LogE(msg.c_str());
    }
}

void LogClient::LogD(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    __android_log_vprint(ANDROID_LOG_DEBUG, LOG_TAG, msg, args);
    va_end(args);
}


void LogClient::LogE(const char *msg, ...) {
    va_list args;
    va_start(args, msg);
    __android_log_vprint(ANDROID_LOG_ERROR, LOG_TAG, msg, args);
    va_end(args);
}
