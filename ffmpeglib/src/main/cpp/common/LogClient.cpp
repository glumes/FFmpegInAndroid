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
                LOGD("%s", msg.substr(i, i + LOG_LENGTH).c_str());
            } else {
                LOGD("%s", msg.substr(i, msg.length()).c_str());
            }
        }
    } else {
        LOGD("%s", msg.c_str());
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
                LOGE("%s", msg.substr(i, i + LOG_LENGTH).c_str());
            } else {
                LOGE("%s", msg.substr(i, msg.length()).c_str());
            }
        }
    } else {
        LOGD("%s", msg.c_str());
    }
}

void LogClient::LogD(const char *tag, const char *fmt, ...) {
//    char buf[LOG_LENGTH];
//    int ret = sprintf(buf,LOG_LENGTH,)
}
