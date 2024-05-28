/*
 * Copyright 2023-2024 AVSystem <avsystem@avsystem.com>
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
 *
 * Licensed under the AVSystem-5-clause License.
 * See the attached LICENSE file for details.
 */

#ifndef AVS_COAP_CONFIG_H
#define AVS_COAP_CONFIG_H

#include <sdkconfig.h>

/**
 * @file avs_coap_config.h
 *
 * avs_coap library configuration.
 *
 * The preferred way to compile avs_coap is to use CMake, in which case this
 * file will be generated automatically by CMake.
 *
 * However, to provide compatibility with various build systems used especially
 * by embedded platforms, it is alternatively supported to compile avs_coap by
 * other means, in which case this file will need to be provided manually.
 *
 * <strong>NOTE: To compile this library without using CMake, you need to
 * configure avs_commons first. Please refer to documentation in the
 * <c>avs_commons_config.h</c> file (provided in the repository as
 * <c>avs_commons_config.h.in</c>) for details.</strong>
 *
 * <strong>avs_coap requires the following avs_commons components to be
 * enabled:</strong>
 *
 * - @c avs_buffer
 * - @c avs_compat_threading
 * - @c avs_list
 * - @c avs_net
 * - @c avs_sched
 * - @c avs_stream
 * - @c avs_utils
 * - @c avs_log (if @c WITH_AVS_COAP_LOGS is enabled)
 * - @c avs_persistence (if @c WITH_AVS_COAP_OBSERVE_PERSISTENCE is enabled)
 * - @c avs_crypto (if @c WITH_AVS_COAP_OSCORE is enabled)
 *
 * In the repository, this file is provided as <c>avs_coap_config.h.in</c>,
 * intended to be processed by CMake. If editing this file manually, please copy
 * or rename it to <c>avs_coap_config.h</c> and for each of the
 * <c>\#cmakedefine</c> directives, please either replace it with regular
 * <c>\#define</c> to enable it, or comment it out to disable. You may also need
 * to replace variables wrapped in <c>\@</c> signs with concrete values. Please
 * refer to the comments above each of the specific definition for details.
 *
 * If you are editing a file previously generated by CMake, these
 * <c>\#cmakedefine</c>s will be already replaced by either <c>\#define</c> or
 * commented out <c>\#undef</c> directives.
 */

/**
 * Enable support for block-wise transfers (RFC 7959).
 *
 * If this flag is disabled, attempting to send a message bigger than the
 * internal buffer will fail; incoming messages may still carry BLOCK1 or BLOCK2
 * options, but they will not be interpreted by the library in any way.
 */
#define WITH_AVS_COAP_BLOCK

/**
 * Enable support for observations (RFC 7641).
 */
#ifdef CONFIG_ANJAY_WITH_OBSERVE
#    define WITH_AVS_COAP_OBSERVE
#endif // CONFIG_ANJAY_WITH_OBSERVE

/**
 * Turn on cancelling observation on a timeout.
 *
 * Only meaningful if <c>WITH_AVS_COAP_OBSERVE</c> is enabled.
 *
 * NOTE: LwM2M specification requires LwM2M server to send Cancel Observation
 * request. Meanwhile CoAP RFC 7641 states that timeout on notification should
 * cancel it. This setting is to enable both of these behaviors with default
 * focused on keeping observations in case of bad connectivity.
 */
/* #undef WITH_AVS_COAP_OBSERVE_CANCEL_ON_TIMEOUT */

/**
 * Enable support for observation persistence (<c>avs_coap_observe_persist()</c>
 * and <c>avs_coap_observe_restore()</c> calls).
 *
 * Only meaningful if <c>WITH_AVS_COAP_OBSERVE</c> is enabled.
 */
#ifdef CONFIG_ANJAY_WITH_OBSERVE_PERSISTENCE
#    define WITH_AVS_COAP_OBSERVE_PERSISTENCE
#endif // CONFIG_ANJAY_WITH_OBSERVE_PERSISTENCE

/**
 * Enable support for the streaming API
 */
#define WITH_AVS_COAP_STREAMING_API

/**
 * Enable support for UDP transport.
 *
 * NOTE: Enabling at least one transport is necessary for the library to be
 * useful.
 */
#define WITH_AVS_COAP_UDP

/**
 * Enable support for TCP transport (RFC 8323).
 *
 * NOTE: Enabling at least one transport is necessary for the library to be
 * useful.
 */
#define WITH_AVS_COAP_TCP

/**
 * Enable support for OSCORE (RFC 8613).
 *
 * IMPORTANT: Only available with the OSCORE feature. Ignored in the open
 * source version.
 */
#ifdef CONFIG_ANJAY_WITH_COAP_OSCORE
#    define WITH_AVS_COAP_OSCORE
#endif // CONFIG_ANJAY_WITH_COAP_OSCORE

/**
 * Use OSCORE version from draft-ietf-core-object-security-08 instead of the
 * final version (RFC 8613).
 *
 * Only meaningful if <c>WITH_AVS_COAP_OSCORE</c> is enabled.
 *
 * IMPORTANT: Only available with the OSCORE feature. Ignored in the open
 * source version.
 */
/* #undef WITH_AVS_COAP_OSCORE_DRAFT_8 */

/**
 * Maximum number of notification tokens stored to match Reset responses to.
 *
 * Only meaningful if <c>WITH_AVS_COAP_OBSERVE</c> and <c>WITH_AVS_COAP_UDP</c>
 * are enabled.
 *
 * If editing this file manually, <c>4</c> shall be
 * replaced with a positive integer literal. The default value defined in CMake
 * build scripts is 4.
 */
#define AVS_COAP_UDP_NOTIFY_CACHE_SIZE 4

/**
 * Enable sending diagnostic payload in error responses.
 */
#define WITH_AVS_COAP_DIAGNOSTIC_MESSAGES

/**
 * Enable logging in avs_coap.
 *
 * If this flag is disabled, no logging is compiled into the binary at all.
 */
#ifdef CONFIG_WITH_AVS_COAP_LOGS
#    define WITH_AVS_COAP_LOGS
#endif // CONFIG_WITH_AVS_COAP_LOGS

/**
 * Enable TRACE-level logs in avs_coap.
 *
 * Only meaningful if <c>WITH_AVS_COAP_LOGS</c> is enabled.
 */
#ifdef CONFIG_WITH_AVS_COAP_TRACE_LOGS
#    define WITH_AVS_COAP_TRACE_LOGS
#endif // CONFIG_WITH_AVS_COAP_TRACE_LOGS

/**
 * Enable poisoning of unwanted symbols when compiling avs_coap.
 *
 * Requires a compiler that supports <c>\#pragma GCC poison</c>.
 *
 * This is mostly useful during development, to ensure that avs_commons do not
 * attempt to call functions considered harmful in this library, such as printf.
 * This is not guaranteed to work as intended on every platform.
 */
/* #undef WITH_AVS_COAP_POISONING */

#endif // AVS_COAP_CONFIG_H
