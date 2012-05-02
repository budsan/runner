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

    void preUpdate(float deltaTime);
    void postDraw();

    const char *getName();
    const char *getVersion();

    void onGainInputFocus();
    void onLoseInputFocus();

    FramesHUD frames;
};

