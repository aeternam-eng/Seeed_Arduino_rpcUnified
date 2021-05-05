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

#ifndef __ESP_EVENT_H__
#define __ESP_EVENT_H__

#include <stdint.h>
#include <stdbool.h>

#include "esp/esp_err.h"
#include "esp/esp_wifi_types.h"
#include "esp/tcpip_adapter.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum : uint8_t{
    RPC_SYSTEM_EVENT_WIFI_READY = 0,           /**< ESP32 WiFi ready */
    RPC_SYSTEM_EVENT_SCAN_DONE,                /**< ESP32 finish scanning AP */
    RPC_SYSTEM_EVENT_STA_START,                /**< ESP32 station start */
    RPC_SYSTEM_EVENT_STA_STOP,                 /**< ESP32 station stop */
    RPC_SYSTEM_EVENT_STA_CONNECTED,            /**< ESP32 station connected to AP */
    RPC_SYSTEM_EVENT_STA_DISCONNECTED,         /**< ESP32 station disconnected from AP */
    RPC_SYSTEM_EVENT_STA_AUTHMODE_CHANGE,      /**< the auth mode of AP connected by ESP32 station changed */
    RPC_SYSTEM_EVENT_STA_GOT_IP,               /**< ESP32 station got IP from connected AP */
    RPC_SYSTEM_EVENT_STA_LOST_IP,              /**< ESP32 station lost IP and the IP is reset to 0 */
    RPC_SYSTEM_EVENT_STA_WPS_ER_SUCCESS,       /**< ESP32 station wps succeeds in enrollee mode */
    RPC_SYSTEM_EVENT_STA_WPS_ER_FAILED,        /**< ESP32 station wps fails in enrollee mode */
    RPC_SYSTEM_EVENT_STA_WPS_ER_TIMEOUT,       /**< ESP32 station wps timeout in enrollee mode */
    RPC_SYSTEM_EVENT_STA_WPS_ER_PIN,           /**< ESP32 station wps pin code in enrollee mode */
    RPC_SYSTEM_EVENT_STA_WPS_ER_PBC_OVERLAP,   /*!< ESP32 station wps overlap in enrollee mode */
    RPC_SYSTEM_EVENT_AP_START,                 /**< ESP32 soft-AP start */
    RPC_SYSTEM_EVENT_AP_STOP,                  /**< ESP32 soft-AP stop */
    RPC_SYSTEM_EVENT_AP_STACONNECTED,          /**< a station connected to ESP32 soft-AP */
    RPC_SYSTEM_EVENT_AP_STADISCONNECTED,       /**< a station disconnected from ESP32 soft-AP */
    RPC_SYSTEM_EVENT_AP_STAIPASSIGNED,         /**< ESP32 soft-AP assign an IP to a connected station */
    RPC_SYSTEM_EVENT_AP_PROBEREQRECVED,        /**< Receive probe request packet in soft-AP interface */
    RPC_SYSTEM_EVENT_GOT_IP6,                  /**< ESP32 station or ap or ethernet interface v6IP addr is preferred */
    RPC_SYSTEM_EVENT_ETH_START,                /**< ESP32 ethernet start */
    RPC_SYSTEM_EVENT_ETH_STOP,                 /**< ESP32 ethernet stop */
    RPC_SYSTEM_EVENT_ETH_CONNECTED,            /**< ESP32 ethernet phy link up */
    RPC_SYSTEM_EVENT_ETH_DISCONNECTED,         /**< ESP32 ethernet phy link down */
    RPC_SYSTEM_EVENT_ETH_GOT_IP,               /**< ESP32 ethernet got IP from connected AP */
    RPC_SYSTEM_EVENT_MAX
} rpc_system_event_id_t;

/* add this macro define for compatible with old IDF version */
#ifndef SYSTEM_EVENT_AP_STA_GOT_IP6
#define SYSTEM_EVENT_AP_STA_GOT_IP6 SYSTEM_EVENT_GOT_IP6
#endif

typedef enum : uint8_t {
    RPC_WPS_FAIL_REASON_NORMAL = 0,                   /**< ESP32 WPS normal fail reason */
    RPC_WPS_FAIL_REASON_RECV_M2D,                       /**< ESP32 WPS receive M2D frame */
    RPC_WPS_FAIL_REASON_MAX
} rpc_system_event_sta_wps_fail_reason_t;
typedef struct {
    uint32_t status;          /**< status of scanning APs */
    uint8_t  number;
    uint8_t  scan_id;
} rpc_system_event_sta_scan_done_t;

typedef struct {
    uint8_t ssid[32];         /**< SSID of connected AP */
    uint8_t ssid_len;         /**< SSID length of connected AP */
    uint8_t bssid[6];         /**< BSSID of connected AP*/
    uint8_t channel;          /**< channel of connected AP*/
    rpc_wifi_auth_mode_t authmode;
} rpc_system_event_sta_connected_t;

typedef struct {
    uint8_t ssid[32];         /**< SSID of disconnected AP */
    uint8_t ssid_len;         /**< SSID length of disconnected AP */
    uint8_t bssid[6];         /**< BSSID of disconnected AP */
    uint8_t reason;           /**< reason of disconnection */
} rpc_system_event_sta_disconnected_t;

typedef struct {
    rpc_wifi_auth_mode_t old_mode;         /**< the old auth mode of AP */
    rpc_wifi_auth_mode_t new_mode;         /**< the new auth mode of AP */
} rpc_system_event_sta_authmode_change_t;

typedef struct {
    rpc_tcpip_adapter_ip_info_t ip_info;
    bool ip_changed;
} rpc_system_event_sta_got_ip_t;

typedef struct {
    uint8_t pin_code[8];         /**< PIN code of station in enrollee mode */
} rpc_system_event_sta_wps_er_pin_t;

typedef struct {
    rpc_tcpip_adapter_if_t if_index;
    rpc_tcpip_adapter_ip6_info_t ip6_info;
} rpc_system_event_got_ip6_t;

typedef struct {
    uint8_t mac[6];           /**< MAC address of the station connected to ESP32 soft-AP */
    uint8_t aid;              /**< the aid that ESP32 soft-AP gives to the station connected to  */
} rpc_system_event_ap_staconnected_t;

typedef struct {
    uint8_t mac[6];           /**< MAC address of the station disconnects to ESP32 soft-AP */
    uint8_t aid;              /**< the aid that ESP32 soft-AP gave to the station disconnects to  */
} rpc_system_event_ap_stadisconnected_t;

typedef struct {
    int rssi;                 /**< Received probe request signal strength */
    uint8_t mac[6];           /**< MAC address of the station which send probe request */
} rpc_system_event_ap_probe_req_rx_t;

typedef union {
    rpc_system_event_sta_connected_t               connected;          /**< ESP32 station connected to AP */
    rpc_system_event_sta_disconnected_t            disconnected;       /**< ESP32 station disconnected to AP */
    rpc_system_event_sta_scan_done_t               scan_done;          /**< ESP32 station scan (APs) done */
    rpc_system_event_sta_authmode_change_t
    auth_change;        /**< the auth mode of AP ESP32 station connected to changed */
    rpc_system_event_sta_got_ip_t
    got_ip;             /**< ESP32 station got IP, first time got IP or when IP is changed */
    rpc_system_event_sta_wps_er_pin_t              sta_er_pin;         /**< ESP32 station WPS enrollee mode PIN code received */
    rpc_system_event_sta_wps_fail_reason_t
    sta_er_fail_reason;/**< ESP32 station WPS enrollee mode failed reason code received */
    rpc_system_event_ap_staconnected_t             sta_connected;      /**< a station connected to ESP32 soft-AP */
    rpc_system_event_ap_stadisconnected_t          sta_disconnected;   /**< a station disconnected to ESP32 soft-AP */
    rpc_system_event_ap_probe_req_rx_t             ap_probereqrecved;  /**< ESP32 soft-AP receive probe request packet */
    rpc_system_event_got_ip6_t
    got_ip6;            /**< ESP32 station　or ap or ethernet ipv6 addr state change to preferred */
} rpc_system_event_info_t;

typedef struct {
    rpc_system_event_id_t     event_id;      /**< event ID */
    rpc_system_event_info_t   event_info;    /**< event information */
} rpc_system_event_t;

typedef rpc_esp_err_t (*rpc_system_event_handler_t)(rpc_system_event_t* event);


#ifdef __cplusplus
}
#endif

#endif /* __ESP_EVENT_H__ */
