#pragma once

#include <memory>

#include "audio/emyl.h"
#include "tools/particleemitter.h"
#include "generic/event.h"

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

	bool failed() { return m_failed; }

	Event<bool> hasFailed;
	Event<bool> hasJumped;
	Event<bool> hasAirJumped;
	Event<bool> hasDashed;

private:

	static std::shared_ptr<SpriteAnimData> s_sprData;
	static std::unique_ptr<emyl::sound> s_sndHdl;
	static std::unique_ptr<ParticleEmitter> s_runEmitter;
	static std::unique_ptr<ParticleEmitter> s_airJumpEmitter;
	static ALuint s_sndJump;
	static ALuint s_sndAirJump;
	static ALuint s_sndDash;
	static ALuint s_sndRun;

	bool m_init;
	bool m_grounded;
	bool m_dashing;
	bool m_almostFail;
	bool m_failed;
	bool m_groundedDash;

	int m_airJumpLeft;
	float m_almostFailTime;
	float m_groundedTime;
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

