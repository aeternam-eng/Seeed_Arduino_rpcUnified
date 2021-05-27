
/*
    The MIT License (MIT)
    Copyright (C) 2020  Seeed Technology Co.,Ltd.
*/
#include <Arduino.h>
#include "erpc/erpc_arduino_uart_transport.h"
#include "erpc/erpc_basic_codec.h"
#include "erpc/erpc_arbitrated_client_manager.h"
#include "erpc/erpc_threading.h"
#include "erpc/erpc_simple_server.h"
#include "erpc/erpc_transport_arbitrator.h"
#include "erpc/erpc_port.h"
#include "erpc/erpc_shim_unified.h"

#include "freertos/FreeRTOS.h"

using namespace erpc;

class MyMessageBufferFactory : public MessageBufferFactory
{
public:
    virtual MessageBuffer create()
    {
        uint8_t *buf = new uint8_t[4096];
        return MessageBuffer(buf, 4096);
    }

    virtual void dispose(MessageBuffer *buf)
    {
        if (*buf)
        {
            delete[] buf->get();
        }
    }
};
#define ble_uart Serial2

UartTransport g_transport(&ble_uart, 614400);
MyMessageBufferFactory g_msgFactory;
BasicCodecFactory g_basicCodecFactory;
ArbitratedClientManager *g_client;
TransportArbitrator g_arbitrator;
SimpleServer g_server;
Crc16 g_crc16;

/**
 * @brief  Initialize erpc server task
 * @return void
 */
void add_services(erpc::SimpleServer *server)
{
    //server->addService(static_cast<erpc::Service *>(create_rpc_ble_callback_service()));
    server->addService(static_cast<erpc::Service *>(create_rpc_wifi_callback_service()));
}

void runServer(void *arg)
{
    (void)arg;
    /* run server */
    log_d("serverThread running");
    for(;;) {
        uint8_t result = g_server.poll();
        if(result != kErpcStatus_Success) {
            log_e("eRPC polling has returned other than success %d", result);
        }
        vTaskDelay(10);
    }
}

Thread serverThread(&runServer, 1, 32767, "runServer");

void erpc_init()
{
    pinMode(16, INPUT_PULLUP);
    pinMode(17, INPUT_PULLUP);

    g_transport.init();
    g_arbitrator.setSharedTransport(&g_transport);
    g_arbitrator.setCodec(g_basicCodecFactory.create());

    g_client = new ArbitratedClientManager();
    g_client->setArbitrator(&g_arbitrator);
    g_client->setCodecFactory(&g_basicCodecFactory);
    g_client->setMessageBufferFactory(&g_msgFactory);

    g_arbitrator.setCrc16(&g_crc16);

    g_server.setTransport(&g_arbitrator);
    g_server.setCodecFactory(&g_basicCodecFactory);
    g_server.setMessageBufferFactory(&g_msgFactory);

    add_services(&g_server);

    serverThread.start();

    g_client->setServer(&g_server);
    g_client->setServerThreadId(serverThread.getThreadId());

    log_d("g_client initialized");
}