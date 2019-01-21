/*
 * Copyright (C) 2019 The LineageOS Project
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

#define LOG_TAG "DisplayModesService"

#include "DisplayModes.h"
#include <android-base/logging.h>
#include <fstream>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

static const std::string kModeBasePath = "/sys/devices/virtual/graphics/fb0/";
static constexpr const char* kDefaultPath = "/data/system/default_display_mode";

const std::map<int32_t, DisplayModes::ModeInfo> DisplayModes::kModeMap = {
    {0, {"Standard", "standard"}},
    {1, {"Adobe RGB", "adobe_rgb"}},
    {2, {"sRGB", "srgb"}},
    {3, {"DCI P3", "dci_p3"}},
    {4, {"HBM", "hbm"}},
    {5, {"Night mode", "night_mode"}},
    {6, {"Adaption mode", "adaption_mode"}},
};

DisplayModes::DisplayModes() : mDefaultModeId(0) {
    std::ifstream defaultFile(kDefaultPath);
    std::string value;
    defaultFile >> value;

    LOG(DEBUG) << "Default file read result " << value << " fail " << defaultFile.fail();
    if (defaultFile.fail()) {
        return;
    }

    for (const auto& entry : kModeMap) {
        // Check if default mode is a valid mode
        //if (value == std::string(itoa(entry.first))) {
        if (value == std::to_string(entry.first)) {
            mDefaultModeId = entry.first;
            break;
        }
    }
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IDisplayModes follow.
Return<void> DisplayModes::getDisplayModes(getDisplayModes_cb resultCb) {
    std::vector<DisplayMode> modes;
    for (const auto& entry : kModeMap) {
        modes.push_back({entry.first, entry.second.name});
    }
    resultCb(modes);
    return Void();
}

Return<void> DisplayModes::getCurrentDisplayMode(getCurrentDisplayMode_cb resultCb) {
    int32_t currentModeId = -1;
    std::string path;
    std::string value;

    for (const auto& entry : kModeMap) {
        if (entry.first == 0) {
            continue;
        }
        path = kModeBasePath + std::string(entry.second.value);
        std::ifstream modeFile(path.c_str());
        if (!modeFile.fail()) {
            modeFile >> value;
            if (value == "1") {
                currentModeId = entry.first;
                break;
            }
        }
    }
    resultCb({currentModeId, kModeMap.at(currentModeId).name});
    return Void();
}

Return<void> DisplayModes::getDefaultDisplayMode(getDefaultDisplayMode_cb resultCb) {
    resultCb({mDefaultModeId, kModeMap.at(mDefaultModeId).name});
    return Void();
}

Return<bool> DisplayModes::setDisplayMode(int32_t modeID, bool makeDefault) {
    std::string path;
    // Disable all modes
    for (const auto& entry : kModeMap) {
        path = kModeBasePath + std::string(entry.second.value);
        std::ofstream modeFile(path.c_str());
        if (!modeFile.fail()) {
            modeFile << 0;
        }
    }
    const auto iter = kModeMap.find(modeID);
    if (iter == kModeMap.end()) {
        return false;
    }
    if (modeID != 0) {
        path = kModeBasePath + std::string(iter->second.value);
        std::ofstream modeFile(path.c_str());
        modeFile << 1;
        if (modeFile.fail()) {
            return false;
        }
    }

    if (makeDefault) {
        std::ofstream defaultFile(kDefaultPath);
        defaultFile << iter->first;
        if (defaultFile.fail()) {
            return false;
        }
        mDefaultModeId = iter->first;
    }

    return true;
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
