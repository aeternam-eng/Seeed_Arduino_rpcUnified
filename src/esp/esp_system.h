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

#ifndef __ESP_SYSTEM_H__
#define __ESP_SYSTEM_H__

#include <stdint.h>
#include <stdbool.h>
#include "esp/esp_err.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    ESP_MAC_WIFI_STA,
    ESP_MAC_WIFI_SOFTAP,
    ESP_MAC_BT,
    ESP_MAC_ETH,
} esp_mac_type_t;

/** @cond */
#define TWO_UNIVERSAL_MAC_ADDR 2
#define FOUR_UNIVERSAL_MAC_ADDR 4
#define UNIVERSAL_MAC_ADDR_NUM CONFIG_NUMBER_OF_UNIVERSAL_MAC_ADDRESS
/** @endcond */

/**
    @brief Reset reasons
*/
typedef enum {
    ESP_RST_UNKNOWN,    //!< Reset reason can not be determined
    ESP_RST_POWERON,    //!< Reset due to power-on event
    ESP_RST_EXT,        //!< Reset by external pin (not applicable for ESP32)
    ESP_RST_SW,         //!< Software reset via esp_restart
    ESP_RST_PANIC,      //!< Software reset due to exception/panic
    ESP_RST_INT_WDT,    //!< Reset (software or hardware) due to interrupt watchdog
    ESP_RST_TASK_WDT,   //!< Reset due to task watchdog
    ESP_RST_WDT,        //!< Reset due to other watchdogs
    ESP_RST_DEEPSLEEP,  //!< Reset after exiting deep sleep mode
    ESP_RST_BROWNOUT,   //!< Brownout reset (software or hardware)
    ESP_RST_SDIO,       //!< Reset over SDIO
} esp_reset_reason_t;

/** @cond */
/**
    @attention  Applications don't need to call this function anymore. It does nothing and will
                be removed in future version.
*/
void system_init(void) __attribute__((deprecated));

/**
    @brief  Reset to default settings.

    Function has been deprecated, please use esp_wifi_restore instead.
    This name will be removed in a future release.
*/
void system_restore(void) __attribute__((deprecated));
/** @endcond */

/**
    Shutdown handler type
*/
typedef void (*shutdown_handler_t)(void);

/**
    @brief  Register shutdown handler

    This function allows you to register a handler that gets invoked before
    the application is restarted using esp_restart function.
*/
rpc_esp_err_t esp_register_shutdown_handler(shutdown_handler_t handle);

/**
    @brief  Restart PRO and APP CPUs.

    This function can be called both from PRO and APP CPUs.
    After successful restart, CPU reset reason will be SW_CPU_RESET.
    Peripherals (except for WiFi, BT, UART0, SPI1, and legacy timers) are not reset.
    This function does not return.
*/
void esp_restart(void) __attribute__((noreturn));

/** @cond */
/**
    @brief  Restart system.

    Function has been renamed to esp_restart.
    This name will be removed in a future release.
*/
void system_restart(void) __attribute__((deprecated, noreturn));
/** @endcond */

/**
    @brief  Get reason of last reset
    @return See description of esp_reset_reason_t for explanation of each value.
*/
esp_reset_reason_t esp_reset_reason(void);

/** @cond */
/**
    @brief  Get system time, unit: microsecond.

    This function is deprecated. Use 'gettimeofday' function for 64-bit precision.
    This definition will be removed in a future release.
*/
uint32_t system_get_time(void)  __attribute__((deprecated));
/** @endcond */

/**
    @brief  Get the size of available heap.

    Note that the returned value may be larger than the maximum contiguous block
    which can be allocated.

    @return Available heap size, in bytes.
*/
uint32_t esp_get_free_heap_size(void);

/** @cond */
/**
    @brief  Get the size of available heap.

    Function has been renamed to esp_get_free_heap_size.
    This name will be removed in a future release.

    @return Available heap size, in bytes.
*/
uint32_t system_get_free_heap_size(void)  __attribute__((deprecated));
/** @endcond */

/**
    @brief Get the minimum heap that has ever been available

    @return Minimum free heap ever available
*/
uint32_t esp_get_minimum_free_heap_size(void);

/**
    @brief  Get one random 32-bit word from hardware RNG

    The hardware RNG is fully functional whenever an RF subsystem is running (ie Bluetooth or WiFi is enabled). For
    random values, call this function after WiFi or Bluetooth are started.

    If the RF subsystem is not used by the program, the function bootloader_random_enable() can be called to enable an
    entropy source. bootloader_random_disable() must be called before RF subsystem or I2S peripheral are used. See these functions'
    documentation for more details.

    Any time the app is running without an RF subsystem (or bootloader_random) enabled, RNG hardware should be
    considered a PRNG. A very small amount of entropy is available due to pre-seeding while the IDF
    bootloader is running, but this should not be relied upon for any use.

    @return Random value between 0 and UINT32_MAX
*/
uint32_t esp_random(void);

/**
    @brief Fill a buffer with random bytes from hardware RNG

    @note This function has the same restrictions regarding available entropy as esp_random()

    @param buf Pointer to buffer to fill with random numbers.
    @param len Length of buffer in bytes
*/
void esp_fill_random(void* buf, size_t len);

/**
    @brief  Set base MAC address with the MAC address which is stored in BLK3 of EFUSE or
            external storage e.g. flash and EEPROM.

    Base MAC address is used to generate the MAC addresses used by the networking interfaces.
    If using base MAC address stored in BLK3 of EFUSE or external storage, call this API to set base MAC
    address with the MAC address which is stored in BLK3 of EFUSE or external storage before initializing
    WiFi/BT/Ethernet.

    @param  mac  base MAC address, length: 6 bytes.

    @return ESP_OK on success
*/
rpc_esp_err_t esp_base_mac_addr_set(uint8_t* mac);

/**
    @brief  Return base MAC address which is set using esp_base_mac_addr_set.

    @param  mac  base MAC address, length: 6 bytes.

    @return ESP_OK on success
            ESP_ERR_INVALID_MAC base MAC address has not been set
*/
rpc_esp_err_t esp_base_mac_addr_get(uint8_t* mac);

/**
    @brief  Return base MAC address which was previously written to BLK3 of EFUSE.

    Base MAC address is used to generate the MAC addresses used by the networking interfaces.
    This API returns the custom base MAC address which was previously written to BLK3 of EFUSE.
    Writing this EFUSE allows setting of a different (non-Espressif) base MAC address. It is also
    possible to store a custom base MAC address elsewhere, see esp_base_mac_addr_set() for details.

    @param  mac  base MAC address, length: 6 bytes.

    @return ESP_OK on success
            ESP_ERR_INVALID_VERSION An invalid MAC version field was read from BLK3 of EFUSE
            ESP_ERR_INVALID_CRC An invalid MAC CRC was read from BLK3 of EFUSE
*/
rpc_esp_err_t esp_efuse_mac_get_custom(uint8_t* mac);

/**
    @brief  Return base MAC address which is factory-programmed by Espressif in BLK0 of EFUSE.

    @param  mac  base MAC address, length: 6 bytes.

    @return ESP_OK on success
*/
rpc_esp_err_t esp_efuse_mac_get_default(uint8_t* mac);

/** @cond */
/**
    @brief  Read hardware MAC address from efuse.

    Function has been renamed to esp_efuse_mac_get_default.
    This name will be removed in a future release.

    @param  mac  hardware MAC address, length: 6 bytes.

    @return ESP_OK on success
*/
rpc_esp_err_t esp_efuse_read_mac(uint8_t* mac) __attribute__((deprecated));

/**
    @brief  Read hardware MAC address.

    Function has been renamed to esp_efuse_mac_get_default.
    This name will be removed in a future release.

    @param  mac  hardware MAC address, length: 6 bytes.
    @return ESP_OK on success
*/
rpc_esp_err_t system_efuse_read_mac(uint8_t* mac) __attribute__((deprecated));
/** @endcond */

/**
    @brief  Read base MAC address and set MAC address of the interface.

    This function first get base MAC address using esp_base_mac_addr_get or reads base MAC address
    from BLK0 of EFUSE. Then set the MAC address of the interface including wifi station, wifi softap,
    bluetooth and ethernet.

    @param  mac  MAC address of the interface, length: 6 bytes.
    @param  type  type of MAC address, 0:wifi station, 1:wifi softap, 2:bluetooth, 3:ethernet.

    @return ESP_OK on success
*/
rpc_esp_err_t esp_read_mac(uint8_t* mac, esp_mac_type_t type);

/**
    @brief  Derive local MAC address from universal MAC address.

    This function derives a local MAC address from an universal MAC address.
    A `definition of local vs universal MAC address can be found on Wikipedia
    <https://en.wikipedia.org/wiki/MAC_address#Universal_vs._local>`.
    In ESP32, universal MAC address is generated from base MAC address in EFUSE or other external storage.
    Local MAC address is derived from the universal MAC address.

    @param  local_mac  Derived local MAC address, length: 6 bytes.
    @param  universal_mac  Source universal MAC address, length: 6 bytes.

    @return ESP_OK on success
*/
rpc_esp_err_t esp_derive_local_mac(uint8_t* local_mac, const uint8_t* universal_mac);

/** @cond */
/**
    Get SDK version

    This function is deprecated and will be removed in a future release.

    @return constant string "master"
*/
const char* system_get_sdk_version(void)  __attribute__((deprecated));
/** @endcond */

/**
    Get IDF version

    @return constant string from IDF_VER
*/
const char* esp_get_idf_version(void);


/**
    @brief Chip models
*/
typedef enum {
    CHIP_ESP32 = 1, //!< ESP32
} esp_chip_model_t;

/* Chip feature flags, used in esp_chip_info_t */
#define CHIP_FEATURE_EMB_FLASH      BIT(0)      //!< Chip has embedded flash memory
#define CHIP_FEATURE_WIFI_BGN       BIT(1)      //!< Chip has 2.4GHz WiFi
#define CHIP_FEATURE_BLE            BIT(4)      //!< Chip has Bluetooth LE
#define CHIP_FEATURE_BT             BIT(5)      //!< Chip has Bluetooth Classic

/**
    @brief The structure represents information about the chip
*/
typedef struct {
    esp_chip_model_t model;  //!< chip model, one of esp_chip_model_t
    uint32_t features;       //!< bit mask of CHIP_FEATURE_x feature flags
    uint8_t cores;           //!< number of CPU cores
    uint8_t revision;        //!< chip revision number
} esp_chip_info_t;

/**
    @brief Fill an esp_chip_info_t structure with information about the chip
    @param[out] out_info structure to be filled
*/
void esp_chip_info(esp_chip_info_t* out_info);


/**
    @brief configure system-time(time.h) through SNTP functions.

    This function first configure SNTP server1 address,
    then get the network time from these servers,
    and then set system time with the time got.

    @param  timezone -11 to 13, 0 means UTC/GMT time.
    @param  reserved for future using, set to 0.
    @server1 SNTP server 1 address, set to NULL if unused.
    @server2 SNTP server 2 address, set to NULL if unused.
    @server3 SNTP server 3 address, set to NULL if unused.

    @return 0 on success, -1 on failed.
*/
int ntp_conf_time(int timezone, int reserved, const char* server1, const char* server2, const char* server3);

#ifdef __cplusplus
}
#endif

#endif /* __ESP_SYSTEM_H__ */
