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

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothGatt;
import android.bluetooth.BluetoothGattCallback;
import android.bluetooth.BluetoothGattCharacteristic;
import android.bluetooth.BluetoothGattService;
import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.content.SharedPreferences;
import android.hardware.input.InputManager;
import android.media.AudioManager;
import android.media.Ringtone;
import android.media.RingtoneManager;
import android.os.Handler;
import android.os.IBinder;
import android.os.Message;
import android.os.SystemClock;
import android.preference.PreferenceManager;
import android.util.Log;
import android.view.InputDevice;
import android.view.KeyCharacterMap;
import android.view.KeyEvent;

import org.lineageos.settings.device.utils.Constants;

import java.util.UUID;

public class OclickService extends Service implements
        SharedPreferences.OnSharedPreferenceChangeListener {

    private static final String TAG = OclickService.class.getSimpleName();

    /* package */ static final UUID TRIGGER_SERVICE_UUID =
            UUID.fromString("0000ffe0-0000-1000-8000-00805f9b34fb");
    private static final UUID TRIGGER_CHARACTERISTIC_V1_UUID =
            UUID.fromString("0000ffe1-0000-1000-8000-00805f9b34fb");
    private static final UUID TRIGGER_CHARACTERISTIC_V2_UUID =
            UUID.fromString("f000ffe1-0451-4000-b000-000000000000");

    /* package */ static final UUID OCLICK2_SERVICE_UUID =
            UUID.fromString("00002200-0000-1000-8000-00805f9b34fb");
    private static final UUID OCLICK2_KEY_CHARACTERISTIC_UUID =
            UUID.fromString("00002201-0000-1000-8000-00805f9b34fb");

    private static final UUID IMMEDIATE_ALERT_SERVICE_UUID =
            UUID.fromString("00001802-0000-1000-8000-00805f9b34fb"); //0-2
    private static final UUID IMMEDIATE_ALERT_CHARACTERISTIC_UUID =
            UUID.fromString("00002a06-0000-1000-8000-00805f9b34fb");

    private static final UUID LINK_LOSS_SERVICE_UUID =
            UUID.fromString("00001803-0000-1000-8000-00805f9b34fb"); //0-3
    private static final UUID LINK_LOSS_CHARACTERISTIC_UUID =
            UUID.fromString("00002a06-0000-1000-8000-00805f9b34fb");

    public static final String CANCEL_ALERT_PHONE = "cancel_alert_phone";

    private static final int RSSI_POLL_INTERVAL = 10000;
    private static final int DOUBLE_TAP_TIMEOUT = 1500;

    private static final class Oclick2Constants {
        private static final int MSG_CLASS_CALL = 1;
        private static final int MSG_CLASS_MESSAGE = 2;
        private static final int MSG_CLASS_LED = 3;
        private static final int MSG_CLASS_KEY = 5;
        private static final int MSG_CLASS_CONNECTION = 7;
        private static final int MSG_CLASS_LINKLOSE = 8;
        private static final int MSG_CLASS_RSSI = 11;

        /* payload: count (16 bit integer) */
        private static final int MSG_TYPE_CALL_SET_INCOMING = 1;
        private static final int MSG_TYPE_CALL_SET_MISSED = 2;
        private static final int MSG_TYPE_CALL_SET_READ = 3;

        /* payload: count (16 bit integer) */
        private static final int MSG_TYPE_MESSAGE_UNREAD = 1;
        private static final int MSG_TYPE_MESSAGE_READ = 2;

        /* payload: color bitmask (1 byte, white = 1, red = 2, green = 4, blue = 8) */
        private static final int MSG_TYPE_LED_ON = 1;
        private static final int MSG_TYPE_LED_FLASH = 2;
        private static final int MSG_TYPE_LED_OFF = 3;

        /* payload:
         * CONNECTION_INTERVAL_MIN (16 bit integer),
         * CONNECTION_INTERVAL_MAX (16 bit integer),
         * CONNECTION_LATENCY (16 bit integer),
         * SUPERVISION_TIMEOUT (16 bit integer)
         */
        private static final int MSG_TYPE_CONNECTION_GET_PARAMS = 1;
        private static final int MSG_TYPE_CONNECTION_SET_PARAMS = 2;

        /* payload: level (1 byte, off = 0, on = 1) */
        private static final int MSG_TYPE_LINKLOSE_GET_LEVEL = 1;
        private static final int MSG_TYPE_LINKLOSE_SET_LEVEL = 2;

        private static final int MSG_TYPE_RSSI_READ_RATE_GET = 1;
        private static final int MSG_TYPE_RSSI_READ_RATE_SET = 2;
        private static final int MSG_TYPE_RSSI_GET = 3;

        private static final int KEYCODE_MIDDLE = 0x10;
        private static final int KEYCODE_UP = 0x20;
        private static final int KEYCODE_RIGHT = 0x30;
        private static final int KEYCODE_DOWN = 0x40;
        private static final int KEYCODE_LEFT = 0x50;
        private static final int KEYCODE_MASK = 0xf0;

        private static final int KEYTYPE_LONG_RELEASE = 0;
        private static final int KEYTYPE_SHORT = 1;
        private static final int KEYTYPE_DOUBLE = 2;
        private static final int KEYTYPE_LONG_PRESS = 3;
        private static final int KEYTYPE_MASK = 0xf;
    }

    private enum ConnectionState {
        INIT,
        CONNECTED,
        RECONNECTING
    };

    private BluetoothDevice mBluetoothDevice;
    private BluetoothGatt mBluetoothGatt;
    private boolean mAlerting;
    private AudioManager mAudioManager;
    private boolean mTapPending = false;
    private boolean mRssiAlertEnabled = false;
    private Ringtone mRingtone;
    private SharedPreferences mPrefs;
    private ConnectionState mConnectionState;

    private static final int MSG_SINGLE_TAP_TIMEOUT = 1;
    private static final int MSG_POLL_RSSI = 2;
    private static final int MSG_TRY_RECONNECT = 3;

    private Handler mHandler = new Handler() {
        @Override
        public void handleMessage(Message msg) {
            switch (msg.what) {
                case MSG_SINGLE_TAP_TIMEOUT:
                    injectKey(KeyEvent.KEYCODE_CAMERA);
                    mTapPending = false;
                    break;
                case MSG_POLL_RSSI:
                    mBluetoothGatt.readRemoteRssi();
                    break;
                case MSG_TRY_RECONNECT:
                    connect();
                    break;
            }
        }
    };

    private BroadcastReceiver mReceiver = new BroadcastReceiver() {
        @Override
        public void onReceive(Context context, Intent intent) {
            if (intent.getAction().equals(CANCEL_ALERT_PHONE)) {
                stopPhoneLocator();
            }
        }
    };

    private BluetoothGattCallback mGattCallback = new BluetoothGattCallback() {
        @Override
        public void onConnectionStateChange(BluetoothGatt gatt, int status, final int newState) {
            Log.d(TAG, "onConnectionStateChange " + status + " " + newState);
            if (newState == BluetoothGatt.STATE_CONNECTED) {
                mConnectionState = ConnectionState.CONNECTED;
                gatt.discoverServices();
            } else if (newState == BluetoothGatt.STATE_DISCONNECTED) {
                mBluetoothGatt.close();
                mBluetoothGatt = null;
                mHandler.removeMessages(MSG_POLL_RSSI);
                mHandler.sendEmptyMessage(MSG_TRY_RECONNECT);
                mConnectionState = ConnectionState.RECONNECTING;
            }
            updateNotification();
        }

        @Override
        public void onServicesDiscovered(final BluetoothGatt gatt, int status) {
            Log.d(TAG, "onServicesDiscovered " + status);

            BluetoothGattService serviceV2 = gatt.getService(OCLICK2_SERVICE_UUID);
            BluetoothGattCharacteristic keyCharacteristic = null;
            if (serviceV2 != null) {
                keyCharacteristic = serviceV2.getCharacteristic(OCLICK2_KEY_CHARACTERISTIC_UUID);
            }

            if (keyCharacteristic != null) {
                // O-Click 2.0 mode
                gatt.setCharacteristicNotification(keyCharacteristic, true);

                // update connection parameters - TODO: use constants
                byte[] params = new byte[] {
                    Oclick2Constants.MSG_CLASS_CONNECTION,
                    Oclick2Constants.MSG_TYPE_CONNECTION_SET_PARAMS,
                    /* CONNECTION_INTERVAL_MIN = 200 */ (byte) 0xc8, 0,
                    /* CONNECTION_INTERVAL_MAX = 400 */ (byte) 0x90, 1,
                    /* CONNECTION_LATENCY = 1 */ 1, 0,
                    /* SUPERVISION_TIMEOUT = 1000 */ (byte) 0xe8, 3
                };
                keyCharacteristic.setValue(params);
                gatt.writeCharacteristic(keyCharacteristic);
            } else {
                // Register trigger notification (Used for camera/alarm)
                BluetoothGattService service = gatt.getService(TRIGGER_SERVICE_UUID);
                BluetoothGattCharacteristic trigger =
                        service.getCharacteristic(TRIGGER_CHARACTERISTIC_V1_UUID);

                if (trigger == null) {
                    trigger = service.getCharacteristic(TRIGGER_CHARACTERISTIC_V2_UUID);
                }
                gatt.setCharacteristicNotification(trigger, true);
                updateLinkLossState();
            }

            toggleRssiListener();
        }

        @Override
        public void onCharacteristicWrite(BluetoothGatt gatt,
                BluetoothGattCharacteristic characteristic, int status) {
            UUID uuid = characteristic.getService().getUuid();
            Log.d(TAG, "onCharacteristicWrite: service UUID " + uuid + " status " + status);

            if (OCLICK2_SERVICE_UUID.equals(uuid)) {
                if (characteristic.getValue()[0] == Oclick2Constants.MSG_CLASS_CONNECTION) {
                    updateLinkLossState();
                }
            } else if (LINK_LOSS_SERVICE_UUID.equals(uuid) && !mAlerting) {
                updateAlertState(false);
            }
        }

        @Override
        public void onReliableWriteCompleted(BluetoothGatt gatt, int status) {
            Log.d(TAG, "onReliableWriteCompleted : " + status);
        }

        @Override
        public void onCharacteristicChanged(BluetoothGatt gatt,
                BluetoothGattCharacteristic characteristic) {
            Log.d(TAG, "Characteristic changed " + characteristic.getUuid());

            if (characteristic.getUuid().equals(OCLICK2_KEY_CHARACTERISTIC_UUID)) {
                byte[] value = characteristic.getValue();
                if (value.length == 3 && value[0] == Oclick2Constants.MSG_CLASS_KEY) {
                    int key = value[2] & Oclick2Constants.KEYCODE_MASK;
                    int action = value[2] & Oclick2Constants.KEYTYPE_MASK;
                    if (key == Oclick2Constants.KEYCODE_MIDDLE) {
                        if (action == Oclick2Constants.KEYTYPE_DOUBLE) {
                            if (mRingtone.isPlaying()) {
                                stopPhoneLocator();
                            } else {
                                startPhoneLocator();
                            }
                        } else if (action == Oclick2Constants.KEYTYPE_SHORT) {
                            injectKey(KeyEvent.KEYCODE_CAMERA);
                        }
                    }

                    Intent keyIntent = new Intent("org.lineageos.device.oppo.ACTION_OCLICK_KEY");
                    keyIntent.putExtra("key", key);
                    keyIntent.putExtra("action", action);
                    sendBroadcast(keyIntent);
                }
            } else {
                if (mTapPending) {
                    if (mRingtone.isPlaying()) {
                        stopPhoneLocator();
                        return;
                    }

                    mHandler.removeMessages(MSG_SINGLE_TAP_TIMEOUT);
                    mTapPending = false;
                    startPhoneLocator();
                    return;
                }
                Log.d(TAG, "Setting single tap runnable");
                mTapPending = true;
                mHandler.sendEmptyMessageDelayed(MSG_SINGLE_TAP_TIMEOUT, DOUBLE_TAP_TIMEOUT);
            }
        }

        @Override
        public void onReadRemoteRssi(BluetoothGatt gatt, int rssi, int status) {
            Log.d(TAG, "Rssi value : " + rssi);
            if (rssi < -90 && !mAlerting) {
                updateAlertState(true);
                mAlerting = true;
            } else if (rssi > -90 && mAlerting) {
                updateAlertState(false);
                mAlerting = false;
            }
            if (mRssiAlertEnabled) {
                mHandler.sendEmptyMessageDelayed(MSG_POLL_RSSI, RSSI_POLL_INTERVAL);
            }
        }
    };

    @Override
    public IBinder onBind(Intent intent) {
        return null;
    }

    @Override
    public void onCreate() {
        mPrefs = PreferenceManager.getDefaultSharedPreferences(this);
        mPrefs.registerOnSharedPreferenceChangeListener(this);

        IntentFilter filter = new IntentFilter();
        filter.addAction(CANCEL_ALERT_PHONE);
        registerReceiver(mReceiver, filter);

        RingtoneManager ringtoneManager = new RingtoneManager(this);
        ringtoneManager.setType(RingtoneManager.TYPE_ALARM);
        int length = ringtoneManager.getCursor().getCount();
        for (int i = 0; i < length; i++) {
            mRingtone = ringtoneManager.getRingtone(i);
            if (mRingtone != null && mRingtone.getTitle(this).toLowerCase().contains("barium")) {
                break;
            }
        }
        mAudioManager = (AudioManager) getSystemService(Context.AUDIO_SERVICE);
    }

    @Override
    public void onDestroy() {
        Log.d(TAG, "Service being killed");
        mHandler.removeCallbacksAndMessages(null);
        if (mBluetoothGatt != null) {
            mBluetoothGatt.disconnect();
            mBluetoothGatt.close();
        }

        mPrefs.unregisterOnSharedPreferenceChangeListener(this);
        unregisterReceiver(mReceiver);
    }

    @Override
    public void onSharedPreferenceChanged(SharedPreferences prefs, String key) {
        if (mBluetoothGatt == null) {
            return;
        }

        if (key.equals(Constants.OCLICK_FENCE_KEY)) {
            toggleRssiListener();
        } else if (key.equals(Constants.OCLICK_DISCONNECT_ALERT_KEY)) {
            updateLinkLossState();
        }
    }

    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
        Log.d(TAG, "onstartCommand");
        if (mBluetoothDevice == null && mPrefs.contains(Constants.OCLICK_DEVICE_ADDRESS_KEY)) {
            BluetoothAdapter adapter = BluetoothAdapter.getDefaultAdapter();
            String address = mPrefs.getString(Constants.OCLICK_DEVICE_ADDRESS_KEY, null);

            mBluetoothDevice = adapter.getRemoteDevice(address);
            mConnectionState = ConnectionState.INIT;
            Log.d(TAG, "Oclick device " + mBluetoothDevice);
        }

        if (mBluetoothDevice == null) {
            Log.e(TAG, "No bluetooth device provided");
            stopSelf();
            return START_NOT_STICKY;
        }

        updateNotification();
        connect();

        return START_REDELIVER_INTENT;
    }

    private void startPhoneLocator() {
        Log.d(TAG, "Executing ring alarm");

        // FIXME: this needs to be reverted
        mAudioManager.setStreamVolume(AudioManager.STREAM_ALARM,
                mAudioManager.getStreamMaxVolume(AudioManager.STREAM_ALARM), 0);
        mRingtone.play();

        Notification.Builder builder = new Notification.Builder(this);
        builder.setSmallIcon(R.drawable.locator_icon);
        builder.setContentTitle(getString(R.string.oclick_locator_notification_title));
        builder.setContentText(getString(R.string.oclick_locator_notification_text));
        builder.setAutoCancel(true);
        builder.setOngoing(true);

        PendingIntent resultPendingIntent = PendingIntent.getBroadcast(this,
                0, new Intent(CANCEL_ALERT_PHONE), 0);
        builder.setContentIntent(resultPendingIntent);

        NotificationManager notificationManager =
                (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);
        notificationManager.notify(0, builder.build());
    }

    private void stopPhoneLocator() {
        NotificationManager notificationManager =
                (NotificationManager) getSystemService(Context.NOTIFICATION_SERVICE);

        Log.d(TAG, "Stopping ring alarm");
        mRingtone.stop();
        notificationManager.cancel(0);
    }

    private void injectKey(int keyCode) {
        long now = SystemClock.uptimeMillis();
        InputManager im = InputManager.getInstance();
        im.injectInputEvent(new KeyEvent(now, now, KeyEvent.ACTION_DOWN, keyCode,
                0, 0, KeyCharacterMap.VIRTUAL_KEYBOARD, 0, 0, InputDevice.SOURCE_KEYBOARD),
                InputManager.INJECT_INPUT_EVENT_MODE_ASYNC);
        im.injectInputEvent(new KeyEvent(now, now, KeyEvent.ACTION_UP, keyCode,
                0, 0, KeyCharacterMap.VIRTUAL_KEYBOARD, 0, 0, InputDevice.SOURCE_KEYBOARD),
                InputManager.INJECT_INPUT_EVENT_MODE_ASYNC);
    }

    private void toggleRssiListener() {
        mRssiAlertEnabled = Constants.isPreferenceEnabled(this, Constants.OCLICK_FENCE_KEY);
        mHandler.removeMessages(MSG_POLL_RSSI);
        if (mRssiAlertEnabled) {
            Log.d(TAG, "Enabling rssi listener");
            mHandler.sendEmptyMessage(MSG_POLL_RSSI);
        }
    }

    private void updateAlertState(boolean doAlert) {
        BluetoothGattService alertService =
                mBluetoothGatt.getService(IMMEDIATE_ALERT_SERVICE_UUID);
        BluetoothGattCharacteristic alertCharacteristic =
                alertService.getCharacteristic(IMMEDIATE_ALERT_CHARACTERISTIC_UUID);

        alertCharacteristic.setValue(new byte[] { (byte) (doAlert ? 2 : 0) });
        mBluetoothGatt.writeCharacteristic(alertCharacteristic);
    }

    private void updateLinkLossState() {
        boolean alert = Constants.isPreferenceEnabled(OclickService.this,
                Constants.OCLICK_DISCONNECT_ALERT_KEY);
        BluetoothGattService service = mBluetoothGatt.getService(LINK_LOSS_SERVICE_UUID);
        BluetoothGattCharacteristic characteristic =
                service.getCharacteristic(LINK_LOSS_CHARACTERISTIC_UUID);

        characteristic.setValue(new byte[] { (byte) (alert ? 2 : 0) });
        mBluetoothGatt.writeCharacteristic(characteristic);
    }

    private void connect() {
        if (mBluetoothDevice != null && mBluetoothGatt == null) {
            Log.d(TAG, "Connecting to device " + mBluetoothDevice);
            mBluetoothGatt = mBluetoothDevice.connectGatt(this, false, mGattCallback);
        }
    }

    private void updateNotification() {
        final PendingIntent clickIntent = PendingIntent.getActivity(this, 0,
                new Intent(this, BluetoothInputSettings.class), 0);

        final Notification.Builder builder = new Notification.Builder(this)
                .setSmallIcon(R.drawable.ic_oclick_notification)
                .setContentIntent(clickIntent)
                .setLocalOnly(true)
                .setOngoing(true)
                .setShowWhen(false)
                .setCategory(Notification.CATEGORY_SERVICE)
                .setVisibility(Notification.VISIBILITY_PUBLIC);

        builder.setColor(getResources().getColor(
                com.android.internal.R.color.system_notification_accent_color));
        builder.setPriority(mConnectionState == ConnectionState.RECONNECTING
                ? Notification.PRIORITY_DEFAULT : Notification.PRIORITY_MIN);
        builder.setContentTitle(getString(mConnectionState == ConnectionState.CONNECTED
                ? R.string.oclick_notification_title_connected
                : R.string.oclick_notification_title_disconnected));
        if (mConnectionState != ConnectionState.CONNECTED) {
            builder.setContentText(getString(R.string.oclick_notification_content_disconnected));
        }

        startForeground(1000, builder.build());
    }
}
