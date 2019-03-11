package com.glumes.ffmpeginandroid.adapter

import android.view.View
import android.view.animation.Animation.RELATIVE_TO_SELF
import android.view.animation.RotateAnimation
import android.widget.ImageView
import android.widget.TextView
import com.glumes.ffmpeginandroid.R
import com.glumes.widget.models.ExpandableGroup
import com.glumes.widget.viewholders.GroupViewHolder

/**
 * @Author  glumes
 */
class FFmpegTypeViewHolder(itemView: View) : GroupViewHolder(itemView) {

    val title: TextView = itemView.findViewById(R.id.textview_ffmpeg_type)
    val arrow: ImageView = itemView.findViewById(R.id.textview_ffmpeg_icon)


    override fun expand() {
        super.expand()
        val rotate = RotateAnimation(360f, 180f, RELATIVE_TO_SELF, 0.5f, RELATIVE_TO_SELF, 0.5f)
        rotate.duration = 300
        rotate.fillAfter = true
        arrow.animation = rotate
    }

    override fun collapse() {
        val rotate = RotateAnimation(180f, 360f, RELATIVE_TO_SELF, 0.5f, RELATIVE_TO_SELF, 0.5f)
        rotate.duration = 300
        rotate.fillAfter = true
        arrow.animation = rotate
    }
}