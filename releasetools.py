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
  AddVendorAssertion(info)

  if "INSTALL/firmware-update/filemap" not in info.input_zip.namelist():
    AddModemAssertion(info)
    return

  filemap = info.input_zip.read("INSTALL/firmware-update/filemap").splitlines()
  for line in filemap:
    filename = line.split(" ")[0]

  for file in filemap:
    filename = line.split(" ")[0]
    if "INSTALL/{}".format(filename) not in info.input_zip.namelist():
      # Firmware files not present, assert
      info.script.AppendExtra('ui_print("Firmware files not present");')
      AddModemAssertion(info)
      return

  # Firmware files present, update if necessary
  info.script.AppendExtra('ui_print("Firmware files are present");')
  AddFirmwareUpdate(info, filemap)
  return

def IncrementalOTA_Assertions(info):
  AddVendorAssertion(info)
  AddModemAssertion(info)
  return

def AddVendorAssertion(info):
  cmd = 'assert(oneplus.file_exists("/dev/block/bootdevice/by-name/vendor") == "1" || \
    abort("Error: Vendor partition doesn\'t exist!"););'
  info.script.AppendExtra(cmd)
  return

def AddModemAssertion(info):
  android_info = info.input_zip.read("OTA/android-info.txt")
  m = re.search(r'require\s+version-modem\s*=\s*(.+)', android_info)
  f = re.search(r'require\s+version-firmware\s*=\s*(.+)', android_info)
  if m and f:
    version_modem = m.group(1).rstrip()
    version_firmware = f.group(1).rstrip()
    if ((len(version_modem) and '*' not in version_modem) and \
        (len(version_firmware) and '*' not in version_firmware)):
      cmd = 'assert(oneplus.verify_modem("' + version_modem + '") == "1" || \
        abort("Error: This package requires firmware version ' + version_firmware + \
        ' or newer. Please upgrade firmware and retry!"););'
      info.script.AppendExtra(cmd)
  return

def AddFirmwareUpdate(info, filemap):
  android_info = info.input_zip.read("OTA/android-info.txt")
  m = re.search(r'require\s+version-modem\s*=\s*(.+)', android_info)
  f = re.search(r'require\s+version-firmware\s*=\s*(.+)', android_info)
  if m and f:
    version_modem = m.group(1).rstrip()
    version_firmware = f.group(1).rstrip()

    if ((len(version_modem) and '*' not in version_modem) and \
        (len(version_firmware) and '*' not in version_firmware)):

      info.script.AppendExtra('ifelse(oneplus.verify_modem("' + version_modem + '") != "1",')
      info.script.AppendExtra('(')
      info.script.AppendExtra('  ui_print("Upgrading firmware");')

      for file in filemap:
        filename = file.split(" ")[0]
        filepath = file.split(" ")[-1]
        info.script.AppendExtra('package_extract_file("install/' + filename + '", "' + filepath + '");')

      info.script.AppendExtra('),')
      info.script.AppendExtra('(')
      info.script.AppendExtra('  ui_print("Firmware is up-to-date");')
      info.script.AppendExtra(')')
      info.script.AppendExtra(');')
