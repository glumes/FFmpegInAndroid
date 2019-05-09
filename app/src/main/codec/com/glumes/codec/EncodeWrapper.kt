package com.glumes.codec

import com.glumes.ffmpeglib.codec.EncodeUtils
import com.glumes.util.FileConstants
import com.glumes.util.TYPE_ENCODE_YUV_TO_H264

/**
 * Created by glumes on 19/04/2018
 */
class EncodeWrapper {

    companion object {

        fun handleEecode(ops: Int) {
            when (ops) {
                TYPE_ENCODE_YUV_TO_H264 -> {

                    EncodeUtils.encode_YUV_to_H264("", FileConstants.FILE_H264)

                }
            }
        }
    }
}