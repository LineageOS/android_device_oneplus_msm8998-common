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

#define LOG_TAG "LineageHW-PictureAdjustmentService"

#include <dlfcn.h>

#include "Constants.h"
#include "PictureAdjustment.h"
#include "Types.h"

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace implementation {

static sp<PictureAdjustment> sInstance;

PictureAdjustment::PictureAdjustment(void* libHandle, uint64_t cookie) {
    sInstance = this;

    mLibHandle = libHandle;
    mCookie = cookie;
    disp_api_get_feature_version =
        reinterpret_cast<int32_t (*)(uint64_t, uint32_t, void*, uint32_t*)>(
            dlsym(mLibHandle, "disp_api_get_feature_version"));
    disp_api_get_global_pa_range = reinterpret_cast<int32_t (*)(uint64_t, uint32_t, void*)>(
        dlsym(mLibHandle, "disp_api_get_global_pa_range"));
    disp_api_get_global_pa_config =
        reinterpret_cast<int32_t (*)(uint64_t, uint32_t, uint32_t*, void*)>(
            dlsym(mLibHandle, "disp_api_get_global_pa_config"));
    disp_api_set_global_pa_config =
        reinterpret_cast<int32_t (*)(uint64_t, uint32_t, uint32_t, void*)>(
            dlsym(mLibHandle, "disp_api_set_global_pa_config"));
    mDefaultPictureAdjustment = HSIC{};
}

bool PictureAdjustment::isSupported() {
    sdm_feature_version version{};
    hsic_ranges r{};
    uint32_t flags = 0;

    if (disp_api_get_feature_version == nullptr ||
        disp_api_get_feature_version(mCookie, PICTURE_ADJUSTMENT_FEATURE, &version, &flags) != 0) {
        return false;
    }

    if (version.x <= 0 && version.y <= 0 && version.z <= 0) {
        return false;
    }

    if (disp_api_get_global_pa_range == nullptr ||
        disp_api_get_global_pa_range(mCookie, 0, &r) != 0) {
        return false;
    }

    return r.hue.max != 0 && r.hue.min != 0 && r.saturation.max != 0.f &&
             r.saturation.min != 0.f && r.intensity.max != 0.f && r.intensity.min != 0.f &&
             r.contrast.max != 0.f && r.contrast.min != 0.f;
}

HSIC PictureAdjustment::getPictureAdjustmentInternal() {
    hsic_config config{};
    uint32_t enable = 0;

    if (disp_api_get_global_pa_config != nullptr) {
        if (disp_api_get_global_pa_config(mCookie, 0, &enable, &config) == 0) {
            return HSIC{static_cast<float>(config.data.hue), config.data.saturation,
                        config.data.intensity, config.data.contrast,
                        config.data.saturationThreshold};
        }
    }

    return HSIC{};
}

void PictureAdjustment::updateDefaultPictureAdjustment() {
    if (sInstance != nullptr) {
        sInstance->mDefaultPictureAdjustment = sInstance->getPictureAdjustmentInternal();
    }
}

// Methods from ::vendor::lineage::livedisplay::V2_0::IPictureAdjustment follow.
Return<void> PictureAdjustment::getHueRange(getHueRange_cb _hidl_cb) {
    FloatRange range{};
    hsic_ranges r{};

    if (disp_api_get_global_pa_range != nullptr) {
        if (disp_api_get_global_pa_range(mCookie, 0, &r) == 0) {
            range.max = r.hue.max;
            range.min = r.hue.min;
            range.step = r.hue.step;
        }
    }

    _hidl_cb(range);
    return Void();
}

Return<void> PictureAdjustment::getSaturationRange(getSaturationRange_cb _hidl_cb) {
    FloatRange range{};
    hsic_ranges r{};

    if (disp_api_get_global_pa_range != nullptr) {
        if (disp_api_get_global_pa_range(mCookie, 0, &r) == 0) {
            range.max = r.saturation.max;
            range.min = r.saturation.min;
            range.step = r.saturation.step;
        }
    }

    _hidl_cb(range);
    return Void();
}

Return<void> PictureAdjustment::getIntensityRange(getIntensityRange_cb _hidl_cb) {
    FloatRange range{};
    hsic_ranges r{};

    if (disp_api_get_global_pa_range != nullptr) {
        if (disp_api_get_global_pa_range(mCookie, 0, &r) == 0) {
            range.max = r.intensity.max;
            range.min = r.intensity.min;
            range.step = r.intensity.step;
        }
    }

    _hidl_cb(range);
    return Void();
}

Return<void> PictureAdjustment::getContrastRange(getContrastRange_cb _hidl_cb) {
    FloatRange range{};
    hsic_ranges r{};

    if (disp_api_get_global_pa_range != nullptr) {
        if (disp_api_get_global_pa_range(mCookie, 0, &r) == 0) {
            range.max = r.contrast.max;
            range.min = r.contrast.min;
            range.step = r.contrast.step;
        }
    }

    _hidl_cb(range);
    return Void();
}

Return<void> PictureAdjustment::getSaturationThresholdRange(getSaturationThresholdRange_cb _hidl_cb) {
    FloatRange range{};
    hsic_ranges r{};

    if (disp_api_get_global_pa_range != nullptr) {
        if (disp_api_get_global_pa_range(mCookie, 0, &r) == 0) {
            range.max = r.saturationThreshold.max;
            range.min = r.saturationThreshold.min;
            range.step = r.saturationThreshold.step;
        }
    }

    _hidl_cb(range);
    return Void();
}

Return<void> PictureAdjustment::getPictureAdjustment(getPictureAdjustment_cb _hidl_cb) {
    _hidl_cb(getPictureAdjustmentInternal());
    return Void();
}

Return<void> PictureAdjustment::getDefaultPictureAdjustment(getDefaultPictureAdjustment_cb _hidl_cb) {
    _hidl_cb(mDefaultPictureAdjustment);
    return Void();
}

Return<bool> PictureAdjustment::setPictureAdjustment(
    const ::vendor::lineage::livedisplay::V2_0::HSIC& hsic) {
    hsic_config config = {0,
                          {static_cast<int32_t>(hsic.hue), hsic.saturation, hsic.intensity,
                           hsic.contrast, hsic.saturationThreshold}};

    if (disp_api_set_global_pa_config != nullptr) {
        return disp_api_set_global_pa_config(mCookie, 0, 1, &config) == 0;
    }

    return false;
}

}  // namespace sdm
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
