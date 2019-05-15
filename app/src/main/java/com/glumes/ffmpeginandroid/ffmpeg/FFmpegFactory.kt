package com.glumes.ffmpeginandroid.ffmpeg

import com.glumes.ffmpeginandroid.adapter.FFmpegOps
import com.glumes.ffmpeginandroid.adapter.FFmpegType
import com.glumes.ffmpeglib.*
import java.util.*

/**
 * @Author  glumes
 */
class FFmpegFactory {


    companion object {
        fun makeData(): MutableList<FFmpegType?>? {
            return Arrays.asList(
                    basicFFmpegOperations(),
                    codefFFmpegOperations(),
                    muxerFFmpegOperations()
            )
        }

        private fun basicFFmpegOperations(): FFmpegType {
            return FFmpegType("基础操作", basicOperationList())
        }

        private fun basicOperationList(): MutableList<FFmpegOps>? {
            val ops1 = FFmpegOps("打印文件 MetaData 信息", FFMPEG_GET_META_DATA_INFO)
            val ops2 = FFmpegOps("打印 FFmpeg 信息", FFMPEG_GET_FFMPEG_INFO)
            return Arrays.asList(ops1, ops2)
        }

        private fun codefFFmpegOperations(): FFmpegType {
            return FFmpegType("编码操作", codecOperationList())
        }

        private fun codecOperationList(): MutableList<FFmpegOps>? {
            val op1 = FFmpegOps("MP4 到 YUV ", FFMPEG_CODEC_MP4_TO_YUV)
            val op2 = FFmpegOps("MP4 到 H264 ", FFMPEG_CODEC_MP4_TO_H264)

            val op3 = FFmpegOps("YUV 到 H264", FFMPEG_CODEC_YUV_TO_H264)
            val op4 = FFmpegOps("H264 到 YUV", FFMPEG_CODEC_H264_TO_YUV)

            val op5 = FFmpegOps("YUV 到 MP4", FFMPEG_CODEC_YUV_TO_MP4)
            val op6 = FFmpegOps("H264 到 MP4", FFMPEG_CODEC_H264_TO_MP4)

            val split = FFmpegOps("--------------------------")
            return Arrays.asList(op1, op2, op3, op4, op5, op6,
                    split)
        }


        private fun muxerFFmpegOperations(): FFmpegType {
            return FFmpegType("封装格式处理", muxerOperationList())
        }

        private fun muxerOperationList(): MutableList<FFmpegOps>? {
            val op1 = FFmpegOps("音视频分离简化版", FFMPEG_MUXER_DEMUXER_SIMPLE)
            val op2 = FFmpegOps("音视频分离标准版", FFMPEG_MUXER_DEMUXER_STANDARD)
            return Arrays.asList(op1, op2)
        }
    }
}