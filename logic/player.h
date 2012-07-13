#pragma once

#include "boost/shared_ptr.hpp"
#include "boost/bind.hpp"
#include "boost/function.hpp"

#include "audio/emyl.h"
#include "tools/particleemitter.h"

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

	boost::function<void()> hasFailed;
	boost::function<void()> hasJumped;
	boost::function<void()> hasAirJumped;
	boost::function<void()> hasDashed;

private:

	static boost::shared_ptr<Guy::SpriteAnimData> s_sprData;
	static boost::shared_ptr<emyl::sound> s_sndHdl;
	static boost::shared_ptr<Guy::ParticleEmitter> s_runEmitter;
	static boost::shared_ptr<Guy::ParticleEmitter> s_airJumpEmitter;
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

	boost::shared_ptr<Guy::ParticleEmitter> m_runEmitter;
	boost::shared_ptr<Guy::ParticleEmitter> m_airJumpEmitter;

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

