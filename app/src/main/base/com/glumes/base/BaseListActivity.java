package com.glumes.base;

import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;

import com.glumes.databindingadapter.DataBindingAdapter;
import com.glumes.databindingadapter.Items;
import com.glumes.ffmpegexamples.base.BaseActivity;
import com.glumes.ffmpeginandroid.R;

import org.jetbrains.annotations.NotNull;

/**
 * Created by glumes on 17/04/2018
 */
public abstract class BaseListActivity extends BaseActivity {


    protected Items mItems;
    protected DataBindingAdapter mBindingAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home);

        mItems = new Items();
        mBindingAdapter = new DataBindingAdapter();

        initData();
        initAdapter();
        initRecyclerView();
    }

    protected abstract void initData();


    protected abstract void initAdapter();

    void initRecyclerView() {

        LinearLayoutManager layoutManager = new LinearLayoutManager(this);
        layoutManager.setOrientation(LinearLayoutManager.VERTICAL);

        RecyclerView recyclerView = findViewById(R.id.recyclerView);
        recyclerView.setLayoutManager(layoutManager);
        recyclerView.setAdapter(mBindingAdapter);
    }


    @NotNull
    @Override
    public String getToolbarTitle() {
        return getResources().getString(R.string.app_name);
    }
}