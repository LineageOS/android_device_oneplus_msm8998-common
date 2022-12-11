OLD_LOCAL_PATH := $(LOCAL_PATH)
LOCAL_PATH:=$(call my-dir)

# Build command line test app: mm-qcamera-app
include $(LOCAL_PATH)/../../../common.mk
include $(CLEAR_VARS)

LOCAL_HEADER_LIBRARIES := libutils_headers
LOCAL_HEADER_LIBRARIES += media_plugin_headers

LOCAL_CFLAGS:= \
        -DAMSS_VERSION=$(AMSS_VERSION) \
        $(mmcamera_debug_defines) \
        $(mmcamera_debug_cflags) \
        $(USE_SERVER_TREE)

ifeq ($(strip $(TARGET_USES_ION)),true)
LOCAL_CFLAGS += -DUSE_ION
endif

LOCAL_CFLAGS += -D_ANDROID_ -DQCAMERA_REDEFINE_LOG

# System header file path prefix
LOCAL_CFLAGS += -DSYSTEM_HEADER_PREFIX=sys
ifeq (1,$(filter 1,$(shell echo "$$(( $(PLATFORM_SDK_VERSION) >= 31 ))" )))
LOCAL_CFLAGS += -Wno-compound-token-split-by-macro
endif

LOCAL_SRC_FILES:= \
        src/mm_qcamera_main_menu.c \
        src/mm_qcamera_app.c \
        src/mm_qcamera_unit_test.c \
        src/mm_qcamera_video.c \
        src/mm_qcamera_preview.c \
        src/mm_qcamera_snapshot.c \
        src/mm_qcamera_rdi.c \
        src/mm_qcamera_reprocess.c\
        src/mm_qcamera_queue.c \
        src/mm_qcamera_socket.c \
        src/mm_qcamera_commands.c
#        src/mm_qcamera_dual_test.c \

LOCAL_C_INCLUDES:=$(LOCAL_PATH)/inc
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
LOCAL_C_INCLUDES+= \
        $(LOCAL_PATH)/../common \
        $(LOCAL_PATH)/../mm-camera-interface/inc \
        $(LOCAL_PATH)/../../../mm-image-codec/qexif \
        $(LOCAL_PATH)/../../../mm-image-codec/qomx_core

LOCAL_C_INCLUDES+= $(kernel_includes)
LOCAL_ADDITIONAL_DEPENDENCIES := $(common_deps)

ifneq ($(TARGET_KERNEL_VERSION),$(filter $(TARGET_KERNEL_VERSION),3.18 4.4 4.9))
LOCAL_CFLAGS += -DCAMERA_ION_HEAP_ID=ION_SYSTEM_HEAP_ID
else
LOCAL_CFLAGS += -DCAMERA_ION_HEAP_ID=ION_IOMMU_HEAP_ID
endif
ifeq ($(TARGET_BOARD_PLATFORM),msm8974)
        LOCAL_CFLAGS += -DCAMERA_ION_FALLBACK_HEAP_ID=ION_IOMMU_HEAP_ID
        LOCAL_CFLAGS += -DCAMERA_GRALLOC_CACHING_ID=0
        LOCAL_CFLAGS += -DNUM_RECORDING_BUFFERS=9
else ifeq ($(filter $(TARGET_BOARD_PLATFORM), apq8084 msm8084),$(TARGET_BOARD_PLATFORM))
        LOCAL_CFLAGS += -DCAMERA_ION_FALLBACK_HEAP_ID=ION_IOMMU_HEAP_ID
        LOCAL_CFLAGS += -DCAMERA_GRALLOC_CACHING_ID=0
        LOCAL_CFLAGS += -DNUM_RECORDING_BUFFERS=9
else ifeq ($(TARGET_BOARD_PLATFORM),msm8994)
        LOCAL_CFLAGS += -DCAMERA_ION_FALLBACK_HEAP_ID=ION_IOMMU_HEAP_ID
        LOCAL_CFLAGS += -DCAMERA_GRALLOC_CACHING_ID=0
        LOCAL_CFLAGS += -DNUM_RECORDING_BUFFERS=9
else ifeq ($(TARGET_BOARD_PLATFORM),msm8916 msm8952 msm8937 msm8953)
        LOCAL_CFLAGS += -DCAMERA_ION_FALLBACK_HEAP_ID=ION_IOMMU_HEAP_ID
        LOCAL_CFLAGS += -DCAMERA_GRALLOC_CACHING_ID=0
        LOCAL_CFLAGS += -DNUM_RECORDING_BUFFERS=9
else ifeq ($(TARGET_BOARD_PLATFORM),msm8226)
        LOCAL_CFLAGS += -DCAMERA_ION_FALLBACK_HEAP_ID=ION_IOMMU_HEAP_ID
        LOCAL_CFLAGS += -DCAMERA_GRALLOC_CACHING_ID=0
        LOCAL_CFLAGS += -DNUM_RECORDING_BUFFERS=9
else ifeq ($(TARGET_BOARD_PLATFORM),msm8610)
        LOCAL_CFLAGS += -DCAMERA_ION_FALLBACK_HEAP_ID=ION_IOMMU_HEAP_ID
        LOCAL_CFLAGS += -DCAMERA_GRALLOC_CACHING_ID=0
        LOCAL_CFLAGS += -DNUM_RECORDING_BUFFERS=9
else ifeq ($(TARGET_BOARD_PLATFORM),msm8960)
        LOCAL_CFLAGS += -DCAMERA_ION_FALLBACK_HEAP_ID=ION_IOMMU_HEAP_ID
        LOCAL_CFLAGS += -DCAMERA_GRALLOC_CACHING_ID=0
        LOCAL_CFLAGS += -DNUM_RECORDING_BUFFERS=5
else ifneq (,$(filter msm8660,$(TARGET_BOARD_PLATFORM)))
        LOCAL_CFLAGS += -DCAMERA_ION_FALLBACK_HEAP_ID=ION_IOMMU_HEAP_ID # EBI
        LOCAL_CFLAGS += -DCAMERA_GRALLOC_CACHING_ID=0
        LOCAL_CFLAGS += -DNUM_RECORDING_BUFFERS=5
else
        LOCAL_CFLAGS += -DCAMERA_GRALLOC_CACHING_ID=GRALLOC_USAGE_PRIVATE_UNCACHED #uncached
        LOCAL_CFLAGS += -DCAMERA_ION_FALLBACK_HEAP_ID=ION_CAMERA_HEAP_ID
        LOCAL_CFLAGS += -DNUM_RECORDING_BUFFERS=5
endif
LOCAL_CFLAGS += -Wall -Wextra -Werror

LOCAL_SHARED_LIBRARIES:= \
         libcutils libdl liblog libmmcamera_interface
ifneq ($(TARGET_KERNEL_VERSION),$(filter $(TARGET_KERNEL_VERSION),3.18 4.4 4.9))
LOCAL_SHARED_LIBRARIES += libion
endif
LOCAL_MODULE_TAGS := optional

LOCAL_32_BIT_ONLY := $(BOARD_QTI_CAMERA_32BIT_ONLY)

LOCAL_MODULE:= mm-qcamera-app
LOCAL_VENDOR_MODULE := true
include $(SDCLANG_COMMON_DEFS)

include $(BUILD_EXECUTABLE)

# Build tuning library
include $(CLEAR_VARS)

LOCAL_HEADER_LIBRARIES := libutils_headers
LOCAL_HEADER_LIBRARIES += media_plugin_headers

LOCAL_CFLAGS:= \
        -DAMSS_VERSION=$(AMSS_VERSION) \
        $(mmcamera_debug_defines) \
        $(mmcamera_debug_cflags) \
        $(USE_SERVER_TREE)

ifeq ($(strip $(TARGET_USES_ION)),true)
LOCAL_CFLAGS += -DUSE_ION
endif

LOCAL_CFLAGS += -D_ANDROID_ -DQCAMERA_REDEFINE_LOG

# System header file path prefix
LOCAL_CFLAGS += -DSYSTEM_HEADER_PREFIX=sys
ifeq (1,$(filter 1,$(shell echo "$$(( $(PLATFORM_SDK_VERSION) >= 31 ))" )))
LOCAL_CFLAGS += -Wno-compound-token-split-by-macro
endif

LOCAL_SRC_FILES:= \
        src/mm_qcamera_main_menu.c \
        src/mm_qcamera_app.c \
        src/mm_qcamera_unit_test.c \
        src/mm_qcamera_video.c \
        src/mm_qcamera_preview.c \
        src/mm_qcamera_snapshot.c \
        src/mm_qcamera_rdi.c \
        src/mm_qcamera_reprocess.c\
        src/mm_qcamera_queue.c \
        src/mm_qcamera_socket.c \
        src/mm_qcamera_commands.c
#        src/mm_qcamera_dual_test.c \

LOCAL_C_INCLUDES:=$(LOCAL_PATH)/inc
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

LOCAL_C_INCLUDES+= \
        $(LOCAL_PATH)/../common \
        $(LOCAL_PATH)/../mm-camera-interface/inc \
        $(LOCAL_PATH)/../../../mm-image-codec/qexif \
        $(LOCAL_PATH)/../../../mm-image-codec/qomx_core

LOCAL_C_INCLUDES+= $(kernel_includes)
LOCAL_ADDITIONAL_DEPENDENCIES := $(common_deps)

ifneq ($(TARGET_KERNEL_VERSION),$(filter $(TARGET_KERNEL_VERSION),3.18 4.4 4.9))
LOCAL_CFLAGS += -DCAMERA_ION_HEAP_ID=ION_SYSTEM_HEAP_ID
else
LOCAL_CFLAGS += -DCAMERA_ION_HEAP_ID=ION_IOMMU_HEAP_ID
endif
ifeq ($(TARGET_BOARD_PLATFORM),msm8974)
        LOCAL_CFLAGS += -DCAMERA_ION_FALLBACK_HEAP_ID=ION_IOMMU_HEAP_ID
        LOCAL_CFLAGS += -DCAMERA_GRALLOC_CACHING_ID=0
        LOCAL_CFLAGS += -DNUM_RECORDING_BUFFERS=9
else ifeq ($(filter $(TARGET_BOARD_PLATFORM), apq8084 msm8084),$(TARGET_BOARD_PLATFORM))
        LOCAL_CFLAGS += -DCAMERA_ION_FALLBACK_HEAP_ID=ION_IOMMU_HEAP_ID
        LOCAL_CFLAGS += -DCAMERA_GRALLOC_CACHING_ID=0
        LOCAL_CFLAGS += -DNUM_RECORDING_BUFFERS=9
else ifeq ($(TARGET_BOARD_PLATFORM),msm8994)
        LOCAL_CFLAGS += -DCAMERA_ION_FALLBACK_HEAP_ID=ION_IOMMU_HEAP_ID
        LOCAL_CFLAGS += -DCAMERA_GRALLOC_CACHING_ID=0
        LOCAL_CFLAGS += -DNUM_RECORDING_BUFFERS=9
else ifeq ($(TARGET_BOARD_PLATFORM),msm8916 msm8952 msm8937 msm8953)
        LOCAL_CFLAGS += -DCAMERA_ION_FALLBACK_HEAP_ID=ION_IOMMU_HEAP_ID
        LOCAL_CFLAGS += -DCAMERA_GRALLOC_CACHING_ID=0
        LOCAL_CFLAGS += -DNUM_RECORDING_BUFFERS=9
else ifeq ($(TARGET_BOARD_PLATFORM),msm8226)
        LOCAL_CFLAGS += -DCAMERA_ION_FALLBACK_HEAP_ID=ION_IOMMU_HEAP_ID
        LOCAL_CFLAGS += -DCAMERA_GRALLOC_CACHING_ID=0
        LOCAL_CFLAGS += -DNUM_RECORDING_BUFFERS=9
else ifeq ($(TARGET_BOARD_PLATFORM),msm8610)
        LOCAL_CFLAGS += -DCAMERA_ION_FALLBACK_HEAP_ID=ION_IOMMU_HEAP_ID
        LOCAL_CFLAGS += -DCAMERA_GRALLOC_CACHING_ID=0
        LOCAL_CFLAGS += -DNUM_RECORDING_BUFFERS=9
else ifeq ($(TARGET_BOARD_PLATFORM),msm8960)
        LOCAL_CFLAGS += -DCAMERA_ION_FALLBACK_HEAP_ID=ION_IOMMU_HEAP_ID
        LOCAL_CFLAGS += -DCAMERA_GRALLOC_CACHING_ID=0
        LOCAL_CFLAGS += -DNUM_RECORDING_BUFFERS=5
else ifneq (,$(filter msm8660,$(TARGET_BOARD_PLATFORM)))
        LOCAL_CFLAGS += -DCAMERA_ION_FALLBACK_HEAP_ID=ION_IOMMU_HEAP_ID # EBI
        LOCAL_CFLAGS += -DCAMERA_GRALLOC_CACHING_ID=0
        LOCAL_CFLAGS += -DNUM_RECORDING_BUFFERS=5
else
        LOCAL_CFLAGS += -DCAMERA_GRALLOC_CACHING_ID=GRALLOC_USAGE_PRIVATE_UNCACHED #uncached
        LOCAL_CFLAGS += -DCAMERA_ION_FALLBACK_HEAP_ID=ION_CAMERA_HEAP_ID
        LOCAL_CFLAGS += -DNUM_RECORDING_BUFFERS=5
endif
LOCAL_CFLAGS += -Wall -Wextra -Werror

LOCAL_SHARED_LIBRARIES:= \
         libcutils libdl liblog libmmcamera_interface
ifneq ($(TARGET_KERNEL_VERSION),$(filter $(TARGET_KERNEL_VERSION),3.18 4.4 4.9))
LOCAL_SHARED_LIBRARIES += libion
endif
LOCAL_MODULE_TAGS := optional

LOCAL_32_BIT_ONLY := $(BOARD_QTI_CAMERA_32BIT_ONLY)

LOCAL_MODULE:= libmm-qcamera
LOCAL_VENDOR_MODULE := true
include $(SDCLANG_COMMON_DEFS)

include $(BUILD_SHARED_LIBRARY)

LOCAL_PATH := $(OLD_LOCAL_PATH)
