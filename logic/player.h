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
	static std::unique_ptr<ParticleEmitter> s_runEmitter;
	static std::unique_ptr<ParticleEmitter> s_airJumpEmitter;
	static ALuint s_sndJump;

	bool m_init;
	bool m_grounded;
	int m_airJumpLeft;
	float m_jumpTimeLeft;
	float m_dashTimeLeft;
	std::unique_ptr<ParticleEmitter> m_runEmitter;
	std::unique_ptr<ParticleEmitter> m_airJumpEmitter;

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

