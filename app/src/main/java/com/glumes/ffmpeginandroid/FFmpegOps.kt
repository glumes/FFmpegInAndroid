package com.glumes.ffmpeginandroid

import android.os.Parcel
import android.os.Parcelable

/**
 * @Author  glumes
 */

class FFmpegOps(var ops: String? = "") : Parcelable {

    constructor(parcel: Parcel) : this(parcel.readString()) {
    }

    override fun writeToParcel(parcel: Parcel, flags: Int) {
        parcel.writeString(ops)
    }

    override fun describeContents(): Int {
        return 0
    }

    companion object CREATOR : Parcelable.Creator<FFmpegOps> {
        override fun createFromParcel(parcel: Parcel): FFmpegOps {
            return FFmpegOps(parcel)
        }

        override fun newArray(size: Int): Array<FFmpegOps?> {
            return arrayOfNulls(size)
        }
    }

}