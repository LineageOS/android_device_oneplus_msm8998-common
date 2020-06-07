#!/bin/bash
#
# Copyright (C) 2017 The LineageOS Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
# http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

set -e

INITIAL_COPYRIGHT_YEAR=2017

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

# Initialize the helper
setup_vendor "$DEVICE_COMMON" "$VENDOR" "$LINEAGE_ROOT" true

# Copyright headers and guards
write_headers "cheeseburger dumpling"

# The standard blobs
write_makefiles "$MY_DIR"/proprietary-files.txt true

# Qualcomm BSP blobs - we put a conditional around here
# in case the BSP is actually being built
printf '\n%s\n' "ifeq (\$(QCPATH),)" >> "$PRODUCTMK"
printf '\n%s\n' "ifeq (\$(QCPATH),)" >> "$ANDROIDMK"

write_makefiles "$MY_DIR"/proprietary-files-qc.txt true

# Qualcomm performance blobs - conditional as well
# in order to support Cyanogen OS builds
cat << EOF >> "$PRODUCTMK"
endif

EOF

cat << EOF >> "$ANDROIDMK"

endif

\$(shell mkdir -p \$(TARGET_OUT_VENDOR)/lib/egl && pushd \$(TARGET_OUT_VENDOR)/lib > /dev/null && ln -sf egl/libEGL_adreno.so libEGL_adreno.so && popd > /dev/null)
\$(shell mkdir -p \$(TARGET_OUT_VENDOR)/lib64/egl && pushd \$(TARGET_OUT_VENDOR)/lib64 > /dev/null && ln -sf egl/libEGL_adreno.so libEGL_adreno.so && popd > /dev/null)
EOF

# We are done!
write_footers

if [ -s "$MY_DIR"/../$DEVICE/proprietary-files.txt ]; then
    # Reinitialize the helper for device
    INITIAL_COPYRIGHT_YEAR="$DEVICE_BRINGUP_YEAR"
    setup_vendor "$DEVICE" "$VENDOR" "$LINEAGE_ROOT" false

    # Copyright headers and guards
    write_headers

    # The standard device blobs
    write_makefiles "$MY_DIR"/../$DEVICE/proprietary-files.txt true

    # We are done!
    write_footers
fi
