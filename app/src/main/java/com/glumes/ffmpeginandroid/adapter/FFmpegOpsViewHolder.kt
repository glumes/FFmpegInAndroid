package com.glumes.ffmpeginandroid.adapter

import android.view.View
import android.widget.TextView
import com.glumes.ffmpeginandroid.R
import com.glumes.widget.viewholders.ChildViewHolder

/**
 * @Author  glumes
 */

class FFmpegOpsViewHolder(itemView: View) : ChildViewHolder(itemView) {

    val title: TextView = itemView.findViewById(R.id.textview_ffmpeg_ops)
}