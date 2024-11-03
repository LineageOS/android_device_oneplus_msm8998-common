#!/usr/bin/env -S PYTHONPATH=../../../tools/extract-utils python3
#
# SPDX-FileCopyrightText: 2024 The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#

from extract_utils.fixups_blob import (
    blob_fixup,
    blob_fixups_user_type,
)
from extract_utils.main import (
    ExtractUtils,
    ExtractUtilsModule,
)

blob_fixups: blob_fixups_user_type = {
    'product/etc/permissions/vendor-qti-hardware-sensorscalibrate.xml': blob_fixup()
        .binary_regex_replace(b'/system/framework/', b'/system/product/framework/'),
    'system_ext/lib64/lib-imsvideocodec.so': blob_fixup()
        .add_needed('libgui_shim.so')
        .replace_needed('libqdMetaData.so', 'libqdMetaData.system.so'),
    'vendor/etc/permissions/com.fingerprints.extension.xml': blob_fixup()
        .binary_regex_replace(b'/system/framework/', b'/vendor/framework/'),
    (
        'vendor/lib/libSonyIMX371RmscLibrary.so',
        'vendor/lib/libmms_gyro_vstab.so',
        'vendor/lib/libmms_gyro_vstab_auth.so',
    ): blob_fixup()
        .replace_needed('libstdc++.so', 'libstdc++_vendor.so'),
    'vendor/lib64/libwvhidl.so': blob_fixup()
        .add_needed('libcrypto_shim.so'),
}  # fmt: skip

module = ExtractUtilsModule(
    'msm8998-common',
    'oneplus',
    blob_fixups=blob_fixups,
    check_elf=False,
)

if __name__ == '__main__':
    utils = ExtractUtils.device(module)
    utils.run()
