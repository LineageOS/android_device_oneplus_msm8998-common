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

#define LOG_TAG "QCamera3VendorTags"

// Camera dependencies
#include "QCamera3HWI.h"
#include "QCamera3VendorTags.h"

extern "C" {
#include "mm_camera_dbg.h"
}

using namespace android;

namespace qcamera {

enum qcamera3_ext_tags qcamera3_ext3_section_bounds[QCAMERA3_SECTIONS_END -
    VENDOR_SECTION] = {
        QCAMERA3_PRIVATEDATA_END,
        QCAMERA3_CDS_END,
        QCAMERA3_OPAQUE_RAW_END,
        QCAMERA3_CROP_END,
        QCAMERA3_TUNING_META_DATA_END,
        QCAMERA3_TEMPORAL_DENOISE_END,
        QCAMERA3_ISO_EXP_PRIORITY_END,
        QCAMERA3_SATURATION_END,
        QCAMERA3_EXPOSURE_METER_END,
        QCAMERA3_AV_TIMER_END,
        QCAMERA3_SENSOR_META_DATA_END,
        QCAMERA3_DUALCAM_LINK_META_DATA_END,
        QCAMERA3_DUALCAM_CALIB_META_DATA_END,
        QCAMERA3_HAL_PRIVATEDATA_END,
        QCAMERA3_JPEG_ENCODE_CROP_END,
        QCAMERA3_VIDEO_HDR_END,
        QCAMERA3_IR_END,
        QCAMERA3_AEC_CONVERGENCE_SPEED_END,
        QCAMERA3_AWB_CONVERGENCE_SPEED_END,
        QCAMERA3_INSTANT_AEC_END,
        QCAMERA3_SHARPNESS_END,
        QCAMERA3_HISTOGRAM_END,
        QCAMERA3_BINNING_CORRECTION_END,
        QCAMERA3_STATS_END,
        QCAMERA3_SIMULTANEOUS_CAMERA_END,
        QCAMERA3_QUADRA_CFA_DATA_END,
        QCAMERA3_HFR_END,
        QCAMERA3_BOKEH_END,
        QCAMERA3_FUSION_END,
        QCAMERA3_LOGICAL_CAM_END,
        QCAMERA3_MANUAL_WB_END,
        QCAMERA3_SAT_MODE_END,
        QCAMERA3_EIS_FLUSH_END,
        QCAMERA3_SWMFNR_END,
};

typedef struct vendor_tag_info {
    const char *tag_name;
    uint8_t     tag_type;
} vendor_tag_info_t;

const char *qcamera3_ext_section_names[QCAMERA3_SECTIONS_END -
        VENDOR_SECTION] = {
    "org.codeaurora.qcamera3.privatedata",
    "org.codeaurora.qcamera3.CDS",
    "org.codeaurora.qcamera3.opaque_raw",
    "org.codeaurora.qcamera3.crop",
    "org.codeaurora.qcamera3.tuning_meta_data",
    "org.codeaurora.qcamera3.temporal_denoise",
    "org.codeaurora.qcamera3.iso_exp_priority",
    "org.codeaurora.qcamera3.saturation",
    "org.codeaurora.qcamera3.exposure_metering",
    "org.codeaurora.qcamera3.av_timer",
    "org.codeaurora.qcamera3.sensor_meta_data",
    "org.codeaurora.qcamera3.dualcam_link_meta_data",
    "org.codeaurora.qcamera3.dualcam_calib_meta_data",
    "org.codeaurora.qcamera3.hal_private_data",
    "org.codeaurora.qcamera3.jpeg_encode_crop",
    "org.codeaurora.qcamera3.video_hdr_mode",
    "org.codeaurora.qcamera3.ir",
    "org.codeaurora.qcamera3.aec_convergence_speed",
    "org.codeaurora.qcamera3.awb_convergence_speed",
    "org.codeaurora.qcamera3.instant_aec",
    "org.codeaurora.qcamera3.sharpness",
    "org.codeaurora.qcamera3.histogram",
    "org.codeaurora.qcamera3.binning_correction",
    "org.codeaurora.qcamera3.stats",
    "org.codeaurora.qcamera3.simultaneous_camera",
    "org.codeaurora.qcamera3.quadra_cfa",
    "org.codeaurora.qcamera3.hfr",
    "org.codeaurora.qcamera3.bokeh",
    "org.codeaurora.qcamera3.fusion",
    "org.codeaurora.qcamera3.logical",
    "org.codeaurora.qcamera3.manualWB",
    "org.codeaurora.qcamera3.sat",
    "org.quic.camera.recording",
    "org.codeaurora.qcamera3.swmfnr"
};

vendor_tag_info_t qcamera3_privatedata[QCAMERA3_PRIVATEDATA_END - QCAMERA3_PRIVATEDATA_START] = {
    { "privatedata_reprocess", TYPE_INT32 }
};

vendor_tag_info_t qcamera3_cds[QCAMERA3_CDS_END - QCAMERA3_CDS_START] = {
    { "cds_mode", TYPE_INT32 },
    { "cds_info", TYPE_BYTE }
};

vendor_tag_info_t qcamera3_opaque_raw[QCAMERA3_OPAQUE_RAW_END -
        QCAMERA3_OPAQUE_RAW_START] = {
    { "opaque_raw_strides", TYPE_INT32 },
    { "opaque_raw_format", TYPE_BYTE }
};

vendor_tag_info_t qcamera3_crop[QCAMERA3_CROP_END- QCAMERA3_CROP_START] = {
    { "count", TYPE_INT32 },
    { "data", TYPE_INT32},
    { "roimap", TYPE_INT32 }
};

vendor_tag_info_t qcamera3_tuning_meta_data[QCAMERA3_TUNING_META_DATA_END -
        QCAMERA3_TUNING_META_DATA_START] = {
    { "tuning_meta_data_blob", TYPE_INT32 }
};

vendor_tag_info_t qcamera3_temporal_denoise[QCAMERA3_TEMPORAL_DENOISE_END -
        QCAMERA3_TEMPORAL_DENOISE_START] = {
    { "enable", TYPE_BYTE },
    { "process_type", TYPE_INT32 }
};

vendor_tag_info qcamera3_iso_exp_priority[QCAMERA3_ISO_EXP_PRIORITY_END -
                                  QCAMERA3_ISO_EXP_PRIORITY_START] = {
    { "use_iso_exp_priority", TYPE_INT64 },
    { "select_priority", TYPE_INT32 },
    { "iso_available_modes", TYPE_INT32 },
    { "exposure_time_range", TYPE_INT64 }
};

vendor_tag_info qcamera3_saturation[QCAMERA3_SATURATION_END -
                                  QCAMERA3_SATURATION_START] = {
    { "use_saturation", TYPE_INT32 },
    { "range", TYPE_INT32 }
};

vendor_tag_info qcamera3_exposure_metering[QCAMERA3_EXPOSURE_METER_END -
                                  QCAMERA3_EXPOSURE_METER_START] = {
    { "exposure_metering_mode", TYPE_INT32},
    { "available_modes", TYPE_INT32 }
};

vendor_tag_info qcamera3_av_timer[QCAMERA3_AV_TIMER_END -
                                  QCAMERA3_AV_TIMER_START] = {
   {"use_av_timer", TYPE_BYTE }
};

vendor_tag_info qcamera3_sensor_meta_data[QCAMERA3_SENSOR_META_DATA_END -
                                  QCAMERA3_SENSOR_META_DATA_START] = {
   {"dynamic_black_level_pattern", TYPE_FLOAT },
   {"is_mono_only",                TYPE_BYTE }
};

vendor_tag_info_t
        qcamera3_dualcam_link_meta_data[QCAMERA3_DUALCAM_LINK_META_DATA_END -
        QCAMERA3_DUALCAM_LINK_META_DATA_START] = {
    { "enable",            TYPE_BYTE },
    { "is_main",           TYPE_BYTE },
    { "related_camera_id", TYPE_INT32 }
};

vendor_tag_info_t
        qcamera3_dualcam_calib_meta_data[QCAMERA3_DUALCAM_CALIB_META_DATA_END -
        QCAMERA3_DUALCAM_CALIB_META_DATA_START] = {
    { "dualcam_calib_meta_data_blob", TYPE_BYTE }
};

vendor_tag_info_t
        qcamera3_hal_privatedata[QCAMERA3_HAL_PRIVATEDATA_END -
        QCAMERA3_HAL_PRIVATEDATA_START] = {
    { "reprocess_flags",      TYPE_BYTE },
    { "reprocess_data_blob",  TYPE_BYTE },
    { "exif_debug_data_blob", TYPE_BYTE }
};

vendor_tag_info_t
        qcamera3_jpep_encode_crop[QCAMERA3_JPEG_ENCODE_CROP_END -
        QCAMERA3_JPEG_ENCODE_CROP_START] = {
    { "enable", TYPE_BYTE },
    { "rect",   TYPE_INT32 },
    { "roi",    TYPE_INT32}
};

vendor_tag_info_t qcamera3_video_hdr[QCAMERA3_VIDEO_HDR_END -
        QCAMERA3_VIDEO_HDR_START] = {
    { "vhdr_mode", TYPE_INT32 },
    { "vhdr_supported_modes", TYPE_INT32 }
};

vendor_tag_info_t qcamera3_ir[QCAMERA3_IR_END -
        QCAMERA3_IR_START] = {
    { "ir_mode", TYPE_INT32 },
    { "ir_supported_modes", TYPE_INT32}
};

vendor_tag_info_t qcamera3_aec_speed[QCAMERA3_AEC_CONVERGENCE_SPEED_END -
        QCAMERA3_AEC_CONVERGENCE_SPEED_START] = {
    {"aec_speed", TYPE_FLOAT }
};

vendor_tag_info_t qcamera3_awb_speed[QCAMERA3_AWB_CONVERGENCE_SPEED_END -
        QCAMERA3_AWB_CONVERGENCE_SPEED_START] = {
    {"awb_speed", TYPE_FLOAT }
};

vendor_tag_info_t
        qcamera3_instant_aec[QCAMERA3_INSTANT_AEC_END -
        QCAMERA3_INSTANT_AEC_START] = {
    { "instant_aec_mode", TYPE_INT32 },
    { "instant_aec_available_modes",   TYPE_INT32 }
};

vendor_tag_info_t qcamera3_sharpness[QCAMERA3_SHARPNESS_END -
        QCAMERA3_SHARPNESS_START] = {
    {"strength", TYPE_INT32 },
    {"range", TYPE_INT32 }
};

vendor_tag_info_t qcamera3_histogram[QCAMERA3_HISTOGRAM_END -
        QCAMERA3_HISTOGRAM_START] = {
    { "enable", TYPE_BYTE },
    { "buckets", TYPE_INT32 },
    { "max_count", TYPE_INT32 },
    { "stats", TYPE_INT32 }
};

vendor_tag_info_t qcamera3_binning_correction[QCAMERA3_BINNING_CORRECTION_END -
        QCAMERA3_BINNING_CORRECTION_START] = {
    { "binning_correction_mode", TYPE_INT32 },
    { "binning_correction_available_modes",   TYPE_INT32 }
};

vendor_tag_info_t qcamera3_stats[QCAMERA3_STATS_END -
        QCAMERA3_STATS_START] = {
    { "is_hdr_scene", TYPE_BYTE },
    { "is_hdr_scene_values", TYPE_BYTE },
    { "is_hdr_scene_confidence",   TYPE_FLOAT },
    { "is_hdr_scene_confidence_range", TYPE_FLOAT },
    { "bsgc_available", TYPE_BYTE },
    { "blink_detected", TYPE_BYTE },
    { "blink_degree", TYPE_BYTE },
    { "smile_degree", TYPE_BYTE },
    { "smile_confidence", TYPE_BYTE },
    { "gaze_angle", TYPE_BYTE },
    { "gaze_direction", TYPE_INT32 },
    { "gaze_degree", TYPE_BYTE }
};

vendor_tag_info_t qcamera3_simultaneous_camera[QCAMERA3_SIMULTANEOUS_CAMERA_END-
        QCAMERA3_SIMULTANEOUS_CAMERA_START] = {
    { "vfe1_reserved_rdi", TYPE_INT32 },
};

vendor_tag_info_t qcamera3_quadra_cfa[QCAMERA3_QUADRA_CFA_DATA_END -
        QCAMERA3_QUADRA_CFA_DATA_START] = {
    { "is_qcfa_sensor", TYPE_BYTE },
    { "qcfa_dimension", TYPE_INT32 },
};

vendor_tag_info_t qcamera3_hfr[QCAMERA3_HFR_END -
        QCAMERA3_HFR_START] = {
    { "sizes", TYPE_INT32 }
};

vendor_tag_info_t qcamera3_bokeh[QCAMERA3_BOKEH_END -
        QCAMERA3_BOKEH_START] = {
    { "enable", TYPE_BYTE },
    { "blurLevel", TYPE_INT32 },
    { "status", TYPE_INT32 }
};

vendor_tag_info_t qcamera3_fusion[QCAMERA3_FUSION_END -
        QCAMERA3_FUSION_START] = {
    { "status", TYPE_BYTE }
};

vendor_tag_info_t qcamera3_logical_cam[QCAMERA3_LOGICAL_CAM_END -
        QCAMERA3_LOGICAL_CAM_START] = {
    { "mode", TYPE_BYTE }
};

vendor_tag_info qcamera3_manual_wb[QCAMERA3_MANUAL_WB_END -
                                  QCAMERA3_MANUAL_WB_START] = {
    { "partial_mwb_mode", TYPE_INT32 },
    { "color_temperature_range", TYPE_INT32 },
    { "gains_range", TYPE_FLOAT },
    { "color_temperature", TYPE_INT32 },
    { "gains", TYPE_FLOAT }
};

vendor_tag_info qcamera3_swmfnr[QCAMERA3_SWMFNR_END -
                                  QCAMERA3_SWMFNR_START] = {
    { "enable", TYPE_BYTE }
};

vendor_tag_info_t qcamera3_sat_cam[QCAMERA3_SAT_MODE_END -
        QCAMERA3_SAT_MODE_START] = {
    { "on", TYPE_BYTE }
};

vendor_tag_info_t qcamera3_eis_flush[QCAMERA3_EIS_FLUSH_END -
        QCAMERA3_EIS_FLUSH_START] = {
    { "endOfStream", TYPE_BYTE }
};

vendor_tag_info_t *qcamera3_tag_info[QCAMERA3_SECTIONS_END -
        VENDOR_SECTION] = {
    qcamera3_privatedata,
    qcamera3_cds,
    qcamera3_opaque_raw,
    qcamera3_crop,
    qcamera3_tuning_meta_data,
    qcamera3_temporal_denoise,
    qcamera3_iso_exp_priority,
    qcamera3_saturation,
    qcamera3_exposure_metering,
    qcamera3_av_timer,
    qcamera3_sensor_meta_data,
    qcamera3_dualcam_link_meta_data,
    qcamera3_dualcam_calib_meta_data,
    qcamera3_hal_privatedata,
    qcamera3_jpep_encode_crop,
    qcamera3_video_hdr,
    qcamera3_ir,
    qcamera3_aec_speed,
    qcamera3_awb_speed,
    qcamera3_instant_aec,
    qcamera3_sharpness,
    qcamera3_histogram,
    qcamera3_binning_correction,
    qcamera3_stats,
    qcamera3_simultaneous_camera,
    qcamera3_quadra_cfa,
    qcamera3_hfr,
    qcamera3_bokeh,
    qcamera3_fusion,
    qcamera3_logical_cam,
    qcamera3_manual_wb,
    qcamera3_sat_cam,
    qcamera3_eis_flush,
    qcamera3_swmfnr,
};

uint32_t qcamera3_all_tags[] = {
    // QCAMERA3_PRIVATEDATA
    (uint32_t)QCAMERA3_PRIVATEDATA_REPROCESS,

    // QCAMERA3_CDS
    (uint32_t)QCAMERA3_CDS_MODE,
    (uint32_t)QCAMERA3_CDS_INFO,

    // QCAMERA3_OPAQUE_RAW
    (uint32_t)QCAMERA3_OPAQUE_RAW_STRIDES,
    (uint32_t)QCAMERA3_OPAQUE_RAW_FORMAT,

    // QCAMERA3_CROP
    (uint32_t)QCAMERA3_CROP_COUNT_REPROCESS,
    (uint32_t)QCAMERA3_CROP_REPROCESS,
    (uint32_t)QCAMERA3_CROP_ROI_MAP_REPROCESS,

    // QCAMERA3_TUNING_META_DATA
    (uint32_t)QCAMERA3_TUNING_META_DATA_BLOB,

    // QCAMERA3_TEMPORAL_DENOISE
    (uint32_t)QCAMERA3_TEMPORAL_DENOISE_ENABLE,
    (uint32_t)QCAMERA3_TEMPORAL_DENOISE_PROCESS_TYPE,

    // QCAMERA3_ISO_EXP_PRIORITY
    (uint32_t)QCAMERA3_USE_ISO_EXP_PRIORITY,
    (uint32_t)QCAMERA3_SELECT_PRIORITY,
    (uint32_t)QCAMERA3_ISO_AVAILABLE_MODES,
    (uint32_t)QCAMERA3_EXP_TIME_RANGE,

    // QCAMERA3_SATURATION
    (uint32_t)QCAMERA3_USE_SATURATION,
    (uint32_t)QCAMERA3_SATURATION_RANGE,

    // QCAMERA3_EXPOSURE_METERING
    (uint32_t)QCAMERA3_EXPOSURE_METER,
    (uint32_t)QCAMERA3_EXPOSURE_METER_AVAILABLE_MODES,

    //QCAMERA3_AVTIMER
    (uint32_t)QCAMERA3_USE_AV_TIMER,

    //QCAMERA3_SENSOR_META_DATA
    (uint32_t)QCAMERA3_SENSOR_DYNAMIC_BLACK_LEVEL_PATTERN,
    (uint32_t)QCAMERA3_SENSOR_IS_MONO_ONLY,

    // QCAMERA3_DUALCAM_LINK_META_DATA
    (uint32_t)QCAMERA3_DUALCAM_LINK_ENABLE,
    (uint32_t)QCAMERA3_DUALCAM_LINK_IS_MAIN,
    (uint32_t)QCAMERA3_DUALCAM_LINK_RELATED_CAMERA_ID,

    // QCAMERA3_DUALCAM_CALIB_META_DATA
    (uint32_t)QCAMERA3_DUALCAM_CALIB_META_DATA_BLOB,

    // QCAMERA3_HAL_PRIVATEDATA
    (uint32_t)QCAMERA3_HAL_PRIVATEDATA_REPROCESS_FLAGS,
    (uint32_t)QCAMERA3_HAL_PRIVATEDATA_REPROCESS_DATA_BLOB,
    (uint32_t)QCAMERA3_HAL_PRIVATEDATA_EXIF_DEBUG_DATA_BLOB,

    // QCAMERA3_JPEG_ENCODE_CROP
    (uint32_t)QCAMERA3_JPEG_ENCODE_CROP_ENABLE,
    (uint32_t)QCAMERA3_JPEG_ENCODE_CROP_RECT,
    (uint32_t)QCAMERA3_JPEG_ENCODE_CROP_ROI,

    // QCAMERA3_VIDEO_HDR
    (uint32_t)QCAMERA3_VIDEO_HDR_MODE,
    (uint32_t)QCAMERA3_AVAILABLE_VIDEO_HDR_MODES,

    // QCAMERA3_IR_MODE_ENABLE
    (uint32_t)QCAMERA3_IR_MODE,
    (uint32_t)QCAMERA3_IR_AVAILABLE_MODES,

    //QCAMERA3_AEC_CONVERGENCE_SPEED
    (uint32_t)QCAMERA3_AEC_CONVERGENCE_SPEED,

    //QCAMERA3_AWB_CONVERGENCE_SPEED
    (uint32_t)QCAMERA3_AWB_CONVERGENCE_SPEED,

    // QCAMERA3_INSTANT_AEC
    (uint32_t)QCAMERA3_INSTANT_AEC_MODE,
    (uint32_t)QCAMERA3_INSTANT_AEC_AVAILABLE_MODES,

    //QCAMERA3_SHARPNESS
    (uint32_t)QCAMERA3_SHARPNESS_STRENGTH,
    (uint32_t)QCAMERA3_SHARPNESS_RANGE,

    //QCAMERA3_HISTOGRAM
    (uint32_t)QCAMERA3_HISTOGRAM_MODE,
    (uint32_t)QCAMERA3_HISTOGRAM_BUCKETS,
    (uint32_t)QCAMERA3_HISTOGRAM_MAX_COUNT,
    (uint32_t)QCAMERA3_HISTOGRAM_STATS,

    // QCAMERA3_BINNING_CORRECTION_END
    (uint32_t)QCAMERA3_BINNING_CORRECTION_MODE,
    (uint32_t)QCAMERA3_AVAILABLE_BINNING_CORRECTION_MODES,

    // QCAMERA3_STATS
    (uint32_t)QCAMERA3_STATS_IS_HDR_SCENE,
    (uint32_t)QCAMERA3_STATS_IS_HDR_SCENE_CONFIDENCE,
    (uint32_t)QCAMERA3_STATS_BSGC_AVAILABLE,
    (uint32_t)QCAMERA3_STATS_BLINK_DETECTED,
    (uint32_t)QCAMERA3_STATS_BLINK_DEGREE,
    (uint32_t)QCAMERA3_STATS_SMILE_DEGREE,
    (uint32_t)QCAMERA3_STATS_SMILE_CONFIDENCE,
    (uint32_t)QCAMERA3_STATS_GAZE_ANGLE,
    (uint32_t)QCAMERA3_STATS_GAZE_DIRECTION,
    (uint32_t)QCAMERA3_STATS_GAZE_DEGREE,

    // QCAMERA3_SIMULTANEOUS_CAMERA
    (uint32_t)QCAMERA3_SIMULTANEOUS_CAMERA_VFE1_RESERVED_RDI,

    // QCAMERA3_QUADRA_CFA_DATA
    (uint32_t)QCAMERA3_IS_QUADRA_CFA_SENSOR,
    (uint32_t)QCAMERA3_SUPPORT_QUADRA_CFA_DIM,

    //QCAMERA3_HFR
    (uint32_t)QCAMERA3_HFR_SIZES,

    //QCAMERA3_BOKEH
    (uint32_t)QCAMERA3_BOKEH_ENABLE,
    (uint32_t)QCAMERA3_BOKEH_BLURLEVEL,
    (uint32_t)QCAMERA3_BOKEH_STATUS,

    //QCAMERA3_FUSION
    (uint32_t)QCAMERA3_FUSION_STATUS,

    //QCAMERA3_LOGICAL_CAM
    (uint32_t)QCAMERA3_LOGICAL_CAM_MODE,

    //QCAMERA3_MANUAL_WB
    (uint32_t)QCAMERA3_MANUAL_WB_MODE,
    (uint32_t)QCAMERA3_MANUAL_WB_CCT_RANGE,
    (uint32_t)QCAMERA3_MANUAL_WB_GAINS_RANGE,
    (uint32_t)QCAMERA3_MANUAL_WB_CCT,
    (uint32_t)QCAMERA3_MANUAL_WB_GAINS,
    //QCAMERA3_SAT
    (uint32_t)QCAMERA3_SAT_MODE_ON,
    (uint32_t)QCAMERA3_EIS_FLUSH_ON,
    //QCAMERA3 SWMFNR
    (uint32_t)QCAMERA3_SWMFNR_ENABLE
};

const vendor_tag_ops_t* QCamera3VendorTags::Ops = NULL;

/*===========================================================================
 * FUNCTION   : get_vendor_tag_ops
 *
 * DESCRIPTION: Get the metadata vendor tag function pointers
 *
 * PARAMETERS :
 *    @ops   : function pointer table to be filled by HAL
 *
 *
 * RETURN     : NONE
 *==========================================================================*/
void QCamera3VendorTags::get_vendor_tag_ops(
                                vendor_tag_ops_t* ops)
{
    LOGL("E");

    Ops = ops;

    ops->get_tag_count = get_tag_count;
    ops->get_all_tags = get_all_tags;
    ops->get_section_name = get_section_name;
    ops->get_tag_name = get_tag_name;
    ops->get_tag_type = get_tag_type;
    ops->reserved[0] = NULL;

    LOGL("X");
    return;
}

/*===========================================================================
 * FUNCTION   : get_tag_count
 *
 * DESCRIPTION: Get number of vendor tags supported
 *
 * PARAMETERS :
 *    @ops   :  Vendor tag ops data structure
 *
 *
 * RETURN     : Number of vendor tags supported
 *==========================================================================*/

int QCamera3VendorTags::get_tag_count(
                const vendor_tag_ops_t * ops)
{
    size_t count = 0;
    if (ops == Ops)
        count = sizeof(qcamera3_all_tags)/sizeof(qcamera3_all_tags[0]);

    LOGL("count is %d", count);
    return (int)count;
}

/*===========================================================================
 * FUNCTION   : get_all_tags
 *
 * DESCRIPTION: Fill array with all supported vendor tags
 *
 * PARAMETERS :
 *    @ops      :  Vendor tag ops data structure
 *    @tag_array:  array of metadata tags
 *
 * RETURN     : Success: the section name of the specific tag
 *              Failure: NULL
 *==========================================================================*/
void QCamera3VendorTags::get_all_tags(
                const vendor_tag_ops_t * ops,
                uint32_t *g_array)
{
    if (ops != Ops)
        return;

    for (size_t i = 0;
            i < sizeof(qcamera3_all_tags)/sizeof(qcamera3_all_tags[0]);
            i++) {
        g_array[i] = qcamera3_all_tags[i];
        LOGD("g_array[%d] is %d", i, g_array[i]);
    }
}

/*===========================================================================
 * FUNCTION   : get_section_name
 *
 * DESCRIPTION: Get section name for vendor tag
 *
 * PARAMETERS :
 *    @ops   :  Vendor tag ops structure
 *    @tag   :  Vendor specific tag
 *
 *
 * RETURN     : Success: the section name of the specific tag
 *              Failure: NULL
 *==========================================================================*/

const char* QCamera3VendorTags::get_section_name(
                const vendor_tag_ops_t * ops,
                uint32_t tag)
{
    LOGL("E");
    if (ops != Ops)
        return NULL;

    const char *ret;
    uint32_t section = tag >> 16;

    if (section < VENDOR_SECTION || section >= QCAMERA3_SECTIONS_END)
        ret = NULL;
    else
        ret = qcamera3_ext_section_names[section - VENDOR_SECTION];

    if (ret)
        LOGL("section_name[%d] is %s", tag, ret);
    LOGL("X");
    return ret;
}

/*===========================================================================
 * FUNCTION   : get_tag_name
 *
 * DESCRIPTION: Get name of a vendor specific tag
 *
 * PARAMETERS :
 *    @tag   :  Vendor specific tag
 *
 *
 * RETURN     : Success: the name of the specific tag
 *              Failure: NULL
 *==========================================================================*/
const char* QCamera3VendorTags::get_tag_name(
                const vendor_tag_ops_t * ops,
                uint32_t tag)
{
    LOGL("E");
    const char *ret;
    uint32_t section = tag >> 16;
    uint32_t section_index = section - VENDOR_SECTION;
    uint32_t tag_index = tag & 0xFFFF;

    if (ops != Ops) {
        ret = NULL;
        goto done;
    }

    if (section < VENDOR_SECTION || section >= QCAMERA3_SECTIONS_END)
        ret = NULL;
    else if (tag >= (uint32_t)qcamera3_ext3_section_bounds[section_index])
        ret = NULL;
    else
        ret = qcamera3_tag_info[section_index][tag_index].tag_name;

    if (ret)
        LOGL("tag name for tag %d is %s", tag, ret);
    LOGL("X");

done:
    return ret;
}

/*===========================================================================
 * FUNCTION   : get_tag_type
 *
 * DESCRIPTION: Get type of a vendor specific tag
 *
 * PARAMETERS :
 *    @tag   :  Vendor specific tag
 *
 *
 * RETURN     : Success: the type of the specific tag
 *              Failure: -1
 *==========================================================================*/
int QCamera3VendorTags::get_tag_type(
                const vendor_tag_ops_t *ops,
                uint32_t tag)
{
    LOGL("E");
    int ret;
    uint32_t section = tag >> 16;
    uint32_t section_index = section - VENDOR_SECTION;
    uint32_t tag_index = tag & 0xFFFF;

    if (ops != Ops) {
        ret = -1;
        goto done;
    }
    if (section < VENDOR_SECTION || section >= QCAMERA3_SECTIONS_END)
        ret = -1;
    else if (tag >= (uint32_t )qcamera3_ext3_section_bounds[section_index])
        ret = -1;
    else
        ret = qcamera3_tag_info[section_index][tag_index].tag_type;

    LOGL("tag type for tag %d is %d", tag, ret);
    LOGL("X");
done:
    return ret;
}

}; //end namespace qcamera
