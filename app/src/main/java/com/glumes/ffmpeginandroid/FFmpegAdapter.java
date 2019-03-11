package com.glumes.ffmpeginandroid;

import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;

import com.glumes.widget.ExpandableRecyclerViewAdapter;
import com.glumes.widget.models.ExpandableGroup;
import com.glumes.widget.models.ExpandableListPosition;

import java.util.List;

/**
 * @Author glumes
 */
public class FFmpegAdapter extends ExpandableRecyclerViewAdapter<FFmpegTypeViewHolder, FFmpegOpsViewHolder> {

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
    
    }

    @Override
    public void onBindGroupViewHolder(FFmpegTypeViewHolder holder, int flatPosition, ExpandableGroup group) {

    }
}
