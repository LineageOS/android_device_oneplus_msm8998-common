#
# Copyright (C) 2018 The LineageOS Project
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

# AANC
PRODUCT_PROPERTY_OVERRIDES += \
    persist.vendor.audio.aanc.enable=true

# Audio
PRODUCT_PROPERTY_OVERRIDES += \
    af.fast_track_multiplier=2 \
    vendor.audio_hal.period_size=192 \
    audio.offload.video=true \
    audio.offload.pcm.16bit.enable=true \
    audio.offload.pcm.24bit.enable=true \
    audio.deep_buffer.media=true \
    audio.cts.media=false \
    audio.offload.min.duration.secs=30 \
    persist.dirac.acs.controller=qem \
    ro.dirac.acs.storeSettings=1 \
    ro.dirac.ignore_error=1

# Audio new
PRODUCT_PROPERTY_OVERRIDES += \
    vendor.audio_hal.period_size=192 \
    ro.vendor.audio.sdk.fluencetype=fluencepro \
    persist.vendor.audio.fluence.voicecall=true \
    persist.vendor.audio.fluence.voicerec=true \
    persist.vendor.audio.fluence.speaker=true \
    vendor.audio.tunnel.encode=false \
    persist.vendor.audio.ras.enabled=false \
    vendor.audio.offload.buffer.size.kb=32 \
    vendor.audio.offload.track.enable=true \
    vendor.voice.path.for.pcm.voip=true \
    vendor.audio.offload.multiaac.enable=true \
    vendor.audio.dolby.ds2.enabled=false \
    vendor.audio.dolby.ds2.hardbypass=false \
    vendor.audio.offload.multiple.enabled=true \
    vendor.audio.offload.passthrough=false \
    ro.vendor.audio.sdk.ssr=false \
    vendor.audio.offload.gapless.enabled=true \
    vendor.audio.safx.pbe.enabled=true \
    vendor.audio.parser.ip.buffer.size=0 \
    vendor.audio.flac.sw.decoder.24bit=true \
    persist.vendor.bt.a2dp_offload_cap=false \
    vendor.audio.use.sw.alac.decoder=true \
    vendor.audio.use.sw.ape.decoder=true \
    vendor.audio.hw.aac.encoder=true \
    vendor.fm.a2dp.conc.disabled=true \
    vendor.audio.noisy.broadcast.delay=600 \
    ro.config.media_vol_steps=25 \
    ro.af.client_heap_size_kbyte=7168

# Battery
PRODUCT_PROPERTY_OVERRIDES += \
    ro.cutoff_voltage_mv=3200

# Bluetooth
PRODUCT_PROPERTY_OVERRIDES += \
    bt.max.hfpclient.connections=1 \
    persist.bt.a2dp_offload_cap=false \
    qcom.bluetooth.soc=cherokee \
    ro.bluetooth.a4wp=false \
    ro.bluetooth.emb_wp_mode=true \
    ro.bluetooth.wipower=true

# Cabl
PRODUCT_PROPERTY_OVERRIDES += \
    ro.qualcomm.cabl=0

# Camera
PRODUCT_PROPERTY_OVERRIDES += \
    vendor.camera.aux.packagelist=org.codeaurora.snapcam,com.oneplus.camera,com.android.camera,org.lineageos.snap \
    persist.camera.mobicat=2 \
    persist.camera.stats.debugexif=3080192 \
    persist.ts.rtmakeup=false

# CNE
PRODUCT_PROPERTY_OVERRIDES += \
    persist.vendor.cne.feature=1

# Core control
PRODUCT_PROPERTY_OVERRIDES += \
    ro.vendor.qti.core_ctl_min_cpu=2 \
    ro.vendor.qti.core_ctl_max_cpu=4

# Data
PRODUCT_PROPERTY_OVERRIDES += \
    ro.use_data_netmgrd=true \
    persist.data.netmgrd.qos.enable=true \
    persist.data.mode=concurrent

# DPM
PRODUCT_PROPERTY_OVERRIDES += \
    persist.vendor.dpm.feature=11

# DRM
PRODUCT_PROPERTY_OVERRIDES += \
    drm.service.enabled=true

# FRP
PRODUCT_PROPERTY_OVERRIDES += \
    ro.frp.pst=/dev/block/bootdevice/by-name/config

# Graphics
PRODUCT_PROPERTY_OVERRIDES += \
    ro.sf.lcd_density=420 \
    ro.opengles.version=196610 \
    debug.gralloc.enable_fb_ubwc=1 \
    debug.sf.hw=1 \
    dev.pm.dyn_samplingrate=1 \
    persist.demo.hdmirotationlock=false \
    persist.debug.wfd.enable=1 \
    persist.sys.force_sw_gles=0 \
    persist.sys.wfd.virtual=0 \
    persist.hwc.enable_vds=1 \
    vendor.gralloc.enable_fb_ubwc=1 \
    vendor.gralloc.disable_ubwc=0 \
    vendor.display.disable_skip_validate=1 \
    vendor.display.enable_default_color_mode=0 \
    vendor.display.perf_hint_window=50 \
    vendor.video.disable.ubwc=1

# Media
PRODUCT_PROPERTY_OVERRIDES += \
    media.stagefright.enable-player=true \
    media.stagefright.enable-http=true \
    media.stagefright.enable-aac=true \
    media.stagefright.enable-qcp=true \
    media.stagefright.enable-scan=true \
    mmp.enable.3g2=true \
    media.aac_51_output_enabled=true \
    mm.enable.smoothstreaming=true \
    persist.mm.enable.prefetch=true \
    vidc.enc.dcvs.extra-buff-count=2 \
    persist.media.treble_omx=true

# NFC
PRODUCT_PROPERTY_OVERRIDES += \
    ro.hardware.nfc_nci=nqx.default \
    ro.nfc.port=I2C \
    persist.nfc.smartcard.config=SIM1,eSE1

# Perf
PRODUCT_PROPERTY_OVERRIDES += \
    ro.sys.fw.bg_apps_limit=60 \
    ro.vendor.extension_library=libqti-perfd-client.so \
    ro.am.reschedule_service=true \
    sched.colocate.enable=1 \
    sys.games.gt.prof=1

# QSEE
PRODUCT_PROPERTY_OVERRIDES += \
    persist.sys.qsee=yes

# QCOM
PRODUCT_PROPERTY_OVERRIDES += \
    persist.vendor.qcomsysd.enabled=1

# QTI
PRODUCT_PROPERTY_OVERRIDES += \
    ro.vendor.at_library=libqti-at.so \
    ro.vendor.gt_library=libqti-gt.so

# RIL
PRODUCT_PROPERTY_OVERRIDES += \
    DEVICE_PROVISIONED=1 \
    rild.libpath=/vendor/lib64/libril-qc-qmi-1.so \
    vendor.rild.libpath=/vendor/lib64/libril-qc-qmi-1.so \
    persist.rild.nitz_plmn="" \
    persist.rild.nitz_long_ons_0="" \
    persist.rild.nitz_long_ons_1="" \
    persist.rild.nitz_long_ons_2="" \
    persist.rild.nitz_long_ons_3="" \
    persist.rild.nitz_short_ons_0="" \
    persist.rild.nitz_short_ons_1="" \
    persist.rild.nitz_short_ons_2="" \
    persist.rild.nitz_short_ons_3="" \
    ril.subscription.types=NV,RUIM \
    ro.telephony.default_network=22,20 \
    telephony.lteOnCdmaDevice=1 \
    keyguard.no_require_sim=true \
    ro.com.android.dataroaming=true \
    persist.sys.oem_smooth=1 \
    persist.radio.VT_CAM_INTERFACE=1 \
    persist.radio.apm_sim_not_pwdn=1 \
    persist.radio.multisim.config=dsds \
    persist.radio.enhance_ecall=true \
    persist.radio.hw_mbn_update=0 \
    persist.radio.sw_mbn_update=0 \
    persist.radio.start_ota_daemon=0 \
    persist.vendor.radio.custom_ecc=1 \
    persist.vendor.radio.data_con_rprt=1 \
    persist.vendor.radio.data_ltd_sys_ind=1 \
    persist.vendor.radio.ignore_dom_time=10 \
    persist.vendor.radio.rat_on=combine \
    persist.vendor.radio.sib16_support=1

# RmNet Data
PRODUCT_PROPERTY_OVERRIDES += \
    persist.rmnet.data.enable=true \
    persist.data.wda.enable=true \
    persist.data.df.dl_mode=5 \
    persist.data.df.ul_mode=5 \
    persist.data.df.agg.dl_pkt=10 \
    persist.data.df.agg.dl_size=4096 \
    persist.data.df.mux_count=8 \
    persist.data.df.iwlan_mux=9 \
    persist.data.df.dev_name=rmnet_usb0 \
    persist.data.iwlan.enable=true

# Sensors
PRODUCT_PROPERTY_OVERRIDES += \
    ro.vendor.sensors.dev_ori=true \
    ro.vendor.sensors.pmd=true \
    ro.vendor.sensors.sta_detect=true \
    ro.vendor.sensors.mot_detect=true \
    ro.vendor.sensors.dpc=true \
    ro.vendor.sensors.multishake=true \
    persist.vendor.sensors.direct_channel=true

# Timeservice
PRODUCT_PROPERTY_OVERRIDES += \
    persist.timed.enable=true

# Oiface
PRODUCT_PROPERTY_OVERRIDES += \
    persist.sys.oiface.enable=0 \
    persist.sys.hardcoder.name=oiface \
    persist.sys.oiface.feature=oiface:0xF,oifaceim:ffffffff

# USB
PRODUCT_PROPERTY_OVERRIDES += \
    sys.usb.controller=a800000.dwc3 \
    sys.usb.rndis.func.name=gsi \
    sys.usb.rmnet.func.name=gsi

# Wi-Fi
PRODUCT_PROPERTY_OVERRIDES += \
    wifi.interface=wlan0
