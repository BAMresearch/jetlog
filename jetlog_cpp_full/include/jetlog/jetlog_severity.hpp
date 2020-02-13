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

#ifndef JETLOG_CPP_FULL_JETLOG_SEVERITY_HPP_INCLUDED
#define JETLOG_CPP_FULL_JETLOG_SEVERITY_HPP_INCLUDED

#include <stdexcept>
#include <string>
#include <type_traits>

namespace jetlog {

enum class Severity { Emergency, Alert, Critical, Error, Warning, Notice, Info, Debug };
constexpr Severity Emergency = Severity::Emergency;
constexpr Severity Alert = Severity::Alert;
constexpr Severity Critical = Severity::Critical;
constexpr Severity Error = Severity::Error;
constexpr Severity Warning = Severity::Warning;
constexpr Severity Notice = Severity::Notice;
constexpr Severity Info = Severity::Info;
constexpr Severity Debug = Severity::Debug;

namespace internal {

std::string toString(Severity severity)
{
    switch (severity)
    {
    case Severity::Debug:     return "debug";
    case Severity::Info:      return "info";
    case Severity::Notice:    return "notice";
    case Severity::Warning:   return "warning";
    case Severity::Error:     return "error";
    case Severity::Critical:  return "critical";
    case Severity::Alert:     return "alert";
    case Severity::Emergency: return "emergency";
    default: throw std::invalid_argument("Invalid severity: " + std::to_string(static_cast<std::underlying_type<Severity>::type>(severity)));
    }
}

} // namespace internal
} // namespace jetlog

#endif // JETLOG_CPP_FULL_JETLOG_SEVERITY_HPP_INCLUDED
