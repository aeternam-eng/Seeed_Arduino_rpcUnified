#if !defined(_rpc_wifi_api__hal_h_)
#define _rpc_wifi_api__hal_h_

#include "Arduino.h"
#include "wifi_unified.h"
#include "erpc/erpc_shim_unified.h"
#include "erpc/erpc_port.h"
#include "esp/esp_lib_unified.h"
//#include "rpc_unified_log.h"
#include "new_lwip/err.h"
#include "new_lwip/tcp.h"
#include "new_lwip/priv/tcpip_priv.h"

#define IW_PASSPHRASE_MAX_SIZE 64
#define NDIS_802_11_LENGTH_SSID 32
#define A_SHA_DIGEST_LEN 20

typedef struct wlan_fast_reconnect
{
    unsigned char psk_essid[NDIS_802_11_LENGTH_SSID + 4];
    unsigned char psk_passphrase[IW_PASSPHRASE_MAX_SIZE + 1];
    unsigned char wpa_global_PSK[A_SHA_DIGEST_LEN * 2];
    uint32_t channel;
    uint32_t security_type;
    uint32_t offer_ip;
} wlan_fast_reconnect_profile_t;

void system_event_callback_reg(rpc_system_event_cb_t system_event_cb);
int wifi_get_reconnect_data(wlan_fast_reconnect_profile_t *wifi_info);
int wifi_clear_reconnect_data();

bool wifi_is_scaning();
uint16_t wifi_scan_get_ap_num();
int32_t wifi_scan_get_ap_records(uint16_t number, rpc_wifi_ap_record_t *_scanResult);
int32_t wifi_scan_start();

void new_tcpip_adapter_init(void);
rpc_esp_err_t new_tcpip_adapter_eth_start(uint8_t *mac, rpc_tcpip_adapter_ip_info_t *ip_info);
rpc_esp_err_t new_tcpip_adapter_sta_start(uint8_t *mac, rpc_tcpip_adapter_ip_info_t *ip_info);
rpc_esp_err_t new_tcpip_adapter_ap_start(uint8_t *mac, rpc_tcpip_adapter_ip_info_t *ip_info);
rpc_esp_err_t new_tcpip_adapter_stop(rpc_tcpip_adapter_if_t tcpip_if);
rpc_esp_err_t new_tcpip_adapter_up(rpc_tcpip_adapter_if_t tcpip_if);
rpc_esp_err_t new_tcpip_adapter_down(rpc_tcpip_adapter_if_t tcpip_if);
rpc_esp_err_t new_tcpip_adapter_get_ip_info(rpc_tcpip_adapter_if_t tcpip_if, rpc_tcpip_adapter_ip_info_t *ip_info);
rpc_esp_err_t new_tcpip_adapter_set_ip_info(rpc_tcpip_adapter_if_t tcpip_if, rpc_tcpip_adapter_ip_info_t *ip_info);
rpc_esp_err_t new_tcpip_adapter_set_dns_info(rpc_tcpip_adapter_if_t tcpip_if, rpc_tcpip_adapter_dns_type_t type, rpc_tcpip_adapter_dns_info_t *dns);
rpc_esp_err_t new_tcpip_adapter_get_dns_info(rpc_tcpip_adapter_if_t tcpip_if, rpc_tcpip_adapter_dns_type_t type, rpc_tcpip_adapter_dns_info_t *dns);
rpc_esp_err_t new_tcpip_adapter_get_mac(rpc_tcpip_adapter_if_t tcpip_if, uint8_t *mac);
rpc_esp_err_t new_tcpip_adapter_set_mac(rpc_tcpip_adapter_if_t tcpip_if, uint8_t *mac);
rpc_esp_err_t new_tcpip_adapter_dhcps_start(rpc_tcpip_adapter_if_t tcpip_if);
rpc_esp_err_t new_tcpip_adapter_dhcps_stop(rpc_tcpip_adapter_if_t tcpip_if);
rpc_esp_err_t new_tcpip_adapter_dhcpc_start(rpc_tcpip_adapter_if_t tcpip_if);
rpc_esp_err_t new_tcpip_adapter_dhcpc_stop(rpc_tcpip_adapter_if_t tcpip_if);
rpc_esp_err_t new_tcpip_adapter_set_hostname(rpc_tcpip_adapter_if_t tcpip_if, const char *hostname);
rpc_esp_err_t new_tcpip_adapter_get_hostname(rpc_tcpip_adapter_if_t tcpip_if, const char **hostname);
rpc_esp_err_t new_tcpip_adapter_dhcps_option(rpc_tcpip_adapter_option_mode_t opt_op, rpc_tcpip_adapter_option_id_t opt_id,
                                     void *opt_val, uint32_t opt_len);
rpc_esp_err_t new_tcpip_adapter_create_ip6_linklocal(rpc_tcpip_adapter_if_t tcpip_if);
rpc_esp_err_t new_tcpip_adapter_get_ip6_linklocal(rpc_tcpip_adapter_if_t tcpip_if, ip6_addr_t *if_ip6);
err_t new_tcpip_api_call(new_tcpip_api_call_fn fn, struct new_tcpip_api_call_data *call);
err_t new_tcp_connect(struct new_tcp_pcb *pcb, const new_ip_addr_t *ipaddr,u16_t port, tcp_connected_fn connected);
void new_tcp_recved(struct new_tcp_pcb *pcb, u16_t len);
void new_tcp_abort(struct new_tcp_pcb *pcb);
err_t new_tcp_write(struct new_tcp_pcb *pcb, const void *dataptr, u16_t len,u8_t apiflags);
err_t new_tcp_output(struct new_tcp_pcb *pcb);
err_t new_tcp_close(struct new_tcp_pcb *pcb);
err_t new_tcp_bind(struct new_tcp_pcb *pcb, const new_ip_addr_t *ipaddr,u16_t port);
struct new_tcp_pcb * new_tcp_listen_with_backlog(struct new_tcp_pcb *pcb, u8_t backlog);
struct new_tcp_pcb * new_tcp_new_ip_type (u8_t type);
void new_tcp_arg(struct new_tcp_pcb *pcb, void *arg);
void new_tcp_accept(struct new_tcp_pcb *pcb, tcp_accept_fn accept);
void new_tcp_err(struct new_tcp_pcb *pcb, tcp_err_fn err);
void new_tcp_recv(struct new_tcp_pcb *pcb, tcp_recv_fn recv);
void new_tcp_sent(struct new_tcp_pcb *pcb, tcp_sent_fn sent);
void new_tcp_poll(struct new_tcp_pcb *pcb, tcp_poll_fn poll, u8_t interval);
u8_t new_pbuf_free(struct pbuf *p);
char * new_ip4addr_ntoa(const new_ip4_addr_t *new_ip4_addr);
u16_t new_inet_chksum(const void *dataptr, u16_t len);

#endif /* _rpc_wifi_api__hal_h_ */