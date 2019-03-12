package com.glumes.ffmpeginandroid.ffmpeg

import com.glumes.ffmpeginandroid.adapter.FFmpegOps
import com.glumes.ffmpeginandroid.listener.OnFFmpegOpsClickListener
import com.glumes.ffmpeglib.FFMPEG_GET_FFMPEG_INFO
import com.glumes.ffmpeglib.FFMPEG_GET_META_DATA_INFO
import com.glumes.ffmpeglib.FFmpegWork

class FFmpegBridge : OnFFmpegOpsClickListener {


    override fun onFFmpegOpsClick(ops: FFmpegOps) {
        when (ops.type) {
            FFMPEG_GET_META_DATA_INFO -> {
                FFmpegWork.printFileMetaData()
            }
            FFMPEG_GET_FFMPEG_INFO -> {
                FFmpegWork.printFFmpegInfo()
            }
        }
    }
}