/*
 * Copyright (C) 2019-2021 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "DisplayModesService"

#include <android-base/logging.h>
#include <fstream>

#include "DisplayModes.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

static const std::string kModeBasePath = "/sys/devices/virtual/graphics/fb0/preset";
static const std::string kAvailableModesPath = "/sys/devices/virtual/graphics/fb0/num_presets";
static const std::string kDefaultPath = "/data/vendor/display/default_display_mode";

const std::map<int32_t, std::string> DisplayModes::kModeMap = {
    {0, "Standard"},
    {1, "sRGB"},
    {2, "DCI-P3"},
    {3, "OnePlus"},
    {4, "Adaptive"},
};

DisplayModes::DisplayModes()
    : mCurrentModeId(0), mDefaultModeId(0) {
    std::ifstream defaultFile(kDefaultPath);

    defaultFile >> mDefaultModeId;
    LOG(DEBUG) << "Default file read result " << mDefaultModeId << " fail " << defaultFile.fail();

    setDisplayMode(mDefaultModeId, false);
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IDisplayModes follow.
Return<void> DisplayModes::getDisplayModes(getDisplayModes_cb resultCb) {
    std::vector<V2_0::DisplayMode> modes;
    std::ifstream numFile(kAvailableModesPath);
    int32_t maxModeCount;

    numFile >> maxModeCount;
    if (numFile.fail()) {
        LOG(ERROR) << "Failed to read available display modes " << numFile.fail();
    }
    if (maxModeCount > 0) {
        for (const auto& entry : kModeMap) {
            modes.push_back({entry.first, entry.second});
            if (entry.first == maxModeCount / 2)
                break;
        }
    }
    resultCb(modes);
    return Void();
}

Return<void> DisplayModes::getCurrentDisplayMode(getCurrentDisplayMode_cb resultCb) {
    resultCb({mCurrentModeId, kModeMap.at(mCurrentModeId)});
    return Void();
}

Return<void> DisplayModes::getDefaultDisplayMode(getDefaultDisplayMode_cb resultCb) {
    resultCb({mDefaultModeId, kModeMap.at(mDefaultModeId)});
    return Void();
}

Return<bool> DisplayModes::setDisplayMode(int32_t modeID, bool makeDefault) {
    const auto iter = kModeMap.find(modeID);
    if (iter == kModeMap.end()) {
        return false;
    }
    std::ofstream file(kModeBasePath);
    // If mode is "Standard" we need to switch off the previously selected mode
    // Modes 0-3 are colors (0=sRGB, etc.), 4-7 to switch them off (4=sRGB off, etc.)
    if (modeID == 0)
        file << mDefaultModeId + 3;
    else
        file << modeID - 1;
    if (file.fail()) {
        LOG(ERROR) << "Failed to write to " << (kModeBasePath);
    }
    mCurrentModeId = iter->first;
    if (makeDefault) {
        std::ofstream defaultFile(kDefaultPath);
        defaultFile << iter->first;
        if (!defaultFile.fail()) {
            mDefaultModeId = iter->first;
        }
    }
    if (mOnDisplayModeSet) {
        mOnDisplayModeSet();
    }
    return true;
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
