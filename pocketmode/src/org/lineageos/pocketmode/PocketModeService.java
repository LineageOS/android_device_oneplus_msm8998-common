/*
 * Copyright (c) 2016 The CyanogenMod Project
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

import android.app.Service;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.database.ContentObserver;
import android.os.Handler;
import android.os.IBinder;
import android.os.UserHandle;
import android.util.Log;

import lineageos.providers.LineageSettings;

public class PocketModeService extends Service {

    private static final String TAG = "PocketModeService";
    private static final boolean DEBUG = false;

    private ProximitySensor mProximitySensor;
    private SettingsObserver mSettingsObserver;

    @Override
    public void onCreate() {
        if (DEBUG) Log.d(TAG, "Creating service");
        mProximitySensor = new ProximitySensor(this);
        mSettingsObserver = new SettingsObserver(new Handler());
        mSettingsObserver.register();
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        if (DEBUG) Log.d(TAG, "Starting service");
        return START_STICKY;
    }

    @Override
    public void onDestroy() {
        if (DEBUG) Log.d(TAG, "Destroying service");
        this.unregisterReceiver(mScreenStateReceiver);
        mProximitySensor.disable();
        mSettingsObserver.unregister();
        super.onDestroy();
    }

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    private BroadcastReceiver mScreenStateReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(Intent.ACTION_SCREEN_ON)) {
                if (DEBUG) Log.d(TAG, "Display on");
                mProximitySensor.disable();
            } else if (intent.getAction().equals(Intent.ACTION_SCREEN_OFF)) {
                if (DEBUG) Log.d(TAG, "Display off");
                mProximitySensor.enable();
            }
        }
    };

    private final class SettingsObserver extends ContentObserver {
        private boolean mIsRegistered = false;

        private SettingsObserver(Handler handler) {
            super(handler);
        }

        public void register() {
            getContentResolver().registerContentObserver(LineageSettings.System.getUriFor(
                    LineageSettings.System.PROXIMITY_ON_WAKE), false, this);

            update();
        }

        public void unregister() {
            getContentResolver().unregisterContentObserver(this);
        }

        @Override
        public void onChange(boolean selfChange) {
            update();
        }

        private void update() {
            boolean defaultProximity = getResources().getBoolean(
                    org.lineageos.platform.internal.R.bool.config_proximityCheckOnWakeEnabledByDefault);
            boolean proximityWakeCheckEnabled = LineageSettings.System.getIntForUser(
                    getContentResolver(), LineageSettings.System.PROXIMITY_ON_WAKE, defaultProximity
                    ? 1 : 0, UserHandle.USER_CURRENT) == 1;

            if (proximityWakeCheckEnabled) {
                IntentFilter screenStateFilter = new IntentFilter(Intent.ACTION_SCREEN_ON);
                screenStateFilter.addAction(Intent.ACTION_SCREEN_OFF);
                registerReceiver(mScreenStateReceiver, screenStateFilter);
                mIsRegistered = true;
            } else {
                mProximitySensor.disable();
                if (mIsRegistered) {
                    unregisterReceiver(mScreenStateReceiver);
                    mIsRegistered = false;
                }
            }
        }
    }
}
