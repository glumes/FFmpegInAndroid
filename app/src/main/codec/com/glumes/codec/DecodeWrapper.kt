package com.glumes.codec

import com.glumes.ffmpeglib.codec.DecodeUtils
import com.glumes.util.FileConstants
import com.glumes.util.TYPE_DECODE_MPEG_TO_PGM

/**
 * Created by glumes on 19/04/2018
 */
class DecodeWrapper {


    companion object {

        fun handleDecode(ops: Int) {
            when (ops) {
                TYPE_DECODE_MPEG_TO_PGM -> {
                    DecodeUtils.decode_MPEG_to_PGM(FileConstants.FILE_M2V, FileConstants.OUT_PUT_FILE)
                }
            }
        }
    }
}