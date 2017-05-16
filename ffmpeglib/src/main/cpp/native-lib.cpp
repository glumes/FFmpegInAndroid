#include <jni.h>
#include <string>



extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libavutil/avutil.h"
};

using namespace std;

extern "C"
jstring
Java_com_glumes_ffmpeglib_FFMPEGTest_stringFromJNI(JNIEnv *env,
                                                   jobject /*this*/,
                                                   jstring name_) {
    const char *name = env->GetStringUTFChars(name_, 0);

    // TODO
    string hello = "Hello from C++ , ";
    string nameStr;
    nameStr = (string) env->GetStringUTFChars(name_, 0);
    av_register_all();
    string s = hello + nameStr + " end ";
    env->ReleaseStringUTFChars(name_, name);

    return env->NewStringUTF(s.c_str());

}



JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_Player_render(JNIEnv *env, jclass type, jstring path_) {
    const char *path = env->GetStringUTFChars(path_, 0);

    // TODO

    env->ReleaseStringUTFChars(path_, path);
}