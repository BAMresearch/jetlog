/**
 * MIT License
 * 
 * Copyright 2020 Bundesanstalt für Materialforschung und -prüfung (BAM)
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef JETLOG_CPP_FULL_JETLOG_TIMESTAMP_HPP_INCLUDED
#define JETLOG_CPP_FULL_JETLOG_TIMESTAMP_HPP_INCLUDED

#include <chrono>
#define HAS_UNCAUGHT_EXCEPTIONS 1
#include <date/date.h>
#include <string>
#include <type_traits>

#include "jetlog_json.hpp"

namespace jetlog::internal {

struct Iso8601 {};

template <typename T>
inline void addTimestampUnit(::jetlog::json&)
{
    static_assert(sizeof(T) == 0, "Type not supported");
}

template <>
inline void addTimestampUnit<Iso8601>(::jetlog::json&)
{
    // std::string is used for ISO 8601 timestamps, no unit required
}

template <>
inline void addTimestampUnit<std::chrono::seconds>(::jetlog::json&)
{
    // seconds do not require a unit
}

template <>
inline void addTimestampUnit<std::chrono::milliseconds>(::jetlog::json& j)
{
    j["timestamp_unit"] = "ms";
}

template <>
inline void addTimestampUnit<std::chrono::microseconds>(::jetlog::json& j)
{
    j["timestamp_unit"] = "us";
}

template <>
inline void addTimestampUnit<std::chrono::nanoseconds>(::jetlog::json& j)
{
    j["timestamp_unit"] = "ns";
}

template <class TimestampUnit, class TimestampPrecision, typename TTimestamp>
std::enable_if_t<std::is_same_v<TimestampUnit, TimestampPrecision>, typename TimestampPrecision::rep> getTimestamp(TTimestamp timestamp)
{
    return std::chrono::duration_cast<TimestampPrecision>(timestamp).count();
}

template <class TimestampUnit, class TimestampPrecision, typename TTimestamp>
std::enable_if_t<!std::is_same_v<TimestampUnit, TimestampPrecision>, double> getTimestamp(TTimestamp timestamp)
{
    // Combining the factors below led to rounding errors (e.g. 1.123 became 1.123000001)
    static constexpr double multiplicator = TimestampPrecision::period::num * TimestampUnit::period::den;
    static constexpr double divisor = TimestampPrecision::period::den * TimestampUnit::period::num;
    return multiplicator * std::chrono::duration_cast<TimestampPrecision>(timestamp).count() / divisor;
}

template <class Precision>
std::string getIsoTimestamp(std::chrono::system_clock::time_point timestamp)
{
    return date::format("%FT%T%z", date::floor<Precision>(timestamp));
}

} // namespace jetlog::internal

#endif // JETLOG_CPP_FULL_JETLOG_TIMESTAMP_HPP_INCLUDED
