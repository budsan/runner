#pragma once
#include "gameframework/game.h"
#include "gameframework/graphics/tools/frameshud.h"

class Runner : public Game
{
public:
    Runner();

protected:
    void Configure();

    void   Load();
    void Unload();

    void Update(float GameTime);
    void Draw();

    const char *getName();
    const char *getVersion();

    FramesHUD frames;
};

