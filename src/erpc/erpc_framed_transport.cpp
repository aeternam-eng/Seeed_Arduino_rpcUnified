/*
 * Copyright (c) 2014-2016, Freescale Semiconductor, Inc.
 * Copyright 2016-2017 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "Arduino.h"
#include "erpc_framed_transport.h"
#include "erpc_message_buffer.h"
#include <cassert>
#include <cstdio>

#include <vector>
#include <iostream>
#include <algorithm>
#include <string>
#include <sstream>
#include <iomanip>

using namespace erpc;

////////////////////////////////////////////////////////////////////////////////
// Code
////////////////////////////////////////////////////////////////////////////////

FramedTransport::FramedTransport(void)
: Transport()
, m_crcImpl(NULL)
#if ERPC_THREADS
, m_sendLock()
, m_receiveLock()
#endif
{
}

FramedTransport::~FramedTransport(void) {}

void FramedTransport::setCrc16(Crc16 *crcImpl)
{
    assert(crcImpl);
    m_crcImpl = crcImpl;
}

erpc_status_t FramedTransport::receive(MessageBuffer *message)
{
    assert(m_crcImpl && "Uninitialized Crc16 object.");
    Header h;

    {
#if ERPC_THREADS
        Mutex::Guard lock(m_receiveLock);
#endif

        // Receive header first.
        erpc_status_t ret = underlyingReceive((uint8_t *)&h, sizeof(h));
        if (ret != kErpcStatus_Success)
        {
            return ret;
        }

        //log_i("ERPC Header - CRC: %d | SIZE: %d", h.m_crc, h.m_messageSize);

        // received size can't be larger then buffer length.
        if (h.m_messageSize > message->getLength())
        {
            log_e("Receive Failed -> msgsize: %d, buflen: %d", h.m_messageSize, message->getLength());
            return kErpcStatus_ReceiveFailed;
        }

        // Receive rest of the message now we know its size.
        ret = underlyingReceive(message->get(), h.m_messageSize);
        
        /*if(ret == kErpcStatus_Success) {
            String a;
            for(int i = 0; i < h.m_messageSize; i++) {
                String b = String(message->get()[i], HEX);
                b.toUpperCase();
                String c = "0";
                if(b.length() == 1) {
                    c.concat(b);
                    a += c;
                } else {
                    a += b;
                }
                a += " ";
            }

            log_d("ERPC Message: %s", a.c_str());
        }*/

        if (ret != kErpcStatus_Success)
        {
            return ret;
        }
    }

    // Verify CRC.
    uint16_t computedCrc = m_crcImpl->computeCRC16(message->get(), h.m_messageSize);
    if (computedCrc != h.m_crc)
    {
        log_d("CRC Check FAILED - exp: %d | rcv: %d", computedCrc, h.m_crc);
        return kErpcStatus_CrcCheckFailed;
    }

    message->setUsed(h.m_messageSize);
    return kErpcStatus_Success;
}

erpc_status_t FramedTransport::send(MessageBuffer *message)
{
    assert(m_crcImpl && "Uninitialized Crc16 object.");
#if ERPC_THREADS
    Mutex::Guard lock(m_sendLock);
#endif

    uint16_t messageLength = message->getUsed();

    // Send header first.
    Header h;
    h.m_messageSize = messageLength;
    h.m_crc = m_crcImpl->computeCRC16(message->get(), messageLength);
    erpc_status_t ret = underlyingSend((uint8_t *)&h, sizeof(h));
    if (ret != kErpcStatus_Success)
    {
        return ret;
    }

    // Send the rest of the message.
    return underlyingSend(message->get(), messageLength);
}
