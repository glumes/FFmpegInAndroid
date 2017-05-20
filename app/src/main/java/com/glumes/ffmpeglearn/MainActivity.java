package com.glumes.ffmpeglearn;

import android.os.Bundle;
import android.os.Environment;
import android.support.v7.app.AppCompatActivity;
import android.widget.Button;

import com.glumes.ffmpeglib.SimpleDecoder;

import java.io.File;

import butterknife.BindView;
import butterknife.ButterKnife;
import butterknife.OnClick;

public class MainActivity extends AppCompatActivity {


    @BindView(R.id.button)
    Button button;

    String fileName = "sintel.mp4";

    String targetFileName = "sintel.yuv";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        ButterKnife.bind(this);
    }

    @OnClick(R.id.button)
    public void onViewClicked() {
        String folderurl = Environment.getExternalStorageDirectory().getAbsolutePath()
                + File.pathSeparator + fileName;

        String outputUrl = Environment.getExternalStorageDirectory().getAbsolutePath()
                + File.pathSeparator + targetFileName;

        SimpleDecoder.decode(folderurl,outputUrl);
    }
}
