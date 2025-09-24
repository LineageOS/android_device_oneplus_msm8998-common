/*
 * Copyright (C) 2019-2021 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "DisplayModesService"

#include <android-base/logging.h>
#include <fstream>

#include "DisplayModes.h"

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {

static const std::string kModeBasePath = "/sys/devices/virtual/graphics/fb0/preset";
static const std::string kAvailableModesPath = "/sys/devices/virtual/graphics/fb0/num_presets";
static const std::string kDefaultPath = "/data/vendor/display/default_display_mode";

const std::map<int32_t, std::string> DisplayModes::kModeMap = {
        {0, "Standard"}, {1, "sRGB"}, {2, "DCI-P3"}, {3, "OnePlus"}, {4, "Adaptive"},
};

DisplayModes::DisplayModes() : mCurrentModeId(0), mDefaultModeId(0) {
    std::ifstream defaultFile(kDefaultPath);

    defaultFile >> mDefaultModeId;
    LOG(DEBUG) << "Default file read result " << mDefaultModeId << " fail " << defaultFile.fail();

    setDisplayMode(mDefaultModeId, false);
}

// Methods from ::aidl::vendor::lineage::livedisplay::BnDisplayModes follow.
ndk::ScopedAStatus DisplayModes::getDisplayModes(std::vector<DisplayMode>* _aidl_return) {
    std::vector<DisplayMode> modes;
    std::ifstream numFile(kAvailableModesPath);
    int32_t maxModeCount;

    numFile >> maxModeCount;
    if (numFile.fail()) {
        LOG(ERROR) << "Failed to read available display modes " << numFile.fail();
    }
    if (maxModeCount > 0) {
        for (const auto& entry : kModeMap) {
            modes.push_back({entry.first, entry.second});
            if (entry.first == maxModeCount - 1) break;
        }
    }
    *_aidl_return = modes;
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayModes::getCurrentDisplayMode(DisplayMode* _aidl_return) {
    *_aidl_return = {mCurrentModeId, kModeMap.at(mCurrentModeId)};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayModes::getDefaultDisplayMode(DisplayMode* _aidl_return) {
    *_aidl_return = {mDefaultModeId, kModeMap.at(mDefaultModeId)};
    return ndk::ScopedAStatus::ok();
}

ndk::ScopedAStatus DisplayModes::setDisplayMode(int32_t modeID, bool makeDefault) {
    const auto iter = kModeMap.find(modeID);
    if (iter == kModeMap.end()) {
        return ndk::ScopedAStatus::fromExceptionCode(EX_UNSUPPORTED_OPERATION);
    }
    std::ofstream file(kModeBasePath);
    file << modeID;
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
    return ndk::ScopedAStatus::ok();
}

}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl
