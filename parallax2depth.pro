TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS += \
    parallax2depth.hpp \
    readflow.hpp

INCLUDEPATH += /usr/include/opencv

LIBS += /usr/lib/x86_64-linux-gnu/libopencv_highgui.so.2.4.9
LIBS += /usr/lib/x86_64-linux-gnu/libopencv_core.so.2.4.9

