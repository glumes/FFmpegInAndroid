package com.glumes.ffmpeginandroid.ffmpeg

import com.glumes.ffmpeginandroid.adapter.FFmpegOps
import com.glumes.ffmpeginandroid.listener.OnFFmpegOpsClickListener
import com.glumes.ffmpeglib.*

class FFmpegBridge : OnFFmpegOpsClickListener {


    override fun onFFmpegOpsClick(ops: FFmpegOps) {
        when (ops.type) {
            FFMPEG_GET_META_DATA_INFO -> {
                FFmpegWork.printFileMetaData()
            }
            FFMPEG_GET_FFMPEG_INFO -> {
                FFmpegWork.printFFmpegInfo()
            }
            FFMPEG_CODEC_DECODE_VIDEO -> {
                FFmpegWork.codecDecodeVideo()
            }
            FFMPEG_CODEC_ENCODE_VIDEO -> {
                FFmpegWork.codecEncodeYUV2H264()
            }
        }
    }
}