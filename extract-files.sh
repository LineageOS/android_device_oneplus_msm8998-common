#!/bin/bash
#
# Copyright (C) 2016 The CyanogenMod Project
# Copyright (C) 2017 The LineageOS Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

set -e

# Load extract_utils and do some sanity checks
MY_DIR="${BASH_SOURCE%/*}"
if [[ ! -d "$MY_DIR" ]]; then MY_DIR="$PWD"; fi

LINEAGE_ROOT="$MY_DIR"/../../..

HELPER="$LINEAGE_ROOT"/vendor/lineage/build/tools/extract_utils.sh
if [ ! -f "$HELPER" ]; then
    echo "Unable to find helper script at $HELPER"
    exit 1
fi
. "$HELPER"

# Default to NOT sanitizing the vendor folder before extraction
CLEAN_VENDOR=false

while [ "$1" != "" ]; do
    case $1 in
        -n | --no-cleanup )     CLEAN_VENDOR=false
                                ;;
        -s | --section )        shift
                                SECTION=$1
                                CLEAN_VENDOR=false
                                ;;
        * )                     SRC=$1
                                ;;
    esac
    shift
done

if [ -z "$SRC" ]; then
    SRC=adb
fi

function blob_fixup() {
    case "${1}" in
    product/lib64/libdpmframework.so | product/lib/libdpmframework.so )
        patchelf --add-needed "libshim_dpmframework.so" "${2}"
        ;;
    esac
}

# Initialize the helper
setup_vendor "$DEVICE_COMMON" "$VENDOR" "$LINEAGE_ROOT" true "$CLEAN_VENDOR"

extract "$MY_DIR"/proprietary-files-qc.txt "$SRC" "$SECTION"
extract "$MY_DIR"/proprietary-files.txt "$SRC" "$SECTION"

if [ -s "$MY_DIR"/../$DEVICE/proprietary-files.txt ]; then
    # Reinitialize the helper for device
    setup_vendor "$DEVICE" "$VENDOR" "$LINEAGE_ROOT" false "$CLEAN_VENDOR"

    extract "$MY_DIR"/../$DEVICE/proprietary-files.txt "$SRC" "$SECTION"
fi

COMMON_BLOB_ROOT="$LINEAGE_ROOT"/vendor/"$VENDOR"/"$DEVICE_COMMON"/proprietary

#
# Fix framework path
#
function fix_framework_path () {
    sed -i \
        's/\/system\/framework\//\/vendor\/framework\//g' \
        "$COMMON_BLOB_ROOT"/"$1"
}

fix_framework_path vendor/etc/permissions/com.fingerprints.extension.xml

#
# Fix product path
#
function fix_product_path () {
    sed -i \
        's/\/system\/framework\//\/system\/product\/framework\//g' \
        "$COMMON_BLOB_ROOT"/"$1"
}

fix_product_path product/etc/permissions/qcnvitems.xml
fix_product_path product/etc/permissions/vendor.qti.hardware.factory.xml
fix_product_path product/etc/permissions/vendor-qti-hardware-sensorscalibrate.xml

#
# Fix xml version
#
function fix_xml_version () {
    sed -i \
        's/xml version="2.0"/xml version="1.0"/' \
        "$COMMON_BLOB_ROOT"/"$1"
}

fix_xml_version product/etc/permissions/vendor.qti.hardware.data.connection-V1.0-java.xml
fix_xml_version product/etc/permissions/vendor.qti.hardware.data.connection-V1.1-java.xml

"$MY_DIR"/setup-makefiles.sh
