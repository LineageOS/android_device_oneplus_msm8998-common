/* Copyright (c) 2019, The Linux Foundation. All rights reserved.
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
#ifndef __QCAMERA_PERF_TRANSLATOR_H__
#define __QCAMERA_PERF_TRANSLATOR_H__

/* Definitions */
#define LINESIZE 100
#define LINES 100
#define FSMROWS LINES
#define FSMCOLS 6
#define MAX_FROM_STREAMS MAX_NUM_STREAMS
#define FSM_MAX_LEVEL 3 /* Modem supports 0,1,2,3 */
#define NO_OF_SENSORS 2

/* Typedefs-1 */
typedef int32_t (*paramPtrFunc)(void*,void*,int32_t);
typedef int32_t (*funcPtr)(int32_t,int32_t);
typedef int32_t (*paramPtr)(void*,void*,int32_t,int32_t);



/* Enums */
enum FSMStates {
    FUNC_RETURN,
    PARAM_TYPE,
    FUNC_TYPE,
    CMP_VALUE,
    TRUE_VALUE,
    FALSE_VALUE,
    RETURN_TYPE
};

/* FSM structures */
struct FSMDB {
    int32_t FSMDB_FUNC_RETURN[FSMROWS];
    int32_t FSMDB_FUNC_TYPE_LOOP[FSMROWS];
    int32_t FSMDB_RETURN[FSMROWS];
    int32_t FSMDB_PARAM_STREAMNO[FSMROWS];
    int32_t FSMDB_CMP_VALUE[FSMROWS];
    int32_t FSMDB_TRUE_VALUE[FSMROWS];
    int32_t FSMDB_FALSE_VALUE[FSMROWS];
    paramPtr FSMDB_PARAM_TYPE[FSMROWS];
    funcPtr FSMDB_FUNC_TYPE[FSMROWS];
};

struct meta2 {
    int32_t sensorW[NO_OF_SENSORS];
    int32_t sensorH[NO_OF_SENSORS];
    int32_t sensorClk[NO_OF_SENSORS];
    int32_t sensorMP[NO_OF_SENSORS];    
    uint8_t hfr;
    uint8_t fd;
    uint8_t tnr;
};

/* Typedefs-2 */
typedef struct FSMDB FSMDB_t;
typedef struct meta2 meta2_t;

/**
 * setupFSM : Load the FSM Node Table
 *
 * @fileName : FSM Node table in text format
 * @return   : predicted value
 * Note on Usage:
 * - This function called at init of camera to initialize the FSM
 *   which depends on the FSM table provided.
 * - The return value is the FSM pointer
 **/

FSMDB_t *setupFSM(char *fileName);

/**
 * predictFSM : predicts the value from the FSM 
 *
 * @FSM     : FSM table created at setup
 * @param1  : basic structure of camera
 * @param2  : strucutre with sensor, TNR info
 * @session : Session No corresponding the above strucutres
 *
 * Note on Usage:
 * - If both param1 & param2 are NULL , then it is indicator
 *   that the session no passed has been closed, subtract
 *   session's predicted value.
 * - The return value is the aggregated predicted value
 * 
 */
int32_t predictFSM(FSMDB_t *FSM,void *param1, void *param2,int32_t session);

// TODO - update the header
void closeFSM();
#endif /* __QCAMERA_PERF_TRANSLATOR_H__ */
