package com.glumes.ffmpeginandroid;

import android.os.Bundle;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;

import com.glumes.base.binder.BasicItem;
import com.glumes.base.binder.BasicItemBinder;
import com.glumes.databindingadapter.DataBindingAdapter;
import com.glumes.databindingadapter.Items;
import com.glumes.ffmpegexamples.base.BaseActivity;

import org.jetbrains.annotations.NotNull;

/**
 * @author glumes
 */
public class MainActivity extends BaseActivity {


    private Items mItems;
    private DataBindingAdapter mBindingAdapter;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_home);

        initData();
        initAdapter();
        initRecyclerView();
    }

    private void initData() {
        mItems = new Items();
        mItems.add(new BasicItem("编解码操作", 0));
    }


    private void initAdapter() {
        mBindingAdapter = new DataBindingAdapter();
        mBindingAdapter.map(BasicItem.class, new BasicItemBinder())
                .setItems(mItems);
    }

    private void initRecyclerView() {

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
