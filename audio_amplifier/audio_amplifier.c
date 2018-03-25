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

typedef struct amp {
    amplifier_device_t amp_dev;
    audio_mode_t audio_mode;
    bool anc_enabled;
} amp_t;

static amp_t *amp = NULL;

#ifdef ANC_HEADSET_ENABLED
extern void audio_extn_set_anc_parameters(struct audio_device *adev,
                                   struct str_parms *parms);
#endif

static int amp_set_parameters(struct amplifier_device *device,
        struct str_parms *parms);

static int amp_set_mode(amplifier_device_t *device, audio_mode_t mode)
{
    amp_t *amp = (amp_t*) device;
    ALOGD("%s: mode=%d\n", __func__, mode);
    amp->audio_mode = mode;
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
    amp_t *amp = (amp_t*) device;

    if (amp->audio_mode == AUDIO_MODE_IN_CALL || amp->audio_mode == AUDIO_MODE_IN_COMMUNICATION) {
        if (!amp->anc_enabled) {
            ALOGI("%s: Enabling ANC\n", __func__);
            str_parms_add_str(parms, AUDIO_PARAMETER_KEY_ANC, "true");
            amp->anc_enabled = true;
        }
    } else {
        if (amp->anc_enabled) {
            ALOGI("%s: Disabling ANC\n", __func__);
            str_parms_add_str(parms, AUDIO_PARAMETER_KEY_ANC, "false");
            amp->anc_enabled = false;
        }
    }
#endif
    return 0;
}

static int amp_dev_close(hw_device_t *device)
{
    if (device) {
        amp_t *amp = (amp_t*) device;
        free(amp);
    }

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

    if (amp) {
        ALOGE("%s:%d: Unable to open second instance of amplifier\n",
                __func__, __LINE__);
        return -EBUSY;
    }

    amp = calloc(1, sizeof(amp_t));
    if (!amp) {
        ALOGE("%s:%d: Unable to allocate memory for amplifier device\n",
                __func__, __LINE__);
        return -ENOMEM;
    }

    amp->amp_dev.common.tag = HARDWARE_DEVICE_TAG;
    amp->amp_dev.common.module = (hw_module_t *) module;
    amp->amp_dev.common.version = HARDWARE_DEVICE_API_VERSION(1, 0);
    amp->amp_dev.common.close = amp_dev_close;

    amp->amp_dev.set_mode = amp_set_mode;
    amp->amp_dev.output_stream_standby = amp_output_stream_standby;
    amp->amp_dev.set_parameters = amp_set_parameters;

    amp->audio_mode = AUDIO_MODE_NORMAL;
    amp->anc_enabled = false;

    *device = (hw_device_t *) amp;

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
