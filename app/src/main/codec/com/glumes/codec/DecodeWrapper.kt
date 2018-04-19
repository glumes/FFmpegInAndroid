package com.glumes.codec

import com.glumes.ffmpeglib.codec.DecodeUtils
import com.glumes.util.FileContants
import com.glumes.util.TYPE_DECODE_MPEG_TO_PGM

/**
 * Created by glumes on 19/04/2018
 */
class DecodeWrapper {


    companion object {

        fun handleDecode(ops: Int) {
            when (ops) {
                TYPE_DECODE_MPEG_TO_PGM -> {
                    DecodeUtils.decode_MPEG_to_PGM(FileContants.FILE_M2V, FileContants.OUT_PUT_FILE)
                }
            }
        }
    }
}