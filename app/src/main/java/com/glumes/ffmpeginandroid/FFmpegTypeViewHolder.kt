package com.glumes.ffmpeginandroid

import android.view.View
import android.widget.TextView
import com.glumes.widget.viewholders.GroupViewHolder

/**
 * @Author  glumes
 */
class FFmpegTypeViewHolder(itemView: View) : GroupViewHolder(itemView) {

    val mTextView: TextView = itemView.findViewById(R.id.textview_ffmpeg_type)

    override fun onClick(v: View?) {
        super.onClick(v)
    }

    override fun expand() {
        super.expand()
    }

    override fun collapse() {
        super.collapse()
    }
}