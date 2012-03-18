include(gameframework/gameframework.pri)

QT -= core gui

INCLUDEPATH += gameframework/

TARGET = runner

SOURCES += \
    enginestate.cpp \
    main.cpp \
    runner.cpp \
    logic/tilemapcharacter.cpp \
    logic/tilemap.cpp \
    logic/player.cpp \
    logic/runnertilemap.cpp \
    logic/generator/runnermapchunk.cpp

    
HEADERS += \
    enginestate.h \
    runner.h \
    keys.h \
    logic/tilemapcharacter.h \
    logic/tilemap.h \
    logic/player.h \
    logic/runnertilemap.h \
    logic/generator/runnermapgenerator.h \
    logic/generator/runnermapchunk.h






