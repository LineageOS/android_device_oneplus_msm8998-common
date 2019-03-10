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

package org.lineageos.settings.device;

import android.app.ProgressDialog;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothManager;
import android.bluetooth.BluetoothProfile;
import android.bluetooth.le.BluetoothLeScanner;
import android.bluetooth.le.ScanCallback;
import android.bluetooth.le.ScanFilter;
import android.bluetooth.le.ScanResult;
import android.bluetooth.le.ScanSettings;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.preference.PreferenceManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.ParcelUuid;
import android.preference.Preference;
import android.preference.PreferenceActivity;
import android.preference.PreferenceScreen;
import android.text.TextUtils;
import android.view.MenuItem;

import org.lineageos.internal.util.ScreenType;
import org.lineageos.settings.device.utils.Constants;

import java.util.ArrayList;
import java.util.List;

@SuppressWarnings("deprecation")
public class BluetoothInputSettings extends PreferenceActivity {
    private static final int BLUETOOTH_REQUEST_CODE = 1;
    private static final String CATEGORY_ACTIONS = "oclick_action_category";
    private static final String CATEGORY_ALERT = "oclick_alert_category";

    private ProgressDialog mProgressDialog;
    private boolean mConnected;
    private Handler mHandler = new Handler();
    private BluetoothAdapter mAdapter;
    private SharedPreferences mPrefs;

    private BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            BluetoothDevice device = intent.getParcelableExtra(BluetoothDevice.EXTRA_DEVICE);
            String address = mPrefs.getString(Constants.OCLICK_DEVICE_ADDRESS_KEY, null);
            if (device != null && TextUtils.equals(address, device.getAddress())) {
                updateConnectedState();
            }
        }
    };

    private ScanCallback mScanCallback = new ScanCallback() {
        @Override
        public void onScanFailed(int errorCode) {
            stopScanning();
        }
        @Override
        public void onScanResult(int callbackType, ScanResult result) {
            handleScanResult(result);
        }
    };

    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        addPreferencesFromResource(R.xml.oclick_panel);
        getActionBar().setDisplayHomeAsUpEnabled(true);

        BluetoothManager bluetoothManager =
                (BluetoothManager) getSystemService(BLUETOOTH_SERVICE);
        mAdapter = bluetoothManager.getAdapter();
        mPrefs = PreferenceManager.getDefaultSharedPreferences(this);
    }

    @Override
    protected void onResume() {
        super.onResume();

        updateConnectedState();

        IntentFilter filter = new IntentFilter(BluetoothDevice.ACTION_ACL_CONNECTED);
        filter.addAction(BluetoothDevice.ACTION_ACL_DISCONNECTED);
        registerReceiver(mReceiver, filter);

        // If running on a phone, remove padding around the listview
        if (!ScreenType.isTablet(this)) {
            getListView().setPadding(0, 0, 0, 0);
        }
    }

    @Override
    protected void onPause() {
        super.onPause();
        unregisterReceiver(mReceiver);
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        // Respond to the action bar's Up/Home button
        case android.R.id.home:
            finish();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    @Override
    public boolean onPreferenceTreeClick(PreferenceScreen preferenceScreen, Preference pref) {
        if (!pref.getKey().equals(Constants.OCLICK_CONNECT_KEY)) {
            return super.onPreferenceTreeClick(preferenceScreen, pref);
        }
        if (mConnected) {
            mPrefs.edit().remove(Constants.OCLICK_DEVICE_ADDRESS_KEY).apply();
            stopService(new Intent(this, OclickService.class));
            updateConnectedState();
        } else if (!mAdapter.isEnabled()) {
            Intent enableBtIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableBtIntent, BLUETOOTH_REQUEST_CODE);
        } else {
            startScanning();
        }
        return true;
    }

    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == BLUETOOTH_REQUEST_CODE && resultCode == RESULT_OK) {
            startScanning();
        }
    }

    private void startScanning() {
        BluetoothLeScanner scanner = mAdapter.getBluetoothLeScanner();
        ScanSettings settings = new ScanSettings.Builder()
                .setScanMode(ScanSettings.SCAN_MODE_LOW_LATENCY)
                .build();

        List<ScanFilter> filters = new ArrayList<ScanFilter>();
        // O-Click 1
        filters.add(new ScanFilter.Builder()
                .setServiceUuid(new ParcelUuid(OclickService.TRIGGER_SERVICE_UUID))
                .build());
        // O-Click 2
        filters.add(new ScanFilter.Builder()
                .setServiceUuid(new ParcelUuid(OclickService.OCLICK2_SERVICE_UUID))
                .build());

        scanner.startScan(filters, settings, mScanCallback);

        String dialogTitle = this.getString(R.string.oclick_dialog_title);
        String dialogMessage = this.getString(R.string.oclick_dialog_connecting_message);
        mProgressDialog = ProgressDialog.show(this, dialogTitle, dialogMessage, true);
        mHandler.postDelayed(new Runnable() {
            @Override
            public void run() {
                stopScanning();
            }
        }, 10000);
    }

    private void stopScanning() {
        BluetoothLeScanner scanner = mAdapter.getBluetoothLeScanner();
        scanner.stopScan(mScanCallback);
        if (mProgressDialog != null) {
            mProgressDialog.dismiss();
            mProgressDialog = null;
        }
    }

    private void handleScanResult(ScanResult result) {
        stopScanning();
        String address = result.getDevice().getAddress();
        mPrefs.edit().putString(Constants.OCLICK_DEVICE_ADDRESS_KEY, address).apply();
        startService(new Intent(this, OclickService.class));
        updateConnectedState();
    }

    private boolean isBluetoothDeviceConnected(String address) {
        BluetoothDevice device = mAdapter.getRemoteDevice(address);
        BluetoothManager bluetoothManager =
                (BluetoothManager) getSystemService(BLUETOOTH_SERVICE);
        int state = bluetoothManager.getConnectionState(device, BluetoothProfile.GATT);
        return state == BluetoothProfile.STATE_CONNECTED;
    }

    private void updateConnectedState() {
        String address = mPrefs.getString(Constants.OCLICK_DEVICE_ADDRESS_KEY, null);
        mConnected = !TextUtils.isEmpty(address);

        findPreference(CATEGORY_ACTIONS).setEnabled(mConnected);
        findPreference(CATEGORY_ALERT).setEnabled(mConnected);

        Preference connectPref = findPreference(Constants.OCLICK_CONNECT_KEY);
        connectPref.setTitle(mConnected ?
                R.string.oclick_disconnect_string : R.string.oclick_connect_string);
        if (mConnected && isBluetoothDeviceConnected(address)) {
            connectPref.setSummary(R.string.oclick_summary_connected);
        } else if (mConnected) {
            connectPref.setSummary(R.string.oclick_summary_paired);
        } else {
            connectPref.setSummary(null);
        }
    }
}
