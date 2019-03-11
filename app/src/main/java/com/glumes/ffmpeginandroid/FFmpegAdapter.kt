package com.glumes.ffmpeginandroid

import android.os.Parcelable
import android.view.LayoutInflater
import android.view.ViewGroup
import com.glumes.widget.ExpandableRecyclerViewAdapter
import com.glumes.widget.models.ExpandableGroup

/**
 * @Author  glumes
 */
class FFmpegAdapter2(groups: MutableList<out ExpandableGroup<Parcelable>>?) : ExpandableRecyclerViewAdapter<FFmpegTypeViewHolder, FFmpegOpsViewHolder>(groups) {

    override fun onCreateGroupViewHolder(parent: ViewGroup, viewType: Int): FFmpegTypeViewHolder {
        val view = LayoutInflater.from(parent.context).inflate(R.layout.item_ffmpeg_type, parent, false)
        return FFmpegTypeViewHolder(view)
    }

    override fun onCreateChildViewHolder(parent: ViewGroup, viewType: Int): FFmpegOpsViewHolder {
        val view = LayoutInflater.from(parent.context).inflate(R.layout.item_ffmpeg_ops, parent, false)
        return FFmpegOpsViewHolder(view)
    }

    override fun onBindChildViewHolder(holder: FFmpegOpsViewHolder?, flatPosition: Int, group: ExpandableGroup<*>?, childIndex: Int) {
    }

    override fun onBindGroupViewHolder(holder: FFmpegTypeViewHolder?, flatPosition: Int, group: ExpandableGroup<*>?) {

    }
}