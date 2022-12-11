/* Copyright (c) 2013-2019, The Linux Foundation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are
* met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above
*       copyright notice, this list of conditions and the following
*       disclaimer in the documentation and/or other materials provided
*       with the distribution.
*     * Neither the name of The Linux Foundation nor the names of its
*       contributors may be used to endorse or promote products derived
*       from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED
* WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT
* ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS
* BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
* CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
* SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR
* BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
* WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE
* OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN
* IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*/
#ifndef __QCAMERA_HALHEADER_H__
#define __QCAMERA_HALHEADER_H__

// System dependencies
#include "hardware/gralloc.h"

// Camera dependencies
#include "cam_types.h"

extern "C" {
#include "mm_jpeg_interface.h"
}

namespace qcamera {

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define IS_USAGE_ZSL(usage)  (((usage) & (GRALLOC_USAGE_HW_CAMERA_ZSL)) \
        == (GRALLOC_USAGE_HW_CAMERA_ZSL))

#define IS_USAGE_SECURE(usage) (((usage) & (GRALLOC_USAGE_PROTECTED)) \
        == (GRALLOC_USAGE_PROTECTED))

#define IS_USAGE_UBWC(usage) (((usage) & (GRALLOC_USAGE_PRIVATE_ALLOC_UBWC)) \
        == (GRALLOC_USAGE_PRIVATE_ALLOC_UBWC))

class QCamera3Channel;
class QCamera3ProcessingChannel;

    typedef enum {
        INVALID,
        VALID,
    } stream_status_t;

    typedef enum {
       REPROCESS_TYPE_NONE,
       REPROCESS_TYPE_JPEG,
       REPROCESS_TYPE_YUV,
       REPROCESS_TYPE_PRIVATE,
       REPROCESS_TYPE_RAW
    } reprocess_type_t;

    typedef struct {
        uint32_t out_buf_index;
        int32_t jpeg_orientation;
        uint8_t jpeg_quality;
        uint8_t jpeg_thumb_quality;
        cam_dimension_t thumbnail_size;
        uint8_t gps_timestamp_valid;
        int64_t gps_timestamp;
        uint8_t gps_coordinates_valid;
        double gps_coordinates[3];
        char gps_processing_method[GPS_PROCESSING_METHOD_SIZE];
        uint8_t image_desc_valid;
        char image_desc[EXIF_IMAGE_DESCRIPTION_SIZE];
        bool hdr_snapshot;
        bool multiframe_snapshot;
        cam_hal3_JPEG_type_t image_type;
        bool is_dim_valid;
        cam_dimension_t output_dim;
        bool is_offset_valid;
        cam_frame_len_offset_t offset;
        bool is_format_valid;
        cam_format_t format;
        bool is_crop_valid;
        cam_rect_t crop;
        mm_jpeg_image_type_t encode_type;
        bool zsl_snapshot;
        uint32_t frame_number;
        bool raw_mfc_snapshot;
    } jpeg_settings_t;

    typedef struct {
        int32_t iso_speed;
        int64_t exposure_time;
    } metadata_response_t;

    typedef struct {
        cam_stream_type_t stream_type;
        cam_format_t stream_format;
        cam_format_t output_stream_format;
        cam_dimension_t input_stream_dim;
        cam_stream_buf_plane_info_t input_stream_plane_info;
        cam_dimension_t output_stream_dim;
        cam_padding_info_t *padding;
        reprocess_type_t reprocess_type;
        cam_hdr_param_t hdr_param;
        QCamera3Channel *src_channel;
    } reprocess_config_t;

};//namespace qcamera

#endif
