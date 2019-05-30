package io.agora.openvcall.ui;

import android.content.Intent;
import android.os.Bundle;
import android.text.Editable;
import android.text.TextUtils;
import android.text.TextWatcher;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.Spinner;
import android.widget.TextView;

import io.agora.openvcall.R;
import io.agora.openvcall.model.ConstantApp;
import io.agora.openvcall.model.BeforeCallEventHandler;
import io.agora.rtc.IRtcEngineEventHandler;
import io.agora.rtc.internal.LastmileProbeConfig;

public class MainActivity extends BaseActivity implements BeforeCallEventHandler {

    private TextView tvLastmileQualityResult;
    private TextView tvLastmileProbeResult;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        tvLastmileQualityResult = (TextView) findViewById(R.id.tv_lastmile_quality_result);
        tvLastmileProbeResult = (TextView) findViewById(R.id.tv_lastmile_Probe_result);
    }

    @Override
    protected void initUIandEvent() {
        resetLastMileInfo();
        EditText v_channel = (EditText) findViewById(R.id.channel_name);
        v_channel.addTextChangedListener(new TextWatcher() {
            @Override
            public void beforeTextChanged(CharSequence s, int start, int count, int after) {

            }

            @Override
            public void onTextChanged(CharSequence s, int start, int before, int count) {

            }

            @Override
            public void afterTextChanged(Editable s) {
                boolean isEmpty = TextUtils.isEmpty(s.toString());
                findViewById(R.id.button_join).setEnabled(!isEmpty);
            }
        });

        Spinner encryptionSpinner = (Spinner) findViewById(R.id.encryption_mode);
        ArrayAdapter<CharSequence> adapter = ArrayAdapter.createFromResource(this,
                R.array.encryption_mode_values, android.R.layout.simple_spinner_item);
        adapter.setDropDownViewResource(android.R.layout.simple_spinner_dropdown_item);
        encryptionSpinner.setAdapter(adapter);

        encryptionSpinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
            @Override
            public void onItemSelected(AdapterView<?> parent, View view, int position, long id) {
                vSettings().mEncryptionModeIndex = position;
            }

            @Override
            public void onNothingSelected(AdapterView<?> parent) {

            }
        });

        encryptionSpinner.setSelection(vSettings().mEncryptionModeIndex);

        String lastChannelName = vSettings().mChannelName;
        if (!TextUtils.isEmpty(lastChannelName)) {
            v_channel.setText(lastChannelName);
            v_channel.setSelection(lastChannelName.length());
        }

        EditText v_encryption_key = (EditText) findViewById(R.id.encryption_key);
        String lastEncryptionKey = vSettings().mEncryptionKey;
        if (!TextUtils.isEmpty(lastEncryptionKey)) {
            v_encryption_key.setText(lastEncryptionKey);
        }
    }

    @Override
    protected void deInitUIandEvent() {
        event().removeEventHandler(this);
    }

    @Override
    public boolean onCreateOptionsMenu(final Menu menu) {
        MenuInflater inflater = getMenuInflater();
        inflater.inflate(R.menu.menu_main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        // Handle presses on the action bar items
        switch (item.getItemId()) {
            case R.id.action_settings:
                forwardToSettings();
                return true;
            default:
                return super.onOptionsItemSelected(item);
        }
    }

    public void onClickJoin(View view) {
        forwardToRoom();
    }

    public void forwardToRoom() {
        EditText v_channel = (EditText) findViewById(R.id.channel_name);
        String channel = v_channel.getText().toString();
        vSettings().mChannelName = channel;

        EditText v_encryption_key = (EditText) findViewById(R.id.encryption_key);
        String encryption = v_encryption_key.getText().toString();
        vSettings().mEncryptionKey = encryption;

        Intent i = new Intent(MainActivity.this, ChatActivity.class);
        i.putExtra(ConstantApp.ACTION_KEY_CHANNEL_NAME, channel);
        i.putExtra(ConstantApp.ACTION_KEY_ENCRYPTION_KEY, encryption);
        i.putExtra(ConstantApp.ACTION_KEY_ENCRYPTION_MODE, getResources().getStringArray(R.array.encryption_mode_values)[vSettings().mEncryptionModeIndex]);

        startActivity(i);
    }

    public void forwardToSettings() {
        Intent i = new Intent(this, SettingsActivity.class);
        startActivity(i);
    }

    public void onLastMileClick(View view) {
        boolean enableLastMileProbeTest = ((CheckBox) view).isChecked();
        if (enableLastMileProbeTest) {
            if (worker().getRtcEngine() != null) {
                LastmileProbeConfig lastmileProbeConfig = new LastmileProbeConfig();
                lastmileProbeConfig.probeUplink = true;
                lastmileProbeConfig.probeDownlink = true;
                lastmileProbeConfig.expectedUplinkBitrate = 5000;
                lastmileProbeConfig.expectedDownlinkBitrate = 5000;
                int result = worker().getRtcEngine().startLastmileProbeTest(lastmileProbeConfig);
            }
        } else {
            if (worker().getRtcEngine() != null) {
                worker().getRtcEngine().stopLastmileProbeTest();
                resetLastMileInfo();
            }
        }
    }

    @Override
    public void workerThreadReady() {
        event().addEventHandler(this);
    }

    @Override
    public void onLastmileQuality(final int quality) {
        this.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                tvLastmileQualityResult.setText(
                        "onLastmileQuality " + quality);
            }
        });

    }

    @Override
    public void onLastmileProbeResult(final IRtcEngineEventHandler.LastmileProbeResult result) {
        this.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                tvLastmileProbeResult.setText(
                        "onLastmileProbeResult state:" + result.state + " " + "rtt:" + result.rtt + "\n" +
                                "uplinkReport { packetLossRate:" + result.uplinkReport.packetLossRate + " " +
                                "jitter:" + result.uplinkReport.jitter + " " +
                                "availableBandwidth:" + result.uplinkReport.availableBandwidth + "}" + "\n" +
                                "downlinkReport { packetLossRate:" + result.downlinkReport.packetLossRate + " " +
                                "jitter:" + result.downlinkReport.jitter + " " +
                                "availableBandwidth:" + result.downlinkReport.availableBandwidth + "}");
            }
        });
    }

    private void resetLastMileInfo() {
        tvLastmileQualityResult.setText("");
        tvLastmileProbeResult.setText("");
    }


}
