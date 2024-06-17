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

#include <cstdarg>
#include <cstdio>
#include <iostream>
#include <limits>
#include <sstream>

#include <testing/testdefs.h>

#include "logging_wrapper/logging.h"

namespace {

char g_buffer[1024];
size_t g_cur_size = 0;

void loggerf(const char* p_fmt, ...)
{
    va_list args;
    va_start(args, p_fmt);
    const int rc = vsprintf(g_buffer + g_cur_size, p_fmt, args);
    va_end(args);
    if (rc >= 0) {
        g_cur_size += rc;
    }
}

} // <anonymous> namespace

TEST(logging_cpp, logging)
{
    std::stringstream str_logger;

    LOG_ERROR(str_logger, "test logging wrapper, '" << 42 << "'");

    const std::string ethalon_example = "1970-01-01 00:00:00.000 [ERROR] test logging wrapper, '42'\n";
    const std::string ethalon = "[ERROR] test logging wrapper, '42'\n";
    std::string str = str_logger.str();
    ASSERT_TRUE(str.size() == ethalon_example.size()) << str.size() << " != " << ethalon_example.size();
    EXPECT_TRUE(str.substr(24) == ethalon) << "'" << str.substr(24) << "' != '" << ethalon << "'";
}

TEST(logging_cpp, severity_level)
{
    std::stringstream str_logger;

    ::wstux::logging::manager::set_global_level(::wstux::logging::severity_level::crit);
    LOG_ERROR(str_logger, "test logging wrapper, '" << 42 << "'");
    LOG_CRIT(str_logger, "test logging wrapper, '" << 42 << "'");

    std::string ethalon_example = "1970-01-01 00:00:00.000 [CRIT ] test logging wrapper, '42'\n";
    std::string ethalon = "[CRIT ] test logging wrapper, '42'\n";
    std::string str = str_logger.str();
    ASSERT_TRUE(str.size() == ethalon_example.size()) << str.size() << " != " << ethalon_example.size();
    EXPECT_TRUE(str.substr(24) == ethalon) << "'" << str.substr(24) << "' != '" << ethalon << "'";

    ::wstux::logging::manager::set_global_level(::wstux::logging::severity_level::info);
    LOG_ERROR(str_logger, "test logging wrapper, '" << 42 << "'");
    ethalon_example = "1970-01-01 00:00:00.000 [CRIT ] test logging wrapper, '42'\n"
                      "1970-01-01 00:00:00.000 [ERROR] test logging wrapper, '42'\n";
    ethalon = "[CRIT ] test logging wrapper, '42'\n";
    str = str_logger.str();
    ASSERT_TRUE(str.size() == ethalon_example.size()) << str.size() << " != " << ethalon_example.size();
    EXPECT_TRUE(str.substr(24, 35) == ethalon) << "'" << str.substr(24, 35) << "' != '" << ethalon << "'";
    ethalon = "[ERROR] test logging wrapper, '42'\n";
    EXPECT_TRUE(str.substr(83, 35) == ethalon) << "'" << str.substr(83, 35) << "' != '" << ethalon << "'";
}

TEST(logging_cpp, severity_level_2)
{
    ::wstux::logging::manager::set_global_level(::wstux::logging::severity_level::info);
    for (int i = std::numeric_limits<int8_t>::min(); i < std::numeric_limits<int8_t>::max(); ++i) {
        ::wstux::logging::manager::set_global_level(i);

        if (i < 0) {
            ASSERT_TRUE(::wstux::logging::manager::global_level() == ::wstux::logging::severity_level::info)
                << "value: " << i << "; log level: " << ::wstux::logging::manager::global_level();
        } else if (i <= ::wstux::logging::severity_level::trace) {
            ASSERT_TRUE(::wstux::logging::manager::global_level() == (::wstux::logging::severity_level)i)
                << "value: " << i << "; log level: " << ::wstux::logging::manager::global_level();
        } else {
            ASSERT_TRUE(::wstux::logging::manager::global_level() == ::wstux::logging::severity_level::trace)
                << "value: " << i << "; log level: " << ::wstux::logging::manager::global_level();
        }
    }
}

TEST(logging_c, logging)
{
    g_cur_size = 0;
    LOGF_ERROR(loggerf, "test logging wrapper, '%d'", 42);

    const std::string ethalon_example = "1970-01-01 00:00:00.000 [ERROR] test logging wrapper, '42'\n";
    const std::string ethalon = "[ERROR] test logging wrapper, '42'\n";
    std::string str(g_buffer, g_cur_size);
    ASSERT_TRUE(str.size() == ethalon_example.size()) << str.size() << " != " << ethalon_example.size();
    EXPECT_TRUE(str.substr(24) == ethalon) << "'" << str.substr(24) << "' != '" << ethalon << "'";
}

TEST(logging_c, severity_level)
{
    g_cur_size = 0;
    ::wstux::logging::manager::set_global_level(::wstux::logging::severity_level::crit);
    LOGF_ERROR(loggerf, "test logging wrapper, '%d'", 42);
    LOGF_CRIT(loggerf, "test logging wrapper, '%d'", 42);

    std::string ethalon_example = "1970-01-01 00:00:00.000 [CRIT ] test logging wrapper, '42'\n";
    std::string ethalon = "[CRIT ] test logging wrapper, '42'\n";
    std::string str(g_buffer, g_cur_size);
    ASSERT_TRUE(str.size() == ethalon_example.size()) << str.size() << " != " << ethalon_example.size();
    EXPECT_TRUE(str.substr(24) == ethalon) << "'" << str.substr(24) << "' != '" << ethalon << "'";

    str.clear();
    ::wstux::logging::manager::set_global_level(::wstux::logging::severity_level::info);
    LOGF_ERROR(loggerf, "test logging wrapper, '%d'", 42);
    ethalon_example = "1970-01-01 00:00:00.000 [CRIT ] test logging wrapper, '42'\n"
                      "1970-01-01 00:00:00.000 [ERROR] test logging wrapper, '42'\n";
    ethalon = "[CRIT ] test logging wrapper, '42'\n";
    str.append(g_buffer, g_cur_size);
    ASSERT_TRUE(str.size() == ethalon_example.size()) << str.size() << " != " << ethalon_example.size();
    EXPECT_TRUE(str.substr(24, 35) == ethalon) << "'" << str.substr(24, 35) << "' != '" << ethalon << "'";
    ethalon = "[ERROR] test logging wrapper, '42'\n";
    EXPECT_TRUE(str.substr(83, 35) == ethalon) << "'" << str.substr(83, 35) << "' != '" << ethalon << "'";
}

int main(int /*argc*/, char** /*argv*/)
{
    return RUN_ALL_TESTS();
}
