#pragma once
#include "gameframework/game.h"
#include "gameframework/graphics/tools/frameshud.h"

class Runner : public Game
{
public:
    Runner();

protected:
    void configure();

    void   load();
    void unload();

    void update(float deltaTime);
    void draw();

    const char *getName();
    const char *getVersion();

    FramesHUD frames;
};

