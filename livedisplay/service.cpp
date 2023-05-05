/*
 * Copyright (C) 2019-2021 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.livedisplay@2.0-service.oneplus_msm8998"

#include <android-base/logging.h>
#include <binder/ProcessState.h>
#include <hidl/HidlTransportSupport.h>
#include <livedisplay/sdm/PictureAdjustment.h>

#include "DisplayModes.h"

using ::vendor::lineage::livedisplay::V2_0::IDisplayModes;
using ::vendor::lineage::livedisplay::V2_0::IPictureAdjustment;
using ::vendor::lineage::livedisplay::V2_0::implementation::DisplayModes;
using ::vendor::lineage::livedisplay::V2_0::sdm::PictureAdjustment;
using ::vendor::lineage::livedisplay::V2_0::sdm::SDMController;

int main() {
    std::shared_ptr<SDMController> controller = std::make_shared<SDMController>();
    android::sp<IDisplayModes> modesService = new DisplayModes();
    android::sp<IPictureAdjustment> paService = new PictureAdjustment(controller);

    LOG(DEBUG) << "LiveDisplay HAL service is starting.";

    android::hardware::configureRpcThreadpool(1 /*threads*/, true /*callerWillJoin*/);

    if (modesService->registerAsService() != android::OK) {
        LOG(ERROR) << "Cannot register display modes HAL service.";
        return 1;
    }

    if (paService->registerAsService() != android::OK) {
        LOG(ERROR) << "Cannot register picture adjustment HAL service.";
        return 1;
    }

    LOG(INFO) << "LiveDisplay HAL service is ready.";

    android::hardware::joinRpcThreadpool();

    LOG(ERROR) << "LiveDisplay HAL service failed to join thread pool.";
    return 1;
}
