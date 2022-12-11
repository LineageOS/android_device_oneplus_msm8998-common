LOCAL_PATH:=$(call my-dir)

# Build command line test app: mm-hal3-app
include $(CLEAR_VARS)

ifeq ($(TARGET_SUPPORT_HAL1),false)
LOCAL_CFLAGS += -DQCAMERA_HAL3_SUPPORT
endif

ifeq ($(TARGET_BOARD_PLATFORM),msm8953)
    LOCAL_CFLAGS += -DCAMERA_CHIPSET_8953
else
    LOCAL_CFLAGS += -DCAMERA_CHIPSET_8937
endif

ifneq ($(TARGET_KERNEL_VERSION),$(filter $(TARGET_KERNEL_VERSION),3.18 4.4 4.9))
  ifneq ($(LIBION_HEADER_PATH_WRAPPER), )
    include $(LIBION_HEADER_PATH_WRAPPER)
    LOCAL_C_INCLUDES += $(LIBION_HEADER_PATHS)
  else
   LOCAL_C_INCLUDES += \
           system/core/libion/kernel-headers \
           system/core/libion/include
  endif
endif

LOCAL_ADDITIONAL_DEPENDENCIES += $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr
LOCAL_C_INCLUDES += $(TARGET_OUT_INTERMEDIATES)/KERNEL_OBJ/usr/include
LOCAL_C_INCLUDES+= $(kernel_includes)

LOCAL_C_INCLUDES += \
    $(LOCAL_PATH)/../ \
    $(LOCAL_PATH)/../../stack/mm-camera-interface/inc \
    hardware/qcom/media/libstagefrighthw \
    hardware/qcom/media/mm-core/inc

LOCAL_HEADER_LIBRARIES := libhardware_headers
LOCAL_HEADER_LIBRARIES += libbinder_headers
LOCAL_HEADER_LIBRARIES += libandroid_sensor_headers

LOCAL_SRC_FILES := \
    QCameraHAL3Base.cpp \
    QCameraHAL3MainTestContext.cpp \
    QCameraHAL3VideoTest.cpp \
    QCameraHAL3PreviewTest.cpp \
    QCameraHAL3SnapshotTest.cpp \
    QCameraHAL3RawSnapshotTest.cpp \
    QCameraHAL3Test.cpp


LOCAL_SHARED_LIBRARIES:= libutils liblog libcamera_metadata libcutils
ifneq ($(TARGET_KERNEL_VERSION),$(filter $(TARGET_KERNEL_VERSION),3.18 4.4 4.9))
LOCAL_SHARED_LIBRARIES += libion
endif

LOCAL_STATIC_LIBRARIES := android.hardware.camera.common@1.0-helper

LOCAL_32_BIT_ONLY := $(BOARD_QTI_CAMERA_32BIT_ONLY)

LOCAL_MODULE:= hal3-test-app
LOCAL_VENDOR_MODULE := true
include $(SDCLANG_COMMON_DEFS)

LOCAL_CFLAGS += -Wall -Wextra -Werror

LOCAL_CFLAGS += -std=c++14 -std=gnu++1z

include $(BUILD_EXECUTABLE)
