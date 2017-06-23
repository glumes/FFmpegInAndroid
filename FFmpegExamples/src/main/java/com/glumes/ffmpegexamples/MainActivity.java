package com.glumes.ffmpegexamples;

import android.databinding.DataBindingUtil;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;

import com.glumes.ffmpegexamples.databinding.ActivityMainBinding;
import com.glumes.ffmpeglib.FFmpegEvent;

public class MainActivity extends AppCompatActivity {


    ActivityMainBinding mBinding;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        mBinding = DataBindingUtil.setContentView(this, R.layout.activity_main);
        mBinding.setClickHandler(new FFmpegEvent());
    }


}
