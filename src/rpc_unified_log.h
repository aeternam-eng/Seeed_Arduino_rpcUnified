#ifndef _RPC_LOG_H
#define _RPC_LOG_H

#include "Arduino.h"

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <stdlib.h>
#ifndef TAG
#define TAG "RPC"
#endif

// #define ENABLE_RPC_DEBUG
// #define ENABLE_RPC_WARN
// #define ENABLE_RPC_INFO
// #define ENABLE_RPC_ERROR
// #define ENABLE_RPC_TRACE

	extern void rpc_printf(const char *format, ...);
/**
 * @brief Debug level logging macro.
 *
 * Macro to expose function, line number as well as desired log message.
 */
#ifdef ENABLE_RPC_DEBUG
#define RPC_DEBUG(...)                                       \
	{                                                        \
		log_v(__VA_ARGS__);									\
	}
#else
#define RPC_DEBUG(...)
#endif

/**
 * @brief Debug level trace logging macro.
 *
 * Macro to print message function entry and exit
 */
#ifdef ENABLE_RPC_TRACE
#define FUNC_ENTRY                                                  \
	{                                                               \
		log_i("FUNC_ENTRY"); \
	}
#define FUNC_EXIT                                                  \
	{                                                              \
		log_i("FUNC_EXIT"); \
	}
#define FUNC_EXIT_RC(x)                                                                \
	{                                                                                  \
		log_i("FUNC_EXIT: Return Code : %d", x); \
		return x;                                                                      \
	}
#else
#define FUNC_ENTRY

#define FUNC_EXIT
#define FUNC_EXIT_RC(x) \
	{                   \
		return x;       \
	}
#endif

/**
 * @brief Info level logging macro.
 *
 * Macro to expose desired log message.  Info messages do not include automatic function names and line numbers.
 */
#ifdef ENABLE_RPC_INFO
#define RPC_INFO(...)              \
	{                              \
		log_i(__VA_ARGS__);	\
	}
#else
#define RPC_INFO(...)
#endif

/**
 * @brief Warn level logging macro.
 *
 * Macro to expose function, line number as well as desired log message.
 */
#ifdef ENABLE_RPC_WARN
#define RPC_WARN(...)                                      \
	{                                                      \
		log_w(__VA_ARGS__);	\
	}
#else
#define RPC_WARN(...)
#endif

/**
 * @brief Error level logging macro.
 *
 * Macro to expose function, line number as well as desired log message.
 */
#ifdef ENABLE_RPC_ERROR
#define RPC_ERROR(...)                                     \
	{                                                      \
		log_e(__VA_ARGS__);\
	}
#else
#define RPC_ERROR(...)
#endif

#ifdef __cplusplus
}
#endif

#endif // _RPC_LOG_H