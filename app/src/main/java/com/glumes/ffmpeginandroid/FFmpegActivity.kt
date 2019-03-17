package com.glumes.ffmpeginandroid

import android.Manifest
import android.os.Bundle
import android.support.v7.widget.LinearLayoutManager
import com.glumes.ffmpegexamples.base.BaseActivity
import com.glumes.ffmpeginandroid.adapter.FFmpegAdapter
import com.glumes.ffmpeginandroid.ffmpeg.FFmpegBridge
import com.glumes.ffmpeginandroid.ffmpeg.FFmpegFactory
import com.tbruyelle.rxpermissions2.RxPermissions
import kotlinx.android.synthetic.main.activity_ffmpeg.*

class FFmpegActivity : BaseActivity() {

    override fun getToolbarTitle(): String {
        return getString(R.string.app_name)
    }

    private val rxPermissions = RxPermissions(this)

    private lateinit var mFFmpegAdapter: FFmpegAdapter

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContentView(R.layout.activity_ffmpeg)

        rxPermissions.request(
                Manifest.permission.WRITE_EXTERNAL_STORAGE,
                Manifest.permission.READ_EXTERNAL_STORAGE)
                .subscribe()

        val layoutManager = LinearLayoutManager(this)

        mFFmpegAdapter = FFmpegAdapter(FFmpegFactory.makeData())

        mFFmpegAdapter.setListener(FFmpegBridge())

        recyclerView.layoutManager = layoutManager
        recyclerView.adapter = mFFmpegAdapter

    }
}
