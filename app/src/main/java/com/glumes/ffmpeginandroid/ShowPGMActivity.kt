package com.glumes.ffmpeginandroid

import android.graphics.Bitmap
import android.os.Bundle
import android.support.v7.app.AppCompatActivity
import android.view.View
import com.glumes.ffmpeglib.utils.LogUtil
import com.glumes.util.FileConstants
import com.glumes.util.PGM
import kotlinx.android.synthetic.main.activity_show_pgm.*
import java.io.File


class ShowPGMActivity : AppCompatActivity(), View.OnClickListener {


    var num = 0
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        setContentView(R.layout.activity_show_pgm)

//        val path = FileConstants.PGM_DRI + File.separator + "test-300.pgm"
//        val iw: Int
//        val ih: Int
//        val pix: IntArray
//        val pgm = PGM()
//        pgm.readPGMHeader(path)
//        iw = pgm.width
//        ih = pgm.height
//
//        LogUtil.d("width is $iw height is $ih")
//
//        pix = pgm.readData(iw, ih, 5)   //P5-Gray image
//        val bitmap = Bitmap.createBitmap(iw, ih, Bitmap.Config.ARGB_4444)
//        bitmap.setPixels(pix, 0, iw, 0, 0, iw, ih)
//
//        image.setImageBitmap(bitmap)

        next.setOnClickListener(this)
    }


    override fun onClick(v: View?) {
        num++
        showImage(num)
    }


    fun showImage(num: Int) {
        val path = FileConstants.PGM_DRI + File.separator + "test-" + num
        val iw: Int
        val ih: Int
        val pix: IntArray
        val pgm = PGM()
        pgm.readPGMHeader(path)
        iw = pgm.width
        ih = pgm.height

        LogUtil.d("width is $iw height is $ih")

        pix = pgm.readData(iw, ih, 5)   //P5-Gray image
        val bitmap = Bitmap.createBitmap(iw, ih, Bitmap.Config.ARGB_4444)
        bitmap.setPixels(pix, 0, iw, 0, 0, iw, ih)
        image.setImageBitmap(bitmap)
    }


}
