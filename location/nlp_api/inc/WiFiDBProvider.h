/* Copyright (c) 2020 The Linux Foundation. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are
 * met:
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above
 *       copyright notice, this list of conditions and the following
 *       disclaimer in the documentation and/or other materials provided
 *       with the distribution.
 *     * Neither the name of The Linux Foundation nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
 * BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
 * OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
 * IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef WIFI_DB_PROV_H
#define WIFI_DB_PROV_H

#include <DBCommon.h>

/******************************************************************************
WiFiDBReceiver
******************************************************************************/

typedef struct {
    void (*requestAPObsLocData)();
} WiFiDBProvider;

/******************************************************************************
ResponseListener
******************************************************************************/
typedef struct {
    uint8_t macAddress[6];
    float rssi;
    uint64_t deltaTime;
    char ssid[8];
    uint16_t channelNumber;
} ApScan;

typedef struct {
    NlpLocation location;
    CellInfo cellInfo;
    uint64_t scanTimestamp;
    ApScan* ap_scan_list;
    uint16_t ap_scan_list_count;
} APObsLocData;

/** @brief
    All the memory pointers returned in these callbacks will be freed after call returns.
    Implementation of these callbacks shall copy the needed data before returning.
*/
typedef struct {
    void (*onApObsLocDataAvailable)(const APObsLocData* ap_obs_list, uint16_t ap_obs_list_count,
            ApBsListStatus ap_status);
    void (*onServiceRequest)();
} WiFiDBProviderResponseListener;

#endif /* WIFI_DB_PROV_H */
