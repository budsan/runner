#pragma once

#include <memory>
#include <functional>

#include "audio/emyl.h"
#include "tools/particleemitter.h"

#include "tilemapcharacter.h"

class World;
class Player : public TilemapCharacter
{
public:
	Player(Tilemap &parent);
	virtual void update(double deltaTime);

	static void load();
	static bool loaded();
	static void unload();
	void reset();

	bool failed() { return m_failed; }

	std::function<void()> hasFailed;
	std::function<void()> hasJumped;
	std::function<void()> hasAirJumped;
	std::function<void()> hasDashed;

private:

	static std::shared_ptr<Guy::SpriteAnimData> s_sprData;
	static std::shared_ptr<emyl::sound> s_sndHdl;
	static std::shared_ptr<Guy::ParticleEmitter> s_runEmitter;
	static std::shared_ptr<Guy::ParticleEmitter> s_airJumpEmitter;
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
	double m_almostFailTime;
	double m_groundedTime;
	double m_jumpTimeLeft;
	double m_dashTimeLeft;

	std::shared_ptr<Guy::ParticleEmitter> m_runEmitter;
	std::shared_ptr<Guy::ParticleEmitter> m_airJumpEmitter;

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

