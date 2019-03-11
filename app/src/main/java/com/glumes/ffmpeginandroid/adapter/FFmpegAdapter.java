package com.glumes.ffmpeginandroid.adapter;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.glumes.ffmpeginandroid.R;
import com.glumes.ffmpeginandroid.listener.OnFFmpegOpsClickListener;
import com.glumes.widget.ExpandableRecyclerViewAdapter;
import com.glumes.widget.models.ExpandableGroup;

import java.util.List;

/**
 * @Author glumes
 */
public class FFmpegAdapter extends ExpandableRecyclerViewAdapter<FFmpegTypeViewHolder, FFmpegOpsViewHolder> {


    private OnFFmpegOpsClickListener listener;


    public FFmpegAdapter(List<? extends ExpandableGroup> groups) {
        super(groups);
    }


    @Override
    public FFmpegTypeViewHolder onCreateGroupViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.item_ffmpeg_type, parent, false);

        return new FFmpegTypeViewHolder(view);
    }

    @Override
    public FFmpegOpsViewHolder onCreateChildViewHolder(ViewGroup parent, int viewType) {
        View view = LayoutInflater.from(parent.getContext()).inflate(R.layout.item_ffmpeg_ops, parent, false);
        return new FFmpegOpsViewHolder(view);
    }

    @Override
    public void onBindChildViewHolder(FFmpegOpsViewHolder holder, int flatPosition, ExpandableGroup group, int childIndex) {

        FFmpegOps ops = ((FFmpegType) group).getItems().get(childIndex);
        holder.getTitle().setText(ops.getTitle());
        holder.itemView.setOnClickListener(v -> {
            if (listener != null) {
                listener.onFFmpegOpsClick(ops);
            }
        });
    }

    @Override
    public void onBindGroupViewHolder(FFmpegTypeViewHolder holder, int flatPosition, ExpandableGroup group) {
        holder.getTitle().setText(group.getTitle());

    }

    public void setListener(OnFFmpegOpsClickListener listener) {
        this.listener = listener;
    }
}
