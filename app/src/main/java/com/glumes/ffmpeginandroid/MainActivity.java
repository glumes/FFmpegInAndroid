package com.glumes.ffmpeginandroid;

import com.glumes.base.BaseListActivity;
import com.glumes.base.binder.BasicItem;
import com.glumes.base.binder.BasicItemBinder;
import com.glumes.base.binder.JumpActivityBinder;
import com.glumes.base.binder.JumpActivityItem;
import com.glumes.codec.DecodeActivity;
import com.glumes.codec.EncodeActivity;

import org.jetbrains.annotations.NotNull;

/**
 * @author glumes
 */
public class MainActivity extends BaseListActivity {


    @Override
    protected void initData() {
        mItems.add(new BasicItem("编解码操作"));
        mItems.add(new JumpActivityItem("编码操作", EncodeActivity.class));
        mItems.add(new JumpActivityItem("解码操作", DecodeActivity.class));
        mItems.add(new JumpActivityItem("图片预览", ShowPGMActivity.class));
    }

    @Override
    protected void initAdapter() {
        mBindingAdapter
                .map(BasicItem.class, new BasicItemBinder())
                .map(JumpActivityItem.class, new JumpActivityBinder())
                .setItems(mItems);
    }

    @NotNull
    @Override
    public String getToolbarTitle() {
        return getResources().getString(R.string.app_name);
    }
}
