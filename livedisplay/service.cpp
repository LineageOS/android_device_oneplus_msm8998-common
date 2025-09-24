/*
 * Copyright (C) 2019-2025 The LineageOS Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#define LOG_TAG "vendor.lineage.livedisplay-service.oneplus_msm8998"

#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>
#include <binder/ProcessState.h>
#include <livedisplay/sysfs/SunlightEnhancement.h>

#include "DisplayModes.h"

using ::aidl::vendor::lineage::livedisplay::DisplayModes;
using ::aidl::vendor::lineage::livedisplay::sysfs::SunlightEnhancement;

int main() {
    android::ProcessState::self()->setThreadPoolMaxThreadCount(1);
    android::ProcessState::self()->startThreadPool();

    std::shared_ptr<DisplayModes> dm = ndk::SharedRefBase::make<DisplayModes>();
    std::shared_ptr<SunlightEnhancement> se = ndk::SharedRefBase::make<SunlightEnhancement>();

    std::string instance = std::string() + DisplayModes::descriptor + "/default";
    binder_status_t status = AServiceManager_addService(dm->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK);

    instance = std::string() + SunlightEnhancement::descriptor + "/default";
    status = AServiceManager_addService(se->asBinder().get(), instance.c_str());
    CHECK_EQ(status, STATUS_OK);

    LOG(INFO) << "LiveDisplay HAL service ready.";

    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
