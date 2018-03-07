/*
 * Copyright (C) 2015 The CyanogenMod Open Source Project
 * Copyright (C) 2018 The LineageOS Project
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

#define LOG_TAG "amplifier_oneplus5"
//#define LOG_NDEBUG 0

#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>

#include <cutils/log.h>
#include <cutils/str_parms.h>

#include <hardware/audio_amplifier.h>
#include <hardware/hardware.h>

#include "platform.h"
#include "audio_hw.h"

#define AUDIO_PARAMETER_KEY_ANC                 "anc_enabled"

static int audio_mode = AUDIO_MODE_NORMAL;

#ifdef ANC_HEADSET_ENABLED
extern void audio_extn_set_anc_parameters(struct audio_device *adev,
                                   struct str_parms *parms);
#endif

static int amp_set_parameters(struct amplifier_device *device,
        struct str_parms *parms);

static int amp_set_mode(amplifier_device_t *device, audio_mode_t mode)
{
    ALOGD("%s: mode=%d\n", __func__, mode);
    audio_mode = mode;
    return 0;
}

static int amp_output_stream_standby(amplifier_device_t *device,
        struct audio_stream_out *stream)
{
#ifdef ANC_HEADSET_ENABLED
    struct stream_out *out = (struct stream_out *)stream;
    struct audio_device *adev = out->dev;
    struct str_parms *parms;

    ALOGD("%s\n", __func__);
    parms = str_parms_create();
    amp_set_parameters(device, parms);
    audio_extn_set_anc_parameters(adev, parms);
#endif
    return 0;
}

static int amp_set_parameters(struct amplifier_device *device,
        struct str_parms *parms)
{
#ifdef ANC_HEADSET_ENABLED
    if (audio_mode == AUDIO_MODE_IN_CALL ||
            audio_mode == AUDIO_MODE_IN_COMMUNICATION) {
        ALOGI("%s: Enabling ANC\n", __func__);
        str_parms_add_str(parms, AUDIO_PARAMETER_KEY_ANC, "true");
    } else {
        ALOGI("%s: Disabling ANC\n", __func__);
        str_parms_add_str(parms, AUDIO_PARAMETER_KEY_ANC, "false");
    }
#endif
    return 0;
}

static int amp_dev_close(hw_device_t *device)
{
    if (device)
        free(device);

    return 0;
}

static int amp_module_open(const hw_module_t *module, const char *name,
        hw_device_t **device)
{
    ALOGD("%s:%d: %s\n",
                __func__, __LINE__, name);

    if (strcmp(name, AMPLIFIER_HARDWARE_INTERFACE)) {
        ALOGE("%s:%d: %s does not match amplifier hardware interface name\n",
                __func__, __LINE__, name);
        return -ENODEV;
    }

    amplifier_device_t *amp_dev = calloc(1, sizeof(amplifier_device_t));
    if (!amp_dev) {
        ALOGE("%s:%d: Unable to allocate memory for amplifier device\n",
                __func__, __LINE__);
        return -ENOMEM;
    }

    amp_dev->common.tag = HARDWARE_DEVICE_TAG;
    amp_dev->common.module = (hw_module_t *) module;
    amp_dev->common.version = HARDWARE_DEVICE_API_VERSION(1, 0);
    amp_dev->common.close = amp_dev_close;

    amp_dev->set_mode = amp_set_mode;
    amp_dev->output_stream_standby = amp_output_stream_standby;
    amp_dev->set_parameters = amp_set_parameters;

    *device = (hw_device_t *) amp_dev;

    return 0;
}

static struct hw_module_methods_t hal_module_methods = {
    .open = amp_module_open,
};

amplifier_module_t HAL_MODULE_INFO_SYM = {
    .common = {
        .tag = HARDWARE_MODULE_TAG,
        .module_api_version = AMPLIFIER_MODULE_API_VERSION_0_1,
        .hal_api_version = HARDWARE_HAL_API_VERSION,
        .id = AMPLIFIER_HARDWARE_MODULE_ID,
        .name = "OnePlus5 audio amplifier HAL",
        .author = "The LineageOS Project",
        .methods = &hal_module_methods,
    },
};
