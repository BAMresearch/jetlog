TEMPLATE = app
CONFIG += console c++17
CONFIG -= app_bundle
CONFIG -= qt

DEFINES += "__cplusplus=201703L"

include(jetlog_cpp_full.pri)

SOURCES += \
    src/example.cpp
