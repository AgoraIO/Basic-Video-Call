package io.agora.openvcall.ui.layout;

import android.app.Activity;
import android.content.Context;
import android.support.v7.widget.RecyclerView;
import android.util.DisplayMetrics;
import android.view.SurfaceView;
import android.view.ViewGroup;
import android.view.WindowManager;

import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.util.HashMap;

import io.agora.propeller.UserStatusData;

public class GridVideoViewContainerAdapter extends VideoViewAdapter {

    private final static Logger log = LoggerFactory.getLogger(GridVideoViewContainerAdapter.class);

    public GridVideoViewContainerAdapter(Activity activity, int localUid, HashMap<Integer, SurfaceView> uids) {
        super(activity, localUid, uids);
        log.debug("GridVideoViewContainerAdapter " + (mLocalUid & 0xFFFFFFFFL));
    }

    @Override
    protected void customizedInit(HashMap<Integer, SurfaceView> uids, boolean force) {
        VideoViewAdapterUtil.composeDataItem1(mUsers, uids, mLocalUid); // local uid

        if (force || mItemWidth == 0 || mItemHeight == 0) {
            WindowManager windowManager = (WindowManager) mContext.getSystemService(Context.WINDOW_SERVICE);
            DisplayMetrics outMetrics = new DisplayMetrics();
            windowManager.getDefaultDisplay().getMetrics(outMetrics);

            int count = uids.size();
            int DividerX = 1;
            int DividerY = 1;

            if (count == 2) {
                DividerY = 2;
            } else if (count >= 3) {
                DividerX = getNearestSqrt(count);
                DividerY = (int) Math.ceil(count * 1.f / DividerX);
            }

            int width = outMetrics.widthPixels;
            int height = outMetrics.heightPixels;

            if (width > height) {
                mItemWidth = width / DividerY;
                mItemHeight = height / DividerX;
            } else {
                mItemWidth = width / DividerX;
                mItemHeight = height / DividerY;
            }
        }
    }

    private int getNearestSqrt(int n) {
        return (int) Math.sqrt(n);
    }

    @Override
    public void notifyUiChanged(HashMap<Integer, SurfaceView> uids, int localUid, HashMap<Integer, Integer> status, HashMap<Integer, Integer> volume) {
        setLocalUid(localUid);

        VideoViewAdapterUtil.composeDataItem(mUsers, uids, localUid, status, volume, mVideoInfo);

        notifyDataSetChanged();
        if (DEBUG) {
            log.debug("notifyUiChanged " + (mLocalUid & 0xFFFFFFFFL) + " " + (localUid & 0xFFFFFFFFL) + " " + uids + " " + status + " " + volume);
        }
    }

    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        return super.onCreateViewHolder(parent, viewType);
    }

    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder holder, int position) {
        super.onBindViewHolder(holder, position);
    }

    @Override
    public int getItemCount() {
        return mUsers.size();
    }

    public UserStatusData getItem(int position) {
        return mUsers.get(position);
    }

    @Override
    public long getItemId(int position) {
        UserStatusData user = mUsers.get(position);

        SurfaceView view = user.mView;
        if (view == null) {
            throw new NullPointerException("SurfaceView destroyed for user " + (user.mUid & 0xFFFFFFFFL) + " " + user.mStatus + " " + user.mVolume);
        }

        return (String.valueOf(user.mUid) + System.identityHashCode(view)).hashCode();
    }
}
