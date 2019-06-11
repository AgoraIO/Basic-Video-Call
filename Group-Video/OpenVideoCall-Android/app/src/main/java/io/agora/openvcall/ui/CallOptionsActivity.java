package io.agora.openvcall.ui;

import android.app.Activity;
import android.os.Bundle;
import android.support.v7.app.ActionBar;
import android.support.v7.app.AppCompatActivity;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.Switch;
import android.widget.TextView;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import io.agora.openvcall.R;
import io.agora.propeller.Constant;

public class CallOptionsActivity extends AppCompatActivity {

    private final static Logger log = LoggerFactory.getLogger(CallOptionsActivity.class);

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_call_options);

        ActionBar ab = getSupportActionBar();
        if (ab != null) {
            ab.setDisplayOptions(ActionBar.DISPLAY_SHOW_CUSTOM);
            ab.setCustomView(R.layout.ard_agora_actionbar_with_back_btn);
        }

        Switch debugSwitch = findViewById(R.id.debug_options);
        debugSwitch.setChecked(Constant.DEBUG_INFO_ENABLED);
        debugSwitch.setOnCheckedChangeListener(new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton buttonView, boolean withDebugInfo) {
                Constant.DEBUG_INFO_ENABLED = withDebugInfo;
            }
        });

        ((TextView) findViewById(R.id.ovc_page_title)).setText(R.string.label_options);
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
}
