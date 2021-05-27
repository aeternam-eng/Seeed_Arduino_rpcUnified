/*
 * Copyright (c) 2020 SeeedStudio.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "erpc_arduino_uart_transport.h"
#include "Arduino.h"
#include "wiring_private.h"

using namespace erpc;

#define NO_RTS_PIN 255
#define NO_CTS_PIN 255
#define RTS_RX_THRESHOLD 10

////////////////////////////////////////////////////////////////////////////////
// Variables
////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

UartTransport::UartTransport(HardwareSerial *uartDrv, unsigned long baudrate)
    : m_uartDrv(uartDrv), m_baudrate(baudrate)
{
}

UartTransport::~UartTransport(void)
{
}

erpc_status_t UartTransport::init(void)
{
  m_uartDrv->begin(m_baudrate);
  return kErpcStatus_Success;
}

erpc_status_t UartTransport::underlyingReceive(uint8_t *data, uint32_t size)
{
  uint32_t bytesRead = 0;
  m_uartDrv->setTimeout(1000);
  while(!m_uartDrv->available()){vTaskDelay(1);}
  bytesRead = m_uartDrv->readBytes(data, size);
  if(bytesRead < size) {
    log_e("Bytes read is smaller than size in receive");
  }

  return kErpcStatus_Success; // return size != bytesRead ? kErpcStatus_ReceiveFailed : kErpcStatus_Success;
}

erpc_status_t UartTransport::underlyingSend(const uint8_t *data, uint32_t size)
{
  uint32_t sentSize = 0;
  while (sentSize < size) {
    const uint32_t sendSize = min(size - sentSize, (uint32_t)256);
    while(!m_uartDrv->availableForWrite()){vTaskDelay(1);}
    sentSize += m_uartDrv->write(&data[sentSize], sendSize);
  }
  return kErpcStatus_Success; // return size != offset ? kErpcStatus_SendFailed : kErpcStatus_Success;
}

bool UartTransport::hasMessage()
{
  if (m_uartDrv->available())
  {
    return true;
  }
  return false;
}
