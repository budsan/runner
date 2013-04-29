include(guyframework/guyframework.pri)

INCLUDEPATH += .

DEFINES += _DEBUG

TARGET = runner

SOURCES += \
    runner.cpp \
    logic/tilemapcharacter.cpp \
    logic/tilemap.cpp \
    logic/player.cpp \
    logic/runnertilemap.cpp \
    logic/generator/runnermapchunk.cpp \
    action.cpp \
    actionsrunner.cpp

    
HEADERS += \
    runner.h \
    action.h \
    logic/tilemapcharacter.h \
    logic/tilemap.h \
    logic/player.h \
    logic/runnertilemap.h \
    logic/generator/runnermapgenerator.h \
    logic/generator/runnermapchunk.h \
    actionsrunner.h






