package io.agora.openvcall.ui;

import android.content.Context;
import android.support.v7.widget.RecyclerView;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.TextView;
import io.agora.openvcall.R;

public class VideoProfileAdapter extends RecyclerView.Adapter {
    private Context context;

    private int selected;

    public VideoProfileAdapter(Context context, int selected) {
        this.context = context;
        this.selected = selected;
    }

    @Override
    public RecyclerView.ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
        View v = LayoutInflater.from(parent.getContext()).inflate(R.layout.video_profile_item, parent, false);
        // set the view's size, margins, paddings and layout parameters

        ProfileHolder ph = new ProfileHolder(v);
        return ph;
    }

    @Override
    public void onBindViewHolder(RecyclerView.ViewHolder holder, int position) {
        String resolution = context.getResources().getStringArray(R.array.string_array_resolutions)[position];
        ((ProfileHolder) holder).resolution.setText(resolution);

        String frameRate = context.getResources().getStringArray(R.array.string_array_frame_rate)[position];
        ((ProfileHolder) holder).frameRate.setText(frameRate);

        holder.itemView.setBackgroundResource(position == selected ? R.color.lightColorAccent : android.R.color.transparent);
    }

    @Override
    public int getItemCount() {
        return context.getResources().getStringArray(R.array.string_array_resolutions).length;
    }

    public class ProfileHolder extends RecyclerView.ViewHolder {
        public TextView resolution;
        public TextView frameRate;

        public ProfileHolder(View itemView) {
            super(itemView);

            resolution = (TextView) itemView.findViewById(R.id.resolution);
            frameRate = (TextView) itemView.findViewById(R.id.frame_rate);

            itemView.setOnClickListener(new View.OnClickListener() {
                @Override
                public void onClick(View v) {
                    selected = getLayoutPosition();
                    notifyDataSetChanged();
                }
            });
        }
    }

    public int getSelected() {
        return selected;
    }
}
