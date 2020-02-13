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

#ifndef JETLOG_CPP_FULL_JETLOG_HPP_INCLUDED
#define JETLOG_CPP_FULL_JETLOG_HPP_INCLUDED

#include "jetlog_json.hpp"
#include "jetlog_severity.hpp"
#include "jetlog_timestamp.hpp"

namespace jetlog {

namespace internal {

template <class TimestampUnit, typename TTimestamp>
inline json getEntryBase(const char *timestamp_specifier, TTimestamp timestamp)
{
    json j({{timestamp_specifier, timestamp}});
    addTimestampUnit<TimestampUnit>(j);
    return j;
}

template <class TimestampUnit, typename TTimestamp>
inline json getEntry(const std::string &class_name, const json &json_object, const char *timestamp_specifier, TTimestamp timestamp)
{
    json j = getEntryBase<TimestampUnit>(timestamp_specifier, timestamp);
    j["class"] = class_name;
    j.update(json_object);
    return j;
}

} // namespace internal

template <class TimestampPrecision = std::chrono::seconds>
json log(const std::string &message, std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::now())
{
    json j = internal::getEntryBase<internal::Iso8601>("t", internal::getIsoTimestamp<TimestampPrecision>(timestamp));
    j["msg"] = message;
    return j;
}

template <class TimestampPrecision = std::chrono::seconds>
json log(const std::string &message, Severity severity,
         std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::now())
{
    json j = log<TimestampPrecision>(message, timestamp);
    j["severity"] = internal::toString(severity);
    return j;
}

template <class TimestampPrecision = std::chrono::seconds>
json log(const std::string &message, Severity severity, const std::string &source,
         std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::now())
{
    json j = log<TimestampPrecision>(message, severity, timestamp);
    j["source"] = source;
    return j;
}

template <class TimestampPrecision = std::chrono::seconds, typename TObject>
json log(const std::string &message, Severity severity, const std::string &source, const TObject &object,
         std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::now())
{
    json j = log<TimestampPrecision>(message, severity, source, timestamp);
    j.update(static_cast<json>(object));
    return j;
}

template <class TimestampPrecision = std::chrono::seconds, typename T>
json getEntry(const std::string &class_name, const T &object, std::chrono::system_clock::time_point timestamp = std::chrono::system_clock::now())
{
    return internal::getEntry<internal::Iso8601>(class_name, object, "t", internal::getIsoTimestamp<TimestampPrecision>(timestamp));
}

namespace internal {

template <class TimestampPrecision = std::chrono::seconds>
json getEntryVariadicHelper(const std::string &class_name, json json_object, std::chrono::system_clock::time_point timestamp, const json &element_to_add)
{
    json_object.update(element_to_add);
    return getEntry<TimestampPrecision>(class_name, json_object, timestamp);
}

template <class TimestampPrecision = std::chrono::seconds, typename ...Args>
json getEntryVariadicHelper(const std::string &class_name, json json_object, std::chrono::system_clock::time_point timestamp, const json &element_to_add, Args&&... args)
{
    json_object.update(element_to_add);
    return getEntryVariadicHelper<TimestampPrecision>(class_name, json_object, timestamp, std::forward<Args>(args)...);
}

} // namespace internal

template <class TimestampPrecision = std::chrono::seconds, typename ...Args>
json getEntryNow(const std::string &class_name, const json &element_to_add, Args&&... args)
{
    return internal::getEntryVariadicHelper<TimestampPrecision>(class_name, {}, std::chrono::system_clock::now(), element_to_add, std::forward<Args>(args)...);
}

template <class TimestampUnit = std::chrono::seconds, class TimestampPrecision = TimestampUnit, typename T>
json getEntryUnixTime(const std::string &class_name, const T &object, std::chrono::nanoseconds timestamp = std::chrono::system_clock::now().time_since_epoch())
{
    return internal::getEntry<TimestampUnit>(class_name, object, "t_unix", internal::getTimestamp<TimestampUnit, TimestampPrecision>(timestamp));
}

template <class TimestampUnit = std::chrono::seconds, class TimestampPrecision = TimestampUnit, typename T>
json getEntrySysTime(const std::string &class_name, const T &object, std::chrono::nanoseconds timestamp = std::chrono::steady_clock::now().time_since_epoch())
{
    return internal::getEntry<TimestampUnit>(class_name, object, "t_sys", internal::getTimestamp<TimestampUnit, TimestampPrecision>(timestamp));
}

template <class TimestampUnit = std::chrono::seconds, class TimestampPrecision = TimestampUnit, typename T>
json getEntrySysTime(const std::string &class_name, const T &object, double timestamp)
{
    return getEntrySysTime<TimestampUnit, TimestampPrecision, T>(class_name, object, std::chrono::nanoseconds(static_cast<std::chrono::nanoseconds::rep>(1e9 * timestamp)));
}

} // namespace jetlog

#endif // JETLOG_CPP_FULL_JETLOG_HPP_INCLUDED
