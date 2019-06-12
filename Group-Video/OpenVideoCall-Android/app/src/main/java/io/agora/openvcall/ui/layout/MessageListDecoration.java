package io.agora.openvcall.ui.layout;

import android.graphics.Rect;
import android.support.v7.widget.RecyclerView;
import android.view.View;

public class MessageListDecoration extends RecyclerView.ItemDecoration {

    private static final int divider = 12;
    private static final int header = 4;
    private static final int footer = 4;

    @Override
    public void getItemOffsets(Rect outRect, View view, RecyclerView parent, RecyclerView.State state) {
        int itemCount = parent.getAdapter().getItemCount();
        int viewPosition = parent.getChildAdapterPosition(view);

        outRect.left = divider;
        outRect.right = divider;

        if (viewPosition == 0) {
            outRect.top = header;
            outRect.bottom = divider / 2;
        } else if (viewPosition == itemCount - 1) {
            outRect.top = divider / 2;
            outRect.bottom = footer;
        } else {
            outRect.top = divider / 2;
            outRect.bottom = divider / 2;
        }
    }
}
