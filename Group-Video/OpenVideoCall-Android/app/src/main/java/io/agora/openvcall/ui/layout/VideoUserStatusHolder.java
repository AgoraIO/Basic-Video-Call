package io.agora.openvcall.ui.layout;

import android.support.v7.widget.RecyclerView;
import android.view.View;
import android.widget.ImageView;
import android.widget.LinearLayout;
import android.widget.RelativeLayout;
import android.widget.TextView;
import io.agora.openvcall.R;

public class VideoUserStatusHolder extends RecyclerView.ViewHolder {
    public final RelativeLayout mMaskView;

    public final ImageView mAvatar;
    public final ImageView mIndicator;

    public final LinearLayout mVideoInfo;

    public final TextView mMetaData;

    public VideoUserStatusHolder(View v) {
        super(v);

        mMaskView = (RelativeLayout) v.findViewById(R.id.user_control_mask);
        mAvatar = (ImageView) v.findViewById(R.id.default_avatar);
        mIndicator = (ImageView) v.findViewById(R.id.indicator);

        mVideoInfo = (LinearLayout) v.findViewById(R.id.video_info_container);

        mMetaData = (TextView) v.findViewById(R.id.video_info_metadata);
    }
}
