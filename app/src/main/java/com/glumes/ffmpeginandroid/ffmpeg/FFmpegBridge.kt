package com.glumes.ffmpeginandroid.ffmpeg

import com.glumes.ffmpeginandroid.adapter.FFmpegOps
import com.glumes.ffmpeginandroid.listener.OnFFmpegOpsClickListener
import com.glumes.ffmpeglib.*

class FFmpegBridge : OnFFmpegOpsClickListener {


    override fun onFFmpegOpsClick(ops: FFmpegOps) {
        when (ops.type) {
            // 基本信息相关内容
            FFMPEG_GET_META_DATA_INFO -> {
                FFmpegWork.printFFmpegConfig()
            }
            FFMPEG_GET_FFMPEG_INFO -> {
                FFmpegWork.printFFmpegInfo()
            }

            // 编解码相关操作
            FFMPEG_CODEC_MP4_TO_YUV -> {
                FFmpegWork.codecMp4ToYUV()
            }
            FFMPEG_CODEC_YUV_TO_H264 -> {
                FFmpegWork.codecYUVToH264()
            }
            FFMPEG_CODEC_H264_TO_MP4 -> {
                FFmpegWork.codecH264ToMp4()
            }

            // 解复用相关操作
            FFMPEG_MUXER_DEMUXER_SIMPLE -> {
                FFmpegWork.muxerDemuxerSample()
            }
            FFMPEG_MUXER_DEMUXER_STANDARD -> {
                FFmpegWork.muxerDemuxerStandard()
            }
            FFMPEG_MUXER_DEMUXER_SIMPLE -> {
                FFmpegWork.muxerDemuxerSample()
            }
        }
    }
}