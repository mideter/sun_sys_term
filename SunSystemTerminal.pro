QT       += core gui widgets

CONFIG += c++11

SOURCES += \
    graphic_scene.cpp \
    main.cpp \
    model_3dobject.cpp \
    objfile_reader.cpp \
    skybox.cpp \
    sst_window.cpp \
    sunsystem_scene.cpp

HEADERS += \
    graphic_scene.h \
    model_3dobject.h \
    objfile_reader.h \
    skybox.h \
    sst_exception.h \
    sst_window.h \
    sunsystem_scene.h

TRANSLATIONS += \
    SunSystemTerminal_en_US.ts

DISTFILES += \
    fragment.shader \
    phong.vert \
    vertex.shader

RESOURCES += \
    resources.qrc
