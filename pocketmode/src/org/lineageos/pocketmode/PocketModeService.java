/*
 * Copyright (c) 2016 The CyanogenMod Project
 *
 * SPDX-License-Identifier: Apache-2.0
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
            if (intent.getAction().equals(Intent.ACTION_USER_PRESENT)) {
                if (DEBUG) Log.d(TAG, "Device unlocked");
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
                IntentFilter screenStateFilter = new IntentFilter(Intent.ACTION_USER_PRESENT);
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
