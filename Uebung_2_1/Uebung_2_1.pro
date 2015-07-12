#-------------------------------------------------
#
# Project created by QtCreator 2015-07-01T13:51:40
#
#-------------------------------------------------

QT       += core

QT       -= gui

TARGET = Uebung_2_1
CONFIG   += console
CONFIG   -= app_bundle
CONFIG   += c++11

QMAKE_CXXFLAGS += -std=c++11

TEMPLATE = app


SOURCES += \
    test_fix_point_collection.cpp \
    fix_point.cpp \
    fix_point_collection.cpp

HEADERS += \
    fix_point.hpp \
    fix_point_collection.hpp
