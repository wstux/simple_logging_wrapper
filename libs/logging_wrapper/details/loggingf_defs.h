/*
 * simple_logging_wrapper
 * Copyright (C) 2024  Chistyakov Alexander
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _SIMPLE_LOGGING_WRAPPER_LOGGING_WRAPPER_LOGGINGF_DEFS_H_
#define _SIMPLE_LOGGING_WRAPPER_LOGGING_WRAPPER_LOGGINGF_DEFS_H_

#include "logging_wrapper/logging_manager.h"
#include "logging_wrapper/severity_level.h"

#if ! defined(LOGGINGF_WRAPPER_IMPL)
    #define LOGGINGF_WRAPPER_IMPL(logger, level, fmt, ...)                  \
        char cur_ts[24];                                                    \
        ::wstux::logging::details::timestamp(cur_ts, 24);                   \
        logger("%s " _LOGF_LEVEL(level) " " fmt "\n", cur_ts __VA_OPT__(,) __VA_ARGS__)
#endif

#define _LOGF(logger, level, fmt, ...)                                      \
    do {                                                                    \
        if (! ::wstux::logging::manager::cal_log(_LOG_LEVEL(level))) {      \
            break;                                                          \
        }                                                                   \
        LOGGINGF_WRAPPER_IMPL(logger, level, fmt, __VA_ARGS__);             \
    }                                                                       \
    while (0)

#endif /* _SIMPLE_LOGGING_WRAPPER_LOGGING_WRAPPER_LOGGINGF_DEFS_H_ */

