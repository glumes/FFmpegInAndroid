package com.glumes.ffmpeginandroid.ffmpeg

import com.glumes.ffmpeginandroid.adapter.FFmpegOps
import com.glumes.ffmpeginandroid.adapter.FFmpegType
import com.glumes.ffmpeglib.FFMPEG_GET_FFMPEG_INFO
import com.glumes.ffmpeglib.FFMPEG_GET_META_DATA_INFO
import java.util.*

/**
 * @Author  glumes
 */
class FFmpegFactory {


    companion object {
        fun makeData(): MutableList<FFmpegType?>? {
            return Arrays.asList(
                    basicFFmpegOperations()
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

    }
}