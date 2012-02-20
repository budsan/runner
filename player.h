#pragma once

#include "gameframework/graphics/graphics.h"
#include "gameframework/graphics/spriteanim.h"

class Player : public SpriteAnim
{
public:
    Player();

    bool Load();
    bool Loaded();
    virtual void Update(float GameTime);

protected:

    SpriteAnimData *data;
    virtual void preDrawing();
};

