#pragma once

#include <memory>

#include "audio/emyl.h"
#include "gameframework/tools/particleemitter.h"
#include "tilemapcharacter.h"

class World;
class Player : public TilemapCharacter
{
public:
    Player(Tilemap &parent);
    virtual void update(float deltaTime);

    static void load();
    static bool loaded();
    static void unload();
    void reset();
private:

    static std::shared_ptr<SpriteAnimData> s_sprData;
    static std::unique_ptr<emyl::sound> s_sndHdl;
    static ALuint s_sndJump;

    bool m_grounded;
    float m_jumpTimeLeft;
    std::unique_ptr<ParticleEmitter> emitter;

protected:

    virtual void preDrawing();
    virtual void postDrawing();

    virtual void noLeftCollision();
    virtual void noRightCollision();
    virtual void noUpCollision();
    virtual void noDownCollision();

    virtual bool onLeftCollision(int x, int j);
    virtual bool onRightCollision(int x, int j);
    virtual bool onUpCollision(int x, int j);
    virtual bool onDownCollision(int x, int j);
};

