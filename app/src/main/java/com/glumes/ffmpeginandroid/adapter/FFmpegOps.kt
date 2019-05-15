package com.glumes.ffmpeginandroid.adapter

import android.os.Parcel
import android.os.Parcelable

/**
 * @Author  glumes
 */

class FFmpegOps(var title: String? = "", var type: Int = -1) : Parcelable {

    constructor(parcel: Parcel) : this(
            parcel.readString(),
            parcel.readInt())

    override fun writeToParcel(parcel: Parcel, flags: Int) {
        parcel.writeString(title)
        parcel.writeInt(type)
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