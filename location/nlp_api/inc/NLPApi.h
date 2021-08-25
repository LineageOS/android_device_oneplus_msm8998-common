/* Copyright (c) 2020 The Linux Foundation. All rights reserved.
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
 */

#ifndef NLP_API_H
#define NLP_API_H

#include <dlfcn.h>
#include <stddef.h>
#include <stdbool.h>
#include <WiFiDBReceiver.h>
#include <WiFiDBProvider.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct {
    /** @brief
        Allows the registration of a SystemStatusListener to get updates on changes
        in system status.

        @param
        listener: instance of SystemStatusListener,
        implementing the required callback functions.
        Should not be freed until disconnect function is called.

        @return bool True if the connection is succesfull.
    */
    bool (*connectToSystemStatus)(const SystemStatusListener* listener);

    /** @brief
        Provides an instance of WiFiDBReceiver object with
        the specified listener.

        @param
        listener: instance of WiFiDBReceiverResponseListener,
        implementing the required callback functions.
        Should not be freed until disconnect function is called.

        @return WiFiDBReceiver
    */
    const WiFiDBReceiver* (*connectToWiFiDBReceiver)(
            const WiFiDBReceiverResponseListener* listener);

    /** @brief
        Provides an instance of WiFiDBProvider object with
        the specified priority listener.

        @param
        listener: instance of WiFiDBProviderResponseListener,
        implementing the required callback functions.
        Should not be freed until disconnect function is called.

        @return WiFiDBProvider
    */
    const WiFiDBProvider* (*connectToWiFiDBProvider)(
            const WiFiDBProviderResponseListener* listener);

    /** @brief
        Disconnect the SystemStatusListener. Indicates that client process is not
        available for any reason.

        @param
        listener: instance of WiFiDBReceiverResponseListener, previously provided
        in the connectToWiFiDBReceiver call.
    */
    void (*disconnectFromSystemStatus)(const SystemStatusListener* listener);

    /** @brief
        Disconnect the WiFiDBReceiver associated with the provided listener.

        @param
        listener: instance of WiFiDBReceiverResponseListener, previously provided
        in the connectToWiFiDBReceiver call.
    */
    void (*disconnectFromWiFiDBReceiver)(
            const WiFiDBReceiverResponseListener* listener);

    /** @brief
        Disconnect the WiFiDBProvider associated with the provided listener.

        @param
        listener: instance of WiFiDBProviderResponseListener, previously provided
        in the connectToWiFiDBProvider call.
    */
    void (*disconnectFromWiFiDBProvider)(
            const WiFiDBProviderResponseListener* listener);
} NLPApi;

/** @brief
    Provides a C pointer to an instance of NLPApi struct after dynamic linking to lobnlp_api.so.
*/
inline const NLPApi* linkGetNLPApi() {
    typedef void* (getNLPApi)();

    getNLPApi* getter = NULL;
    const char *error = NULL;
    dlerror();
    void *handle = dlopen("libnlp_api.so", RTLD_NOW);
    if (NULL != handle || (error = dlerror()) == NULL)  {
        getter = (getNLPApi*)dlsym(handle, "getNLPApi");
        if ((error = dlerror()) != NULL)  {
            getter = NULL;
        }
    }

    return (NLPApi*)(*getter)();
}

#ifdef __cplusplus
}
#endif

#endif /* NLP_API_H */
