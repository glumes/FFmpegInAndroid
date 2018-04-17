package com.glumes.ffmpegexamples.base

import android.annotation.SuppressLint
import android.graphics.Color
import android.os.Bundle
import android.support.v7.widget.Toolbar
import com.glumes.sampleutil.BaseToolbarActivity

abstract class BaseActivity : BaseToolbarActivity() {


    @SuppressLint("MissingSuperCall")
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
    }

    override fun setUpToolbar(toolbar: Toolbar) {
        toolbar.setTitleTextColor(Color.WHITE)
        toolbar.title = getToolbarTitle()
    }

    abstract fun getToolbarTitle(): String

}
