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


#define LOG_TAG "QCameraPerf"

/* Includes */
#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
#include<string.h>
#include<stdlib.h>
#include "mm_camera_dbg.h"
#ifdef __cplusplus
} /* extern "C" */
#endif /* __cplusplus */
#include "cam_types.h"
#include "QCameraCommon.h"
#include "QCameraPerfTranslator.h"



static FSMDB_t *FSM=NULL;

/* Functions */
static int32_t readlines(FILE *fp,char *table){

    int32_t linesize=0;
    int32_t lines=0;
    char value;

    while(fread((char*)&value,1,1,fp)){
        if(value == '\r'){
            continue;
        } else if (value == '\n') {
            table[linesize]='\0';
            lines++;
            return linesize;
        } else {
            table[linesize]=value;
            linesize++;
        }
        if(linesize > LINESIZE || lines > LINES) {
            LOGE("ERROR: Size allocated for tables exceeded: lines -> %d, linesize -> %d",lines,linesize);
        }

    }

    return 0;
}

/********* Get Params from strcut *********/
static int32_t get_type2(
    void *param1,
    __attribute__ ((unused))void *param2,
    int32_t streamNo,
    __attribute__ ((unused))int32_t sensorNo
    ){
    cam_stream_size_info_t *p1 = (cam_stream_size_info_t *)param1;
    int32_t temp;
    temp=p1->type[streamNo];
    return temp;
}

static int32_t get_istype2(
    void *param1,
    __attribute__ ((unused))void *param2,
    int32_t streamNo,
    __attribute__ ((unused))int32_t sensorNo
    ){
    cam_stream_size_info_t *p1 = (cam_stream_size_info_t *)param1;
    int32_t temp;
    temp=p1->is_type[streamNo];
    return temp;
}

static int32_t get_width2(
    void *param1,
    __attribute__ ((unused))void *param2,
    int32_t streamNo,
    __attribute__ ((unused))int32_t sensorNo
    ){
    cam_stream_size_info_t *p1 = (cam_stream_size_info_t *)param1;
    int32_t temp;
    temp=p1->stream_sizes[streamNo].width;
    return temp;
}

static int32_t get_height2(
    void *param1,
    __attribute__ ((unused))void *param2,
    int32_t streamNo,
    __attribute__ ((unused))int32_t sensorNo
    ){
    cam_stream_size_info_t *p1 = (cam_stream_size_info_t *)param1;
    int32_t temp;
    temp=p1->stream_sizes[streamNo].height;
    return temp;
}

static int32_t get_format2(
    void *param1,
    __attribute__ ((unused))void *param2,
    int32_t streamNo,
    __attribute__ ((unused))int32_t sensorNo
    ){
    cam_stream_size_info_t *p1 = (cam_stream_size_info_t *)param1;
    int32_t temp;
    temp=p1->format[streamNo];
    return temp;
}

static int32_t get_hfr2(
    __attribute__ ((unused))void *param1,
    void *param2,
    __attribute__ ((unused))int32_t streamNo,
    __attribute__ ((unused))int32_t sensorNo
    ){
    meta2 *p2 = (meta2 *)param2;
    int32_t temp;
    temp=p2->hfr;
    return temp;
}

static int32_t get_fd2(
    __attribute__ ((unused))void *param1,
    void *param2,
    __attribute__ ((unused))int32_t streamNo,
    __attribute__ ((unused))int32_t sensorNo
    ){
    meta2 *p2 = (meta2 *)param2;
    int32_t temp;
    temp=p2->fd;
    return temp;
}

static int32_t get_tnr2(
    __attribute__ ((unused))void *param1,
    void *param2,
    __attribute__ ((unused))int32_t streamNo,
    __attribute__ ((unused))int32_t sensorNo
    ){
    meta2 *p2 = (meta2 *)param2;
    int32_t temp;
    temp=p2->tnr;
    return temp;
}

static int32_t get_sensorW2(
    __attribute__ ((unused))void *param1,
    void *param2,
    __attribute__ ((unused))int32_t streamNo,
    int32_t sensorNo
    ){
    meta2 *p2 = (meta2 *)param2;
    int32_t temp;
    temp=p2->sensorW[sensorNo];
    return temp;
}

static int32_t get_sensorH2(
    __attribute__ ((unused))void *param1,
    void *param2,
    __attribute__ ((unused))int32_t streamNo,
    int32_t sensorNo
    ){
    meta2 *p2 = (meta2 *)param2;
    int32_t temp;
    temp=p2->sensorH[sensorNo];
    return temp;
}

static int32_t get_sensorC2(
    __attribute__ ((unused))void *param1,
    void *param2,
    __attribute__ ((unused))int32_t streamNo,
    int32_t sensorNo
    ){
    meta2 *p2 = (meta2 *)param2;
    int32_t temp;
    temp=p2->sensorClk[sensorNo];
    return temp;
}

static int32_t get_sensorMP(
    __attribute__ ((unused))void *param1,
    void *param2,
    __attribute__ ((unused))int32_t streamNo,
    int32_t sensorNo
    ){
    meta2 *p2 = (meta2 *)param2;
    int32_t temp;
    temp=p2->sensorMP[sensorNo];
    return temp;
}
// static int32_t checkFSM(
//     void *param1,
//     void *param2,
//     int32_t *streamNo,
//     paramPtr paramType,
//     funcPtr func,
//     int32_t cmpValue,
//     int32_t loop,
//     int32_t sensorNo
//     ){
//     uint32_t i;
//     cam_stream_size_info_t *p1 = (cam_stream_size_info_t *)param1;
//     if (loop) {
//         /* Search through all the streams and find the stream which matches
//          * After finding the match return the streamNo. */
//         for (i = 0; i < p1->num_streams; i++) {
//             if(paramType(func,param1,param2,cmpValue,i)){
//                 /* Update the streamNo, to be used for further FSM indexing */
//                 streamNo=i;
//                 return true;
//             }
//         }
//         /* No match of th cmpValue with any of the params */
//         streamNo=MAX_NUM_STREAMS;
//         return false;
//     } else {
//         /* Don't change the streamNo */
//         return paramType(func,param1,param2,cmpValue,streamNo);
//     }

// }

/********* FSM Version 2 *****************/

/****** End of FSM Version 2 *************/

// static int get_type(void *param1,__attribute__ ((unused))void *param2,int32_t streamno) {
//     cam_stream_size_info_t *p1 = (cam_stream_size_info_t *)param1;
//     int32_t type=0;
//     int32_t temp;
//     uint32_t i;
//     /* Get param1 info */
//     if(streamno == MAX_FROM_STREAMS){
//         /* Not a particular stream, but the max value from all the streams */

//         for (i = 0; i < p1->num_streams; i++) {
//             temp=p1->type[i];
//             /* Get the max value */
//             if(temp > type){
//                 type=temp;
//             }
//         }
//     } else {
//         /* From a particular stream */
//         type=p1->type[streamno];
//     }
//     /* Get param2 info */
//     return type;
// }

// static int get_istype(void *param1,__attribute__ ((unused))void *param2,int32_t streamno) {
//     cam_stream_size_info_t *p1 = (cam_stream_size_info_t *)param1;
//     int32_t istype=0;
//     int32_t temp;
//     uint32_t i;
//     /* Get param1 info */
//     if(streamno == MAX_FROM_STREAMS){
//         /* Not a particular stream, but the max value from all the streams */

//         for (i = 0; i < p1->num_streams; i++) {
//             temp=p1->is_type[i];
//             /* Get the max value */
//             if(temp > istype){
//                 istype=temp;
//             }
//         }
//     } else {
//         /* From a particular stream */
//         istype=p1->is_type[streamno];
//     }
//     /* Get param2 info */
//     return istype;
// }

// static int32_t get_width(void *param1,__attribute__ ((unused))void *param2,int32_t streamno) {
//     cam_stream_size_info_t *p1 = (cam_stream_size_info_t *)param1;
//     int32_t width=0;
//     int32_t temp;
//     uint32_t i;
//     /* Get param1 info */
//     if(streamno == MAX_FROM_STREAMS){
//         /* Not a particular stream, but the max value from all the streams */

//         for (i = 0; i < p1->num_streams; i++) {
//             temp=p1->stream_sizes[i].width;
//             /* Get the max value */
//             if(temp > width){
//                 width=temp;
//             }
//         }
//     } else {
//         /* From a particular stream */
//         width=p1->stream_sizes[streamno].width;
//     }
//     /* Get param2 info */
//     return width;
// }

// static int32_t get_format(void *param1,__attribute__ ((unused))void *param2,int32_t streamno) {
//     cam_stream_size_info_t *p1 = (cam_stream_size_info_t *)param1;
//     int32_t format=0;
//     int32_t temp;
//     uint32_t i;
//     /* Get param1 info */
//     if(streamno == MAX_FROM_STREAMS){
//         /* Not a particular stream, but the max value from all the streams */

//         for (i = 0; i < p1->num_streams; i++) {
//             temp=p1->format[i];
//             /* Get the max value */
//             if(temp > format){
//                 format=temp;
//             }
//         }
//     } else {
//         /* From a particular stream */
//         format=p1->format[streamno];
//     }
//     /* Get param2 info */
//     return format;
// }

// static int32_t get_height(void *param1,__attribute__ ((unused))void *param2,int32_t streamno) {
//     cam_stream_size_info_t *p1 = (cam_stream_size_info_t *)param1;
//     int32_t height=0;
//     int32_t temp;
//     uint32_t i;
//     /* Get param1 info */
//     if(streamno == MAX_FROM_STREAMS){
//         /* Not a particular stream, but the max value from all the streams */

//         for (i = 0; i < p1->num_streams; i++) {
//             temp=p1->stream_sizes[i].height;
//             /* Get the max value */
//             if(temp > height){
//                 height=temp;
//             }
//         }
//     } else {
//         /* From a particular stream */
//         height=p1->stream_sizes[streamno].height;
//     }
//     /* Get param2 info */
//     return height;
// }

// static int32_t get_EIS30(void *param1,__attribute__ ((unused))void *param2,__attribute__ ((unused))int32_t streamno) {
//     cam_stream_size_info_t *p1 = (cam_stream_size_info_t *)param1;
//     int32_t istype=0;
//     int32_t temp;
//     uint32_t i;
//     /* Get param1 info */

//     for (i = 0; i < p1->num_streams; i++) {
//         temp=p1->is_type[i];
//         /* Get the max value */
//         if(temp > istype){
//             istype=temp;
//         }
//     }
//     if(istype == IS_TYPE_EIS_3_0){
//         return true;
//     }
//     return false;
// }

// static int32_t get_hfr(__attribute__ ((unused))void *param1,void *param2,__attribute__ ((unused))int32_t streamno) {
//     meta2 *p2 = (meta2 *)param2;
//     return p2->hfr;
// }

// static int32_t get_tnr(__attribute__ ((unused))void *param1,void *param2,__attribute__ ((unused))int32_t streamno) {
//     meta2 *p2 = (meta2 *)param2;
//     return p2->tnr;
// }

// static int32_t get_fd(__attribute__ ((unused))void *param1,void *param2,__attribute__ ((unused))int32_t streamno) {
//     meta2 *p2 = (meta2 *)param2;
//     return p2->fd;
// }

// static int32_t get_sensorW(__attribute__ ((unused))void *param1,void *param2,int32_t streamno) {
//     meta2 *p2 = (meta2 *)param2;
//     return p2->sensorW[streamno];
// }

// static int32_t get_sensorH(__attribute__ ((unused))void *param1,void *param2,int32_t streamno) {
//     meta2 *p2 = (meta2 *)param2;
//     return p2->sensorH[streamno];
// }

// static int32_t get_sensorC(__attribute__ ((unused))void *param1,void *param2,int32_t streamno) {
//     meta2 *p2 = (meta2 *)param2;
//     return p2->sensorClk[streamno];
// }


static paramPtr get_param_type(char *type, int32_t *streamno,int32_t *loop){
    char stream_check[7]={'\0'};
    char *token;
    char *rest=type;
    int32_t no_of_tokens=0;
    char paramToken[10];
    *streamno=MAX_FROM_STREAMS;
    strlcpy(stream_check,type,6);
    if((!strcmp(stream_check,"stream")) || (!strcmp(stream_check,"sensor"))){
        /* Stream specific command */
        while ((token = strtok_r(rest, "_", &rest))) {
            if(!no_of_tokens){
                /* First token is stream */
            } else if (no_of_tokens == 1){
                /* Second token is the command */
                strlcpy(paramToken,token,sizeof(paramToken));
            } else if (no_of_tokens == 2){
                /* Final token is the stream no */
                *streamno=atoi(token);
            }
            no_of_tokens++;
        }
    } else {
        LOGE("ERROR: function should start with stream_*\n");
    }

    if (!strcmp(paramToken,"type")) {
        *loop = 1;
        return &get_type2;
    } else if (!strcmp(paramToken,"istype")) {
        *loop = 1;
        return &get_istype2;
    } else if (!strcmp(paramToken,"format")) {
        *loop = 0;
        return &get_format2;
    } else if (!strcmp(paramToken,"width")) {
        *loop = 0;
        return &get_width2;
    } else if (!strcmp(paramToken,"height")) {
        *loop = 0;
        return &get_height2;
    } else if (!strcmp(paramToken,"tnr")) {
        *loop = 0;
        return &get_tnr2;
    } else if (!strcmp(paramToken,"hfr")) {
        *loop = 0;
        return &get_hfr2;
    } else if (!strcmp(paramToken,"fd")) {
        *loop = 0;
        return &get_fd2;
    } else if (!strcmp(paramToken,"sensorW")) {
        *loop = 0;
        return &get_sensorW2;
    } else if (!strcmp(paramToken,"sensorH")) {
        *loop = 0;
        return &get_sensorH2;
    } else if (!strcmp(paramToken,"sensorC")) {
        *loop = 0;
        return &get_sensorC2;
    } else if (!strcmp(paramToken,"sensorMP")) {
        *loop = 0;
        return &get_sensorMP;
    } else {
        LOGE("The FSM table in file has errors 3\n");
        exit(0);
    }
    return NULL;
}
/********* END - Get Params from strcut *********/

/********* Decision Tree Comparator funcs **********/
static int32_t func_le(int32_t param_value,int32_t comp_value) {
    return (param_value <= comp_value);
}

static int32_t func_lt(int32_t param_value,int32_t comp_value) {
    return (param_value < comp_value);
}

static int32_t func_ge(int32_t param_value,int32_t comp_value) {
    return (param_value >= comp_value);
}

static int32_t func_gt(int32_t param_value,int32_t comp_value) {
    return (param_value > comp_value);
}

static int32_t func_eq(int32_t param_value,int32_t comp_value) {
    return (param_value == comp_value);
}


static funcPtr get_func_type(char *type){
    if (!strcmp("LE",type))
        return &func_le;
    else if(!strcmp("LT",type))
        return &func_lt;
    else if(!strcmp("GT",type))
        return &func_gt;
    else if(!strcmp("GE",type))
        return &func_ge;
    else if(!strcmp("EQ",type))
        return &func_eq;
    else {
        LOGE("The FSM table in file has errors 2\n");
        exit(0);
    }
    return NULL;
}
/********* END - Decision Tree Comparator funcs **********/

static uint8_t setupFSMDB(FSMDB_t *FSM,char *line){
    FSMStates state=FUNC_RETURN;
    int32_t stream_no=MAX_FROM_STREAMS;
    char * token;
    char * rest=line;
    int32_t value;
    int32_t loop;
    static int32_t FSMrowCount=0;

    while ((token = strtok_r(rest, ":", &rest))) {
        //LOGE("Token : %s\n", token);
        switch(state){
        case FUNC_RETURN:
            /*
             * FUNC = 0
             * RETURN = 1
             */
            value = atoi(token);
            if(value > 1){
                LOGE("The FSM table in file has errors 1\n");
                return false;
            }
            FSM->FSMDB_FUNC_RETURN[FSMrowCount]=value;
            if (!value) {
                state=PARAM_TYPE;
            } else {
                state=RETURN_TYPE;
            }
            break;
        case RETURN_TYPE:
            value = atoi(token);
            FSM->FSMDB_RETURN[FSMrowCount]=value;
            break;
        case PARAM_TYPE:
            FSM->FSMDB_PARAM_TYPE[FSMrowCount]=get_param_type(token,&stream_no,&loop);
            FSM->FSMDB_PARAM_STREAMNO[FSMrowCount]=stream_no;
            FSM->FSMDB_FUNC_TYPE_LOOP[FSMrowCount]=loop;
            state=FUNC_TYPE;
            break;
        case FUNC_TYPE:
            FSM->FSMDB_FUNC_TYPE[FSMrowCount]=get_func_type(token);
            state=CMP_VALUE;
            break;
        case CMP_VALUE:
            /* Compare Value will be float */
            value = atoi(token);
            FSM->FSMDB_CMP_VALUE[FSMrowCount]=value;
            state=TRUE_VALUE;
            break;
        case TRUE_VALUE:
            /* True value is index - hence integer */
            value = atoi(token);
            FSM->FSMDB_TRUE_VALUE[FSMrowCount]=value;
            state=FALSE_VALUE;
            break;
        case FALSE_VALUE:
            /* False value is index - hence integer */
            value = atoi(token);
            FSM->FSMDB_FALSE_VALUE[FSMrowCount]=value;
            break;
        }
    }
    FSMrowCount++;
    if (FSMrowCount>=1)
        return true;
    else
        return false;
}



static int32_t runFSM(FSMDB_t *FSM,int index,void *param1, void* param2,int32_t session,int32_t streamNo){
    /*
     * Extract data from the structures and crank it through the FSM.
     */
    int32_t isReturn;
    paramPtr param;
    funcPtr func;
    int32_t cmpValue;
    int32_t trueValue;
    int32_t falseValue;
    int32_t sensorNo;
    int32_t result;
    int32_t loop;
    uint32_t i;
    cam_stream_size_info_t *p1 = (cam_stream_size_info_t *)param1;
    isReturn=FSM->FSMDB_FUNC_RETURN[index];
    if(!isReturn){
        param=FSM->FSMDB_PARAM_TYPE[index];
        /* Get params from the structures */
        sensorNo=FSM->FSMDB_PARAM_STREAMNO[index];
        func=FSM->FSMDB_FUNC_TYPE[index];
        cmpValue=FSM->FSMDB_CMP_VALUE[index];
        trueValue=FSM->FSMDB_TRUE_VALUE[index];
        falseValue=FSM->FSMDB_FALSE_VALUE[index];
        /* Compare the paramValue with cmpValue and based
         * on the truth value get the index to the next node
         */
        loop = FSM->FSMDB_FUNC_TYPE_LOOP[index];
        //streamNo=MAX_NUM_STREAMS;
        result=false;
        if (loop) {
            /* Search through all the streams and find the stream which matches
             * After finding the match return the streamNo. */
            for (i = 0; i < p1->num_streams; i++) {

                if(func(param(param1,param2,i,sensorNo),cmpValue)){
                    /* Update the streamNo, to be used for further FSM indexing */
                    streamNo=i;
                    result=true;
                    break;
                }
            }
            /* No match of th cmpValue with any of the params */

        } else {
            /* Don't change the streamNo */
            result= func(param(param1,param2,streamNo,sensorNo),cmpValue);
        }
        index = (result?trueValue:falseValue);
        LOGH("FSM : Param = %p,StrNo = %d, func= %p, cmp= %d, true = %d, false = %d :: index = %d\n",param,streamNo,func,cmpValue,trueValue,falseValue,index);
        return runFSM(FSM,index,param1,param2,session,streamNo);
    }else{
        return FSM->FSMDB_RETURN[index];
    }
}




struct FSMDB* setupFSM(char *fileName){
    // 0 - Function execute it
    // 1 - Return the value
    /*  | Func/Return | ParamType | FuncType | CmpValue  | TrueValue | FalseValue
     *  |             |           |          |           |           |
     */
    FILE *fp;
    char table[LINESIZE];
    int filesizecheck=false;

    if(FSM){
        /* The FSM has already been setup-> don't create again.
         * This can happend for Dual camera
         */
        return FSM;
    }

    FSM=(FSMDB_t *)malloc(sizeof(FSMDB_t));
    if(!FSM){
        LOGE("Unable to setup the FSM\n");
        return NULL;
    }


    if (!(fp=(FILE *)fopen(fileName,"r"))){
        LOGE("Unable to open file\n");
        return NULL;
    }

    while(readlines(fp,(char *)table)){
        LOGH("The node is : %s\n",table);
        if(!setupFSMDB(FSM,table)){
            LOGE("Failed to setup FSMDB from the config file\n");
            free(FSM);
            FSM=NULL;
            break;
        }
        filesizecheck=true;
        // TODO - error handling - file wrong errors
    }
    if(!filesizecheck) {
        LOGE("Failed to setup FSMDB from the config file\n");
        free(FSM);
        FSM=NULL;
    }
    fclose(fp);
    return FSM;
}

int32_t predictFSM(FSMDB_t *FSM,void *param1, void *param2,int32_t session){
    static meta2_t metaData={{0,0},{0,0},{0,0},{0,0},0,0,0};
    static int32_t perfLevel[2]={0,0};
    int32_t totalPerfLevel=0,prevPerfLevel;
    cam_perf_info_t *meta=(cam_perf_info_t *)param2;
    // returning value 0 means no-throttling

    // IMP: Not tracking no of sessions.
    if(!FSM){
        LOGE("Unable to predict as NULL FSM passed\n");
        return 0;
    }
    prevPerfLevel=perfLevel[0]+perfLevel[1];
    // 1. struct1 & struct2 = NULL indicates session_no is getting closed
    if ((NULL == (int32_t *)param1) || (NULL == (int32_t *)param2)) {
        // Either structs is NULL, then the session_no is de-activated
        perfLevel[session]=0;
    } else if( (meta->sensorW * meta->sensorH) == 0) {
        // invalid case if either sensorW or sensorH is 0
        // sensor values should always be updated
        perfLevel[session]=0;
    } else {
    // 2. Based on Session No compute the perf level
        metaData.sensorW[session]=meta->sensorW;
        metaData.sensorH[session]=meta->sensorH;
        metaData.sensorClk[session]=meta->sensorClk;
        metaData.sensorMP[session]=((meta->sensorW)*(meta->sensorH))/(1024*1024);
        metaData.hfr=meta->hfr;
        metaData.fd=meta->fd;
        metaData.tnr=meta->tnr;
        perfLevel[session]=runFSM(FSM,0,param1,&metaData,session,8);
    }
    // 3. Add the perf levels everytime for all sessions
    totalPerfLevel=perfLevel[0]+perfLevel[1];

    totalPerfLevel=(totalPerfLevel>FSM_MAX_LEVEL?FSM_MAX_LEVEL:totalPerfLevel);
    // Return the total perf level based on no of sessions
    LOGH("The predicted value from FSM : %d\n",totalPerfLevel);

    if (prevPerfLevel > totalPerfLevel) {
        // First send 0 then totalPerfLevel
        LOGH("Prev Level greater than New Level\n");
    } else if (prevPerfLevel == totalPerfLevel) {
        // no need to send
        LOGH("Prev Level same as New Level\n");
    } else {
        // send
        LOGH("Prev Level smaller than New Level\n");
    }

    return totalPerfLevel;
}

void closeFSM(){
    if (FSM) {
        free(FSM);
        FSM = NULL;
    }
}
