include(guyframework/guyframework.pri)

QT -= core gui

INCLUDEPATH += guyframework/

DEFINES += _DEBUG

TARGET = runner

SOURCES += \
    main.cpp \
    runner.cpp \
    logic/tilemapcharacter.cpp \
    logic/tilemap.cpp \
    logic/player.cpp \
    logic/runnertilemap.cpp \
    logic/generator/runnermapchunk.cpp

    
HEADERS += \
    runner.h \
    logic/tilemapcharacter.h \
    logic/tilemap.h \
    logic/player.h \
    logic/runnertilemap.h \
    logic/generator/runnermapgenerator.h \
    logic/generator/runnermapchunk.h \
    logic/action.h






