/*
 *
 *  Copyright (c) 2013, The Linux Foundation. All rights reserved.
 *  Not a Contribution, Apache license notifications and license are retained
 *  for attribution purposes only.
 *
 * Copyright (C) 2012 The Android Open Source Project
 * Copyright (C) 2018-2020 The LineageOS Project
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

#ifndef _BDROID_BUILDCFG_H
#define _BDROID_BUILDCFG_H

#pragma push_macro("PROPERTY_VALUE_MAX")

#include <cutils/properties.h>
#include <string.h>

static inline const char *BtmGetDefaultName()
{
    char product_device[PROPERTY_VALUE_MAX];
    property_get("ro.lineage.device", product_device, "");

    if (strstr(product_device, "cheeseburger"))
        return "OnePlus 5";
    if (strstr(product_device, "dumpling"))
        return "OnePlus 5T";

    // Fallback to ro.product.model
    return "";
}

#define BTM_DEF_LOCAL_NAME BtmGetDefaultName()
// Disables read remote device feature
#define MAX_ACL_CONNECTIONS   16
#define MAX_L2CAP_CHANNELS    16
#define BLE_VND_INCLUDED   TRUE

#pragma pop_macro("PROPERTY_VALUE_MAX")

#endif
