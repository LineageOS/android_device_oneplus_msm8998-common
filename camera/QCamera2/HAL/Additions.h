enum {
    CAMERA_CMD_VENDOR_START = 0,
    /**
     * Commands to enable/disable preview histogram
     *
     * Based on user's input to enable/disable histogram from the camera
     * UI, send the appropriate command to the HAL to turn on/off the histogram
     * stats and start sending the data to the application.
     */
    CAMERA_CMD_HISTOGRAM_ON = CAMERA_CMD_VENDOR_START,
    CAMERA_CMD_HISTOGRAM_OFF = CAMERA_CMD_VENDOR_START + 1,
    CAMERA_CMD_HISTOGRAM_SEND_DATA  = CAMERA_CMD_VENDOR_START + 2,
    CAMERA_CMD_LONGSHOT_ON = CAMERA_CMD_VENDOR_START + 3,
    CAMERA_CMD_LONGSHOT_OFF = CAMERA_CMD_VENDOR_START + 4,
    CAMERA_CMD_STOP_LONGSHOT = CAMERA_CMD_VENDOR_START + 5,
    CAMERA_CMD_METADATA_ON = CAMERA_CMD_VENDOR_START + 6,
    CAMERA_CMD_METADATA_OFF = CAMERA_CMD_VENDOR_START + 7,
    CAMERA_CMD_VENDOR_END = 200,

    CAMERA_MSG_VENDOR_START = 0x1000,
    CAMERA_MSG_STATS_DATA = CAMERA_MSG_VENDOR_START,
    CAMERA_MSG_META_DATA = 0x2000,
};


/**
 * The information of a data type received in a camera frame.
 */
typedef enum {
    /** Data buffer */
    CAMERA_FRAME_DATA_BUF = 0x000,
    /** File descriptor */
    CAMERA_FRAME_DATA_FD = 0x100
} camera_frame_data_type_t;

/** meta data type in CameraMetaDataCallback */
enum {
    CAMERA_META_DATA_ASD = 0x001,    //ASD data
    CAMERA_META_DATA_FD = 0x002,     //FD/FP data
    CAMERA_META_DATA_HDR = 0x003,    //Auto HDR data
};
