QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

SOURCES += \
    graphic_scene.cpp \
    main.cpp \
    main_window.cpp \
    model_3dobject.cpp \
    objfile_reader.cpp \
    sunsystem_scene.cpp

HEADERS += \
    graphic_scene.h \
    main_window.h \
    model_3dobject.h \
    objfile_reader.h \
    sst_exeption.h \
    sunsystem_scene.h

TRANSLATIONS += \
    SunSystemTerminal_en_US.ts

DISTFILES += \
    phong.frag \
    phong.vert

RESOURCES += \
    resources.qrc
