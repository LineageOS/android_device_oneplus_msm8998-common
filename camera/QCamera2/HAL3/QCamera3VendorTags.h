/* Copyright (c) 2014-2016,2019 The Linux Foundation. All rights reserved.
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

#ifndef __QCAMERA3VENDORTAGS_H__
#define __QCAMERA3VENDORTAGS_H__

// Camera dependencies
#include "system/camera_metadata.h"

namespace qcamera {

enum qcamera3_ext_section {
    QCAMERA3_PRIVATEDATA = VENDOR_SECTION,
    QCAMERA3_CDS,
    QCAMERA3_OPAQUE_RAW,
    QCAMERA3_CROP,
    QCAMERA3_TUNING_META_DATA,
    QCAMERA3_TEMPORAL_DENOISE,
    QCAMERA3_ISO_EXP_PRIORITY,
    QCAMERA3_SATURATION,
    QCAMERA3_EXPOSURE_METERING_MODE,
    QCAMERA3_AV_TIMER,
    QCAMERA3_SENSOR_META_DATA,
    QCAMERA3_DUALCAM_LINK_META_DATA,
    QCAMERA3_DUALCAM_CALIB_META_DATA,
    QCAMERA3_HAL_PRIVATEDATA,
    QCAMERA3_JPEG_ENCODE_CROP,
    QCAMERA3_VIDEO_HDR,
    QCAMERA3_IR,
    QCAMERA3_AEC_CONVERGENCE,
    QCAMERA3_AWB_CONVERGENCE,
    QCAMERA3_INSTANT_AEC,
    QCAMERA3_SHARPNESS_DATA,
    QCAMERA3_HISTOGRAM,
    QCAMERA3_BINNING_CORRECTION,
    QCAMERA3_STATS,
    QCAMERA3_SIMULTANEOUS_CAMERA,
    QCAMERA3_QUADRA_CFA_DATA,
    QCAMERA3_HFR,
    QCAMERA3_BOKEH,
    QCAMERA3_FUSION,
    QCAMERA3_LOGICAL_CAM,
    QCAMERA3_MANUAL_WB,
    QCAMERA3_SAT_MODE,
    QCAMERA3_EIS_FLUSH,
    QCAMERA3_SWMFNR,
    QCAMERA3_SECTIONS_END,
};

enum qcamera3_ext_section_ranges {
    QCAMERA3_PRIVATEDATA_START = QCAMERA3_PRIVATEDATA << 16,
    QCAMERA3_CDS_START = QCAMERA3_CDS << 16,
    QCAMERA3_OPAQUE_RAW_START = QCAMERA3_OPAQUE_RAW << 16,
    QCAMERA3_CROP_START = QCAMERA3_CROP << 16,
    QCAMERA3_TUNING_META_DATA_START = QCAMERA3_TUNING_META_DATA << 16,
    QCAMERA3_TEMPORAL_DENOISE_START = QCAMERA3_TEMPORAL_DENOISE << 16,
    QCAMERA3_ISO_EXP_PRIORITY_START = QCAMERA3_ISO_EXP_PRIORITY << 16,
    QCAMERA3_SATURATION_START = QCAMERA3_SATURATION << 16,
    QCAMERA3_EXPOSURE_METER_START = QCAMERA3_EXPOSURE_METERING_MODE << 16,
    QCAMERA3_AV_TIMER_START = QCAMERA3_AV_TIMER << 16,
    QCAMERA3_SENSOR_META_DATA_START = QCAMERA3_SENSOR_META_DATA << 16,
    QCAMERA3_DUALCAM_LINK_META_DATA_START = QCAMERA3_DUALCAM_LINK_META_DATA << 16,
    QCAMERA3_DUALCAM_CALIB_META_DATA_START = QCAMERA3_DUALCAM_CALIB_META_DATA << 16,
    QCAMERA3_HAL_PRIVATEDATA_START = QCAMERA3_HAL_PRIVATEDATA << 16,
    QCAMERA3_JPEG_ENCODE_CROP_START = QCAMERA3_JPEG_ENCODE_CROP << 16,
    QCAMERA3_VIDEO_HDR_START = QCAMERA3_VIDEO_HDR << 16,
    QCAMERA3_IR_START = QCAMERA3_IR << 16,
    QCAMERA3_AEC_CONVERGENCE_SPEED_START = QCAMERA3_AEC_CONVERGENCE << 16,
    QCAMERA3_AWB_CONVERGENCE_SPEED_START = QCAMERA3_AWB_CONVERGENCE << 16,
    QCAMERA3_INSTANT_AEC_START = QCAMERA3_INSTANT_AEC << 16,
    QCAMERA3_SHARPNESS_START = QCAMERA3_SHARPNESS_DATA << 16,
    QCAMERA3_HISTOGRAM_START = QCAMERA3_HISTOGRAM << 16,
    QCAMERA3_BINNING_CORRECTION_START = QCAMERA3_BINNING_CORRECTION << 16,
    QCAMERA3_STATS_START = QCAMERA3_STATS << 16,
    QCAMERA3_SIMULTANEOUS_CAMERA_START = QCAMERA3_SIMULTANEOUS_CAMERA << 16,
    QCAMERA3_QUADRA_CFA_DATA_START = QCAMERA3_QUADRA_CFA_DATA << 16,
    QCAMERA3_HFR_START = QCAMERA3_HFR << 16,
    QCAMERA3_BOKEH_START = QCAMERA3_BOKEH << 16,
    QCAMERA3_FUSION_START = QCAMERA3_FUSION << 16,
    QCAMERA3_LOGICAL_CAM_START = QCAMERA3_LOGICAL_CAM << 16,
    QCAMERA3_MANUAL_WB_START = QCAMERA3_MANUAL_WB << 16,
    QCAMERA3_SAT_MODE_START = QCAMERA3_SAT_MODE << 16,
    QCAMERA3_EIS_FLUSH_START = QCAMERA3_EIS_FLUSH << 16,
    QCAMERA3_SWMFNR_START = QCAMERA3_SWMFNR << 16,
};

enum qcamera3_ext_tags {
    QCAMERA3_PRIVATEDATA_REPROCESS = QCAMERA3_PRIVATEDATA_START,
    QCAMERA3_PRIVATEDATA_END,
    QCAMERA3_CDS_MODE = QCAMERA3_CDS_START,
    QCAMERA3_CDS_INFO,
    QCAMERA3_CDS_END,

    //Property Name:  org.codeaurora.qcamera3.opaque_raw.opaque_raw_strides
    //
    //Type: int32 * n * 3 [public]
    //
    //Description: Distance in bytes from the beginning of one row of opaque
    //raw image data to the beginning of next row.
    //
    //Details: The strides are listed as (raw_width, raw_height, stride)
    //triplets. For each supported raw size, there will be a stride associated
    //with it.
    QCAMERA3_OPAQUE_RAW_STRIDES = QCAMERA3_OPAQUE_RAW_START,

    //Property Name: org.codeaurora.qcamera3.opaque_raw.opaque_raw_format
    //
    //Type: byte(enum) [public]
    //  * LEGACY - The legacy raw format where 8, 10, or 12-bit
    //    raw data is packed into a 64-bit word.
    //  * MIPI - raw format matching the data packing described
    //    in MIPI CSI-2 specification. In memory, the data
    //    is constructed by packing sequentially received pixels
    //    into least significant parts of the words first.
    //    Within each pixel, the least significant bits are also
    //    placed towards the least significant part of the word.
    //
    //Details: Lay out of opaque raw data in memory is decided by two factors:
    //         opaque_raw_format and bit depth (implied by whiteLevel). Below
    //         list illustrates their relationship:
    //  LEGACY8:  P7(7:0) P6(7:0) P5(7:0) P4(7:0) P3(7:0) P2(7:0) P1(7:0) P0(7:0)
    //            8 pixels occupy 8 bytes, no padding needed
    //            min_stride = CEILING8(raw_width)
    // LEGACY10:  0000 P5(9:0) P4(9:0) P3(9:0) P2(9:0) P1(9:0) P0(9:0)
    //            6 pixels occupy 8 bytes, 4 bits padding at MSB
    //            min_stride = (raw_width+5)/6 * 8
    // LEGACY12:  0000 P4(11:0) P3(11:0) P2(11:0) P1(11:0) P0(11:0)
    //            5 pixels occupy 8 bytes, 4 bits padding at MSB
    //            min_stride = (raw_width+4)/5 * 8
    //    MIPI8:  P0(7:0)
    //            1 pixel occupy 1 byte
    //            min_stride = raw_width
    //   MIPI10:  P3(1:0) P2(1:0) P1(1:0) P0(1:0) P3(9:2) P2(9:2) P1(9:2) P0(9:2)
    //            4 pixels occupy 5 bytes
    //            min_stride = (raw_width+3)/4 * 5
    //   MIPI12:  P1(3:0) P0(3:0) P1(11:4) P0(11:4)
    //            2 pixels occupy 3 bytes
    //            min_stride = (raw_width+1)/2 * 3
    //Note that opaque_raw_stride needs to be at least the required minimum
    //stride from the table above. ISP hardware may need more generous stride
    //setting. For example, for LEGACY8, the actual stride may be
    //CEILING16(raw_width) due to bus burst length requirement.
    QCAMERA3_OPAQUE_RAW_FORMAT,
    QCAMERA3_OPAQUE_RAW_END,

    QCAMERA3_CROP_COUNT_REPROCESS = QCAMERA3_CROP_START,
    QCAMERA3_CROP_REPROCESS,
    QCAMERA3_CROP_ROI_MAP_REPROCESS,
    QCAMERA3_CROP_END,

    QCAMERA3_TUNING_META_DATA_BLOB = QCAMERA3_TUNING_META_DATA_START,
    QCAMERA3_TUNING_META_DATA_END,

    QCAMERA3_TEMPORAL_DENOISE_ENABLE = QCAMERA3_TEMPORAL_DENOISE_START,
    QCAMERA3_TEMPORAL_DENOISE_PROCESS_TYPE,
    QCAMERA3_TEMPORAL_DENOISE_END,

    QCAMERA3_USE_ISO_EXP_PRIORITY = QCAMERA3_ISO_EXP_PRIORITY_START,
    QCAMERA3_SELECT_PRIORITY,
    /* Property Name:  org.codeaurora.qcamera3.iso_exp_priority.iso_available_modes
       Type: int32[8]
       Description: Available iso mode values
       iso_available_modes[0] = 0 (auto)
       iso_available_modes[1] = 1 (deblur)
       iso_available_modes[2] = 2 (100)
       iso_available_modes[3] = 3 (200)
       iso_available_modes[4] = 4 (400)
       iso_available_modes[5] = 5 (800)
       iso_available_modes[6] = 6 (1600)
       iso_available_modes[7] = 7 (3200)
    */
    QCAMERA3_ISO_AVAILABLE_MODES,
    /* Property Name:  org.codeaurora.qcamera3.iso_exp_priority.exposure_time_range
       Type: int64[2]
       Description: Min/max exposure time values (ns)
       exposure_time_range[0] = min
       exposure_time_range[1] = max
    */
    QCAMERA3_EXP_TIME_RANGE,
    QCAMERA3_ISO_EXP_PRIORITY_END,

    QCAMERA3_USE_SATURATION = QCAMERA3_SATURATION_START,
    /* Property Name:  org.codeaurora.qcamera3.saturation.range
       Type: int32[4]
       Description: Range of possible saturation values
       range[0] = min
       range[1] = max
       range[2] = default
       range[3] = step
    */
    QCAMERA3_SATURATION_RANGE,
    QCAMERA3_SATURATION_END,

    QCAMERA3_EXPOSURE_METER = QCAMERA3_EXPOSURE_METER_START,
    /* Property Name:  org.codeaurora.qcamera3.exposure_metering.available_modes
       Type: int32[7]
       Description: Available exposure metering mode values
       available_modes[0] = 0 (FRAME_AVERAGE)
       available_modes[1] = 1 (CENTER_WEIGHTED)
       available_modes[2] = 2 (SPOT_METERING)
       available_modes[3] = 3 (SMART_METERING)
       available_modes[4] = 4 (USER_METERING)
       available_modes[5] = 5 (SPOT_METERING_ADV)
       available_modes[6] = 6 (CENTER_WEIGHTED_ADV)
    */
    QCAMERA3_EXPOSURE_METER_AVAILABLE_MODES,
    QCAMERA3_EXPOSURE_METER_END,

    QCAMERA3_USE_AV_TIMER = QCAMERA3_AV_TIMER_START,
    QCAMERA3_AV_TIMER_END,

    QCAMERA3_SENSOR_DYNAMIC_BLACK_LEVEL_PATTERN = QCAMERA3_SENSOR_META_DATA_START,
    QCAMERA3_SENSOR_IS_MONO_ONLY,
    QCAMERA3_SENSOR_META_DATA_END,

    QCAMERA3_DUALCAM_LINK_ENABLE = QCAMERA3_DUALCAM_LINK_META_DATA_START,
    QCAMERA3_DUALCAM_LINK_IS_MAIN,
    QCAMERA3_DUALCAM_LINK_RELATED_CAMERA_ID,
    QCAMERA3_DUALCAM_LINK_META_DATA_END,

    QCAMERA3_DUALCAM_CALIB_META_DATA_BLOB = QCAMERA3_DUALCAM_CALIB_META_DATA_START,
    QCAMERA3_DUALCAM_CALIB_META_DATA_END,

    QCAMERA3_HAL_PRIVATEDATA_REPROCESS_FLAGS = QCAMERA3_HAL_PRIVATEDATA_START,
    QCAMERA3_HAL_PRIVATEDATA_REPROCESS_DATA_BLOB,
    QCAMERA3_HAL_PRIVATEDATA_EXIF_DEBUG_DATA_BLOB,

    QCAMERA3_HAL_PRIVATEDATA_END,

    /* Property Name:  org.codeaurora.qcamera3.jpeg_encode_crop.enable
       Type: byte
       Description: If JPEG crop is enable
    */
    QCAMERA3_JPEG_ENCODE_CROP_ENABLE = QCAMERA3_JPEG_ENCODE_CROP_START,
    /* Property Name:  org.codeaurora.qcamera3.jpeg_encode_crop.rect
       Type: int32[4]
       Description: Crop image into size width x height
                    from [left, top] coordinate
       rect[0] = left
       rect[1] = top
       rect[2] = width
       rect[3] = height
    */
    QCAMERA3_JPEG_ENCODE_CROP_RECT,
    /* Property Name:  org.codeaurora.qcamera3.jpeg_encode_crop.roi
       Type: int32[4]
       Description: Scale the crop image into size width x height
                    from [left, top] coordinate.
       roi[0] = left
       roi[1] = top
       roi[2] = width
       roi[3] = height
    */
    QCAMERA3_JPEG_ENCODE_CROP_ROI,
    QCAMERA3_JPEG_ENCODE_CROP_END,

    QCAMERA3_VIDEO_HDR_MODE = QCAMERA3_VIDEO_HDR_START,
    QCAMERA3_AVAILABLE_VIDEO_HDR_MODES,
    QCAMERA3_VIDEO_HDR_END,

    QCAMERA3_IR_MODE = QCAMERA3_IR_START,
    QCAMERA3_IR_AVAILABLE_MODES,
    QCAMERA3_IR_END,

    QCAMERA3_AEC_CONVERGENCE_SPEED = QCAMERA3_AEC_CONVERGENCE_SPEED_START,
    QCAMERA3_AEC_CONVERGENCE_SPEED_END,

    QCAMERA3_AWB_CONVERGENCE_SPEED = QCAMERA3_AWB_CONVERGENCE_SPEED_START,
    QCAMERA3_AWB_CONVERGENCE_SPEED_END,

    QCAMERA3_INSTANT_AEC_MODE = QCAMERA3_INSTANT_AEC_START,
    QCAMERA3_INSTANT_AEC_AVAILABLE_MODES,
    QCAMERA3_INSTANT_AEC_END,

    QCAMERA3_SHARPNESS_STRENGTH = QCAMERA3_SHARPNESS_START,
    QCAMERA3_SHARPNESS_RANGE,
    QCAMERA3_SHARPNESS_END,

    QCAMERA3_HISTOGRAM_MODE = QCAMERA3_HISTOGRAM_START,
    QCAMERA3_HISTOGRAM_BUCKETS,
    QCAMERA3_HISTOGRAM_MAX_COUNT,
    QCAMERA3_HISTOGRAM_STATS,
    QCAMERA3_HISTOGRAM_END,

    QCAMERA3_BINNING_CORRECTION_MODE = QCAMERA3_BINNING_CORRECTION_START,
    QCAMERA3_AVAILABLE_BINNING_CORRECTION_MODES,
    QCAMERA3_BINNING_CORRECTION_END,

    QCAMERA3_STATS_IS_HDR_SCENE = QCAMERA3_STATS_START,
    /* Property Name:  org.codeaurora.qcamera3.stats.is_hdr_scene_values
       Type: byte[2]
       Description: Possible values for is_hdr_scene
       roi[0] = 0 (false)
       roi[1] = 1 (true)
    */
    QCAMERA3_STATS_IS_HDR_SCENE_VALUES,
    QCAMERA3_STATS_IS_HDR_SCENE_CONFIDENCE,
    /* Property Name:  org.codeaurora.qcamera3.stats.is_hdr_scene_confidence_range
       Type: float[2]
       Description: Range of values for is_hdr_scene_confidence
       roi[0] = 0.0 (min)
       roi[1] = 1.0 (max)
    */
    QCAMERA3_STATS_IS_HDR_SCENE_CONFIDENCE_RANGE,
    QCAMERA3_STATS_BSGC_AVAILABLE,
    QCAMERA3_STATS_BLINK_DETECTED,
    /* Property Name:  org.codeaurora.qcamera3.stats.blink_degree
       Type: int8[2]
       Description: Blink degree
       blink_degree[0] = left
       blink_degree[1] = right
    */
    QCAMERA3_STATS_BLINK_DEGREE,
    QCAMERA3_STATS_SMILE_DEGREE,
    QCAMERA3_STATS_SMILE_CONFIDENCE,
    QCAMERA3_STATS_GAZE_ANGLE,
    /* Property Name:  org.codeaurora.qcamera3.stats.gaze_direction
       Type: int32[3]
       Description: Gaze direction
       gaze_direction[0] = up-down
       gaze_direction[1] = left-right
       gaze_direction[2] = roll
    */
    QCAMERA3_STATS_GAZE_DIRECTION,
    /* Property Name:  org.codeaurora.qcamera3.stats.gaze_degree
       Type: int8[2]
       Description: Gaze degree
       gaze_degree[0] = left-right
       gaze_degree[1] = top-bottom
    */
    QCAMERA3_STATS_GAZE_DEGREE,
    QCAMERA3_STATS_END,

    QCAMERA3_SIMULTANEOUS_CAMERA_VFE1_RESERVED_RDI = QCAMERA3_SIMULTANEOUS_CAMERA_START,
    /* Property Name:  org.codeaurora.qcamera3.simultaneous_camera.vfe1_reserved_rdi
       Type: int32
       Description: number of RDI interface reserved on VFE1(the smaller VFE)
       Details: It will guarantee the available RDI interfaces for RDI streams,
                and any RDI stream requests that exceed the reserved number will
                be allocated  to the other VFE.
                Range from -1 to 3, -1 means the feature is disabled.
    */
    QCAMERA3_SIMULTANEOUS_CAMERA_END,

    QCAMERA3_IS_QUADRA_CFA_SENSOR = QCAMERA3_QUADRA_CFA_DATA_START,
    QCAMERA3_SUPPORT_QUADRA_CFA_DIM,
    QCAMERA3_QUADRA_CFA_DATA_END,

    QCAMERA3_HFR_SIZES = QCAMERA3_HFR_START,
    QCAMERA3_HFR_END,

    QCAMERA3_BOKEH_ENABLE = QCAMERA3_BOKEH_START,
    QCAMERA3_BOKEH_BLURLEVEL,
    QCAMERA3_BOKEH_STATUS,
    QCAMERA3_BOKEH_END,

    QCAMERA3_FUSION_STATUS = QCAMERA3_FUSION_START,
    QCAMERA3_FUSION_END,

    QCAMERA3_LOGICAL_CAM_MODE = QCAMERA3_LOGICAL_CAM_START,
    QCAMERA3_LOGICAL_CAM_END,

    QCAMERA3_MANUAL_WB_MODE = QCAMERA3_MANUAL_WB_START,
    QCAMERA3_MANUAL_WB_CCT_RANGE,
    QCAMERA3_MANUAL_WB_GAINS_RANGE,
    QCAMERA3_MANUAL_WB_CCT,
    QCAMERA3_MANUAL_WB_GAINS,
    QCAMERA3_MANUAL_WB_END,

    QCAMERA3_SAT_MODE_ON = QCAMERA3_SAT_MODE_START,
    QCAMERA3_SAT_MODE_END,

    QCAMERA3_EIS_FLUSH_ON = QCAMERA3_EIS_FLUSH_START,
    QCAMERA3_EIS_FLUSH_END,

    QCAMERA3_SWMFNR_ENABLE = QCAMERA3_SWMFNR_START,
    QCAMERA3_SWMFNR_END
};

// QCAMERA3_OPAQUE_RAW_FORMAT
typedef enum qcamera3_ext_opaque_raw_format {
    QCAMERA3_OPAQUE_RAW_FORMAT_LEGACY,
    QCAMERA3_OPAQUE_RAW_FORMAT_MIPI
} qcamera3_ext_opaque_raw_format_t;


// QCAMERA3_VIDEO_HDR_MODE
typedef enum camera_metadata_enum_android_video_hdr_mode {
    QCAMERA3_VIDEO_HDR_MODE_OFF,
    QCAMERA3_VIDEO_HDR_MODE_ON
} camera_metadata_enum_android_video_hdr_mode_t;

// QCAMERA3_IR_MODE
typedef enum camera_metadata_enum_android_ir_mode {
    QCAMERA3_IR_MODE_OFF,
    QCAMERA3_IR_MODE_ON,
    QCAMERA3_IR_MODE_AUTO
} camera_metadata_enum_android_ir_mode_t;

// QCAMERA3_INSTANCE_AEC_MODE
typedef enum qcamera3_ext_instant_aec_mode {
    QCAMERA3_INSTANT_AEC_NORMAL_CONVERGENCE,
    QCAMERA3_INSTANT_AEC_AGGRESSIVE_CONVERGENCE,
    QCAMERA3_INSTANT_AEC_FAST_CONVERGENCE
} qcamera3_ext_instant_aec_mode_t;

// QCAMERA3_HISTOGRAM_MODE
typedef enum qcamera3_histogram_mode {
    QCAMERA3_HISTOGRAM_MODE_OFF,
    QCAMERA3_HISTOGRAM_MODE_ON
} histogram_mode;

// QCAMERA3_BINNING_CORRECTION_MODE
typedef enum camera_metadata_enum_android_binning_correction_mode {
    QCAMERA3_BINNING_CORRECTION_MODE_OFF,
    QCAMERA3_BINNING_CORRECTION_MODE_ON
} camera_metadata_enum_android_binning_correction_mode_t;

// QCAMERA3_EXPOSURE_METER
typedef enum qcamera3_ext_exposure_meter_mode {
    QCAMERA3_EXP_METER_MODE_FRAME_AVERAGE,
    QCAMERA3_EXP_METER_MODE_CENTER_WEIGHTED,
    QCAMERA3_EXP_METER_MODE_SPOT_METERING,
    QCAMERA3_EXP_METER_MODE_SMART_METERING,
    QCAMERA3_EXP_METER_MODE_USER_METERING,
    QCAMERA3_EXP_METER_MODE_SPOT_METERING_ADV,
    QCAMERA3_EXP_METER_MODE_CENTER_WEIGHTED_ADV
} qcamera3_ext_exposure_meter_mode_t;

// QCAMERA3_ISO_EXP_PRIORITY
typedef enum qcamera3_ext_iso_mode {
    QCAMERA3_ISO_MODE_AUTO,
    QCAMERA3_ISO_MODE_DEBLUR,
    QCAMERA3_ISO_MODE_100,
    QCAMERA3_ISO_MODE_200,
    QCAMERA3_ISO_MODE_400,
    QCAMERA3_ISO_MODE_800,
    QCAMERA3_ISO_MODE_1600,
    QCAMERA3_ISO_MODE_3200,
} qcamera3_ext_iso_mode_t;

typedef enum {
    /* RAW only mode for devices that will simultaneously use more than two
     * cameras. This is required because by default the HAL will start support
     * stream along with RAW stream. This will allocate resources which
     * otherwise can be used for another camera. Using this mode indicates HAL
     * that the additional functionality provided by support stream is not
     * needed.
     * This mode has following limitations:
     *    - Back end 3A, Face Detect or any additional functionality depending
     *   on image/sensor statistics and YUV streams will be disabled.
     */
    QCAMERA3_VENDOR_STREAM_CONFIGURATION_RAW_ONLY_MODE = 0x8000,

    /* Disables all support streams for non raw mode configurations. */
    QCAMERA3_VENDOR_STREAM_CONFIGURATION_DISABLE_SUPPORT_STREAMS = 0x8001,

    /*YUV ZSL*/
    QCAMERA3_VENDOR_STREAM_CONFIGURATION_YUV_ZSL_MODE = 0x8002,
    /* Controlling Postprocessing of YUV stream from App. This is required to
     * reduce the performance overhead while multiple callback streams are running
     * by disabling postprocessing which improves power, performance.
     * When App notifies this mode to HAL, HAL doesnt link CPP with any stream
     * This mode has following limitations:
     *    - All YUV stream under this mode wont be connecting CPP
     *    - Postprocessing cant be done.
     */
    QCAMERA3_VENDOR_STREAM_CONFIGURATION_PP_DISABLED_MODE = 0x8004
} QCamera3VendorStreamConfiguration;

class QCamera3VendorTags {

public:
    static void get_vendor_tag_ops(vendor_tag_ops_t* ops);
    static int get_tag_count(
            const vendor_tag_ops_t *ops);
    static void get_all_tags(
            const vendor_tag_ops_t *ops,
            uint32_t *tag_array);
    static const char* get_section_name(
            const vendor_tag_ops_t *ops,
            uint32_t tag);
    static const char* get_tag_name(
            const vendor_tag_ops_t *ops,
            uint32_t tag);
    static int get_tag_type(
            const vendor_tag_ops_t *ops,
            uint32_t tag);

    static const vendor_tag_ops_t *Ops;
};

}; // namespace qcamera

#endif /* __QCAMERA3VENDORTAGS_H__ */
