/*
 * Copyright 2023 AVSystem <avsystem@avsystem.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef AVS_COMMONS_CONFIG_H
#define AVS_COMMONS_CONFIG_H

#include <sdkconfig.h>

/**
 * @file avs_commons_config.h
 *
 * avs_commons library configuration.
 *
 * The preferred way to compile avs_commons is to use CMake, in which case this
 * file will be generated automatically by CMake.
 *
 * However, to provide compatibility with various build systems used especially
 * by embedded platforms, it is alternatively supported to compile avs_commons
 * by other means, in which case this file will need to be provided manually.
 *
 * In the repository, this file is provided as <c>avs_commons_config.h.in</c>,
 * intended to be processed by CMake. If editing this file manually, please copy
 * or rename it to <c>avs_commons_config.h</c> and for each of the
 * <c>#cmakedefine</c> directives, please either replace it with regular
 * <c>#define</c> to enable it, or comment it out to disable. You may also need
 * to replace variables wrapped in <c>@</c> signs with concrete values. Please
 * refer to the comments above each of the specific definition for details.
 *
 * If you are editing a file previously generated by CMake, these
 * <c>#cmakedefine</c>s will be already replaced by either <c>#define</c> or
 * commented out <c>#undef</c> directives.
 */

/**
 * Options that describe capabilities of the build environment.
 *
 * NOTE: If you leave some of these macros undefined, even though the given
 * feature is actually available in the system, avs_commons will attempt to use
 * its own substitutes, which may be incompatible with the definition in the
 * system and lead to undefined behaviour.
 */
/**@{*/
/**
 * Is the target platform big-endian?
 *
 * If undefined, little-endian is assumed. Mixed-endian architectures are not
 * supported.
 *
 * Affects <c>avs_convert_be*()</c> and <c>avs_[hn]to[hn]*()</c> calls in
 * avs_utils and, by extension, avs_persistence.
 */
/* #undef AVS_COMMONS_BIG_ENDIAN */

/**
 * Is GNU __builtin_add_overflow() extension available?
 *
 * Affects time handling functions in avs_utils. If disabled, software overflow
 * checking will be compiled. Note that this software overflow checking code
 * relies on U2 representation of signed integers.
 */
#define AVS_COMMONS_HAVE_BUILTIN_ADD_OVERFLOW

/**
 * Is GNU __builtin_mul_overflow() extension available?
 *
 * Affects time handling functions in avs_utils. If disabled, software overflow
 * checking will be compiled. Note that this software overflow checking code
 * relies on U2 representation of signed integers.
 */
#define AVS_COMMONS_HAVE_BUILTIN_MUL_OVERFLOW

/**
 * Is net/if.h available in the system?
 *
 * NOTE: If the header is indeed available, but this option is not defined, the
 * <c>IF_NAMESIZE</c> macro will be defined <strong>publicly by avs_commons
 * headers</strong>, which may conflict with system definitions.
 */
/* #undef AVS_COMMONS_HAVE_NET_IF_H */

/**
 * Are GNU diagnostic pragmas (#pragma GCC diagnostic push/pop/ignored)
 * available?
 *
 * If defined, those pragmas will be used to suppress compiler warnings for some
 * code known to generate them and cannot be improved in a more robust way, e.g.
 * for code that is known to generate warnings from within system headers.
 */
#define AVS_COMMONS_HAVE_PRAGMA_DIAGNOSTIC

/**
 * Are GNU visibility pragmas (#pragma GCC visibility push/pop) available?
 *
 * Meaningful mostly if avs_commons will be directly or indirectly linked into
 * a shared library. Causes all symbols except those declared in public headers
 * to be hidden, i.e. not exported outside the shared library. If not defined,
 * default compiler visibility settings will be used, but you still may use
 * compiler flags and linker version scripts to replicate this manually if
 * needed.
 */
#define AVS_COMMONS_HAVE_VISIBILITY

/**
 * Specify an optional compatibility header that allows use of POSIX-specific
 * code that is not compliant with POSIX enough to be compiled directly.
 *
 * This header, if specified, will be included only by the following components,
 * which may be enabled or disabled depending on state of the referenced flags:
 * - avs_compat_threading implementation based on POSIX Threads
 *   (@ref AVS_COMMONS_COMPAT_THREADING_WITH_PTHREAD)
 * - default avs_net socket implementation
 *   (@ref AVS_COMMONS_NET_WITH_POSIX_AVS_SOCKET)
 * - avs_unit (@ref AVS_COMMONS_WITH_AVS_UNIT)
 * - default implementation of avs_time routines
 *   (@ref AVS_COMMONS_UTILS_WITH_POSIX_AVS_TIME)
 *
 * Compatibility headers for lwIP and Microsoft Windows are provided with the
 * library (see the <c>compat</c> directory).
 *
 * If this macro is not defined, the afore-mentioned components, if enabled,
 * will use system headers directly, assuming they are POSIX-compliant.
 *
 * If this macro is enabled, the specified file will be included through an
 * <c>#include AVS_COMMONS_POSIX_COMPAT_HEADER</c> statement. Thus, if editing
 * this file manually, <c>avsystem/commons/lwip-posix-compat.h</c> shall be
 * replaced with a path to such file.
 */
#define AVS_COMMONS_POSIX_COMPAT_HEADER "avsystem/commons/lwip-posix-compat.h"

/**
 * Set if printf implementation doesn't support 64-bit format specifiers.
 * If defined, custom implementation of conversion is used in
 * @c AVS_UINT64_AS_STRING instead of using @c snprintf .
 */
/* #undef AVS_COMMONS_WITHOUT_64BIT_FORMAT_SPECIFIERS */

/**
 * Set if printf implementation doesn't support floating-point numbers.
 * If defined, custom implementation of conversion is used in
 * @c AVS_DOUBLE_AS_STRING instead of using @c snprintf . This might increase
 * compatibility with some embedded libc implementations that do not provide
 * this functionality.
 *
 * NOTE: In order to keep the custom implementation small in code size, it is
 * not intended to be 100% accurate. Rounding errors may occur - according to
 * empirical checks, they show up around the 16th significant decimal digit.
 */
/* #undef AVS_COMMONS_WITHOUT_FLOAT_FORMAT_SPECIFIERS */
/**@}*/

/**
 * Enable poisoning of unwanted symbols when compiling avs_commons.
 *
 * Requires a compiler that supports #pragma GCC poison.
 *
 * This is mostly useful during development, to ensure that avs_commons do not
 * attempt to call functions considered harmful in this library, such as printf.
 * This is not guaranteed to work as intended on every platform, e.g. on macOS
 * it is known to generate false positives due to different dependencies between
 * system headers.
 */
/* #undef AVS_COMMONS_WITH_POISONING */

/**
 * Options that control compilation of avs_commons components.
 *
 * Each of the configuration options below enables, if defined, one of the core
 * components of the avs_commons library.
 *
 * NOTE: Enabling avs_unit will cause an object file with an implementation of
 * main() to be generated.
 */
/**@{*/
#define AVS_COMMONS_WITH_AVS_ALGORITHM
#define AVS_COMMONS_WITH_AVS_BUFFER
#define AVS_COMMONS_WITH_AVS_COMPAT_THREADING
#define AVS_COMMONS_WITH_AVS_CRYPTO
#ifdef CONFIG_ANJAY_WITH_HTTP_DOWNLOAD
#    define AVS_COMMONS_WITH_AVS_HTTP
#endif // CONFIG_ANJAY_WITH_HTTP_DOWNLOAD
#define AVS_COMMONS_WITH_AVS_LIST
#define AVS_COMMONS_WITH_AVS_LOG
#define AVS_COMMONS_WITH_AVS_NET
#if defined(CONFIG_ANJAY_WITH_CORE_PERSISTENCE)           \
        || defined(CONFIG_ANJAY_WITH_ATTR_STORAGE)        \
        || defined(CONFIG_ANJAY_WITH_MODULE_BOOTSTRAPPER) \
        || defined(AVS_COMMONS_NET_WITH_TLS_SESSION_PERSISTENCE)
#    define AVS_COMMONS_WITH_AVS_PERSISTENCE
#endif // defined(CONFIG_ANJAY_WITH_CORE_PERSISTENCE) ||
       // defined(CONFIG_ANJAY_WITH_ATTR_STORAGE)     ||
       // defined(CONFIG_ANJAY_WITH_MODULE_BOOTSTRAPPER) ||
       // defined(AVS_COMMONS_NET_WITH_TLS_SESSION_PERSISTENCE)
/* #undef AVS_COMMONS_WITH_AVS_RBTREE */
#if defined(CONFIG_ANJAY_WITH_OBSERVE) \
        || defined(CONFIG_ANJAY_WITH_MODULE_ACCESS_CONTROL)
#    define AVS_COMMONS_WITH_AVS_SORTED_SET
#endif // defined(CONFIG_ANJAY_WITH_OBSERVE)
       // || defined(CONFIG_ANJAY_WITH_MODULE_ACCESS_CONTROL)
#define AVS_COMMONS_WITH_AVS_SCHED
#define AVS_COMMONS_WITH_AVS_STREAM
/* #undef AVS_COMMONS_WITH_AVS_UNIT */
#define AVS_COMMONS_WITH_AVS_URL
#define AVS_COMMONS_WITH_AVS_UTILS
/* #undef AVS_COMMONS_WITH_AVS_VECTOR */
/**@}*/

/**
 * Options that control compilation of avs_compat_threading implementations.
 *
 * If CMake is not used, in the typical scenario at most one of the following
 * implementations may be enabled at the same time. If none is enabled, the
 * relevant symbols will need to be provided by the user, if used.
 *
 * These are meaningful only if <c>AVS_COMMONS_WITH_AVS_COMPAT_THREADING</c> is
 * defined.
 */
/**@{*/
/**
 * Enable implementation based on spinlocks.
 *
 * This implementation is usually very inefficient, and requires C11 stdatomic.h
 * header to be available.
 */
/* #undef AVS_COMMONS_COMPAT_THREADING_WITH_ATOMIC_SPINLOCK */

/**
 * Enable implementation based on the POSIX Threads library.
 *
 * This implementation is preferred over the spinlock-based one, but the POSIX
 * Threads library is normally available only in UNIX-like environments.
 */
#define AVS_COMMONS_COMPAT_THREADING_WITH_PTHREAD

/**
 * Is the <c>pthread_condattr_setclock()</c> function available?
 *
 * This flag only makes sense when
 * <c>AVS_COMMONS_COMPAT_THREADING_WITH_PTHREAD</c> is enabled.
 *
 * If this flag is disabled, or if <c>CLOCK_MONOTONIC</c> macro is not
 * available, the <c>avs_condvar_wait()</c> will internally use the real-time
 * clock instead of the monotonic clock. Time values will be converted so that
 * this change does not affect API usage.
 */
/* #undef AVS_COMMONS_COMPAT_THREADING_PTHREAD_HAVE_PTHREAD_CONDATTR_SETCLOCK */
/**@}*/

/**
 * Options that control compilation of code depending on TLS backend library.
 *
 * If CMake is not used, in the typical scenario at most one of the following
 * DTLS backends may be enabled at the same time. If none is enabled,
 * functionalities that depends on cryptography will be disabled.
 *
 * Affects avs_crypto, avs_net, and avs_stream (for the MD5 implementation).
 *
 * mbed TLS is the main development backend, and is preferred as such. OpenSSL
 * backend supports most functionality as well, but is not as thoroughly tested.
 * TinyDTLS support is only rudimentary.
 */
/**@{*/
#ifdef CONFIG_MBEDTLS_TLS_ENABLED
#    define AVS_COMMONS_WITH_MBEDTLS
#endif // CONFIG_MBEDTLS_TLS_ENABLED
/* #undef AVS_COMMONS_WITH_OPENSSL */
/* #undef AVS_COMMONS_WITH_TINYDTLS */

/**
 * Enable support for custom TLS socket implementation.
 *
 * If enabled, the user needs to provide their own implementations of
 * <c>_avs_net_create_ssl_socket()</c>, <c>_avs_net_create_dtls_socket()</c>,
 * <c>_avs_net_initialize_global_ssl_state() and
 * <c>_avs_net_cleanup_global_ssl_state()</c>.
 */
/* #undef AVS_COMMONS_WITH_CUSTOM_TLS */
/**@}*/

/**
 * Options related to avs_crypto.
 */
/**@{*/
/**
 * Enable AEAD and HKDF support in avs_crypto. Requires MbedTLS in version at
 * least 2.14.0, OpenSSL in version at least 1.1.0, or custom implementation in
 * case of <c>AVS_COMMONS_WITH_CUSTOM_TLS</c>.
 */
#if defined(CONFIG_ANJAY_WITH_COAP_OSCORE) || defined(CONFIG_ANJAY_WITH_EST)
#    define AVS_COMMONS_WITH_AVS_CRYPTO_ADVANCED_FEATURES
#endif // defined(CONFIG_ANJAY_WITH_COAP_OSCORE) ||
       // defined(CONFIG_ANJAY_WITH_EST)

/**
 * If the TLS backend is either mbed TLS or OpenSSL, enables APIs related to
 * public-key cryptography.
 *
 * Public-key cryptography is not currently supported with TinyDTLS.
 *
 * It also enables support for X.509 certificates in avs_net, if that module is
 * also enabled.
 */
#define AVS_COMMONS_WITH_AVS_CRYPTO_PKI

/**
 * If the TLS backend is either mbed TLS, OpenSSL or TinyDTLS, enables support
 * of pre-shared key security.
 *
 * PSK is the only supported security mode for the TinyDTLS backend, so this
 * option MUST be enabled to utilize it.
 *
 * It also enables support for pre-shared key security in avs_net, if that
 * module is also enabled.
 */
#define AVS_COMMONS_WITH_AVS_CRYPTO_PSK

/**
 * Enables usage of Valgrind API to suppress some of the false positives
 * generated by the OpenSSL backend.
 */
/* #undef AVS_COMMONS_WITH_AVS_CRYPTO_VALGRIND */

/**
 * Enables high-level support for hardware-based PKI security, i.e. loading,
 * generating and managing key pairs and certificates via external engines.
 *
 * Requires @ref AVS_COMMONS_WITH_AVS_CRYPTO_PKI to be enabled.
 *
 * An actual implementation is required to use this feature. You may provide
 * your own, or use one of the default ones that come with the HSM engine
 * commercial feature (see @ref AVS_COMMONS_WITH_MBEDTLS_PKCS11_ENGINE,
 * @ref AVS_COMMONS_WITH_MBEDTLS_PSA_ENGINE and
 * @ref AVS_COMMONS_WITH_OPENSSL_PKCS11_ENGINE).
 *
 * The functions that need to be provided in case of a custom implementation:
 * - <c>avs_crypto_pki_engine_certificate_rm()</c>
 * - <c>avs_crypto_pki_engine_certificate_store()</c>
 * - <c>avs_crypto_pki_engine_key_gen()</c>
 * - <c>avs_crypto_pki_engine_key_rm()</c>
 * - <c>avs_crypto_pki_engine_key_store()</c>
 * - When targeting the Mbed TLS backend:
 *   - <c>_avs_crypto_mbedtls_engine_initialize_global_state()</c>
 *   - <c>_avs_crypto_mbedtls_engine_cleanup_global_state()</c>
 *   - <c>_avs_crypto_mbedtls_engine_append_cert()</c>
 *   - <c>_avs_crypto_mbedtls_engine_append_crl()</c>
 *   - <c>_avs_crypto_mbedtls_engine_load_private_key()</c>
 * - When targeting the OpenSSL backend:
 *   - <c>_avs_crypto_openssl_engine_initialize_global_state()</c>
 *   - <c>_avs_crypto_openssl_engine_cleanup_global_state()</c>
 *   - <c>_avs_crypto_openssl_engine_load_certs()</c>
 *   - <c>_avs_crypto_openssl_engine_load_crls()</c>
 *   - <c>_avs_crypto_openssl_engine_load_private_key()</c>
 *
 * External PKI engines are NOT supported in the TinyDTLS backend.
 */
/* #undef AVS_COMMONS_WITH_AVS_CRYPTO_PKI_ENGINE */

/**
 * Enables high-level support for hardware-based PSK security, i.e. loading
 * and managing PSK keys and identities via external engine.
 *
 * Requires @ref AVS_COMMONS_WITH_AVS_CRYPTO_PKI to be enabled.
 *
 * An actual implementation is required to use this feature. You may provide
 * your own, or use the default PSA-based one that comes with the HSM engine
 * commercial feature (see @ref AVS_COMMONS_WITH_MBEDTLS_PSA_ENGINE).
 *
 * The functions that need to be provided in case of a custom implementation:
 * - <c>avs_crypto_psk_engine_identity_store()</c>
 * - <c>avs_crypto_psk_engine_identity_rm()</c>
 * - <c>avs_crypto_psk_engine_key_store()</c>
 * - <c>avs_crypto_psk_engine_key_rm()</c>
 * - When targeting the Mbed TLS backend:
 *   - <c>_avs_crypto_mbedtls_engine_initialize_global_state()</c>
 *   - <c>_avs_crypto_mbedtls_engine_cleanup_global_state()</c>
 *   - <c>_avs_crypto_mbedtls_engine_load_psk_key()</c>
 *
 * External PSK engines are NOT supported in the OpenSSL and TinyDTLS backend.
 */
/* #undef AVS_COMMONS_WITH_AVS_CRYPTO_PSK_ENGINE */

/**
 * Enables the default implementation of avs_crypto engine, based on Mbed TLS
 * and PKCS#11.
 *
 * Requires @ref AVS_COMMONS_WITH_AVS_CRYPTO_PKI_ENGINE to be enabled.
 *
 * NOTE: Query string format for this engine is a subset of the PKCS#11 URI
 * scheme (see RFC 7512), modelled after the format accepted by libp11 OpenSSL
 * engine.
 *
 * NOTE: The unit tests for this feature depend on SoftHSM and pkcs11-tool.
 * These must be installed for the tests to pass.
 *
 * IMPORTANT: Only available as part of the HSM support commercial feature.
 * Ignored in the open source version.
 */
/* #undef AVS_COMMONS_WITH_MBEDTLS_PKCS11_ENGINE */

/**
 * Enables the default implementation of avs_crypto engine, based on Mbed TLS
 * and Platform Security Architecture (PSA).
 *
 * Requires @ref AVS_COMMONS_WITH_AVS_CRYPTO_PKI_ENGINE or
 * @ref AVS_COMMONS_WITH_AVS_CRYPTO_PSK_ENGINE to be enabled.
 *
 * NOTE: Query string format for this engine is:
 *
 * <pre>
 * kid=<key_ID>[,lifetime=<lifetime>]|uid=<persistent_storage_UID>
 * </pre>
 *
 * The values are parsed using strtoull() with base=0, so may be in decimal,
 * 0-prefixed octal or 0x-prefixed hexadecimal. On key generation and
 * certificate storage, the specified lifetime will be used, or lifetime 1
 * (default persistent storage) will be used if not. On key or certificate use,
 * the lifetime of the actual key will be verified if present on the query
 * string and the key will be rejected if different.
 *
 * Certificates are stored as PSA_KEY_TYPE_RAW_DATA key entries containing
 * X.509 DER data. Alternatively, the PSA Protected Storage API can be used if
 * @ref AVS_COMMONS_WITH_MBEDTLS_PSA_ENGINE_PROTECTED_STORAGE is enabled, by
 * using the <c>uid=...</c> syntax.
 *
 * IMPORTANT: Only available as part of the HSM support commercial feature.
 * Ignored in the open source version.
 */
/* #undef AVS_COMMONS_WITH_MBEDTLS_PSA_ENGINE */

/**
 * Enables support for the PSA Protected Storage API in the PSA-based avs_crypto
 * engine.
 *
 * Requires @ref AVS_COMMONS_WITH_MBEDTLS_PSA_ENGINE to be enabled.
 *
 * IMPORTANT: Only available as part of the HSM support commercial feature.
 * Ignored in the open source version.
 */
/* #undef AVS_COMMONS_WITH_MBEDTLS_PSA_ENGINE_PROTECTED_STORAGE */

/**
 * Is the <c>dlsym()</c> function available?
 *
 * This is currently only used if @ref AVS_COMMONS_WITH_MBEDTLS_PKCS11_ENGINE is
 * enabled. If enabled, the PKCS#11 module is loaded dynamically from a library
 * specified by the <c>PKCS11_MODULE_PATH</c> environment variable. If disabled,
 * a function with the following signature, realizing the PKCS#11
 * <c>C_GetFunctionList</c> method, must be provided manually:
 *
 * <pre>
 * CK_RV _avs_crypto_mbedtls_pkcs11_get_function_list(CK_FUNCTION_LIST_PTR_PTR);
 * </pre>
 */
/* #undef AVS_COMMONS_HAVE_DLSYM */

/**
 * Enables the default implementation of avs_crypto engine, based on OpenSSL and
 * PKCS#11.
 *
 * Requires @ref AVS_COMMONS_WITH_AVS_CRYPTO_PKI_ENGINE to be enabled.
 *
 * NOTE: Query string format for this engine is a subset of the PKCS#11 URI
 * scheme (see RFC 7512), modelled after the format accepted by libp11 OpenSSL
 * engine.
 *
 * NOTE: The unit tests for this feature depend on SoftHSM and pkcs11-tool.
 * These must be installed for the tests to pass.
 *
 * IMPORTANT: Only available as part of the HSM support commercial feature.
 * Ignored in the open source version.
 */
/* #undef AVS_COMMONS_WITH_OPENSSL_PKCS11_ENGINE */
/**@}*/

/**
 * Enable support for HTTP content compression in avs_http.
 *
 * Requires linking with zlib.
 */
/* #undef AVS_COMMONS_HTTP_WITH_ZLIB */

/**
 * Options related to avs_log and logging support within avs_commons.
 */
/**@{*/
/* clang-format off */
/**
 * Size, in bytes, of the avs_log buffer.
 *
 * Log messages that would (including the level, module name and code location)
 * otherwise be longer than this value minus one (for the terminating null
 * character) will be truncated.
 *
 * NOTE: This macro MUST be defined if avs_log is enabled.
 *
 * If editing this file manually, <c>512</c> shall
 * be replaced with a positive integer literal. The default value defined in
 * CMake build scripts is 512.
 */
#define AVS_COMMONS_LOG_MAX_LINE_LENGTH 512
/* clang-format on */

/**
 * Configures avs_log to use a synchronized global buffer instead of allocating
 * a buffer on the stack when constructing log messages.
 *
 * Requires avs_compat_threading to be enabled.
 *
 * Enabling this option would reduce the stack space required to use avs_log, at
 * the expense of global storage and the complexity of using a mutex.
 */
#define AVS_COMMONS_LOG_USE_GLOBAL_BUFFER

/**
 * Provides a default avs_log handler that prints log messages on stderr.
 *
 * Disabling this option will cause logs to be discarded by default, until a
 * custom log handler is set using <c>avs_log_set_handler()</c>.
 */
#define AVS_COMMONS_LOG_WITH_DEFAULT_HANDLER

/**
 * Enables the "micro logs" feature.
 *
 * Replaces all occurrences of the <c>AVS_DISPOSABLE_LOG()</c> macro with single
 * space strings. This is intended to reduce the size of the compiled code, by
 * stripping it of almost all log string data.
 *
 * Note that this setting will propagate both to avs_commons components
 * themselves (as all its internal logs make use of <c>AVS_DISPOSABLE_LOG()</c>)
 * and the user code that uses it.
 */
#ifdef CONFIG_ANJAY_WITH_MICRO_LOGS
#    define AVS_COMMONS_WITH_MICRO_LOGS
#endif // CONFIG_ANJAY_WITH_MICRO_LOGS

/**
 * Enables logging inside avs_commons.
 *
 * Requires @ref AVS_COMMONS_WITH_AVS_LOG to be enabled.
 *
 * If this macro is not defined at avs_commons compile time, calls to avs_log
 * will not be generated inside avs_commons components.
 */
#ifdef CONFIG_AVS_COMMONS_WITH_INTERNAL_LOGS
#    define AVS_COMMONS_WITH_INTERNAL_LOGS
#endif // CONFIG_AVS_COMMONS_WITH_INTERNAL_LOGS

/**
 * Enables TRACE-level logs inside avs_commons.
 *
 * Only meaningful if AVS_COMMONS_WITH_INTERNAL_LOGS is enabled.
 *
 * If this macro is not defined at avs_commons compile time, calls to avs_log
 * with the level set to TRACE will not be generated inside avs_commons
 * components.
 */
#ifdef CONFIG_AVS_COMMONS_WITH_INTERNAL_TRACE
#    define AVS_COMMONS_WITH_INTERNAL_TRACE
#endif // CONFIG_AVS_COMMONS_WITH_INTERNAL_TRACE

/**
 * Enables external implementation of logger subsystem with provided header.
 *
 * Default logger implementation can be found in avs_log_impl.h
 */
/* #undef AVS_COMMONS_WITH_EXTERNAL_LOGGER_HEADER */

/**
 * If specified, the process of checking if avs_log should be written out
 * takes place in compile time.
 *
 * Specify an optional header with a list of modules for which log level
 * is set. If a log level for specific module is not set, the DEFAULT level
 * will be taken into account. Value of the default logging level is set to
 * DEBUG, but can be overwritten in this header file with AVS_LOG_LEVEL_DEFAULT
 * define. Messages with lower level than the one set will be removed during
 * compile time. Possible values match @ref avs_log_level_t.
 *
 * That file should contain C preprocesor defines in the:
 * - "#define AVS_LOG_LEVEL_FOR_MODULE_<Module> <Level>" format,
 *   where <Module> is the module name and <Level> is allowed logging level
 * - "#define AVS_LOG_LEVEL_DEFAULT <Level>" format, where <Level> is the
 *   allowed logging level
 *
 * Example file content:
 *
 * <code>
 * #ifndef AVS_COMMONS_EXTERNAL_LOG_LEVELS_H
 * #define AVS_COMMONS_EXTERNAL_LOG_LEVELS_H
 *
 * // global log level value
 * #define AVS_LOG_LEVEL_DEFAULT INFO
 *
 * //for "coap" messages only WARNING and ERROR messages will be present
 * #define AVS_LOG_LEVEL_FOR_MODULE_coap WARNING
 *
 * //logs are disable for "net" module
 * #define AVS_LOG_LEVEL_FOR_MODULE_net QUIET
 *
 * #endif
 * </code>
 */
/* #undef AVS_COMMONS_WITH_EXTERNAL_LOG_LEVELS_HEADER */

/**
 * Disable log level check in runtime. Allows to save at least 1.3kB of memory.
 *
 * The macros avs_log_set_level and avs_log_set_default_level
 * will not be available.
 *
 */
/* #undef AVS_COMMONS_WITHOUT_LOG_CHECK_IN_RUNTIME */
/**@}*/

/**
 * Options related to avs_net.
 */
/**@{*/
/**
 * Enables support for IPv4 connectivity.
 *
 * At least one of AVS_COMMONS_NET_WITH_IPV4 and AVS_COMMONS_NET_WITH_IPV6
 * MUST be defined if avs_net is enabled.
 */
#define AVS_COMMONS_NET_WITH_IPV4

/**
 * Enables support for IPv6 connectivity.
 *
 * At least one of AVS_COMMONS_NET_WITH_IPV4 and AVS_COMMONS_NET_WITH_IPV6
 * MUST be defined if avs_net is enabled.
 */
#ifdef CONFIG_LWIP_IPV6
#    define AVS_COMMONS_NET_WITH_IPV6
#endif // CONFIG_LWIP_IPV6

/**
 * If the TLS backend is set to OpenSSL, enables support for DTLS.
 *
 * DTLS is always enabled for the mbed TLS and TinyDTLS backends.
 */
/* #undef AVS_COMMONS_NET_WITH_DTLS */

/**
 * Enables debug logs generated by mbed TLS.
 *
 * An avs_log-backed handler, logging for the "mbedtls" module on the TRACE
 * level, is installed using <c>mbedtls_ssl_conf_dbg()</c> for each (D)TLS
 * socket created if this option is enabled.
 */
/* #undef AVS_COMMONS_NET_WITH_MBEDTLS_LOGS */

/**
 * Enables the default implementation of avs_net TCP and UDP sockets.
 *
 * Requires either a UNIX-like operating environment, or a compatibility layer
 * with a high degree of compatibility with standard BSD sockets with an
 * appropriate compatibility header (see @ref AVS_COMMONS_POSIX_COMPAT_HEADER) -
 * lwIP and Winsock are currently supported for this scenario.
 */
#if defined(CONFIG_AVS_COMMONS_NET_WITH_POSIX_AVS_SOCKET)
#    define AVS_COMMONS_NET_WITH_POSIX_AVS_SOCKET
#endif // defined(CONFIG_AVS_COMMONS_NET_WITH_POSIX_AVS_SOCKET)

/**
 * Enables support for logging socket communication to file.
 *
 * If this option is enabled, avs_net_socket_debug() can be used to enable
 * logging all communication to a file called DEBUG.log. If disabled,
 * avs_net_socket_debug() will always return an error.
 */
/* #undef AVS_COMMONS_NET_WITH_SOCKET_LOG */

/**
 * If the TLS backend is either mbed TLS or OpenSSL, enables support for (D)TLS
 * session persistence.
 *
 * Session persistence is not currently supported for the TinyDTLS backend.
 */
#define AVS_COMMONS_NET_WITH_TLS_SESSION_PERSISTENCE
/**@}*/

/**
 * Options related to avs_net's default implementation of TCP and UDP sockets.
 *
 * These options make sense only when @ref AVS_COMMONS_NET_WITH_POSIX_AVS_SOCKET
 * is enabled. They describe capabilities of the Unix-like environment in which
 * the library is built.
 *
 * Note that if @ref AVS_COMMONS_POSIX_COMPAT_HEADER is defined, it might
 * redefine these flags independently of the settings in this file.
 */
/**@{*/
/**
 * Is the <c>gai_strerror()</c> function available?
 *
 * Enabling this flag will provide more detailed log messages in case that
 * <c>getaddrinfo()</c> fails. If this flag is disabled, numeric error codes
 * values will be logged.
 */
/* #undef AVS_COMMONS_NET_POSIX_AVS_SOCKET_HAVE_GAI_STRERROR */

/**
 * Is the <c>getifaddrs()</c> function available?
 *
 * Disabling this flag will cause <c>avs_net_socket_interface_name()</c> to use
 * a less optimal implementation based on the <c>SIOCGIFCONF</c> ioctl.
 *
 * If <c>SIOCGIFCONF</c> is not defined, either, then
 * <c>avs_net_socket_interface_name()</c> will always return an error.
 */
/* #undef AVS_COMMONS_NET_POSIX_AVS_SOCKET_HAVE_GETIFADDRS */

/**
 * Is the <c>getnameinfo()</c> function available?
 *
 * Disabling this flag will cause <c>avs_net_socket_receive_from()</c>,
 * <c>avs_net_socket_accept()</c>,
 * <c>avs_net_resolved_endpoint_get_host_port()</c>,
 * <c>avs_net_resolved_endpoint_get_host()</c> and
 * <c>avs_net_resolve_host_simple()</c> to use a custom reimplementation of
 * <c>getnameinfo()</c> based on <c>inet_ntop()</c>.
 */
/* #undef AVS_COMMONS_NET_POSIX_AVS_SOCKET_HAVE_GETNAMEINFO */

/**
 * Is the <c>IN6_IS_ADDR_V4MAPPED</c> macro available and usable?
 *
 * Disabling this flag will cause a custom code that compares IPv6 addresses
 * with the <c>::ffff:0.0.0.0/32</c> mask to be used instead.
 */
/* #undef AVS_COMMONS_NET_POSIX_AVS_SOCKET_HAVE_IN6_IS_ADDR_V4MAPPED */

/**
 * Should be defined if IPv4-mapped IPv6 addresses (<c>::ffff:0.0.0.0/32</c>)
 * are <strong>NOT</strong> supported by the underlying platform.
 *
 * Enabling this flag will prevent avs_net from using IPv4-mapped IPv6 addresses
 * and instead re-open and re-bind the socket if a connection to an IPv4 address
 * is requested on a previously created IPv6 socket.
 *
 * This may result in otherwise redundant <c>socket()</c>, <c>bind()</c> and
 * <c>close()</c> system calls to be performed, but may be necessary for
 * interoperability with some platforms.
 */
/* #undef AVS_COMMONS_NET_POSIX_AVS_SOCKET_WITHOUT_IN6_V4MAPPED_SUPPORT */

/**
 * Is the <c>inet_ntop()</c> function available?
 *
 * Disabling this flag will cause an internal implementation of this function
 * adapted from BIND 4.9.4 to be used instead.
 */
/* #undef AVS_COMMONS_NET_POSIX_AVS_SOCKET_HAVE_INET_NTOP */

/**
 * Is the <c>poll()</c> function available?
 *
 * Disabling this flag will cause a less robust code based on <c>select()</c> to
 * be used instead.
 */
/* #undef AVS_COMMONS_NET_POSIX_AVS_SOCKET_HAVE_POLL */

/**
 * Is the <c>recvmsg()</c> function available?
 *
 * Disabling this flag will cause <c>recvfrom()</c> to be used instead. Note
 * that for UDP sockets, this will cause false positives for datagram truncation
 * detection (<c>AVS_EMSGSIZE</c>) to be reported when the received message is
 * exactly the size of the buffer.
 */
/* #undef AVS_COMMONS_NET_POSIX_AVS_SOCKET_HAVE_RECVMSG */
/**@}*/

/**
 * Enable thread safety in avs_sched.
 *
 * Makes all scheduler accesses synchronized and thread-safe, at the cost of
 * requiring avs_compat_threading to be enabled, and higher resource usage.
 */
#ifdef CONFIG_ANJAY_WITH_THREAD_SAFETY
#    define AVS_COMMONS_SCHED_THREAD_SAFE
#endif // CONFIG_ANJAY_WITH_THREAD_SAFETY

/**
 * Enable support for file I/O in avs_stream.
 *
 * Disabling this flag will cause the functions declared in
 * <c>avs_stream_file.h</c> to not be defined.
 */
#define AVS_COMMONS_STREAM_WITH_FILE

/**
 * Enable usage of <c>backtrace()</c> and <c>backtrace_symbols()</c> when
 * reporting assertion failures from avs_unit.
 *
 * Requires the afore-mentioned GNU-specific functions to be available.
 *
 * If this flag is disabled, stack traces will not be displayed with assertion
 * failures.
 */
/* #undef AVS_COMMONS_UNIT_POSIX_HAVE_BACKTRACE */

/**
 * Options related to avs_utils.
 */
/**@{*/
/**
 * Enable the default implementation of avs_time_real_now() and
 * avs_time_monotonic_now().
 *
 * Requires an operating environment that supports a clock_gettime() call
 * compatible with POSIX.
 */
#define AVS_COMMONS_UTILS_WITH_POSIX_AVS_TIME

/**
 * Enable the default implementation of avs_malloc(), avs_free(), avs_calloc()
 * and avs_realloc() that forwards to system malloc(), free(), calloc() and
 * realloc() calls.
 *
 * You might disable this option if for any reason you need to use a custom
 * allocator.
 */
#define AVS_COMMONS_UTILS_WITH_STANDARD_ALLOCATOR

/**
 * Enable the alternate implementation of avs_malloc(), avs_free(), avs_calloc()
 * and avs_realloc() that uses system malloc(), free() and realloc() calls, but
 * includes additional fixup code that ensures proper alignment to
 * <c>AVS_ALIGNOF(avs_max_align_t)</c> (usually 8 bytes on common platforms).
 *
 * <c>AVS_COMMONS_UTILS_WITH_STANDARD_ALLOCATOR</c> and
 * <c>AVS_COMMONS_UTILS_WITH_ALIGNFIX_ALLOCATOR</c> cannot be enabled at the
 * same time.
 *
 * NOTE: This implementation is only intended for platforms where the system
 * allocator does not properly conform to the alignment requirements.
 *
 * It comes with an additional runtime costs:
 *
 * - <c>AVS_ALIGNOF(avs_max_align_t)</c> bytes (usually 8) of additional
 *   overhead for each allocated memory block
 * - Additional memmove() for every realloc() that returned a block that is not
 *   properly aligned
 * - avs_calloc() is implemented as avs_malloc() followed by an explicit
 *   memset(); this may be suboptimal on some platforms
 *
 * If these costs are unacceptable for you, you may want to consider fixing,
 * replacing or reconfiguring your system allocator for conformance, or
 * implementing a custom one instead.
 *
 * Please note that some code in avs_commons and dependent projects (e.g. Anjay)
 * may include runtime assertions for proper memory alignment that will be
 * triggered when using a non-conformant standard allocator. Such allocators are
 * relatively common in embedded SDKs. This "alignfix" allocator is intended to
 * work around these issues. On some platforms (e.g. x86) those alignment issues
 * may not actually cause any problems - so you may want to consider disabling
 * runtime assertions instead. Please carefully examine your target platform's
 * alignment requirements and behavior of misaligned memory accesses (including
 * 64-bit data types such as <c>int64_t</c> and <c>double</c>) before doing so.
 */
/* #undef AVS_COMMONS_UTILS_WITH_ALIGNFIX_ALLOCATOR */
/**@}*/

#endif /* AVS_COMMONS_CONFIG_H */
