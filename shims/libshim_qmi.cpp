/*
 * Copyright (C) 2022 The LineageOS Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

namespace android {
    boolean qcril_qmi_encode_npb(pb_ostream_t *os, const pb_field_t pb_fields[], void *msg) {
        boolean status = qcril_qmi_npb_encode_preparation(pb_fields, msg);

        if (status)
        {
            status = pb_encode(os, pb_fields, msg);
         /*   if (!status)
            {
                QCRIL_LOG_DEBUG("Encoding failed: %s", PB_GET_ERROR(os));
            }*/
        }
        return status;
    } // qcril_qmi_encode_npb
}
