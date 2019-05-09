package com.glumes.util

import timber.log.Timber

class Log {

    companion object {

        private const val tag = "FFmpegInAndroid"

        fun d(msg: String, vararg args: Any) {
            Timber.tag(tag).d(msg, args)
        }

        fun e(msg: String, vararg args: Any){
            Timber.tag(tag).e(msg, args)
        }
    }

}