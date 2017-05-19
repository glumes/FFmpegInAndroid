//#include <jni.h>
#include <string>
#include <stdlib.h>
#include <iostream>

extern "C" {
#include "libavcodec/avcodec.h"
#include "libavformat/avformat.h"
#include "libswscale/swscale.h"
#include "libavutil/imgutils.h"
#include "libavutil/avutil.h"
};

#ifdef  ANDROID

#include <jni.h>
#include <android/log.h>

#define LOGE(format, ...)  __android_log_print(ANDROID_LOG_ERROR, "(>_<)", format, ##__VA_ARGS__)
#define LOGI(format, ...)  __android_log_print(ANDROID_LOG_INFO,  "(^_^)", format, ##__VA_ARGS__)
#else
#define LOGE(format, ...)  printf("(>_<) " format "\n", ##__VA_ARGS__)
#define LOGI(format, ...)  printf("(^_^) " format "\n", ##__VA_ARGS__)
#endif

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


/**
 * 解码函数
 */
JNIEXPORT jint JNICALL
Java_com_glumes_ffmpeglib_SimpleDecoder_decode(JNIEnv *env, jobject instance, jstring input_jstr,
                                               jstring output_jstr) {
    const char *inputurl = env->GetStringUTFChars(input_jstr, 0);
    const char *outputurl = env->GetStringUTFChars(output_jstr, 0);

    // TODO

    AVFormatContext *pFormatContext;
    int i, videoindex;
    AVCodecContext *pCodecCtx;
    AVCodec *pCodec;
    AVFrame *pFrame, *pFrameYUV;
    uint8_t *out_buffer;
    AVPacket *packet;
    int y_size;
    int ret, got_picture;
    struct SwsContext *img_convert_ctx;

    FILE *fp_yuv;
    int frame_cnt;
    clock_t time_start, time_finish;
    double time_duration = 0.0;

    char input_str[500] = {0};
    char output_str[500] = {0};

    char info[1000] = {0};

//    sprintf(input_str, "%s", (*env)->GetStringUTFChars(env, input_jstr, NULL));

//    av_log_set_callback(custom_log);


    av_register_all();

    avformat_network_init();

    pFormatContext = avformat_alloc_context();

    if (avformat_open_input(&pFormatContext, input_str, NULL, NULL) != 0) {
        LOGE("Couldn't open input stream\n");
        return -1;
    }

    if (avformat_find_stream_info(pFormatContext, NULL) < 0) {
        LOGE("Could't find stream information.\n");
        return -1;
    }

    videoindex = -1;

    for (int i = 0; i < pFormatContext->nb_streams; ++i) {
        if (pFormatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoindex = i;
            break;
        }
    }

    if (videoindex == -1) {
        LOGE("Could't find a video stream.\n");
        return -1;
    }

    pCodecCtx = pFormatContext->streams[videoindex]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);

    if (pCodec == NULL) {
        LOGE("Couldn't find Codec.\n");
        return -1;
    }

    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        LOGE("Could't find Codec.\n");
        return -1;
    }

    pFrame = av_frame_alloc();

    pFrameYUV = av_frame_alloc();

    out_buffer = (unsigned char *) av_malloc(
            av_image_get_buffer_size(AV_PIX_FMT_YUV420P, pCodecCtx->width, pCodecCtx->height, 1));

    av_image_fill_arrays(pFrameYUV->data, pFrameYUV->linesize, out_buffer, AV_PIX_FMT_YUV420P,
                         pCodecCtx->width, pCodecCtx->height, 1);

    packet = (AVPacket *) av_malloc(sizeof(AVPacket));

    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height, pCodecCtx->pix_fmt,
                                     pCodecCtx->width, pCodecCtx->height, AV_PIX_FMT_YUV420P,
                                     SWS_BICUBIC, NULL, NULL, NULL);

    cout << info << "input is " << input_str << endl;
    cout << info << "output is " << output_str << endl;
    cout << info << "format is " << pFormatContext->iformat->name << endl;
    cout << info << "codec is " << pCodecCtx->codec->name << endl;
    cout << info << "Resolution is " << pCodecCtx->width << pCodecCtx->height << endl;


    fp_yuv = fopen(output_str, "wb+");

    if (fp_yuv == NULL) {
        cout << "cannot open output file " << endl;
        return -1;
    }

    frame_cnt = 0;
    time_start = clock();

    while (av_read_frame(pFormatContext, packet) >= 0) {
        if (packet->stream_index == videoindex){
            ret = avcodec_decode_video2(pCodecCtx,pFrame,&got_picture,packet);
            if (ret < 0){
                LOGE("Decode Error.\n");
                return -1 ;
            }
            if (got_picture){

            }
        }
    }

    env->ReleaseStringUTFChars(input_jstr, inputurl);
    env->ReleaseStringUTFChars(output_jstr, outputurl);
}


//Output FFmpeg's av_log()
void custom_log(void *ptr, int level, const char *fmt, va_list vl) {
    FILE *fp = fopen("/storage/emulated/0/av_log.txt", "a+");
    if (fp) {
        vfprintf(fp, fmt, vl);
        fflush(fp);
        fclose(fp);
    }
}