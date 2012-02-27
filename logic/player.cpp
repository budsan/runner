#include "player.h"

#include <memory>

#include "log.h"
#include "graphics/graphics.h"
#include "audio/emyl.h"

#include "keys.h"
#include "input/input.h"

std::shared_ptr<SpriteAnimData> Player::s_sprData = nullptr;
std::unique_ptr<emyl::sound> Player::s_sndHdl = nullptr;
std::unique_ptr<ParticleEmitter> Player::s_runEmitter = nullptr;
std::unique_ptr<ParticleEmitter> Player::s_airJumpEmitter = nullptr;
ALuint Player::s_sndJump = 0;
ALuint Player::s_sndAirJump = 0;
ALuint Player::s_sndDash = 0;
ALuint Player::s_sndRun = 0;



Player::Player(Tilemap &parent) : TilemapCharacter(parent), m_runEmitter(nullptr)
{
	m_jumpTimeLeft = 0.0f;
	m_init = false;
	m_dashing = false;
	m_failed = false;
}

void Player::load()
{
	if (s_sprData == nullptr)
	{
		s_sprData = std::shared_ptr<SpriteAnimData> (new SpriteAnimData());
		if (!s_sprData->load("data/scripts/runner.anim")) {
			s_sprData.reset();
			//LOG << "CRITICAL: data/scripts/mario01.anim doesn't exist." << std::endl;
		}
	}

	if (s_runEmitter == nullptr) {
		s_runEmitter = std::unique_ptr<ParticleEmitter>(new ParticleEmitter());
		if (!s_runEmitter->load("data/scripts/runner_dust.emp")) {
			s_runEmitter.reset();
		}
	}

	if (s_airJumpEmitter == nullptr) {
		s_airJumpEmitter = std::unique_ptr<ParticleEmitter>(new ParticleEmitter());
		if (!s_airJumpEmitter->load("data/scripts/runner_airjump.emp")) {
			s_airJumpEmitter.reset();
		}
	}

	if (s_sndHdl == nullptr)
	{
		s_sndHdl = std::unique_ptr<emyl::sound>(new emyl::sound());
		s_sndHdl->set_source();
	}


	emyl::manager* audiomng = emyl::manager::get_instance();
	if (s_sndJump == 0) s_sndJump = audiomng->get_buffer("data/sound/jump.ogg");
	if (s_sndAirJump == 0) s_sndAirJump = audiomng->get_buffer("data/sound/airjump.ogg");
	if (s_sndRun  == 0) s_sndRun  = audiomng->get_buffer("data/sound/running.ogg");
	if (s_sndDash  == 0) s_sndDash  = audiomng->get_buffer("data/sound/running.ogg");
}

bool Player::loaded()
{
	return s_sprData != nullptr;
}

void Player::unload()
{
	s_sprData.reset();
	s_sprData.reset();
	s_sndHdl.reset();

	s_runEmitter.reset();
	s_airJumpEmitter.reset();
}

void Player::update(float deltaTime)
{
	static const math::vec2f gra_acc(   0,-1200);
	static const math::vec2f vel_run( 300,  800);
	static const math::vec2f vel_jmp(   0,  300);
	static const float jump_time = 0.2f;
	static const float dash_time = 0.5f;

	if (!m_init) reset();

	if (m_failed) {
		ensureAnim("Death");
		s_sndHdl->stop();
		m_fri = math::vec2f(1000, 0);
		m_acc = gra_acc;
		m_velLim = vel_run;
	}
	else
	{
		const InputState &state = Input::Instance().getInputState();

		if (m_grounded) {
			m_airJumpLeft = 1;
			m_groundedTime += deltaTime;
			m_groundedDash = state.getKeyState(K_DASH);
			m_dashTimeLeft = dash_time;

			if(ensureAnim("Run")) s_sndHdl->play_buffer(s_sndRun, 1);
		}
		else {
			m_groundedDash = false;
			m_groundedTime = 0;

			if (m_dashing)
			{
				ensureAnim("Dash");
			}
			else
			{
				if (m_vel.y > 0) ensureAnim("Jump");
				else ensureAnim("Fall");
			}
		}

		m_acc = math::vec2f(0,0);
		m_fri = math::vec2f(0,0);
		m_velLim = vel_run;
		m_vel.x = vel_run.x;

		if ( state.getKeyDown(K_JUMP))
		{
			if (m_grounded)
			{
				s_sndHdl->play_buffer(s_sndJump, 0);
				m_jumpTimeLeft = jump_time;
				m_dashTimeLeft = dash_time;

				hasJumped(true);

			}
			else if (m_airJumpLeft > 0)
			{
				s_sndHdl->play_buffer(s_sndAirJump, 0);
				m_airJumpLeft--;
				m_jumpTimeLeft = jump_time;
				m_dashTimeLeft = dash_time;
				m_dashing = false;

				m_airJumpEmitter->restart();
				hasAirJumped(true);
			}
		}

		if ( state.getKeyDown (K_DASH) && !m_grounded)
		{
			s_sndHdl->play_buffer(s_sndDash, 1);
			m_dashing = true;
			hasDashed(true);
		}
		if (!state.getKeyState(K_DASH) || m_dashTimeLeft <= 0)
		{
			if (m_dashing) s_sndHdl->stop();
			m_dashing = false;
		}

		if (m_dashing)
		{
			m_vel.y = 0;
			if (m_dashTimeLeft <= 0) m_acc += gra_acc;
			else m_dashTimeLeft -= deltaTime;
		}
		else
		{
			if (state.getKeyState(K_JUMP) && m_jumpTimeLeft > 0)
				m_vel.y = vel_jmp.y;
			else m_jumpTimeLeft = 0;

			if (m_jumpTimeLeft <= 0) m_acc += gra_acc;
			else m_jumpTimeLeft -= deltaTime;
		}
	}
	TilemapCharacter::update(deltaTime);

	if (m_almostFail)
	{
		m_almostFailTime += deltaTime;
		if (m_almostFailTime > 0.1)
		{
			hasFailed(true);
			m_failed = true;
		}
	}

	if (m_runEmitter == nullptr)
	{
		if (s_runEmitter != nullptr)
			m_runEmitter = std::unique_ptr<ParticleEmitter>(new ParticleEmitter(*s_runEmitter));
	}
	else {
		m_runEmitter->update(deltaTime);
	}

	if (m_runEmitter != nullptr) {
		m_runEmitter->setPosition(m_pos - math::vec2f(8,0));
		if (m_lastAnim == "Run" && m_groundedTime > 0.1f)
			m_runEmitter->setParticleNumber(-1);
		else m_runEmitter->setParticleNumber(0);

		m_runEmitter->update(deltaTime);
	}

	if (m_airJumpEmitter != nullptr) {
		m_airJumpEmitter->update(deltaTime);
	}
}

void Player::preDrawing()
{
	TilemapCharacter::preDrawing();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);	
}

void Player::postDrawing()
{
	TilemapCharacter::postDrawing();

	if (m_runEmitter != nullptr) m_runEmitter->draw();

	glPushMatrix();
	glMultMatrixf(math::mat4f::fromTranslate(m_pos).v);
	if (m_airJumpEmitter != nullptr) m_airJumpEmitter->draw();
	glPopMatrix();
}

void Player::noLeftCollision()
{

}
void Player::noRightCollision()
{
	m_almostFailTime = 0;
	m_almostFail = false;
}

void Player::noUpCollision()
{
	m_grounded = false;
}

void Player::noDownCollision()
{
	m_dashing = m_groundedDash;
	m_grounded = false;
}


bool Player::onLeftCollision(int x, int j)
{
	m_vel.x = 0;
	return true;
}

bool Player::onRightCollision(int x, int j)
{
	m_almostFail = true;
	m_vel.x = 0;
	return true;
}

bool Player::onUpCollision(int x, int j)
{
	m_vel.y = 0;
	m_jumpTimeLeft = 0;
	m_grounded = false;
	return true;
}

bool Player::onDownCollision(int x, int j)
{
	m_vel.y = 0;
	m_grounded = true;
	return true;
}

void Player::reset()
{
	m_init = true;
	m_pos = math::vec2f(0,16);
	m_vel = math::vec2f(0,0);
	m_dashing = false;
	m_groundedTime = 0;
	m_failed = false;

	if (s_sprData != nullptr) {
		TilemapCharacter::setAnimData(s_sprData);
		TilemapCharacter::ensureAnim("Run");
	}

	if (m_runEmitter == nullptr) {
		if (s_runEmitter != nullptr)
			m_runEmitter = std::unique_ptr<ParticleEmitter>(new ParticleEmitter(*s_runEmitter));
	}

	if (m_airJumpEmitter == nullptr) {
		if (s_airJumpEmitter != nullptr)
			m_airJumpEmitter = std::unique_ptr<ParticleEmitter>(new ParticleEmitter(*s_airJumpEmitter));
	}
}
