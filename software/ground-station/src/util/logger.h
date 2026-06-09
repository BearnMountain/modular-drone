#pragma once

#include <stdint.h>
#include <string_view>
#include <string>
#include <print>
#include <format>
#include <chrono>
enum class LogCode : uint16_t {
	// -- Generic --
	NONE = 0,

	// -- System / OS  (1xx) --
	FILE_NOT_FOUND    = 101,
	PERMISSION_DENIED = 102,
	OUT_OF_MEMORY     = 103,
	IO_ERROR          = 104,

	// -- Network      (2xx) --
	CONNECTION_REFUSED   = 201,
	TIMEOUT              = 202,
	DNS_FAILURE          = 203,
	SSL_HANDSHAKE_FAILED = 204,

	// -- Application  (3xx) --
	INVALID_ARGUMENT   = 301,
	NULL_POINTER       = 302,
	INDEX_OUT_OF_RANGE = 303,
	PARSE_ERROR        = 304,

	// -- Database     (4xx) --
	QUERY_FAILED			  = 401,
	TRANSACTION_FAILED 		  = 402,
	CONNECTION_POOL_EXHAUSTED = 403,

	// -- Auth / Security (5xx) --
	UNAUTHORIZED     = 501,
	FORBIDDEN        = 502,
	TOKEN_EXPIRED    = 503,

	// -- Internal / Unknown (9xx) --
	UNKNOWN         = 999,
};

constexpr const char* RESET_CLR = "\033[0m";
constexpr const char* TIME_CLR  = "\033[2m";    
constexpr const char* INFO_CLR  = "\033[38;5;75m";
constexpr const char* WARN_CLR  = "\033[38;5;178m";
constexpr const char* ERROR_CLR = "\033[38;5;203m";
constexpr const char* DEBUG_CLR = "\033[38;5;71m";
constexpr const char* FATAL_CLR = "\033[38;5;168m";

#define GENERATE_LOG_FUNCTION(name, level_str, color)                          \
    template<typename... Args>                                                 \
    inline void name(std::format_string<Args...> fmt, Args&&... args) {        \
        auto now = std::chrono::system_clock::now();                           \
        auto days_dp = std::chrono::floor<std::chrono::days>(now);             \
        auto duration_since_midnight =                                         \
            std::chrono::floor<std::chrono::seconds>(now - days_dp);           \
        std::chrono::hh_mm_ss hms{duration_since_midnight};                    \
        constexpr int pad = 5 - std::string_view(level_str).size();            \
        std::println(stderr,                                                   \
            "{}{}{} [{}{}{}]{} {}",                                            \
            TIME_CLR, hms, RESET_CLR,                                          \
            color, level_str, RESET_CLR,                                       \
            std::string(pad, ' '),                                             \
            std::format(fmt, std::forward<Args>(args)...)                      \
        );                                                                     \
    }

namespace Log {

	GENERATE_LOG_FUNCTION(warn,  "WARN",  WARN_CLR)
    GENERATE_LOG_FUNCTION(info,  "INFO",  INFO_CLR)
    GENERATE_LOG_FUNCTION(debug, "DEBUG", DEBUG_CLR)
    GENERATE_LOG_FUNCTION(error, "ERROR", ERROR_CLR)
    GENERATE_LOG_FUNCTION(fatal, "FATAL", FATAL_CLR)

	// inline std::string_view error_code_name(ErrorCode code) {
	// 	static std::string_view storage;
	// 	switch (code) {
	// 		case ErrorCode::NONE: storage = "None";
	// 		case ErrorCode::FILE_NOT_FOUND: storage = "FileNotFound";
	// 		case ErrorCode::PERMISSION_DENIED: storage = "PermissionDenied";
	// 		case ErrorCode::OUT_OF_MEMORY: storage = "OutOfMemory";
	// 		case ErrorCode::IO_ERROR: storage = "IOError";
	// 		case ErrorCode::CONNECTION_REFUSED: storage = "ConnectionRefused";
	// 		case ErrorCode::TIMEOUT: storage = "Timeout";
	// 		case ErrorCode::DNS_FAILURE: storage = "DNSFailure";
	// 		case ErrorCode::SSL_HANDSHAKE_FAILED: storage = "SSLHandshakeFailed";
	// 		case ErrorCode::INVALID_ARGUMENT: storage = "InvalidArgument";
	// 		case ErrorCode::NULL_POINTER: storage = "NullPointer";
	// 		case ErrorCode::INDEX_OUT_OF_RANGE: storage = "IndexOutOfRange";
	// 		case ErrorCode::PARSE_ERROR: storage = "ParseError";
	// 		case ErrorCode::QUERY_FAILED: storage = "QueryFailed";
	// 		case ErrorCode::TRANSACTION_FAILED: storage = "TransactionFailed";
	// 		case ErrorCode::CONNECTION_POOL_EXHAUSTED: storage = "ConnectionPoolExhausted";
	// 		case ErrorCode::UNAUTHORIZED: storage = "Unauthorized";
	// 		case ErrorCode::FORBIDDEN: storage = "Forbidden";
	// 		case ErrorCode::TOKEN_EXPIRED: storage = "TokenExpired";
	// 		default: storage = "Unknown";
	// 	}
	//
	// 	return storage;
	// }	

	// inline std::string error_code_value(ErrorCode code) {
	// 	return std::to_string(static_cast<uint16_t>(code));
	// }

};

#undef GENERATE_LOG_FUNCTION
