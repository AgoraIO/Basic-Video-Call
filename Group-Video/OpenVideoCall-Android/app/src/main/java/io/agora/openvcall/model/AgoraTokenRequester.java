package io.agora.openvcall.model;

import org.json.JSONException;
import org.json.JSONObject;
import org.slf4j.Logger;
import org.slf4j.LoggerFactory;

import java.io.IOException;

import okhttp3.OkHttpClient;

public class AgoraTokenRequester {
    public static String tokenURL;
    public static String fetchToken(String urlBase, String channelName, int userId) {
        Logger log = LoggerFactory.getLogger("AgoraTokenRequester");

        OkHttpClient client = new OkHttpClient();
        String url = urlBase + "/rtc/" + channelName + "/publisher/uid/" + userId + "/";

        okhttp3.Request request = new okhttp3.Request.Builder()
                .url(url)
                .method("GET", null)
                .build();

        try (okhttp3.Response response = client.newCall(request).execute()) {
            if (!response.isSuccessful()) {
                log.debug("Unexpected code " + response);
            } else {
                JSONObject jObject = new JSONObject(response.body().string());
                return jObject.getString("rtcToken");
            }
        } catch (IOException | JSONException e) {
            e.printStackTrace();
        }
        return "";
    }
}
