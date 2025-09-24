/*
 * Copyright (C) 2019-2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef VENDOR_LINEAGE_LIVEDISPLAY_DISPLAYMODES_H
#define VENDOR_LINEAGE_LIVEDISPLAY_DISPLAYMODES_H

#include <aidl/vendor/lineage/livedisplay/BnDisplayModes.h>
#include <map>

namespace aidl {
namespace vendor {
namespace lineage {
namespace livedisplay {

class DisplayModes : public BnDisplayModes {
  public:
    DisplayModes();

    using DisplayModeSetCallback = std::function<void()>;
    inline void registerDisplayModeSetCallback(DisplayModeSetCallback callback) {
        mOnDisplayModeSet = callback;
    }

    // Methods from ::aidl::vendor::lineage::livedisplay::BnDisplayModes follow.
    ndk::ScopedAStatus getDisplayModes(std::vector<DisplayMode>* _aidl_return) override;
    ndk::ScopedAStatus getCurrentDisplayMode(DisplayMode* _aidl_return) override;
    ndk::ScopedAStatus getDefaultDisplayMode(DisplayMode* _aidl_return) override;
    ndk::ScopedAStatus setDisplayMode(int32_t modeID, bool makeDefault) override;

  private:
    static const std::map<int32_t, std::string> kModeMap;
    int32_t mCurrentModeId;
    int32_t mDefaultModeId;
    DisplayModeSetCallback mOnDisplayModeSet;
};

}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
}  // namespace aidl

#endif  // VENDOR_LINEAGE_LIVEDISPLAY_DISPLAYMODES_H
