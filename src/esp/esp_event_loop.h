// Copyright 2015-2016 Espressif Systems (Shanghai) PTE LTD
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at

//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#ifndef __NEW_ESP_EVENT_LOOP_H__
#define __NEW_ESP_EVENT_LOOP_H__

#include <stdint.h>
#include <stdbool.h>

#include "esp/esp_err.h"
#include "esp/esp_event.h"
#include "freertos/FreeRTOS.h"


#ifdef __cplusplus
extern "C" {
#endif

/**
    @brief  Application specified event callback function

    @param  void *ctx : reserved for user
    @param  rpc_system_event_t *event : event type defined in this file

    @return ESP_OK : succeed
    @return others : fail
*/
typedef rpc_esp_err_t (*rpc_system_event_cb_t)(void* ctx, rpc_system_event_t* event);

/**
    @brief  Initialize event loop
            Create the event handler and task

    @param  rpc_system_event_cb_t cb : application specified event callback, it can be modified by call esp_event_set_cb
    @param  void *ctx : reserved for user

    @return ESP_OK : succeed
    @return others : fail
*/
rpc_esp_err_t rpc_esp_event_loop_init(rpc_system_event_cb_t cb, void* ctx);

/**
    @brief  Set application specified event callback function

    @attention 1. If cb is NULL, means application don't need to handle
                  If cb is not NULL, it will be call when an event is received, after the default event callback is completed

    @param  rpc_system_event_cb_t cb : callback
    @param  void *ctx : reserved for user

    @return rpc_system_event_cb_t : old callback
*/
rpc_system_event_cb_t rpc_esp_event_loop_set_cb(rpc_system_event_cb_t cb, void* ctx);

/**
    @brief  Get the queue used by event loop

    @attention : currently this API is used to initialize "q" parameter
    of wifi_init structure.

    @return QueueHandle_t : event queue handle
*/
QueueHandle_t rpc_esp_event_loop_get_queue(void);


#ifdef __cplusplus
}
#endif

#endif /* __ESP_EVENT_LOOP_H__ */
