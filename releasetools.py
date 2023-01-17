#
# Copyright (C) 2009 The Android Open Source Project
# Copyright (c) 2011, The Linux Foundation. All rights reserved.
# Copyright (C) 2017 The LineageOS Project
#
# SPDX-License-Identifier: Apache-2.0
#

import hashlib
import common
import re

def FullOTA_Assertions(info):
  AddVendorAssertion(info)

  if "RADIO/filemap" not in info.input_zip.namelist():
    AddModemAssertion(info)
    return

  filemap = info.input_zip.read("RADIO/filemap").decode('utf-8').splitlines()

  for file in filemap:
    filename = file.split(" ")[0]
    if "RADIO/{}".format(filename) not in info.input_zip.namelist():
      # Firmware files not present, assert
      AddModemAssertion(info)
      return

  # Firmware files present, copy to OTA zip
  CopyBlobs(info.input_zip, info.output_zip)
  # And flash if necessary
  AddFirmwareUpdate(info, filemap)
  return

def IncrementalOTA_Assertions(info):
  AddVendorAssertion(info)
  AddModemAssertion(info)
  return

def CopyBlobs(input_zip, output_zip):
  for info in input_zip.infolist():
    f = info.filename
    # Copy files in 'RADIO' to output zip 'firmware-update'
    if f.startswith("RADIO/") and (f.__len__() > len("RADIO/")):
      fn = f[6:]
      common.ZipWriteStr(output_zip, "firmware-update/" + fn, input_zip.read(f))

def AddVendorAssertion(info):
  cmd = 'assert(oneplus.file_exists("/dev/block/bootdevice/by-name/vendor") == "1" || \
    abort("Error: Vendor partition doesn\'t exist!"););'
  info.script.AppendExtra(cmd)
  return

def AddModemAssertion(info):
  android_info = info.input_zip.read("OTA/android-info.txt").decode('utf-8')
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
  android_info = info.input_zip.read("OTA/android-info.txt").decode('utf-8')
  m = re.search(r'require\s+version-modem\s*=\s*(.+)', android_info)
  f = re.search(r'require\s+version-firmware\s*=\s*(.+)', android_info)
  if m and f:
    version_modem = m.group(1).rstrip()
    version_firmware = f.group(1).rstrip()
    if ((len(version_modem) and '*' not in version_modem) and \
        (len(version_firmware) and '*' not in version_firmware)):
      info.script.AppendExtra('ifelse(oneplus.verify_modem("' + version_modem + '") != "1",')
      info.script.AppendExtra('(')
      info.script.AppendExtra('  ui_print("Upgrading firmware to ' + version_firmware + '");')
      for file in filemap:
        filename = file.split(" ")[0]
        filepath = file.split(" ")[-1]
        info.script.AppendExtra('package_extract_file("firmware-update/' + filename + '", "' + filepath + '");')
      info.script.AppendExtra('),')
      info.script.AppendExtra('(')
      info.script.AppendExtra('  ui_print("Firmware is up-to-date");')
      info.script.AppendExtra(')')
      info.script.AppendExtra(');')
