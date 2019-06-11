package io.agora.openvcall.ui.layout;

import android.graphics.Rect;
import android.support.v7.widget.RecyclerView;
import android.view.View;

public class SmallVideoViewDecoration extends RecyclerView.ItemDecoration {

    private static final int divider = 12;
    private static final int header = 10;
    private static final int footer = 10;

    @Override
    public void getItemOffsets(Rect outRect, View view, RecyclerView parent, RecyclerView.State state) {
        int itemCount = parent.getAdapter().getItemCount();
        int viewPosition = parent.getChildAdapterPosition(view);

        if (viewPosition == 0) {
            outRect.left = header;
            outRect.right = divider / 2;
        } else if (viewPosition == itemCount - 1) {
            outRect.left = divider / 2;
            outRect.right = footer;
        } else {
            outRect.left = divider / 2;
            outRect.right = divider / 2;
        }
    }
}
