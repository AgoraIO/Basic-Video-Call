package io.agora.openvcall.ui;

import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import android.support.v7.widget.GridLayoutManager;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Spinner;
import android.widget.TextView;

import io.agora.openvcall.R;
import io.agora.openvcall.model.ConstantApp;
import io.agora.openvcall.ui.layout.SettingsButtonDecoration;
import io.agora.openvcall.ui.layout.VideoEncResolutionAdapter;

public class SettingsActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_settings);

        ActionBar ab = getSupportActionBar();
        if (ab != null) {
            ab.setDisplayOptions(ActionBar.DISPLAY_SHOW_CUSTOM);
            ab.setCustomView(R.layout.ard_agora_actionbar_with_back_btn);
        }

        setupUI();
    }

    private void setupUI() {
        ((TextView) findViewById(R.id.ovc_page_title)).setText(R.string.label_settings);

        RecyclerView videoResolutionList = (RecyclerView) findViewById(R.id.settings_video_resolution);
        videoResolutionList.setHasFixedSize(true);
        videoResolutionList.addItemDecoration(new SettingsButtonDecoration());

        SharedPreferences pref = PreferenceManager.getDefaultSharedPreferences(this);
        int resolutionIdx = pref.getInt(ConstantApp.PrefManager.PREF_PROPERTY_VIDEO_ENC_RESOLUTION, ConstantApp.DEFAULT_VIDEO_ENC_RESOLUTION_IDX);
        int fpsIdx = pref.getInt(ConstantApp.PrefManager.PREF_PROPERTY_VIDEO_ENC_FPS, ConstantApp.DEFAULT_VIDEO_ENC_FPS_IDX);

        VideoEncResolutionAdapter videoResolutionAdapter = new VideoEncResolutionAdapter(this, resolutionIdx);
        videoResolutionAdapter.setHasStableIds(true);

        RecyclerView.LayoutManager layoutManager = new GridLayoutManager(this, 3, LinearLayoutManager.VERTICAL, false);
        videoResolutionList.setLayoutManager(layoutManager);

        videoResolutionList.setAdapter(videoResolutionAdapter);

        Spinner videoFpsSpinner = (Spinner) findViewById(R.id.settings_video_frame_rate);

        ArrayAdapter<CharSequence> videoFpsAdapter = ArrayAdapter.createFromResource(this,
                R.array.string_array_frame_rate, R.layout.simple_spinner_item_light);
        videoFpsAdapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);

        videoFpsSpinner.setAdapter(videoFpsAdapter);

        videoFpsSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                SharedPreferences pref = PreferenceManager.getDefaultSharedPreferences(getApplicationContext());
                SharedPreferences.Editor editor = pref.edit();
                editor.putInt(ConstantApp.PrefManager.PREF_PROPERTY_VIDEO_ENC_FPS, position);
                editor.apply();
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });

        videoFpsSpinner.setSelection(fpsIdx);
    }

    public void onBackPressed(View view) {
        onBackPressed();
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        return false;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        return super.onOptionsItemSelected(item);
    }

}
