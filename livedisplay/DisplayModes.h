/*
 * Copyright (C) 2019-2021 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_V2_0_DISPLAYMODES_H
#define VENDOR_LINEAGE_LIVEDISPLAY_V2_0_DISPLAYMODES_H

#include <vendor/lineage/livedisplay/2.0/IDisplayModes.h>
#include <map>

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

using ::android::hardware::Return;
using ::android::hardware::Void;
using ::android::sp;

class DisplayModes : public IDisplayModes {
  public:
    DisplayModes();

    using DisplayModeSetCallback = std::function<void()>;
    inline void registerDisplayModeSetCallback(DisplayModeSetCallback callback) {
        mOnDisplayModeSet = callback;
    }

    // Methods from ::vendor::lineage::livedisplay::V2_0::IDisplayModes follow.
    Return<void> getDisplayModes(getDisplayModes_cb resultCb) override;
    Return<void> getCurrentDisplayMode(getCurrentDisplayMode_cb resultCb) override;
    Return<void> getDefaultDisplayMode(getDefaultDisplayMode_cb ResultCb) override;
    Return<bool> setDisplayMode(int32_t modeID, bool makeDefault) override;

  private:
    static const std::map<int32_t, std::string> kModeMap;
    int32_t mCurrentModeId;
    int32_t mDefaultModeId;
    DisplayModeSetCallback mOnDisplayModeSet;
};

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor

#endif  // VENDOR_LINEAGE_LIVEDISPLAY_V2_0_DISPLAYMODES_H
