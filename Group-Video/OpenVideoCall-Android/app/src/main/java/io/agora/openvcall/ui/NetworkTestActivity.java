package io.agora.openvcall.ui;

import android.os.Bundle;
import android.support.v7.app.ActionBar;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.TextView;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import io.agora.openvcall.R;
import io.agora.openvcall.model.BeforeCallEventHandler;
import io.agora.openvcall.model.ConstantApp;
import io.agora.rtc.IRtcEngineEventHandler;
import io.agora.rtc.internal.LastmileProbeConfig;

public class NetworkTestActivity extends BaseActivity implements BeforeCallEventHandler {

    private final static Logger log = LoggerFactory.getLogger(NetworkTestActivity.class);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_network_test);

        ActionBar ab = getSupportActionBar();
        if (ab != null) {
            ab.setDisplayOptions(ActionBar.DISPLAY_SHOW_CUSTOM);
            ab.setCustomView(R.layout.ard_agora_actionbar_with_back_btn);
        }
    }

    @Override
    protected void initUIandEvent() {
        event().addEventHandler(this);

        ((TextView) findViewById(R.id.ovc_page_title)).setText(R.string.label_network_testing);

        LastmileProbeConfig lastmileProbeConfig = new LastmileProbeConfig();
        lastmileProbeConfig.probeUplink = true;
        lastmileProbeConfig.probeDownlink = true;
        lastmileProbeConfig.expectedUplinkBitrate = 5000;
        lastmileProbeConfig.expectedDownlinkBitrate = 5000;
        rtcEngine().startLastmileProbeTest(lastmileProbeConfig);
    }

    @Override
    protected void deInitUIandEvent() {
        rtcEngine().stopLastmileProbeTest();
        event().removeEventHandler(this);
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        return false;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        return super.onOptionsItemSelected(item);
    }

    public void onBackPressed(View view) {
        onBackPressed();
    }

    @Override
    public void onLastmileQuality(final int quality) {
        this.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                String qualityInString = ConstantApp.getNetworkQualityDescription(quality);
                String networkResult = "onLastmileQuality quality: " + qualityInString;
                log.debug(networkResult);
                updateNetworkTestResult(qualityInString);
            }
        });
    }

    @Override
    public void onLastmileProbeResult(final IRtcEngineEventHandler.LastmileProbeResult result) {
        this.runOnUiThread(new Runnable() {
            @Override
            public void run() {
                String networkResult =
                        "onLastmileProbeResult state: " + result.state + " " + "rtt: " + result.rtt + "\n" +
                                "uplinkReport { packetLossRate: " + result.uplinkReport.packetLossRate + " " +
                                "jitter: " + result.uplinkReport.jitter + " " +
                                "availableBandwidth: " + result.uplinkReport.availableBandwidth + "}" + "\n" +
                                "downlinkReport { packetLossRate: " + result.downlinkReport.packetLossRate + " " +
                                "jitter: " + result.downlinkReport.jitter + " " +
                                "availableBandwidth: " + result.downlinkReport.availableBandwidth + "}";
                log.debug(networkResult);
                updateNetworkTestResult(result.rtt, result.uplinkReport.packetLossRate, result.downlinkReport.packetLossRate);
            }
        });
    }

    public void updateNetworkTestResult(String qualityInString) {
        ((TextView) findViewById(R.id.ovc_page_title)).setText(R.string.label_network_test_result);

        ((TextView) findViewById(R.id.network_test_quality)).setText(qualityInString);
    }

    public void updateNetworkTestResult(int rtt, int uplinkPacketLoss, int downlinkPacketLoss) {
        ((TextView) findViewById(R.id.ovc_page_title)).setText(R.string.label_network_test_result);

        ((TextView) findViewById(R.id.network_test_rtt)).setText(rtt + "ms");

        ((TextView) findViewById(R.id.network_test_uplink_packet_loss)).setText(uplinkPacketLoss + "%");

        ((TextView) findViewById(R.id.network_test_downlink_packet_loss)).setText(downlinkPacketLoss + "%");
    }
}
