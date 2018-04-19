package com.glumes.codec


import com.glumes.base.BaseListActivity
import com.glumes.base.binder.BackOperationBinder
import com.glumes.base.binder.BackOpsItem
import com.glumes.ffmpeginandroid.R
import com.glumes.util.OPS_TYPE_ENCODE
import com.glumes.util.TYPE_ENCODE_YUV_TO_H264

class EncodeActivity : BaseListActivity() {


    override fun initData() {
        mItems.add(BackOpsItem("YUV Encode TO H264 ", OPS_TYPE_ENCODE, TYPE_ENCODE_YUV_TO_H264))
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
