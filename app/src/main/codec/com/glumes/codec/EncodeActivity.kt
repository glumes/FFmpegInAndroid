package com.glumes.codec


import com.glumes.base.BaseListActivity
import com.glumes.base.binder.BackOperationBinder
import com.glumes.base.binder.BackOpsItem
import com.glumes.ffmpeginandroid.R

class EncodeActivity : BaseListActivity() {


    override fun initData() {

    }

    override fun initAdapter() {
        mBindingAdapter
                .map(BackOpsItem::class.java, BackOperationBinder())
                .setItems(mItems)
    }

    override fun getToolbarTitle(): String {
        return getString(R.string.encode_ops)
    }
}
