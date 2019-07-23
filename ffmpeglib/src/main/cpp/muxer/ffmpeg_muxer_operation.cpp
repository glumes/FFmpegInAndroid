//
// Created by glumes on 2019/3/17.
//

#include "ffmpeg_muxer_operation.h"
#include "FFmpegMuxer.h"


static FFmpegMuxer *muxer;

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_muxer_FFmpegMuxerOperation_demuxerSample__Ljava_lang_String_2Ljava_lang_String_2Ljava_lang_String_2(
        JNIEnv *env, jobject instance, jstring input_file_, jstring output_audio_file_,
        jstring output_video_file_) {
    const char *input_file = env->GetStringUTFChars(input_file_, 0);
    const char *output_audio_file = env->GetStringUTFChars(output_audio_file_, 0);
    const char *output_video_file = env->GetStringUTFChars(output_video_file_, 0);

    muxer = new FFmpegMuxer();

    muxer->muxer_simple(input_file,output_audio_file,output_video_file);

    env->ReleaseStringUTFChars(input_file_, input_file);
    env->ReleaseStringUTFChars(output_audio_file_, output_audio_file);
    env->ReleaseStringUTFChars(output_video_file_, output_video_file);
}
