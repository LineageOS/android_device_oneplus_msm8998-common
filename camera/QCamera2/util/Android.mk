LOCAL_PATH:=$(call my-dir)
include $(CLEAR_VARS)
LOCAL_MODULE:= cameraconfig.txt
LOCAL_MODULE_CLASS := EXECUTABLES
LOCAL_SRC_FILES := cameraconfig.txt
LOCAL_MODULE_TAGS := optional
LOCAL_MODULE_PATH := $(TARGET_OUT_VENDOR)/etc/camera
LOCAL_MODULE_OWNER := qti
include $(BUILD_PREBUILT)