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

// System dependencies
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <linux/msm_ion.h>
#define MMAN_H <SYSTEM_HEADER_PREFIX/mman.h>
#include MMAN_H

// JPEG dependencies
#include "mm_jpeg_ionbuf.h"
#if TARGET_ION_ABI_VERSION >= 2
#include <ion/ion.h>
#include <linux/dma-buf.h>
#ifndef CAM_CACHE_OPS
#define CAM_CACHE_OPS
enum {
    CAM_CLEAN_CACHE,
    CAM_INV_CACHE,
    CAM_CLEAN_INV_CACHE
};
#define ION_IOC_CLEAN_CACHES CAM_CLEAN_CACHE
#define ION_IOC_INV_CACHES CAM_INV_CACHE
#define ION_IOC_CLEAN_INV_CACHES CAM_CLEAN_INV_CACHE
#endif //CAM_CACHE_OPS
#endif //TARGET_ION_ABI_VERSION

/** buffer_allocate:
 *
 *  Arguments:
 *     @p_buffer: ION buffer
 *
 *  Return:
 *     buffer address
 *
 *  Description:
 *      allocates ION buffer
 *
 **/
void *buffer_allocate(buffer_t *p_buffer, int cached)
{
  void *l_buffer = NULL;

  int lrc = 0;

   p_buffer->alloc.len = p_buffer->size;
   p_buffer->alloc.align = 4096;
   p_buffer->alloc.flags = (cached) ? ION_FLAG_CACHED : 0;
#ifndef TARGET_ION_ABI_VERSION
  struct ion_handle_data lhandle_data;
   p_buffer->alloc.heap_id_mask = 0x1 << ION_IOMMU_HEAP_ID;
#else
   p_buffer->alloc.heap_id_mask = 0x1 << ION_SYSTEM_HEAP_ID;
#endif //TARGET_ION_ABI_VERSION

#ifndef TARGET_ION_ABI_VERSION
   p_buffer->ion_fd = open("/dev/ion", O_RDONLY);
#else
   p_buffer->ion_fd = ion_open();
#endif //TARGET_ION_ABI_VERSION
   if(p_buffer->ion_fd < 0) {
    LOGE("Ion open failed");
    goto ION_ALLOC_FAILED;
  }

  /* Make it page size aligned */
  p_buffer->alloc.len = (p_buffer->alloc.len + 4095U) & (~4095U);
#ifndef TARGET_ION_ABI_VERSION
  lrc = ioctl(p_buffer->ion_fd, ION_IOC_ALLOC, &p_buffer->alloc);
#else
  lrc = ion_alloc_fd(p_buffer->ion_fd, p_buffer->alloc.len, p_buffer->alloc.align,
                 p_buffer->alloc.heap_id_mask, p_buffer->alloc.flags,
                 &p_buffer->ion_info_fd.fd);
#endif //TARGET_ION_ABI_VERSION
  if (lrc < 0) {
    LOGE("ION allocation failed len %zu",
      p_buffer->alloc.len);
    goto ION_ALLOC_FAILED;
  }

  p_buffer->ion_info_fd.handle = p_buffer->alloc.handle;
#ifndef TARGET_ION_ABI_VERSION
  lrc = ioctl(p_buffer->ion_fd, ION_IOC_SHARE,
    &p_buffer->ion_info_fd);
  if (lrc < 0) {
    LOGE("ION map failed %s", strerror(errno));
    goto ION_MAP_FAILED;
  }
#else
  p_buffer->ion_info_fd.handle = p_buffer->ion_info_fd.fd;
#endif //TARGET_ION_ABI_VERSION

  p_buffer->p_pmem_fd = p_buffer->ion_info_fd.fd;

  l_buffer = mmap(NULL, p_buffer->alloc.len, PROT_READ  | PROT_WRITE,
    MAP_SHARED,p_buffer->p_pmem_fd, 0);

  if (l_buffer == MAP_FAILED) {
    LOGE("ION_MMAP_FAILED: %s (%d)",
      strerror(errno), errno);
    goto ION_MAP_FAILED;
  }

  return l_buffer;

ION_MAP_FAILED:
#ifndef TARGET_ION_ABI_VERSION
  lhandle_data.handle = p_buffer->ion_info_fd.handle;
  ioctl(p_buffer->ion_fd, ION_IOC_FREE, &lhandle_data);
#else
  close(p_buffer->ion_info_fd.fd);
  ion_close(p_buffer->ion_fd);
#endif //TARGET_ION_ABI_VERSION
  return NULL;
ION_ALLOC_FAILED:
if (p_buffer->ion_fd > 0) {
#ifndef TARGET_ION_ABI_VERSION
  close(p_buffer->ion_fd);
#else
  ion_close(p_buffer->ion_fd);
#endif //TARGET_ION_ABI_VERSION
}
  return NULL;

}

/** buffer_deallocate:
 *
 *  Arguments:
 *     @p_buffer: ION buffer
 *
 *  Return:
 *     buffer address
 *
 *  Description:
 *      deallocates ION buffer
 *
 **/
int buffer_deallocate(buffer_t *p_buffer)
{
  int lrc = 0;
  size_t lsize = (p_buffer->size + 4095U) & (~4095U);

  struct ion_handle_data lhandle_data;

  lrc = munmap(p_buffer->addr, lsize);

  lhandle_data.handle = p_buffer->ion_info_fd.handle;
#ifndef TARGET_ION_ABI_VERSION
  close(p_buffer->ion_info_fd.fd);
  ioctl(p_buffer->ion_fd, ION_IOC_FREE, &lhandle_data);
  close(p_buffer->ion_fd);
#else
  close(lhandle_data.handle);
  ion_close(p_buffer->ion_fd);
#endif //TARGET_ION_ABI_VERSION

  return lrc;
}

/** buffer_invalidate:
 *
 *  Arguments:
 *     @p_buffer: ION buffer
 *
 *  Return:
 *     error val
 *
 *  Description:
 *      Invalidates the cached buffer
 *
 **/
int buffer_invalidate(buffer_t *p_buffer)
{
  return buffer_cache_ops(p_buffer, ION_IOC_INV_CACHES);
}

/** buffer_clean:
 *
 *  Arguments:
 *     @p_buffer: ION buffer
 *
 *  Return:
 *     error val
 *
 *  Description:
 *      Clean the cached buffer
 *
 **/
int buffer_clean(buffer_t *p_buffer)
{
  return buffer_cache_ops(p_buffer, ION_IOC_CLEAN_CACHES);
}

int buffer_cache_ops(buffer_t *p_buffer, uint32_t cmd)
{
  int lrc = 0;
#ifndef TARGET_ION_ABI_VERSION
  struct ion_flush_data cache_clean_data;
  struct ion_custom_data custom_data;

  memset(&cache_clean_data, 0, sizeof(cache_clean_data));
  memset(&custom_data, 0, sizeof(custom_data));
  cache_clean_data.vaddr = p_buffer->addr;
  cache_clean_data.fd = p_buffer->ion_info_fd.fd;
  cache_clean_data.handle = p_buffer->ion_info_fd.handle;
  cache_clean_data.length = (unsigned int)p_buffer->size;
  custom_data.cmd = (unsigned int)cmd;
  custom_data.arg = (unsigned long)&cache_clean_data;

  lrc = ioctl(p_buffer->ion_fd, ION_IOC_CUSTOM, &custom_data);
  if (lrc < 0)
    LOGW("Cache clean failed: %s\n", strerror(errno));
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

    lrc = ioctl(p_buffer->ion_info_fd.fd, DMA_BUF_IOCTL_SYNC, &buf_sync_start.flags);
    if (lrc) {
        LOGE("Failed first DMA_BUF_IOCTL_SYNC start\n");
    }
    lrc = ioctl(p_buffer->ion_info_fd.fd, DMA_BUF_IOCTL_SYNC, &buf_sync_end.flags);
    if (lrc) {
        LOGE("Failed first DMA_BUF_IOCTL_SYNC End\n");
    }
#endif //TARGET_ION_ABI_VERSION
    return lrc;
}
