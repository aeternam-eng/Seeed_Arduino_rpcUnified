
/*
    The MIT License (MIT)
    Copyright (C) 2020  Seeed Technology Co.,Ltd.
*/

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
/*#define PIN_BLE_SERIAL_X_RX (84ul)
#define PIN_BLE_SERIAL_X_TX (85ul)
#define PAD_BLE_SERIAL_X_RX (SERCOM_RX_PAD_2)
#define PAD_BLE_SERIAL_X_TX (UART_TX_PAD_0)
#define SERCOM_BLE_SERIAL_X sercom0
#define INTERRUPT_HANDLER_IMPLEMENT_BLE_SERIAL_X(uart) \
    void SERCOM0_0_Handler()                           \
    {                                                  \
        (uart).IrqHandler();                           \
    }                                                  \
    void SERCOM0_1_Handler()                           \
    {                                                  \
        (uart).IrqHandler();                           \
    }                                                  \
    void SERCOM0_2_Handler()                           \
    {                                                  \
        (uart).IrqHandler();                           \
    }                                                  \
    void SERCOM0_3_Handler()                           \
    {                                                  \
        (uart).IrqHandler();                           \
    }

EUart ble_uart(&SERCOM_BLE_SERIAL_X, PIN_BLE_SERIAL_X_RX, PIN_BLE_SERIAL_X_TX, PAD_BLE_SERIAL_X_RX, PAD_BLE_SERIAL_X_TX);
extern "C"
{
    INTERRUPT_HANDLER_IMPLEMENT_BLE_SERIAL_X(ble_uart)
}*/

//HardwareSerial ble_uart = HardwareSerial(2);
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
    server->addService(static_cast<erpc::Service *>(create_rpc_ble_callback_service()));
    server->addService(static_cast<erpc::Service *>(create_rpc_wifi_callback_service()));
}

void runClient(void *arg)
{
    (void)arg;
    delay(100);

    log_d("clientThread running");
    //loopTask(NULL);
}

void runServer(void *arg)
{
    (void)arg;
    /* run server */
    log_d("serverThread running");
    for(;;) {
        uint8_t result = g_server.poll();
        //log_d("Running poll: %d\n", result);
        if(result != kErpcStatus_Success) {
            //vTaskDelete(NULL);
            log_d("eRPC polling has returned other than success %d", result);
            //g_server.poll();
            //esp_restart();
            //break;
        }
        //taskYIELD();
        //vTaskDelay(100);
        delay(20);
    }
}

Thread serverThread(&runServer, configMAX_PRIORITIES, 32768, "runServer");
//Thread clientThread(&runClient, configMAX_PRIORITIES - 2, 64575, "runClient");

void erpc_init()
{
    pinMode(16, INPUT_PULLUP);
    pinMode(17, INPUT_PULLUP);

    #define RTL8720D_CHIP_PU 4

    pinMode(RTL8720D_CHIP_PU, OUTPUT);
    digitalWrite(RTL8720D_CHIP_PU, LOW);
    delay(100);
    digitalWrite(RTL8720D_CHIP_PU, HIGH);
    delay(500);

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
    //clientThread.start();

    g_client->setServer(&g_server);
    g_client->setServerThreadId(serverThread.getThreadId());

    log_d("g_client initialized");
}

void init() {
    Serial.begin(115200);
    erpc_init();
}