/*
 * Copyright (c) 2016, Freescale Semiconductor, Inc.
 * Copyright 2016 NXP
 * All rights reserved.
 *
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include "erpc_port.h"
#include <new>
#include "Arduino.h"

extern "C" {
#include "freertos/FreeRTOS.h"
};

// using namespace std;
// void *operator new(std::size_t count) THROW_BADALLOC
// {
//     void *p = malloc(count);
//     return p;
// }

// void *operator new(std::size_t count, const std::nothrow_t &tag) THROW
// {

//     void *p = malloc(count);
//     return p;
// }

// void *operator new[](std::size_t count) THROW_BADALLOC
// {
//     void *p = malloc(count);
//     return p;
// }

// void *operator new[](std::size_t count, const std::nothrow_t &tag) THROW

// {
//     void *p = malloc(count);
//     return p;
// }

// void operator delete(void *ptr) THROW
// {
//     free(ptr);
// }

// void operator delete[](void *ptr) THROW
// {
//     free(ptr);
// }

void *erpc_malloc(size_t size)
{
    if(size == 0)
    {
        return NULL;
    }
    
    void *p = pvPortMalloc(size);
    return p;
}

void erpc_free(void *ptr)
{
    vPortFree(ptr);
}

/* Provide function for pure virtual call to avoid huge demangling code being linked in ARM GCC */
#if ((defined(__GNUC__)) && (defined(__arm__)))
extern "C" void __cxa_pure_virtual(void) __attribute__ ((weak));

extern "C" void __cxa_pure_virtual()
{
    while (1)
        ;
}
#endif
