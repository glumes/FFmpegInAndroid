package com.glumes.codec

import android.os.Bundle
import com.glumes.base.BaseListActivity
import com.glumes.base.binder.BackOperationBinder
import com.glumes.base.binder.BackOpsItem
import com.glumes.ffmpeginandroid.R
import com.glumes.util.FileConstants
import com.glumes.util.OPS_TYPE_DECODE
import com.glumes.util.TYPE_DECODE_MPEG_TO_PGM
import java.io.File

class DecodeActivity : BaseListActivity() {


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        val file = File(FileConstants.PGM_DRI)
        if (!file.exists()) {
            file.mkdir()
        }

        val outFile = File(FileConstants.OUT_PUT_FILE)
        if (!outFile.exists()) {
            outFile.createNewFile()
        }
    }


    override fun initData() {
        mItems.add(BackOpsItem("MPEG TO YUV DECODE", OPS_TYPE_DECODE, TYPE_DECODE_MPEG_TO_PGM))
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
