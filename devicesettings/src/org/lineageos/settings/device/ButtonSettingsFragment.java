/*
 * Copyright (C) 2015 The CyanogenMod Project
 *               2017 The LineageOS Project
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

import android.os.Bundle;
import android.support.v14.preference.PreferenceFragment;
import android.support.v14.preference.SwitchPreference;
import android.support.v7.preference.ListPreference;
import android.support.v7.preference.Preference;
import android.support.v7.preference.Preference.OnPreferenceChangeListener;
import android.support.v7.preference.PreferenceGroup;
import android.text.TextUtils;
import android.view.MenuItem;

import org.lineageos.internal.util.FileUtils;
import org.lineageos.settings.device.utils.Constants;

public class ButtonSettingsFragment extends PreferenceFragment
        implements OnPreferenceChangeListener {

    @Override
    public void onCreatePreferences(Bundle savedInstanceState, String rootKey) {
        addPreferencesFromResource(R.xml.button_panel);
        getActivity().getActionBar().setDisplayHomeAsUpEnabled(true);
    }

    @Override
    public boolean onPreferenceChange(Preference preference, Object newValue) {
        String node = Constants.sBooleanNodePreferenceMap.get(preference.getKey());
        if (!TextUtils.isEmpty(node) && FileUtils.isFileWritable(node)) {
            Boolean value = (Boolean) newValue;
            FileUtils.writeLine(node, value ? "1" : "0");
            return true;
        }
        node = Constants.sStringNodePreferenceMap.get(preference.getKey());
        if (!TextUtils.isEmpty(node) && FileUtils.isFileWritable(node)) {
            FileUtils.writeLine(node, (String) newValue);
            return true;
        }
        return false;
    }

    @Override
    public void addPreferencesFromResource(int preferencesResId) {
        super.addPreferencesFromResource(preferencesResId);
        // Initialize node preferences
        for (String pref : Constants.sBooleanNodePreferenceMap.keySet()) {
            SwitchPreference b = (SwitchPreference) findPreference(pref);
            if (b == null) continue;
            String node = Constants.sBooleanNodePreferenceMap.get(pref);
            if (FileUtils.isFileReadable(node)) {
                String curNodeValue = FileUtils.readOneLine(node);
                b.setChecked(curNodeValue.equals("1"));
                b.setOnPreferenceChangeListener(this);
            } else {
                removePref(b);
            }
        }
        for (String pref : Constants.sStringNodePreferenceMap.keySet()) {
            ListPreference l = (ListPreference) findPreference(pref);
            if (l == null) continue;
            String node = Constants.sStringNodePreferenceMap.get(pref);
            if (FileUtils.isFileReadable(node)) {
                l.setValue(FileUtils.readOneLine(node));
                l.setOnPreferenceChangeListener(this);
            } else {
                removePref(l);
            }
        }
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        switch (item.getItemId()) {
        // Respond to the action bar's Up/Home button
        case android.R.id.home:
            getActivity().finish();
            return true;
        }
        return super.onOptionsItemSelected(item);
    }

    private void removePref(Preference pref) {
        PreferenceGroup parent = pref.getParent();
        if (parent == null) {
            return;
        }
        parent.removePreference(pref);
        if (parent.getPreferenceCount() == 0) {
            removePref(parent);
        }
    }
}
