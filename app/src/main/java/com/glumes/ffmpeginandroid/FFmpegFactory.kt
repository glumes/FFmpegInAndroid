package com.glumes.ffmpeginandroid

import java.util.*

/**
 * @Author  glumes
 */
class FFmpegFactory {


    companion object {
        fun makeData(): MutableList<FFmpegType?>? {
            return Arrays.asList(
                    makeFFmpegInfo()
            )

        }

        private fun makeFFmpegInfo(): FFmpegType {
            return FFmpegType("Info", makeInfoList())
        }

        private fun makeInfoList(): MutableList<FFmpegOps>? {
            val info1 = FFmpegOps("new 1")
            val info2 = FFmpegOps("new 2")
            val info3 = FFmpegOps("new 3")
            return Arrays.asList(info1, info2, info3)
        }

    }
}