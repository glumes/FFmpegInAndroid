package com.glumes.codec

import android.os.Bundle
import com.glumes.base.BaseListActivity
import com.glumes.base.binder.BackOperationBinder
import com.glumes.base.binder.BackOpsItem
import com.glumes.ffmpeginandroid.R

class DecodeActivity : BaseListActivity() {


    override fun initData() {
        mItems.add(BackOpsItem("MPEG TO YUV DECODE", 0))
    }

    override fun initAdapter() {
        mBindingAdapter
                .map(BackOpsItem::class.java, BackOperationBinder())
                .setItems(mItems)
    }

    override fun getToolbarTitle(): String {
        return getString(R.string.decode_ops)
    }

}
