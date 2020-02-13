# Jetlog - JSON-based extendible logging

DRAFT - Comments are welcome.

A specification for an extendible log format based on Newline Delimited JSON ([NDJSON](https://github.com/ndjson/ndjson-spec)).

## 1. Introduction

This specification was created because there is currently no standard for an extendible log format that can be read by both humans and machines.

Jetlog is based on Newline Delimited JSON ([NDJSON](https://github.com/ndjson/ndjson-spec)).

## 2. Example

```
{"t_sys":2.384, "msg":"System ready", "severity":"debug"}
{"t_sys":2484, "t_unit":"ms", "msg":"System still ready", "severity":"debug"}
{"t":"2020-02-28T15:11:23.000+01", "msg":"Connected to server", "severity":"info", "source":"client.connection_manager"}
{"t":"2020-02-28T15:11:23.050+01", "class":"transmission_start", "stream_id":1, "file":"readme.txt", "encrypted":true}
{"t":"2020-02-28T15:11:26.000+01", "class":"transmission_done", "stream_id":1}
{"t_unix":1582902690.800, "msg":"Connection lost", "severity":"error"}
```

`\n` is used for line separation.

## 3. Specification

### 3.1 Terminology

The key words "MUST", "MUST NOT", "REQUIRED", "SHALL", "SHALL NOT", "SHOULD", "SHOULD NOT", "RECOMMENDED",  "MAY", and "OPTIONAL" in this document are to be interpreted as described in [RFC 2119](https://tools.ietf.org/html/rfc2119).

### 3.2 General requirements

In general, the document MUST be NDJSON compliant.

In addition, the following conditions MUST be met:

1. Each line MUST contain one and only one JSON object.
2. The keys in each JSON object MUST be unique.
3. Each JSON object MUST contain a timestamp in one of the formats specified below.
4. Each JSON object MUST contain a key/value pair with `class` as key and a string as value, unless the class is `log`. If the class is `log`, the explicit specification SHOULD be omitted.
5. The value string for the key `class` MAY have any value. If its value is `log`, the conditions given below MUST be met.
6. In each line, the timestamp SHOULD be in the first position, followed by the timestamp unit, if necessary. If the class is `log`, the field `msg` SHOULD follow, otherwise `class` SHOULD follow.

Reserved keys, which MUST NOT be used in any other way than specified in this document on the root level of the JSON object, are:

- `t`
- `t_unix`
- `t_sys`
- `t_unit`
- `class`

### 3.3 Timestamps

Three different types of timestamps are supported:

#### 3.3.1 ISO 8601 timestamps

A timestamp might be encoded in [ISO 8601](https://en.wikipedia.org/wiki/ISO_8601) format. Example:

```
"t":"2020-02-28T15:11:26.000+01"
```

Decimal fractions SHOULD be supported. They MUST NOT cause parsing errors.

The ISO 8601 format is the RECOMMENDED format for timestamps.

#### 3.3.2 Unix epoch timestamps

Another option is to use timestamps based on the Unix epoch (seconds since 1970-01-01T00:00:00Z). Example:

```
"t_unix":1582902690.800
```

The default unit is seconds, but other units MAY be specified. See the section on timestamp units.

#### 3.3.3 System timestamps

For systems that do not have access to an absolute time base, a system timestamp SHALL be used. Example:

```
"t_sys":2.384
```

The default unit is seconds, but other units MAY be specified. See the section on timestamp units.

#### 3.3.4 Timestamp units

If a unit other than seconds is used, it MUST be specified with the `timestamp_unit` key. The following units MUST be supported:

- `s` Seconds. This is the default value and SHOULD be omitted.
- `ms` Milliseconds.
- `us` Microseconds.
- `ns` Nanoseconds.

If the timestamp unit is set to seconds, decimal fractions SHOULD be supported. They MUST NOT cause parsing errors. If a unit other than seconds is used, decimal fraction MUST NOT be used.

Example:

```
{"t_unix":1582902690800, "t_unit":"ms", "msg":"Connection lost", "severity":"error"}
{"t_sys":2384405, "t_unit":"us", "msg":"System ready", "severity":"debug"}
```

### 3.4 The `log` class

If the class of a JSON object is set to `log`, the object

1. MUST have a key/value pair with `msg` as key and a string as value.
2. SHOULD have a key/value pair with `severity` as key and a value as specified below.
3. MAY have a key/value pair with `source` as key and a value as specified below.
4. MUST NOT use these keywords in any other way than specified here.

#### 3.4.1 Severity levels

The supported severity levels correspond to [RFC 5424](https://tools.ietf.org/html/rfc5424).

For the `log` class, the key/value pair with `severity` as key MUST have one of the following strings as value:

0. `emergency`: system is unusable
1. `alert`: action must be taken immediately
2. `critical`: critical conditions
3. `error`: error conditions
4. `warning`: warning conditions
5. `notice`: normal but significant condition
6. `info`: informational messages
7. `debug`: debug-level messages

#### 3.4.2 Source specification

For the `log` class, the key/value pair with `source` as key MUST have a string as value. That string corresponds to the way loggers are named in e.g. Java or ROS 2:

Logger names represent a hierarchy. Example: The root logger may have the name `client`, then its child `connection_manager` would have the name `client.connection_manager`. Therefore, the name of a logger MUST contain only characters [a-z], [A-Z], numbers [0-9], underscore `_` and dot `.`. The dot `.` MUST be used only to define a logger hierarchy. In addition, the name MUST start with a character from the set [a-z] or [A-Z].

## 4. Copyright

This specification is copyrighted by Bundesanstalt für Materialforschung und -prüfung (BAM). It is free to use for any purposes commercial or non-commercial.
