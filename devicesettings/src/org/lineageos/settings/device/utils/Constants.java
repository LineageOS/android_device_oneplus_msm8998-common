/*
 * Copyright (C) 2015 The CyanogenMod Project
 * Copyright (C) 2017 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package org.lineageos.settings.device.utils;

import java.util.HashMap;
import java.util.Map;

import android.content.Context;
import android.content.SharedPreferences;
import android.support.v14.preference.SwitchPreference;
import android.support.v7.preference.PreferenceManager;

public class Constants {

    // Preference keys
    public static final String OCLICK_CONNECT_KEY = "oclick_connect";
    public static final String OCLICK_DEVICE_ADDRESS_KEY = "oclick_device_address";
    public static final String OCLICK_SNAPSHOT_KEY = "oclick_take_snapshot";
    public static final String OCLICK_FIND_PHONE_KEY = "oclick_find_my_phone";
    public static final String OCLICK_FENCE_KEY = "oclick_fence";
    public static final String OCLICK_DISCONNECT_ALERT_KEY = "oclick_disconnect_alert";
    public static final String NOTIF_SLIDER_TOP_KEY = "keycode_top_position";
    public static final String NOTIF_SLIDER_MIDDLE_KEY = "keycode_middle_position";
    public static final String NOTIF_SLIDER_BOTTOM_KEY = "keycode_bottom_position";

    // Button nodes
    public static final String NOTIF_SLIDER_TOP_NODE = "/proc/tri-state-key/keyCode_top";
    public static final String NOTIF_SLIDER_MIDDLE_NODE = "/proc/tri-state-key/keyCode_middle";
    public static final String NOTIF_SLIDER_BOTTOM_NODE = "/proc/tri-state-key/keyCode_bottom";

    // Holds <preference_key> -> <proc_node> mapping
    public static final Map<String, String> sBooleanNodePreferenceMap = new HashMap<>();
    public static final Map<String, String> sStringNodePreferenceMap = new HashMap<>();

    // Holds <preference_key> -> <default_values> mapping
    public static final Map<String, Object> sNodeDefaultMap = new HashMap<>();

    public static final String[] sButtonPrefKeys = {
        NOTIF_SLIDER_TOP_KEY,
        NOTIF_SLIDER_MIDDLE_KEY,
        NOTIF_SLIDER_BOTTOM_KEY
    };

    static {
        sStringNodePreferenceMap.put(NOTIF_SLIDER_TOP_KEY, NOTIF_SLIDER_TOP_NODE);
        sStringNodePreferenceMap.put(NOTIF_SLIDER_MIDDLE_KEY, NOTIF_SLIDER_MIDDLE_NODE);
        sStringNodePreferenceMap.put(NOTIF_SLIDER_BOTTOM_KEY, NOTIF_SLIDER_BOTTOM_NODE);

        sNodeDefaultMap.put(NOTIF_SLIDER_TOP_KEY, "601");
        sNodeDefaultMap.put(NOTIF_SLIDER_MIDDLE_KEY, "602");
        sNodeDefaultMap.put(NOTIF_SLIDER_BOTTOM_KEY, "603");

        sNodeDefaultMap.put(OCLICK_FENCE_KEY, true);
        sNodeDefaultMap.put(OCLICK_DISCONNECT_ALERT_KEY, true);
    }

    public static boolean isPreferenceEnabled(Context context, String key) {
        SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(context);
        return preferences.getBoolean(key, (Boolean) sNodeDefaultMap.get(key));
    }

    public static String getPreferenceString(Context context, String key) {
        SharedPreferences preferences = PreferenceManager.getDefaultSharedPreferences(context);
        return preferences.getString(key, (String) sNodeDefaultMap.get(key));
    }
}
