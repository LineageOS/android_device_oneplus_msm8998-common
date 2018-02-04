LOCAL_PATH := $(call my-dir)

sgdisk_src_files := \
    sgdisk.cc \
    gptcl.cc \
    crc32.cc \
    support.cc \
    guid.cc \
    gptpart.cc \
    mbrpart.cc \
    basicmbr.cc \
    mbr.cc \
    gpt.cc \
    bsd.cc \
    parttypes.cc \
    attributes.cc \
    diskio.cc \
    diskio-unix.cc \
    android_popt.cc \

include $(CLEAR_VARS)

LOCAL_CPP_EXTENSION := .cc

LOCAL_C_INCLUDES := $(LOCAL_PATH) external/e2fsprogs/lib
LOCAL_SRC_FILES := $(sgdisk_src_files)
LOCAL_CFLAGS += -Wno-unused-parameter -Werror

LOCAL_STATIC_LIBRARIES := libext2_uuid

LOCAL_MODULE := sgdisk-op5
LOCAL_MODULE_PATH := $(PRODUCT_OUT)/install/bin

include $(BUILD_EXECUTABLE)
