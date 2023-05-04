/*
 * Copyright (C) 2023 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#include "SunlightEnhancement.h"

#include <android-base/file.h>
#include <android-base/strings.h>

namespace {
constexpr const char* kFileHbm = "/sys/devices/virtual/graphics/fb0/hbm";
};  // anonymous namespace

using ::android::base::ReadFileToString;
using ::android::base::Trim;
using ::android::base::WriteStringToFile;

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

SunlightEnhancement::SunlightEnhancement() {
}

bool SunlightEnhancement::isSupported() {
    return !access(kFileHbm, R_OK | W_OK);
}

// Methods from ::vendor::lineage::livedisplay::V2_0::ISunlightEnhancement follow.
Return<bool> SunlightEnhancement::isEnabled() {
    std::string tmp;
    int32_t contents = 0;

    if (ReadFileToString(kFileHbm, &tmp)) {
        contents = std::stoi(Trim(tmp));
    }

    return contents > 0;
}

Return<bool> SunlightEnhancement::setEnabled(bool enabled) {
    return WriteStringToFile(enabled ? "1" : "0", kFileHbm, true);
}

}  // namespace implementation
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
