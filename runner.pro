include(gameframework/gameframework.pri)

INCLUDEPATH += gameframework/

TARGET = runner

SOURCES += \
    enginestate.cpp \
    main.cpp \
    runner.cpp \
    player.cpp

    
HEADERS += \
    enginestate.h \
    runner.h \
    keys.h \
    player.h



