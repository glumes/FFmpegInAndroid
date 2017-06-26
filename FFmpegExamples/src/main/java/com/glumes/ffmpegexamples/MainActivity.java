package com.glumes.ffmpegexamples;

import android.databinding.DataBindingUtil;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.glumes.ffmpegexamples.databinding.ActivityMainBinding;
import com.glumes.ffmpeglib.FFmpegEvent;

import java.io.File;
import java.io.Flushable;

import timber.log.Timber;

public class MainActivity extends AppCompatActivity {


    ActivityMainBinding mBinding;


//    String mpegFilePath = Environment.getExternalStorageDirectory() + File.separator + Constants.MEGP_1_FILE_NAME;

    String targFilePath = Environment.getExternalStorageDirectory() + File.separator + "test.pgm";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mBinding = DataBindingUtil.setContentView(this, R.layout.activity_main);
        mBinding.setClickHandler(new FFmpegEvent());
    }

}
