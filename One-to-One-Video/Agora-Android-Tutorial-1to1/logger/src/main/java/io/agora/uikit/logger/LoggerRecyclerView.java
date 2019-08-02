package io.agora.uikit.logger;

import android.content.Context;
import android.graphics.Rect;
import android.support.annotation.NonNull;
import android.support.annotation.Nullable;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.util.AttributeSet;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.LinearLayout;
import android.widget.TextView;

import java.util.ArrayList;
import java.util.List;

public class LoggerRecyclerView extends RecyclerView {
    private static final int MAX_ITEMS = 10;

    private LogRecyclerViewAdapter mAdapter = new LogRecyclerViewAdapter();
    private MarginDecoration mDecoration = new MarginDecoration();
    private LayoutInflater mInflater;
    private List<LogItem> mLogList = new ArrayList<>();
    private int mVerticalSpacing = 0;

    public LoggerRecyclerView(@NonNull Context context) {
        super(context);
        init(context);
    }

    public LoggerRecyclerView(@NonNull Context context, @Nullable AttributeSet attrs) {
        super(context, attrs);
        init(context);
    }

    public LoggerRecyclerView(@NonNull Context context, @Nullable AttributeSet attrs, int defStyle) {
        super(context, attrs, defStyle);
        init(context);
    }

    private void init(Context context) {
        mInflater = LayoutInflater.from(context);
        setAdapter(mAdapter);
        setLayout(context);
    }

    private void setLayout(Context context) {
        LinearLayoutManager manager = new LinearLayoutManager(context);
        manager.setOrientation(LinearLayoutManager.VERTICAL);
        setLayoutManager(manager);
        addItemDecoration(mDecoration);
        mVerticalSpacing = getOffset(context);
    }

    private int getOffset(Context context) {
        return (int) context.getResources().getDimension(R.dimen.log_item_offset);
    }

    @Nullable
    @Override
    public LogRecyclerViewAdapter getAdapter() {
        return mAdapter;
    }

    public void logI(String text) {
        mAdapter.logI(text);
    }

    public void logW(String text) {
        mAdapter.logW(text);
    }

    public void logE(String text) {
        mAdapter.logE(text);
    }

    private enum LogLevel {
        ERROR, WARN, INFO
    }

    public static class LogItem {
        LogLevel level;
        String text;

        LogItem(LogLevel level, String text) {
            this.level = level;
            this.text = text;
        }
    }

    public class LogRecyclerViewAdapter extends RecyclerView.Adapter<ViewHolder> {

        @NonNull
        @Override
        public ViewHolder onCreateViewHolder(@NonNull ViewGroup viewGroup, int position) {
            View view = mInflater.inflate(R.layout.log_item_layout, viewGroup, false);
            return new ViewHolder(view);
        }

        @Override
        public void onBindViewHolder(@NonNull ViewHolder viewHolder, int position) {
            viewHolder.update(mLogList.get(position));
        }

        @Override
        public int getItemCount() {
            return mLogList.size();
        }

        public void logI(String text) {
            addLogItem(LogLevel.INFO, text);
        }

        public void logW(String text) {
            addLogItem(LogLevel.WARN, text);
        }

        public void logE(String text) {
            addLogItem(LogLevel.ERROR, text);
        }

        private void addLogItem(LogLevel level, String text) {
            LogItem item = new LogItem(level, text);
            if (mLogList.size() == MAX_ITEMS) {
                mLogList.remove(0);
            }
            mLogList.add(item);
            scrollToPosition(mLogList.size() - 1);
            notifyDataSetChanged();
        }
    }

    private class ViewHolder extends RecyclerView.ViewHolder {
        LinearLayout layout;
        TextView text;

        ViewHolder(@NonNull View itemView) {
            super(itemView);
            layout = itemView.findViewById(R.id.log_back);
            text = itemView.findViewById(R.id.log_text);
        }

        void update(LogItem item) {
            int backRes = -1;
            switch (item.level) {
                case INFO:
                    backRes = R.drawable.log_back_info;
                    break;
                case WARN:
                    backRes = R.drawable.log_back_warn;
                    break;
                case ERROR:
                    backRes = R.drawable.log_back_error;
            }

            layout.setBackgroundResource(backRes);
            text.setText(item.text);
        }
    }

    private class MarginDecoration extends ItemDecoration {
        @Override
        public void getItemOffsets(@NonNull Rect outRect, @NonNull View view,
                                   @NonNull RecyclerView parent, @NonNull State state) {
            outRect.top = mVerticalSpacing;
            outRect.bottom = mVerticalSpacing;
        }
    }
}
