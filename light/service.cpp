/*
 * Copyright (C) 2018-2021 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "android.hardware.light@2.0-service.oneplus_msm8998"

#include <android-base/logging.h>
#include <hidl/HidlTransportSupport.h>

#include "Light.h"

// Generated HIDL files
using android::hardware::light::V2_0::ILight;
using android::hardware::light::V2_0::implementation::Light;

int main() {
    android::sp<ILight> service = new Light();

    android::hardware::configureRpcThreadpool(1 /*threads*/, true /*callerWillJoin*/);

    if (service->registerAsService() != android::OK) {
        LOG(ERROR) << "Cannot register Light HAL service.";
        return 1;
    }

    LOG(INFO) << "Light HAL ready.";

    android::hardware::joinRpcThreadpool();

    LOG(ERROR) << "Light HAL failed to join thread pool.";
    return 1;
}
