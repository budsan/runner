include(gameframework/gameframework.pri)

INCLUDEPATH += gameframework/

TARGET = runner

SOURCES += \
    enginestate.cpp \
    main.cpp \
    runner.cpp \
    logic/tilemapcharacter.cpp \
    logic/tilemap.cpp \
    logic/player.cpp \
    logic/runnertilemap.cpp

    
HEADERS += \
    enginestate.h \
    runner.h \
    keys.h \
    logic/tilemapcharacter.h \
    logic/tilemap.h \
    logic/player.h \
    logic/runnertilemap.h






