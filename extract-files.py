#!/usr/bin/env -S PYTHONPATH=../../../tools/extract-utils python3
#
# SPDX-FileCopyrightText: 2024 The LineageOS Project
# SPDX-License-Identifier: Apache-2.0
#

from extract_utils.fixups_blob import (
    blob_fixup,
    blob_fixups_user_type,
)
from extract_utils.fixups_lib import (
    lib_fixup_remove,
    lib_fixups,
    lib_fixups_user_type,
)
from extract_utils.main import (
    ExtractUtils,
    ExtractUtilsModule,
)

namespace_imports = [
    'device/oneplus/msm8998-common',
    'hardware/qcom-caf/msm8998',
    'hardware/qcom-caf/wlan',
    'hardware/oneplus',
    'vendor/oneplus/cheeseburger',
    'vendor/oneplus/dumpling',
    'vendor/qcom/opensource/dataservices',
]

def lib_fixup_vendor_suffix(lib: str, partition: str, *args, **kwargs):
    return f'{lib}_{partition}' if partition == 'vendor' else None

lib_fixups: lib_fixups_user_type = {
    **lib_fixups,
    (
        'com.qualcomm.qti.dpm.api@1.0',
        'vendor.qti.imsrtpservice@3.0',
    ): lib_fixup_vendor_suffix,
    (
        'libarcsoft_smart_denoise',
        'libarcsoft_high_dynamic_range',
        'libvpplibrary',
        'libwpa_client',
        'lib-rcsconfig',
    ): lib_fixup_remove,
}

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
    lib_fixups=lib_fixups,
    namespace_imports=namespace_imports,
)

if __name__ == '__main__':
    utils = ExtractUtils.device(module)
    utils.run()
