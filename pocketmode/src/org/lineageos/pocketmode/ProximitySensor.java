/*
 * Copyright (c) 2016 The CyanogenMod Project
 * Copyright (c) 2018 The LineageOS Project
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

package org.lineageos.pocketmode;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.os.FileUtils;
import android.os.SystemProperties;
import android.util.Log;

import java.io.IOException;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;

public class ProximitySensor implements SensorEventListener {

    private static final boolean DEBUG = false;
    private static final String TAG = "PocketModeProximity";

    private final String FPC_FILE;

    private SensorManager mSensorManager;
    private Sensor mSensor;
    private Context mContext;
    private ExecutorService mExecutorService;

    public ProximitySensor(Context context) {
        mContext = context;
        mSensorManager = mContext.getSystemService(SensorManager.class);
        mSensor = mSensorManager.getDefaultSensor(Sensor.TYPE_PROXIMITY);
        mExecutorService = Executors.newSingleThreadExecutor();

        switch (SystemProperties.get("ro.lineage.device", "")) {
            case "cheeseburger":
                FPC_FILE = "/sys/devices/soc/soc:fpc_fpc1020/proximity_state";
                break;
            case "dumpling":
                FPC_FILE = "/sys/devices/soc/soc:goodix_fp/proximity_state";
                break;
            default:
                FPC_FILE = "";
                Log.e(TAG, "Device model for proximity state file not found!");
                break;
        }
    }

    @Override
    public void onSensorChanged(SensorEvent event) {
        setFPProximityState(event.values[0] < mSensor.getMaximumRange());
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int accuracy) {
        /* Empty */
    }

    private void setFPProximityState(boolean isNear) {
        try {
            FileUtils.stringToFile(FPC_FILE, isNear ? "1" : "0");
        } catch (IOException e) {
            Log.e(TAG, "Failed to write to " + FPC_FILE, e);
        }
    }

    protected void enable() {
        if (DEBUG) Log.d(TAG, "Enabling");
        mExecutorService.submit(() -> {
            mSensorManager.registerListener(this, mSensor, SensorManager.SENSOR_DELAY_NORMAL);
        });
    }

    protected void disable() {
        if (DEBUG) Log.d(TAG, "Disabling");
        mExecutorService.submit(() -> {
            mSensorManager.unregisterListener(this, mSensor);
            // Ensure FP is left enabled
            setFPProximityState(/* isNear */ false);
        });
    }
}
