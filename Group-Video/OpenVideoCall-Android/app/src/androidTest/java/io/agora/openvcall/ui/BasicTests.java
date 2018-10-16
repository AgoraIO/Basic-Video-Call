package io.agora.openvcall.ui;

import android.test.ActivityInstrumentationTestCase2;

import com.robotium.solo.Condition;
import com.robotium.solo.Solo;

import io.agora.openvcall.BuildConfig;
import io.agora.openvcall.R;

public class BasicTests extends ActivityInstrumentationTestCase2<MainActivity> {

    private Solo solo;

    public BasicTests() {
        super(MainActivity.class);
    }

    @Override
    public void setUp() throws Exception {
        solo = new Solo(getInstrumentation(), getActivity());
    }

    @Override
    public void tearDown() throws Exception {
        solo.finishOpenedActivities();
    }

    public String getString(int resId) {
        return solo.getString(resId);
    }

    public void testJoinChannel() throws Exception {
        String AUTO_TEST_CHANNEL_NAME = "for_auto_test_" + BuildConfig.VERSION_NAME + BuildConfig.VERSION_CODE;

        solo.unlockScreen();

        solo.assertCurrentActivity("Expected MainActivity activity", "MainActivity");
        solo.clearEditText(0);
        solo.enterText(0, AUTO_TEST_CHANNEL_NAME);
        solo.waitForText(AUTO_TEST_CHANNEL_NAME, 1, 2000L);

        solo.clickOnView(solo.getView(R.id.button_join));

        String targetActivity = ChatActivity.class.getSimpleName();

        solo.waitForLogMessage("onJoinChannelSuccess " + AUTO_TEST_CHANNEL_NAME, JOIN_CHANNEL_SUCCESS_THRESHOLD + 1000);

        solo.waitForLogMessage("onFirstLocalVideoFrame ", FIRST_LOCAL_VIDEO_SHOWN_THRESHOLD + 500);

        solo.assertCurrentActivity("Expected " + targetActivity + " activity", targetActivity);

        long firstRemoteVideoTs = System.currentTimeMillis();
        solo.waitForLogMessage("onFirstRemoteVideoDecoded ", FIRST_REMOTE_VIDEO_RECEIVED_THRESHOLD + 500);

        assertTrue("first remote video frame not received", System.currentTimeMillis() - firstRemoteVideoTs <= FIRST_REMOTE_VIDEO_RECEIVED_THRESHOLD);

        solo.waitForCondition(new Condition() { // stay at the channel for some time
            @Override
            public boolean isSatisfied() {
                return false;
            }
        }, FIRST_REMOTE_VIDEO_RECEIVED_THRESHOLD);
    }

    private static final int FIRST_REMOTE_VIDEO_RECEIVED_THRESHOLD = 60 * 1000;
    private static final int FIRST_LOCAL_VIDEO_SHOWN_THRESHOLD = 1500;
    private static final int JOIN_CHANNEL_SUCCESS_THRESHOLD = 5000;

}
