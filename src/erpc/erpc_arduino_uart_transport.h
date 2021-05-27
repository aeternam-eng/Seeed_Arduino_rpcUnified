/*
 * Copyright (c) 2020 SeeedStudio.
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _EMBEDDED_RPC__ARDUINO_UART_TRANSPORT_H_
#define _EMBEDDED_RPC__ARDUINO_UART_TRANSPORT_H_

#include "erpc_framed_transport.h"
#include "Arduino.h"
#include <stdlib.h>

/*!
 * @addtogroup uart_transport
 * @{
 * @file
 */

////////////////////////////////////////////////////////////////////////////////
// Classes
////////////////////////////////////////////////////////////////////////////////

namespace erpc {


#include "HardwareSerial.h"
#include "RingBuffer.h"

#include <cstddef>

/*!
 * @brief Very basic transport to send/receive messages via EUart.
 *
 * @ingroup uart_transport
 */
class UartTransport : public FramedTransport
{
public:
    /*!
     * @brief Constructor.
     *
     * @param[in] uartDrv Cmsis EUart.
     */
    UartTransport(HardwareSerial *uartDrv, unsigned long baudrate);

    /*!
     * @brief Destructor.
     */
    virtual ~UartTransport(void);

    /*!
     * @brief Initialize CMSIS EUart peripheral configuration structure with values specified in UartTransport
     * constructor.
     *
     * @retval kErpcStatus_InitFailed When EUart init function failed.
     * @retval kErpcStatus_Success When EUart init function was executed successfully.
     */
    virtual erpc_status_t init(void);

    virtual bool hasMessage(void);

protected:
    HardwareSerial *m_uartDrv; /*!< Access structure of the USART Driver */
    unsigned long m_baudrate;  /*!< EUart baud rate*/

private:
    /*!
     * @brief Receive data from EUart peripheral.
     *
     * @param[inout] data Preallocated buffer for receiving data.
     * @param[in] size Size of data to read.
     *
     * @retval kErpcStatus_ReceiveFailed EUart failed to receive data.
     * @retval kErpcStatus_Success Successfully received all data.
     */
    virtual erpc_status_t underlyingReceive(uint8_t *data, uint32_t size);

    /*!
     * @brief Write data to EUart peripheral.
     *
     * @param[in] data Buffer to send.
     * @param[in] size Size of data to send.
     *
     * @retval kErpcStatus_Success Always returns success status.
     */
    virtual erpc_status_t underlyingSend(const uint8_t *data, uint32_t size);
};

} // namespace erpc

/*! @} */

#endif // _EMBEDDED_RPC__ARDUINO_UART_TRANSPORT_H_
