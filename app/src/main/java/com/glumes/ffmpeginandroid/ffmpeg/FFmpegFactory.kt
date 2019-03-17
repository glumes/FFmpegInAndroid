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
            val op1 = FFmpegOps("视频解码操作", FFMPEG_CODEC_DECODE_VIDEO)
            val op2 = FFmpegOps("视频编码操作", FFMPEG_CODEC_ENCODE_VIDEO)
            val op3 = FFmpegOps("音频解码操作", FFMPEG_CODEC_DECODE_AUDIO)
            val op4 = FFmpegOps("音频编码操作", FFMPEG_CODEC_ENCODE_AUDIO)
            return Arrays.asList(op1, op2, op3, op4)
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