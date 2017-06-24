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
#include "libsimple_mediadata/simple_mediadata.h"
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

extern "C"
JNIEXPORT void JNICALL
Java_com_glumes_ffmpeglib_SimpleDecoder_test(JNIEnv *env, jobject instance) {

    // TODO

}





/**
 * 解码函数
 */
extern "C"
JNIEXPORT jint JNICALL
Java_com_glumes_ffmpeglib_SimpleDecoder_decode(JNIEnv *env, jobject /*this*/, jstring input_jstr,
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

//    char input_str[500] = {0};
//    char output_str[500] = {0};

    char info[1000] = {0};

//    sprintf(input_str, "%s", env->GetStringUTFChars(input_jstr, NULL));
//    sprintf(output_str, "%s", env->GetStringUTFChars(output_jstr, NULL));

//    av_log_set_callback(custom_log);


    av_register_all();

    avformat_network_init();

    pFormatContext = avformat_alloc_context();

    LOGI("input str is %s", inputurl);

    if (avformat_open_input(&pFormatContext, inputurl, NULL, NULL) != 0) {
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

    cout << info << "input is " << inputurl << endl;
    cout << info << "output is " << inputurl << endl;
    cout << info << "format is " << pFormatContext->iformat->name << endl;
    cout << info << "codec is " << pCodecCtx->codec->name << endl;
    cout << info << "Resolution is " << pCodecCtx->width << pCodecCtx->height << endl;


    fp_yuv = fopen(outputurl, "wb+");

    if (fp_yuv == NULL) {
        cout << "cannot open output file " << endl;
        return -1;
    }

    frame_cnt = 0;
    time_start = clock();

    while (av_read_frame(pFormatContext, packet) >= 0) {
        if (packet->stream_index == videoindex) {
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
            if (ret < 0) {
                LOGE("Decode Error.\n");
                return -1;
            }
            if (got_picture) {
                sws_scale(img_convert_ctx, (const uint8_t *const *) pFrame->data, pFrame->linesize,
                          0, pCodecCtx->height,
                          pFrameYUV->data, pFrameYUV->linesize);
                y_size = pCodecCtx->width * pCodecCtx->height;
                fwrite(pFrameYUV->data[0], 1, y_size, fp_yuv);
                fwrite(pFrameYUV->data[1], 1, y_size / 4, fp_yuv);
                fwrite(pFrameYUV->data[2], 1, y_size / 4, fp_yuv);

                char pictype_str[10] = {0};
                switch (pFrame->pict_type) {
                    case AV_PICTURE_TYPE_I:
                        cout << pictype_str << "I" << endl;
                        break;
                    case AV_PICTURE_TYPE_P:
                        cout << pictype_str << "P" << endl;
                        break;
                    case AV_PICTURE_TYPE_B:
                        cout << pictype_str << "B" << endl;
                        break;
                    default:
                        cout << pictype_str << "Other" << endl;
                }
                LOGI("Frame index: %5d,Type:%s", frame_cnt, pictype_str);
                frame_cnt++;
            }
        }
        av_free_packet(packet);
    }

    while (1) {
        ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture, packet);
        if (ret < 0) {
            break;
        }

        if (!got_picture) {
            break;
        }
        sws_scale(img_convert_ctx, (const uint8_t *const *) pFrame->data, pFrame->linesize, 0,
                  pCodecCtx->height,
                  pFrameYUV->data, pFrameYUV->linesize);

        int y_size = pCodecCtx->width * pCodecCtx->height;
        fwrite(pFrameYUV->data[0], 1, y_size, fp_yuv);    //Y
        fwrite(pFrameYUV->data[1], 1, y_size / 4, fp_yuv);  //U
        fwrite(pFrameYUV->data[2], 1, y_size / 4, fp_yuv);  //V


        //output info
        char pictype_str[10] = {0};
        switch (pFrame->pict_type) {
            case AV_PICTURE_TYPE_I:
                cout << pictype_str << "I" << endl;
                break;
            case AV_PICTURE_TYPE_P:
                cout << pictype_str << "P" << endl;
                break;
            case AV_PICTURE_TYPE_B:
                cout << pictype_str << "B" << endl;
                break;
            default:
                cout << pictype_str << "Other" << endl;
                break;
        }
        LOGI("Frame index : %5d,.Type:%s", frame_cnt, pictype_str);
        frame_cnt++;
    }
    time_finish = clock();

    time_duration = time_finish - time_start;

    sprintf(info, "%s[Time      ]%fms\n", info, time_duration);
    sprintf(info, "%s[Count     ]%d\n", info, frame_cnt);

    sws_freeContext(img_convert_ctx);

    fclose(fp_yuv);

    av_frame_free(&pFrameYUV);

    av_frame_free(&pFrame);

    avcodec_close(pCodecCtx);

    avformat_close_input(&pFormatContext);


    env->ReleaseStringUTFChars(input_jstr, inputurl);
    env->ReleaseStringUTFChars(output_jstr, outputurl);

    return 0;
}

// JNI 调用 C 和 C++ 代码是有区别的，调用 C++ 代码，是要加 extern "C" 的。
//extern  "C"
//JNIEXPORT jint JNICALL
//Java_com_glumes_ffmpeglib_SimpleDecoder_decode(JNIEnv *env, jobject instance, jstring inputurl_,
//                                               jstring outputurl_) {
//    const char *inputurl = env->GetStringUTFChars(inputurl_, 0);
//    const char *outputurl = env->GetStringUTFChars(outputurl_, 0);
//
//    // TODO
//
//    env->ReleaseStringUTFChars(inputurl_, inputurl);
//    env->ReleaseStringUTFChars(outputurl_, outputurl);
//
//    return 0 ;
//}JNIEXPORT void JNICALL

