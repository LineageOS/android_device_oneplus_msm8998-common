/*
 * Copyright (c) 2016 The CyanogenMod Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
package org.lineageos.pocketmode;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.os.UserHandle;
import android.util.Log;

public class BootCompletedReceiver extends BroadcastReceiver {

    private static final String TAG = "OnePlusPocketMode";

    @Override
    public void onReceive(final Context context, Intent intent) {
        Log.d(TAG, "Starting");
        context.startServiceAsUser(new Intent(context, PocketModeService.class),
                UserHandle.CURRENT);
    }
}
