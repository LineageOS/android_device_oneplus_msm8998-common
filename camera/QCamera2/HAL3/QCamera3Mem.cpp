/* Copyright (c) 2012-2019, The Linux Foundation. All rights reserved.
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

#define LOG_TAG "QCameraHWI_Mem"

// System dependencies
#include <fcntl.h>
#define MMAN_H <SYSTEM_HEADER_PREFIX/mman.h>
#include MMAN_H
#include "gralloc_priv.h"

// Display dependencies
#include "qdMetaData.h"

// Camera dependencies
#include "QCamera3HWI.h"
#include "QCamera3Mem.h"
#include "QCameraTrace.h"

#include "fdleak.h"

extern "C" {
#include "mm_camera_dbg.h"
#include "mm_camera_interface.h"
}

using namespace android;

namespace qcamera {

// QCaemra2Memory base class

/*===========================================================================
 * FUNCTION   : QCamera3Memory
 *
 * DESCRIPTION: default constructor of QCamera3Memory
 *
 * PARAMETERS : none
 *
 * RETURN     : None
 *==========================================================================*/
QCamera3Memory::QCamera3Memory()
{
    mBufferCount = 0;
    for (int i = 0; i < MM_CAMERA_MAX_NUM_FRAMES; i++) {
        mMemInfo[i].fd = -1;
        mMemInfo[i].main_ion_fd = -1;
        mMemInfo[i].handle = 0;
        mMemInfo[i].size = 0;
        mCurrentFrameNumbers[i] = -1;
    }
}

/*===========================================================================
 * FUNCTION   : ~QCamera3Memory
 *
 * DESCRIPTION: deconstructor of QCamera3Memory
 *
 * PARAMETERS : none
 *
 * RETURN     : None
 *==========================================================================*/
QCamera3Memory::~QCamera3Memory()
{
}

/*===========================================================================
 * FUNCTION   : cacheOpsInternal
 *
 * DESCRIPTION: ion related memory cache operations
 *
 * PARAMETERS :
 *   @index   : index of the buffer
 *   @cmd     : cache ops command
 *   @vaddr   : ptr to the virtual address
 *
 * RETURN     : int32_t type of status
 *              NO_ERROR  -- success
 *              none-zero failure code
 *==========================================================================*/
int QCamera3Memory::cacheOpsInternal(uint32_t index, unsigned int cmd, void *vaddr)
{
    ATRACE_CALL();
    Mutex::Autolock lock(mLock);

    int ret = OK;
#ifndef TARGET_ION_ABI_VERSION
    struct ion_flush_data cache_inv_data;
    struct ion_custom_data custom_data;

    if (MM_CAMERA_MAX_NUM_FRAMES <= index) {
        LOGE("index %d out of bound [0, %d)",
                 index, MM_CAMERA_MAX_NUM_FRAMES);
        return BAD_INDEX;
    }

    if (0 == mMemInfo[index].handle) {
        LOGE("Buffer at %d not registered", index);
        return BAD_INDEX;
    }

    memset(&cache_inv_data, 0, sizeof(cache_inv_data));
    memset(&custom_data, 0, sizeof(custom_data));
    cache_inv_data.vaddr = vaddr;
    cache_inv_data.fd = mMemInfo[index].fd;
    cache_inv_data.handle = mMemInfo[index].handle;
    cache_inv_data.length = (unsigned int)mMemInfo[index].size;
    custom_data.cmd = cmd;
    custom_data.arg = (unsigned long)&cache_inv_data;

    LOGD("addr = %p, fd = %d, handle = %lx length = %d, ION Fd = %d",
          cache_inv_data.vaddr, cache_inv_data.fd,
         (unsigned long)cache_inv_data.handle, cache_inv_data.length,
         mMemInfo[index].main_ion_fd);
    ret = ioctl(mMemInfo[index].main_ion_fd, ION_IOC_CUSTOM, &custom_data);
    if (ret < 0)
        LOGE("Cache Invalidate failed: %s\n", strerror(errno));
#else
  struct dma_buf_sync buf_sync_start;
  struct dma_buf_sync buf_sync_end;

  /* ION_IOC_CLEAN_CACHES-->call the DMA_BUF_IOCTL_SYNC IOCTL with flags DMA_BUF_SYNC_START
     and DMA_BUF_SYNC_WRITE and then call the DMA_BUF_IOCTL_SYNC IOCTL with flags DMA_BUF_SYNC_END
     and DMA_BUF_SYNC_WRITE
     ION_IOC_INV_CACHES-->call the DMA_BUF_IOCTL_SYNC IOCT with flags DMA_BUF_SYNC_START and
     DMA_BUF_SYNC_WRITE and then call the DMA_BUF_IOCTL_SYNC IOCT with flags DMA_BUF_SYNC_END
     and DMA_BUF_SYNC_READ
  */

  switch (cmd) {
  case CAM_INV_CACHE:
    buf_sync_start.flags = DMA_BUF_SYNC_START | DMA_BUF_SYNC_WRITE;
    buf_sync_end.flags   = DMA_BUF_SYNC_END   | DMA_BUF_SYNC_READ;
    break;
  case CAM_CLEAN_CACHE:
    buf_sync_start.flags = DMA_BUF_SYNC_START | DMA_BUF_SYNC_WRITE;
    buf_sync_end.flags   = DMA_BUF_SYNC_END   | DMA_BUF_SYNC_WRITE;
    break;
  default:
  case CAM_CLEAN_INV_CACHE:
    buf_sync_start.flags = DMA_BUF_SYNC_START | DMA_BUF_SYNC_RW;
    buf_sync_end.flags   = DMA_BUF_SYNC_END   | DMA_BUF_SYNC_RW;
    break;
  }

  ret = ioctl(mMemInfo[index].fd, DMA_BUF_IOCTL_SYNC, &buf_sync_start.flags);
  if (ret) {
      LOGE("Failed first DMA_BUF_IOCTL_SYNC start\n");
  }
  ret = ioctl(mMemInfo[index].fd, DMA_BUF_IOCTL_SYNC, &buf_sync_end.flags);
  if (ret) {
      LOGE("Failed first DMA_BUF_IOCTL_SYNC End\n");
  }
  (void) vaddr;
#endif //TARGET_ION_ABI_VERSION
    return ret;
}

/*===========================================================================
 * FUNCTION   : getFd
 *
 * DESCRIPTION: return file descriptor of the indexed buffer
 *
 * PARAMETERS :
 *   @index   : index of the buffer
 *
 * RETURN     : file descriptor
 *==========================================================================*/
int QCamera3Memory::getFd(uint32_t index)
{
    Mutex::Autolock lock(mLock);

    if (MM_CAMERA_MAX_NUM_FRAMES <= index) {
        return BAD_INDEX;
    }

    if (0 == mMemInfo[index].handle) {
        return BAD_INDEX;
    }

    return mMemInfo[index].fd;
}

/*===========================================================================
 * FUNCTION   : getSize
 *
 * DESCRIPTION: return buffer size of the indexed buffer
 *
 * PARAMETERS :
 *   @index   : index of the buffer
 *
 * RETURN     : buffer size
 *==========================================================================*/
ssize_t QCamera3Memory::getSize(uint32_t index)
{
    Mutex::Autolock lock(mLock);

    if (MM_CAMERA_MAX_NUM_FRAMES <= index) {
        return BAD_INDEX;
    }

    if (0 == mMemInfo[index].handle) {
        return BAD_INDEX;
    }

    return (ssize_t)mMemInfo[index].size;
}

/*===========================================================================
 * FUNCTION   : getCnt
 *
 * DESCRIPTION: query number of buffers allocated
 *
 * PARAMETERS : none
 *
 * RETURN     : number of buffers allocated
 *==========================================================================*/
uint32_t QCamera3Memory::getCnt()
{
    Mutex::Autolock lock(mLock);

    return mBufferCount;
}

/*===========================================================================
 * FUNCTION   : getBufDef
 *
 * DESCRIPTION: query detailed buffer information
 *
 * PARAMETERS :
 *   @offset  : [input] frame buffer offset
 *   @bufDef  : [output] reference to struct to store buffer definition
 *   @index   : [input] index of the buffer
 *
 * RETURN     : int32_t type of status
 *              NO_ERROR  -- success
 *              none-zero failure code
 *==========================================================================*/
int32_t QCamera3Memory::getBufDef(const cam_frame_len_offset_t &offset,
        mm_camera_buf_def_t &bufDef, uint32_t index)
{
    Mutex::Autolock lock(mLock);

    if (!mBufferCount) {
        LOGE("Memory not allocated");
        return NO_INIT;
    }

    bufDef.fd = mMemInfo[index].fd;
    bufDef.frame_len = mMemInfo[index].size;
    bufDef.mem_info = (void *)this;
    bufDef.buffer = getPtrLocked(index);
    bufDef.planes_buf.num_planes = (int8_t)offset.num_planes;
    bufDef.buf_idx = (uint8_t)index;

    /* Plane 0 needs to be set separately. Set other planes in a loop */
    bufDef.planes_buf.planes[0].length = offset.mp[0].len;
    bufDef.planes_buf.planes[0].m.userptr = (long unsigned int)mMemInfo[index].fd;
    bufDef.planes_buf.planes[0].data_offset = offset.mp[0].offset;
    bufDef.planes_buf.planes[0].reserved[0] = 0;
    for (int i = 1; i < bufDef.planes_buf.num_planes; i++) {
         bufDef.planes_buf.planes[i].length = offset.mp[i].len;
         bufDef.planes_buf.planes[i].m.userptr = (long unsigned int)mMemInfo[i].fd;
         bufDef.planes_buf.planes[i].data_offset = offset.mp[i].offset;
         bufDef.planes_buf.planes[i].reserved[0] =
                 bufDef.planes_buf.planes[i-1].reserved[0] +
                 bufDef.planes_buf.planes[i-1].length;
    }

    return NO_ERROR;
}

/*===========================================================================
 * FUNCTION   : QCamera3HeapMemory
 *
 * DESCRIPTION: constructor of QCamera3HeapMemory for ion memory used internally in HAL
 *
 * PARAMETERS : none
 *
 * RETURN     : none
 *==========================================================================*/
QCamera3HeapMemory::QCamera3HeapMemory(uint32_t maxCnt, bool isSecure)
    : QCamera3Memory()
{
    mMaxCnt = MIN(maxCnt, MM_CAMERA_MAX_NUM_FRAMES);
    for (uint32_t i = 0; i < mMaxCnt; i ++)
        mPtr[i] = NULL;
    m_bIsSecureMode = isSecure;
}

/*===========================================================================
 * FUNCTION   : ~QCamera3HeapMemory
 *
 * DESCRIPTION: deconstructor of QCamera3HeapMemory
 *
 * PARAMETERS : none
 *
 * RETURN     : none
 *==========================================================================*/
QCamera3HeapMemory::~QCamera3HeapMemory()
{
}

/*===========================================================================
 * FUNCTION   : allocOneBuffer
 *
 * DESCRIPTION: impl of allocating one buffers of certain size
 *
 * PARAMETERS :
 *   @memInfo : [output] reference to struct to store additional memory allocation info
 *   @heap    : [input] heap id to indicate where the buffers will be allocated from
 *   @size    : [input] lenght of the buffer to be allocated
 *
 * RETURN     : int32_t type of status
 *              NO_ERROR  -- success
 *              none-zero failure code
 *==========================================================================*/
int QCamera3HeapMemory::allocOneBuffer(QCamera3MemInfo &memInfo,
        unsigned int heap_id, size_t size)
{
    int rc = OK;
    struct ion_handle_data handle_data;
    struct ion_allocation_data allocData;
    struct ion_fd_data ion_info_fd;
    int main_ion_fd = -1;

#ifndef TARGET_ION_ABI_VERSION
    main_ion_fd = open("/dev/ion", O_RDONLY);
#else
    main_ion_fd = ion_open();
#endif
    if (main_ion_fd < 0) {
        LOGE("Ion dev open failed: %s\n", strerror(errno));
        goto ION_OPEN_FAILED;
    }

    memset(&ion_info_fd, 0, sizeof(ion_info_fd));
    memset(&allocData, 0, sizeof(allocData));
    allocData.len = size;
    /* to make it page size aligned */
    allocData.len = (allocData.len + 4095U) & (~4095U);
    allocData.align = 4096;
    allocData.heap_id_mask = heap_id;
    allocData.flags = ION_FLAG_CACHED;

    if (m_bIsSecureMode) {
        LOGD("Allocate secure buffer\n");
        if (QCameraCommon::is_target_SDM450())
            allocData.heap_id_mask = ION_HEAP(ION_CP_MM_HEAP_ID);
        else
            allocData.heap_id_mask = ION_HEAP(ION_SECURE_DISPLAY_HEAP_ID);
        allocData.flags = ION_FLAG_SECURE | ION_FLAG_CP_CAMERA;
        allocData.align = 2097152; // 2 MiB alignment to be able to protect later
        allocData.len = (allocData.len + 2097152U) & (~2097152U);
    }

#ifndef TARGET_ION_ABI_VERSION
    rc = ioctl(main_ion_fd, ION_IOC_ALLOC, &allocData);
#else
    rc = ion_alloc_fd(main_ion_fd, allocData.len, allocData.align, allocData.heap_id_mask,
              allocData.flags, &ion_info_fd.fd);
#endif //TARGET_ION_ABI_VERSION
    if (rc < 0) {
        LOGE("ION allocation for len %d failed: %s\n", allocData.len,
            strerror(errno));
        goto ION_ALLOC_FAILED;
    }

#ifndef TARGET_ION_ABI_VERSION
    ion_info_fd.handle = allocData.handle;
    rc = ioctl(main_ion_fd, ION_IOC_SHARE, &ion_info_fd);
#else
    ion_info_fd.handle = ion_info_fd.fd;
#endif //TARGET_ION_ABI_VERSION
    if (rc < 0) {
        LOGE("ION map failed %s\n", strerror(errno));
        goto ION_MAP_FAILED;
    }

    memInfo.main_ion_fd = main_ion_fd;
    memInfo.fd = ion_info_fd.fd;
    memInfo.handle = ion_info_fd.handle;
    memInfo.size = allocData.len;
    return OK;

ION_MAP_FAILED:
    memset(&handle_data, 0, sizeof(handle_data));
    handle_data.handle = ion_info_fd.handle;
#ifndef TARGET_ION_ABI_VERSION
    ioctl(main_ion_fd, ION_IOC_FREE, &handle_data);
#else
    close(ion_info_fd.fd);
#endif //TARGET_ION_ABI_VERSION
ION_ALLOC_FAILED:
#ifndef TARGET_ION_ABI_VERSION
    close(main_ion_fd);
#else
    ion_close(main_ion_fd);
#endif //TARGET_ION_ABI_VERSION
ION_OPEN_FAILED:
    return NO_MEMORY;
}

/*===========================================================================
 * FUNCTION   : deallocOneBuffer
 *
 * DESCRIPTION: impl of deallocating one buffers
 *
 * PARAMETERS :
 *   @memInfo : reference to struct that stores additional memory allocation info
 *
 * RETURN     : none
 *==========================================================================*/
void QCamera3HeapMemory::deallocOneBuffer(QCamera3MemInfo &memInfo)
{
    struct ion_handle_data handle_data;

    if (memInfo.fd >= 0) {
        close(memInfo.fd);
        memInfo.fd = -1;
    }

    if (memInfo.main_ion_fd >= 0) {
        memset(&handle_data, 0, sizeof(handle_data));
        handle_data.handle = memInfo.handle;
#ifndef TARGET_ION_ABI_VERSION
        ioctl(memInfo.main_ion_fd, ION_IOC_FREE, &handle_data);
        close(memInfo.main_ion_fd);
#else
       ion_close(memInfo.main_ion_fd);
#endif //TARGET_ION_ABI_VERSION
        memInfo.main_ion_fd = -1;
    }
    memInfo.handle = 0;
    memInfo.size = 0;
}

/*===========================================================================
 * FUNCTION   : getPtrLocked
 *
 * DESCRIPTION: Return buffer pointer.
 *
 * PARAMETERS :
 *   @index   : index of the buffer
 *
 * RETURN     : buffer ptr
 *==========================================================================*/
void *QCamera3HeapMemory::getPtrLocked(uint32_t index)
{
    if (index >= mBufferCount) {
        LOGE("index out of bound");
        return NULL;
    }
    return mPtr[index];
}

/*===========================================================================
 * FUNCTION   : markFrameNumber
 *
 * DESCRIPTION: We use this function from the request call path to mark the
 *              buffers with the frame number they are intended for this info
 *              is used later when giving out callback & it is duty of PP to
 *              ensure that data for that particular frameNumber/Request is
 *              written to this buffer.
 * PARAMETERS :
 *   @index   : index of the buffer
 *   @frame#  : Frame number from the framework
 *
 * RETURN     : int32_t type of status
 *              NO_ERROR  -- success
 *              none-zero failure code
 *==========================================================================*/
int32_t QCamera3HeapMemory::markFrameNumber(uint32_t index, uint32_t frameNumber)
{
    Mutex::Autolock lock(mLock);

    if (index >= mBufferCount) {
        LOGE("Index %d out of bounds, current buffer count is %d",
                 index, mBufferCount);
        return BAD_INDEX;
    }

    if (0 == mMemInfo[index].handle) {
        LOGE("Buffer at %d not allocated", index);
        return BAD_INDEX;
    }

    mCurrentFrameNumbers[index] = (int32_t)frameNumber;

    return NO_ERROR;
}


/*===========================================================================
 * FUNCTION   : getFrameNumber
 *
 * DESCRIPTION: We use this to fetch the frameNumber for the request with which
 *              this buffer was given to HAL
 *
 *
 * PARAMETERS :
 *   @index   : index of the buffer
 *
 * RETURN     : int32_t frameNumber
 *              positive/zero  -- success
 *              negative failure
 *==========================================================================*/
int32_t QCamera3HeapMemory::getFrameNumber(uint32_t index)
{
    Mutex::Autolock lock(mLock);

    if (index >= mBufferCount) {
        LOGE("Index %d out of bounds, current buffer count is %d",
                 index, mBufferCount);
        return -1;
    }

    if (0 == mMemInfo[index].handle) {
        LOGE("Buffer at %d not registered", index);
        return -1;
    }

    return mCurrentFrameNumbers[index];
}


/*===========================================================================
 * FUNCTION   : getOldestFrameNumber
 *
 * DESCRIPTION: We use this to fetch the oldest frame number expected per FIFO
 *
 *
 * PARAMETERS :
 *
 *
 * RETURN     : int32_t frameNumber
 *              negative failure
 *==========================================================================*/
int32_t QCamera3HeapMemory::getOldestFrameNumber(uint32_t &bufIndex)
{
    Mutex::Autolock lock(mLock);

    int32_t oldest = INT_MAX;
    bool empty = true;

    for (uint32_t index = 0;
            index < mBufferCount; index++) {
        if (mMemInfo[index].handle) {
            if ((empty) || (!empty && oldest > mCurrentFrameNumbers[index]
                && mCurrentFrameNumbers[index] != -1)) {
                oldest = mCurrentFrameNumbers[index];
                bufIndex = index;
            }
            empty = false;
        }
    }
    if (empty)
        return -1;
    else
        return oldest;
}


/*===========================================================================
 * FUNCTION   : getBufferIndex
 *
 * DESCRIPTION: We use this to fetch the buffer index for the request with
 *              a particular frame number
 *
 *
 * PARAMETERS :
 *   @frameNumber  : frame number of the buffer
 *
 * RETURN     : int32_t buffer index
 *              negative failure
 *==========================================================================*/
int32_t QCamera3HeapMemory::getBufferIndex(uint32_t frameNumber)
{
    Mutex::Autolock lock(mLock);

    for (uint32_t index = 0;
            index < mBufferCount; index++) {
        if (mMemInfo[index].handle &&
                mCurrentFrameNumbers[index] == (int32_t)frameNumber)
            return (int32_t)index;
    }
    return -1;
}

/*===========================================================================
 * FUNCTION   : getPtr
 *
 * DESCRIPTION: Return buffer pointer
 *
 * PARAMETERS :
 *   @index   : index of the buffer
 *
 * RETURN     : buffer ptr
 *==========================================================================*/
void *QCamera3HeapMemory::getPtr(uint32_t index)
{
    return getPtrLocked(index);
}

/*===========================================================================
 * FUNCTION   : allocate
 *
 * DESCRIPTION: allocate requested number of buffers of certain size
 *
 * PARAMETERS :
 *   @size    : lenght of the buffer to be allocated
 *
 * RETURN     : int32_t type of status
 *              NO_ERROR  -- success
 *              none-zero failure code
 *==========================================================================*/
int QCamera3HeapMemory::allocate(size_t size)
{
#ifndef TARGET_ION_ABI_VERSION
    unsigned int heap_id_mask = 0x1 << ION_IOMMU_HEAP_ID;
#else
    uint32_t heap_id_mask = 0x1 << ION_SYSTEM_HEAP_ID;
#endif // TARGET_ION_ABI_VERSION
    uint32_t i;
    int rc = NO_ERROR;

    //Note that now we allow incremental allocation. In other words, we allow
    //multiple alloc being called as long as the sum of count does not exceed
    //mMaxCnt.
    if (mBufferCount > 0) {
        LOGE("There is already buffer allocated.");
        return BAD_INDEX;
    }

    for (i = 0; i < mMaxCnt; i ++) {
        rc = allocOneBuffer(mMemInfo[i], heap_id_mask, size);
        if (rc < 0) {
            LOGE("AllocateIonMemory failed");
            goto ALLOC_FAILED;
        }

        if (m_bIsSecureMode) {
            continue;
        }
        void *vaddr = mmap(NULL,
                    mMemInfo[i].size,
                    PROT_READ | PROT_WRITE,
                    MAP_SHARED,
                    mMemInfo[i].fd, 0);
        if (vaddr == MAP_FAILED) {
            deallocOneBuffer(mMemInfo[i]);
            LOGE("mmap failed for buffer %d", i);
            goto ALLOC_FAILED;
        } else {
            mPtr[i] = vaddr;
        }
    }
    if (rc == 0)
        mBufferCount = mMaxCnt;

    return OK;

ALLOC_FAILED:
    for (uint32_t j = 0; j < i; j++) {
        if (mPtr[j] != NULL) {
            munmap(mPtr[j], mMemInfo[j].size);
            mPtr[j] = NULL;
        }
        deallocOneBuffer(mMemInfo[j]);
    }
    return NO_MEMORY;
}

/*===========================================================================
 * FUNCTION   : allocateOne
 *
 * DESCRIPTION: allocate one buffer
 *
 * PARAMETERS :
 *   @size    : lenght of the buffer to be allocated
 *
 * RETURN     : int32_t type of status
 *              NO_ERROR  -- success
 *              none-zero failure code
 *==========================================================================*/
int QCamera3HeapMemory::allocateOne(size_t size)
{
    void *vaddr = NULL;
#ifndef TARGET_ION_ABI_VERSION
    unsigned int heap_id_mask = 0x1 << ION_IOMMU_HEAP_ID;
#else
    uint32_t heap_id_mask = 0x1 << ION_SYSTEM_HEAP_ID;
#endif // TARGET_ION_ABI_VERSION
    int rc = NO_ERROR;

    //Note that now we allow incremental allocation. In other words, we allow
    //multiple alloc being called as long as the sum of count does not exceed
    //mMaxCnt.
    if (mBufferCount + 1 > mMaxCnt) {
        LOGE("Buffer count %d + 1 out of bound. Max is %d",
                mBufferCount, mMaxCnt);
        return BAD_INDEX;
    }

    rc = allocOneBuffer(mMemInfo[mBufferCount], heap_id_mask, size);
    if (rc < 0) {
        LOGE("AllocateIonMemory failed");
        return NO_MEMORY;
    }

    if (m_bIsSecureMode) {
        goto ALLOC_DONE;
    }
    vaddr = mmap(NULL,
                mMemInfo[mBufferCount].size,
                PROT_READ | PROT_WRITE,
                MAP_SHARED,
                mMemInfo[mBufferCount].fd, 0);
    if (vaddr == MAP_FAILED) {
        deallocOneBuffer(mMemInfo[mBufferCount]);
        LOGE("mmap failed for buffer");
        return NO_MEMORY;
    } else {
        mPtr[mBufferCount] = vaddr;
    }

ALLOC_DONE:
    if (rc == 0)
        mBufferCount += 1;

    return mBufferCount-1;
}

/*===========================================================================
 * FUNCTION   : deallocate
 *
 * DESCRIPTION: deallocate buffers
 *
 * PARAMETERS : none
 *
 * RETURN     : none
 *==========================================================================*/
void QCamera3HeapMemory::deallocate()
{
    for (uint32_t i = 0; i < mBufferCount; i++) {
        if (mPtr[i] != NULL) {
            munmap(mPtr[i], mMemInfo[i].size);
            mPtr[i] = NULL;
        }
        deallocOneBuffer(mMemInfo[i]);
        mCurrentFrameNumbers[i] = -1;
    }
    mBufferCount = 0;
}

/*===========================================================================
 * FUNCTION   : cacheOps
 *
 * DESCRIPTION: ion related memory cache operations
 *
 * PARAMETERS :
 *   @index   : index of the buffer
 *   @cmd     : cache ops command
 *
 * RETURN     : int32_t type of status
 *              NO_ERROR  -- success
 *              none-zero failure code
 *==========================================================================*/
int QCamera3HeapMemory::cacheOps(uint32_t index, unsigned int cmd)
{
    if (index >= mBufferCount)
        return BAD_INDEX;
    return cacheOpsInternal(index, cmd, mPtr[index]);
}

/*===========================================================================
 * FUNCTION   : getMatchBufIndex
 *
 * DESCRIPTION: query buffer index by object ptr
 *
 * PARAMETERS :
 *   @object  : object ptr
 *
 * RETURN     : buffer index if match found,
 *              -1 if failed
 *==========================================================================*/
int QCamera3HeapMemory::getMatchBufIndex(void * /*object*/)
{

/*
    TODO for HEAP memory type, would there be an equivalent requirement?

    int index = -1;
    buffer_handle_t *key = (buffer_handle_t*) object;
    if (!key) {
        return BAD_VALUE;
    }
    for (int i = 0; i < mBufferCount; i++) {
        if (mBufferHandle[i] == key) {
            index = i;
            break;
        }
    }
    return index;
*/
    LOGE("FATAL: Not supposed to come here");
    return -1;
}

/*===========================================================================
 * FUNCTION   : QCamera3GrallocMemory
 *
 * DESCRIPTION: constructor of QCamera3GrallocMemory
 *              preview stream buffers are allocated from gralloc native_windoe
 *
 * PARAMETERS :
 *   @startIdx : start index of array after which we can register buffers in.
 *
 * RETURN     : none
 *==========================================================================*/
QCamera3GrallocMemory::QCamera3GrallocMemory(uint32_t startIdx, bool isSecure)
        : QCamera3Memory(), mStartIdx(startIdx), mMasterCam(CAM_TYPE_MAIN)
{
    for (int i = 0; i < MM_CAMERA_MAX_NUM_FRAMES; i ++) {
        mBufferHandle[i] = NULL;
        mPrivateHandle[i] = NULL;
    }
    m_bIsSecureMode = isSecure;
}

/*===========================================================================
 * FUNCTION   : ~QCamera3GrallocMemory
 *
 * DESCRIPTION: deconstructor of QCamera3GrallocMemory
 *
 * PARAMETERS : none
 *
 * RETURN     : none
 *==========================================================================*/
QCamera3GrallocMemory::~QCamera3GrallocMemory()
{
}

/*===========================================================================
 * FUNCTION   : registerBuffer
 *
 * DESCRIPTION: registers frameworks-allocated gralloc buffer_handle_t
 *
 * PARAMETERS :
 *   @buffers : buffer_handle_t pointer
 *   @type :    cam_stream_type_t
 *
 * RETURN     : int32_t type of status
 *              NO_ERROR  -- success
 *              none-zero failure code
 *==========================================================================*/
int QCamera3GrallocMemory::registerBuffer(buffer_handle_t *buffer,
        __unused cam_stream_type_t type)
{
    status_t ret = NO_ERROR;
    struct ion_fd_data ion_info_fd;
    void *vaddr = NULL;
    int32_t colorSpace = ITU_R_601_FR;
    int32_t idx = -1;

    LOGD("E");

    memset(&ion_info_fd, 0, sizeof(ion_info_fd));

    if (0 <= getMatchBufIndex((void *) buffer)) {
        LOGL("Buffer already registered");
        return ALREADY_EXISTS;
    }

    Mutex::Autolock lock(mLock);
    if (mBufferCount >= (MM_CAMERA_MAX_NUM_FRAMES - mStartIdx)) {
        LOGE("Number of buffers %d greater than what's supported %d",
                 mBufferCount, MM_CAMERA_MAX_NUM_FRAMES - mStartIdx);
        return BAD_INDEX;
    }

    idx = getFreeIndexLocked();
    if (0 > idx) {
        LOGE("No available memory slots");
        return BAD_INDEX;
    }

    mBufferHandle[idx] = buffer;
    mPrivateHandle[idx] = (struct private_handle_t *)(*mBufferHandle[idx]);

    setMetaData(mPrivateHandle[idx], UPDATE_COLOR_SPACE, &colorSpace);

#ifndef TARGET_ION_ABI_VERSION
    mMemInfo[idx].main_ion_fd = open("/dev/ion", O_RDONLY);
#else
    mMemInfo[idx].main_ion_fd = ion_open();
#endif //TARGET_ION_ABI_VERSION
    if (mMemInfo[idx].main_ion_fd < 0) {
        LOGE("failed: could not open ion device");
        ret = NO_MEMORY;
        goto end;
    } else {
        ion_info_fd.fd = mPrivateHandle[idx]->fd;
#ifndef TARGET_ION_ABI_VERSION
        if (ioctl(mMemInfo[idx].main_ion_fd,
                  ION_IOC_IMPORT, &ion_info_fd) < 0) {
            LOGE("ION import failed\n");
            close(mMemInfo[idx].main_ion_fd);
            ret = NO_MEMORY;
            goto end;
        }
#endif //TARGET_ION_ABI_VERSION
    }
    LOGD("idx = %d, fd = %d, size = %d, offset = %d",
             idx, mPrivateHandle[idx]->fd,
            mPrivateHandle[idx]->size,
            mPrivateHandle[idx]->offset);
    mMemInfo[idx].fd = mPrivateHandle[idx]->fd;
    mMemInfo[idx].size =
            ( /* FIXME: Should update ION interface */ size_t)
            mPrivateHandle[idx]->size;
#ifndef TARGET_ION_ABI_VERSION
    mMemInfo[idx].handle = ion_info_fd.handle;
#else
    mMemInfo[idx].handle = ion_info_fd.fd;
#endif //TARGET_ION_ABI_VERSION

    mBufferCount++;

    if (m_bIsSecureMode) {
        goto end;
    }
    vaddr = mmap(NULL,
            mMemInfo[idx].size,
            PROT_READ | PROT_WRITE,
            MAP_SHARED,
            mMemInfo[idx].fd, 0);
    if (vaddr == MAP_FAILED) {
        LOGE("mmap failed");
        /* we have to close the main_ion_fd when mmap fails*/
        close(mMemInfo[idx].main_ion_fd);
        mMemInfo[idx].main_ion_fd = -1;
        mMemInfo[idx].handle = 0;
        ret = NO_MEMORY;
        goto end;
    } else {
        mPtr[idx] = vaddr;
    }
end:
    LOGD("X ");
    return ret;
}
/*===========================================================================
 * FUNCTION   : unregisterBufferLocked
 *
 * DESCRIPTION: Unregister buffer. Please note that this method has to be
 *              called with 'mLock' acquired.
 *
 * PARAMETERS :
 *   @idx     : unregister buffer at index 'idx'
 *
 * RETURN     : int32_t type of status
 *              NO_ERROR  -- success
 *              none-zero failure code
 *==========================================================================*/
int32_t QCamera3GrallocMemory::unregisterBufferLocked(size_t idx)
{
    if (mPtr[idx] != NULL) {
        munmap(mPtr[idx], mMemInfo[idx].size);
        mPtr[idx] = NULL;
    }
    /*This function is called to remove the fd from wrapper.
    This will not close the fd.*/
    remFdCheck(mMemInfo[idx].fd);

    struct ion_handle_data ion_handle;
    memset(&ion_handle, 0, sizeof(ion_handle));
    ion_handle.handle = mMemInfo[idx].handle;
#ifndef TARGET_ION_ABI_VERSION
    if (ioctl(mMemInfo[idx].main_ion_fd, ION_IOC_FREE, &ion_handle) < 0) {
        LOGE("ion free failed");
    }
    close(mMemInfo[idx].main_ion_fd);
#else
    ion_close(mMemInfo[idx].main_ion_fd);
#endif  // TARGET_ION_ABI_VERSION
    memset(&mMemInfo[idx], 0, sizeof(struct QCamera3MemInfo));
    mMemInfo[idx].main_ion_fd = -1;
    mBufferHandle[idx] = NULL;
    mPrivateHandle[idx] = NULL;
    mCurrentFrameNumbers[idx] = -1;
    mBufferCount--;

    return NO_ERROR;
}

/*===========================================================================
 * FUNCTION   : unregisterBuffer
 *
 * DESCRIPTION: unregister buffer
 *
 * PARAMETERS :
 *   @idx     : unregister buffer at index 'idx'
 *
 * RETURN     : int32_t type of status
 *              NO_ERROR  -- success
 *              none-zero failure code
 *==========================================================================*/
int32_t QCamera3GrallocMemory::unregisterBuffer(size_t idx)
{
    int32_t rc = NO_ERROR;
    Mutex::Autolock lock(mLock);

    LOGD("E ", __FUNCTION__);

    if (MM_CAMERA_MAX_NUM_FRAMES <= idx) {
        LOGE("Buffer index %d greater than what is supported %d",
                 idx, MM_CAMERA_MAX_NUM_FRAMES);
        return BAD_VALUE;
    }
    if (idx < mStartIdx) {
        LOGE("buffer index %d less than starting index %d",
                 idx, mStartIdx);
        return BAD_INDEX;
    }

    if (0 == mMemInfo[idx].handle) {
        LOGE("Trying to unregister buffer at %d which still not registered",
                 idx);
        return BAD_VALUE;
    }

    rc = unregisterBufferLocked(idx);

    LOGD("X ",__FUNCTION__);

    return rc;
}

/*===========================================================================
 * FUNCTION   : unregisterBuffers
 *
 * DESCRIPTION: unregister buffers
 *
 * PARAMETERS : none
 *
 * RETURN     : none
 *==========================================================================*/
void QCamera3GrallocMemory::unregisterBuffers()
{
    int err = NO_ERROR;
    Mutex::Autolock lock(mLock);

    LOGD("E ", __FUNCTION__);

    for (uint32_t cnt = mStartIdx; cnt < MM_CAMERA_MAX_NUM_FRAMES; cnt++) {
        if (0 == mMemInfo[cnt].handle) {
            continue;
        }
        err = unregisterBufferLocked(cnt);
        if (NO_ERROR != err) {
            LOGE("Error unregistering buffer %d error %d",
                     cnt, err);
        }
    }
    mBufferCount = 0;
    LOGD("X ",__FUNCTION__);
}

/*===========================================================================
 * FUNCTION   : markFrameNumber
 *
 * DESCRIPTION: We use this function from the request call path to mark the
 *              buffers with the frame number they are intended for this info
 *              is used later when giving out callback & it is duty of PP to
 *              ensure that data for that particular frameNumber/Request is
 *              written to this buffer.
 * PARAMETERS :
 *   @index   : index of the buffer
 *   @frame#  : Frame number from the framework
 *
 * RETURN     : int32_t type of status
 *              NO_ERROR  -- success
 *              none-zero failure code
 *==========================================================================*/
int32_t QCamera3GrallocMemory::markFrameNumber(uint32_t index, uint32_t frameNumber)
{
    Mutex::Autolock lock(mLock);

    if (index >= MM_CAMERA_MAX_NUM_FRAMES) {
        LOGE("Index out of bounds");
        return BAD_INDEX;
    }
    if (index < mStartIdx) {
        LOGE("buffer index %d less than starting index %d",
                 index, mStartIdx);
        return BAD_INDEX;
    }

    if (0 == mMemInfo[index].handle) {
        LOGE("Buffer at %d not registered", index);
        return BAD_INDEX;
    }

    mCurrentFrameNumbers[index] = (int32_t)frameNumber;

    return NO_ERROR;
}

/*===========================================================================
 * FUNCTION   : getFrameNumber
 *
 * DESCRIPTION: We use this to fetch the frameNumber for the request with which
 *              this buffer was given to HAL
 *
 *
 * PARAMETERS :
 *   @index   : index of the buffer
 *
 * RETURN     : int32_t frameNumber
 *              positive/zero  -- success
 *              negative failure
 *==========================================================================*/
int32_t QCamera3GrallocMemory::getFrameNumber(uint32_t index)
{
    Mutex::Autolock lock(mLock);

    if (index >= MM_CAMERA_MAX_NUM_FRAMES) {
        LOGE("Index out of bounds");
        return -1;
    }
    if (index < mStartIdx) {
        LOGE("buffer index %d less than starting index %d",
                 index, mStartIdx);
        return BAD_INDEX;
    }

    if (0 == mMemInfo[index].handle) {
        LOGE("Buffer at %d not registered", index);
        return -1;
    }

    return mCurrentFrameNumbers[index];
}

/*===========================================================================
 * FUNCTION   : getOldestFrameNumber
 *
 * DESCRIPTION: We use this to fetch the oldest frame number expected per FIFO
 *
 *
 * PARAMETERS :
 *
 *
 * RETURN     : int32_t frameNumber
 *              negative failure
 *==========================================================================*/
int32_t QCamera3GrallocMemory::getOldestFrameNumber(uint32_t &bufIndex)
{
    int32_t oldest = INT_MAX;
    bool empty = true;
    for (uint32_t index = mStartIdx;
            index < MM_CAMERA_MAX_NUM_FRAMES; index++) {
        if (mMemInfo[index].handle) {
            if ((empty) ||
                (!empty && oldest > mCurrentFrameNumbers[index]
                && mCurrentFrameNumbers[index] != -1)) {
                oldest = mCurrentFrameNumbers[index];
                bufIndex = index;
            }
            empty = false;
        }
    }
    if (empty)
        return -1;
    else
        return oldest;
}


/*===========================================================================
 * FUNCTION   : getBufferIndex
 *
 * DESCRIPTION: We use this to fetch the buffer index for the request with
 *              a particular frame number
 *
 *
 * PARAMETERS :
 *   @frameNumber  : frame number of the buffer
 *
 * RETURN     : int32_t buffer index
 *              negative failure
 *==========================================================================*/
int32_t QCamera3GrallocMemory::getBufferIndex(uint32_t frameNumber)
{
    for (uint32_t index = mStartIdx;
            index < MM_CAMERA_MAX_NUM_FRAMES; index++) {
        if (mMemInfo[index].handle &&
                mCurrentFrameNumbers[index] == (int32_t)frameNumber)
            return (int32_t)index;
    }
    return -1;
}

/*===========================================================================
 * FUNCTION   : cacheOps
 *
 * DESCRIPTION: ion related memory cache operations
 *
 * PARAMETERS :
 *   @index   : index of the buffer
 *   @cmd     : cache ops command
 *
 * RETURN     : int32_t type of status
 *              NO_ERROR  -- success
 *              none-zero failure code
 *==========================================================================*/
int QCamera3GrallocMemory::cacheOps(uint32_t index, unsigned int cmd)
{
    int rc = 0;
    bool needToInvalidate = false;
    struct private_handle_t *privateHandle = NULL;

    if (index >= MM_CAMERA_MAX_NUM_FRAMES) {
        LOGE("Index out of bounds");
        return -1;
    }
    if (index < mStartIdx) {
        LOGE("buffer index %d less than starting index %d",
                 index, mStartIdx);
        return BAD_INDEX;
    }

    privateHandle = mPrivateHandle[index];
    if (privateHandle != NULL){
        if(privateHandle->flags &
             (private_handle_t::PRIV_FLAGS_NON_CPU_WRITER)){
               needToInvalidate = true;
        }
    }
#ifndef TARGET_ION_ABI_VERSION
    LOGD("needToInvalidate %d buf idx %d", needToInvalidate, index);
    if(((cmd == ION_IOC_INV_CACHES) || (cmd == ION_IOC_CLEAN_INV_CACHES))
        && needToInvalidate) {
        rc = cacheOpsInternal(index, cmd, mPtr[index]);
    }
    else if(cmd == ION_IOC_CLEAN_CACHES) {
        rc = cacheOpsInternal(index, cmd, mPtr[index]);
    }
#else
    (void) cmd;
#endif //TARGET_ION_ABI_VERSION
    return rc;
}

/*===========================================================================
 * FUNCTION   : getMatchBufIndex
 *
 * DESCRIPTION: query buffer index by object ptr
 *
 * PARAMETERS :
 *   @opaque  : opaque ptr
 *
 * RETURN     : buffer index if match found,
 *              -1 if failed
 *==========================================================================*/
int QCamera3GrallocMemory::getMatchBufIndex(void *object)
{
    Mutex::Autolock lock(mLock);

    int index = -1;
    buffer_handle_t *key = (buffer_handle_t*) object;
    if (!key) {
        return BAD_VALUE;
    }
    for (uint32_t i = mStartIdx; i < MM_CAMERA_MAX_NUM_FRAMES; i++) {
        if (mBufferHandle[i] == key) {
            index = (int)i;
            break;
        }
    }

    return index;
}

/*===========================================================================
 * FUNCTION   : getFreeIndexLocked
 *
 * DESCRIPTION: Find free index slot. Note 'mLock' needs to be acquired
 *              before calling this method.
 *
 * PARAMETERS : None
 *
 * RETURN     : free buffer index if found,
 *              -1 if failed
 *==========================================================================*/
int QCamera3GrallocMemory::getFreeIndexLocked()
{
    int index = -1;
    //Main session : 0 (infact maxHeapBuffers) to MM_CAMERA_MAX_NUM_FRAMES/2 -1
    //Aux session  : MM_CAMERA_MAX_NUM_FRAMES/2 to MM_CAMERA_MAX_NUM_FRAMES -1
    uint32_t startIdx =
            (mMasterCam == CAM_TYPE_MAIN) ? mStartIdx : (mStartIdx + (MM_CAMERA_MAX_NUM_FRAMES/2));

    if (mBufferCount >= (MM_CAMERA_MAX_NUM_FRAMES)) {
        LOGE("Number of buffers %d greater than what's supported %d",
             mBufferCount, MM_CAMERA_MAX_NUM_FRAMES);
        return index;
    }

    for (size_t i = startIdx; i < MM_CAMERA_MAX_NUM_FRAMES; i++) {
        if (0 == mMemInfo[i].handle) {
            index = i;
            break;
        }
    }

    return index;
}

/*===========================================================================
 * FUNCTION   : getPtrLocked
 *
 * DESCRIPTION: Return buffer pointer. Please note 'mLock' must be acquired
 *              before calling this method.
 *
 * PARAMETERS :
 *   @index   : index of the buffer
 *
 * RETURN     : buffer ptr
 *==========================================================================*/
void *QCamera3GrallocMemory::getPtrLocked(uint32_t index)
{
    if (MM_CAMERA_MAX_NUM_FRAMES <= index) {
        LOGE("index %d out of bound [0, %d)",
                 index, MM_CAMERA_MAX_NUM_FRAMES);
        return NULL;
    }
    if (index < mStartIdx) {
        LOGE("buffer index %d less than starting index %d",
                 index, mStartIdx);
        return NULL;
    }


    if (0 == mMemInfo[index].handle) {
        LOGE("Buffer at %d not registered", index);
        return NULL;
    }

    return mPtr[index];
}

/*===========================================================================
 * FUNCTION   : getPtr
 *
 * DESCRIPTION: Return buffer pointer.
 *
 * PARAMETERS :
 *   @index   : index of the buffer
 *
 * RETURN     : buffer ptr
 *==========================================================================*/
void *QCamera3GrallocMemory::getPtr(uint32_t index)
{
    Mutex::Autolock lock(mLock);
    return getPtrLocked(index);
}

/*===========================================================================
 * FUNCTION   : getBufferHandle
 *
 * DESCRIPTION: return framework pointer
 *
 * PARAMETERS :
 *   @index   : index of the buffer
 *
 * RETURN     : buffer ptr if match found
                NULL if failed
 *==========================================================================*/
void *QCamera3GrallocMemory::getBufferHandle(uint32_t index)
{
    Mutex::Autolock lock(mLock);

    if (MM_CAMERA_MAX_NUM_FRAMES <= index) {
        LOGE("index %d out of bound [0, %d)",
                 index, MM_CAMERA_MAX_NUM_FRAMES);
        return NULL;
    }
    if (index < mStartIdx) {
        LOGE("buffer index %d less than starting index %d",
                 index, mStartIdx);
        return NULL;
    }

    if (0 == mMemInfo[index].handle) {
        LOGE("Buffer at %d not registered", index);
        return NULL;
    }

    return mBufferHandle[index];
}

void QCamera3GrallocMemory::switchMaster(uint32_t masterCam)
{
    mMasterCam = masterCam;
}

}; //namespace qcamera
