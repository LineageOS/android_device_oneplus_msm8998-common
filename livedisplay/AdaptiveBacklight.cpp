/*
 * Copyright (C) 2019-2021 The LineageOS Project
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

#define LOG_TAG "AdaptiveBacklightService"

#include <android-base/logging.h>
#include <android-base/properties.h>
#include <android-base/unique_fd.h>
#include <cutils/sockets.h>
#include <poll.h>

#include "AdaptiveBacklight.h"

namespace {

android::status_t SendDppsCommand(const char* cmd) {
    android::base::unique_fd sock(
            socket_local_client("pps", ANDROID_SOCKET_NAMESPACE_RESERVED, SOCK_STREAM));
    if (sock < 0) {
        LOG(ERROR) << "Failed to connect to DPPS";
        return android::NO_INIT;
    }
    if (TEMP_FAILURE_RETRY(write(sock, cmd, strlen(cmd))) <= 0) {
        LOG(ERROR) << "Failed to send command to DPPS";
        return android::FAILED_TRANSACTION;
    }

    std::string result(10, 0);
    size_t len = result.length();
    char* buf = &result[0];
    ssize_t ret;
    while ((ret = TEMP_FAILURE_RETRY(read(sock, buf, len))) > 0) {
        if (ret == len) {
            break;
        }
        len -= ret;
        buf += ret;

        struct pollfd p = {.fd = sock, .events = POLLIN, .revents = 0};

        ret = poll(&p, 1, 20);
        if ((ret <= 0) || !(p.revents & POLLIN)) {
            break;
        }
    }

    if (result.compare(0, 7, "Success") != 0) {
        LOG(ERROR) << "Failed to send DPPS command: " << result;
        return android::BAD_VALUE;
    }
    return android::OK;
}

}  // anonymous namespace

namespace vendor {
namespace lineage {
namespace livedisplay {
namespace V2_0 {
namespace sdm {

using ::android::base::GetBoolProperty;

// Methods from ::vendor::lineage::livedisplay::V2_0::IAdaptiveBacklight follow.
Return<bool> AdaptiveBacklight::isEnabled() {
    return mEnabled;
}

Return<bool> AdaptiveBacklight::setEnabled(bool enabled) {
    if (mEnabled == enabled) {
        return true;
    }

    if (SendDppsCommand(enabled ? "foss:on" : "foss:off") == android::OK) {
        mEnabled = enabled;
        return true;
    }

    return false;
}

}  // namespace sdm
}  // namespace V2_0
}  // namespace livedisplay
}  // namespace lineage
}  // namespace vendor
