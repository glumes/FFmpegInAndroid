package com.glumes.ffmpeginandroid

import android.os.Bundle
import android.os.Parcelable
import android.support.v7.app.AppCompatActivity
import android.support.v7.widget.LinearLayoutManager
import android.support.v7.widget.Toolbar
import android.util.Log
import com.glumes.ffmpegexamples.base.BaseActivity
import com.glumes.ffmpeginandroid.GenreDataFactory.makeGenres
import com.glumes.sampleutil.BaseToolbarActivity
import com.glumes.widget.models.ExpandableGroup
import kotlinx.android.synthetic.main.activity_ffmpeg.*

class FFmpegActivity : AppCompatActivity() {

//    override fun setUpToolbar(toolbar: Toolbar) {
//        TODO("not implemented") //To change body of created functions use File | Settings | File Templates.
//    }

    private lateinit var mFFmpegAdapter: FFmpegAdapter

//    override fun getToolbarTitle(): String {
//        return resources.getString(R.string.app_name)
//    }

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_ffmpeg)
        val layoutManager = LinearLayoutManager(this)

//
//        mFFmpegAdapter = FFmpegAdapter2(FFmpegFactory.makeData() as MutableList<out ExpandableGroup<Parcelable>>)

        mFFmpegAdapter = FFmpegAdapter(makeGenres())


        recyclerView.layoutManager = layoutManager
        recyclerView.adapter = mFFmpegAdapter

        Log.d("adapter", "log num is ${mFFmpegAdapter.itemCount}")
    }
}
