# Copyright (C) 2009 The Android Open Source Project
# Copyright (c) 2011, The Linux Foundation. All rights reserved.
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

import hashlib
import common
import re

def FullOTA_Assertions(info):
  AddModemAssertion(info)
  return

def IncrementalOTA_Assertions(info):
  AddModemAssertion(info)
  return

def FullOTA_InstallBegin(info):
  info.script.Mount("/system")
  UnlockVendorPartition(info)
  info.script.Unmount("/system")
  return

def IncrementalOTA_InstallBegin(info):
  info.script.Mount("/system")
  UnlockVendorPartition(info)
  info.script.Unmount("/system")
  return

def AddModemAssertion(info):
  android_info = info.input_zip.read("OTA/android-info.txt")
  m = re.search(r'require\s+version-modem\s*=\s*(.+)', android_info)
  if m:
    version = m.group(1).rstrip()
    if len(version) and '*' not in version:
      cmd = 'assert(oneplus.verify_modem("' + version + '") == "1");'
      info.script.AppendExtra(cmd)
  return

def UnlockVendorPartition(info):
  info.script.AppendExtra('package_extract_file("install/bin/sgdisk-op5", "/tmp/sgdisk-op5");');
  info.script.AppendExtra('package_extract_file("install/bin/unlock-vendor.sh", "/tmp/unlock-vendor.sh");');
  info.script.AppendExtra('set_metadata("/tmp/sgdisk-op5", "uid", 0, "gid", 0, "mode", 0755);');
  info.script.AppendExtra('set_metadata("/tmp/unlock-vendor.sh", "uid", 0, "gid", 0, "mode", 0755);');
  info.script.AppendExtra('ui_print("Checking for vendor partition...");');
  info.script.AppendExtra('if run_program("/tmp/unlock-vendor.sh") != 0 then');
  info.script.AppendExtra('abort("Unlocking vendor partition failed.");');
  info.script.AppendExtra('endif;');
