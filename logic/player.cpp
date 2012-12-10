#include "player.h"
#include "actionsrunner.h"
#include "runner.h"

#include "guyframework/environment.h"
#include "guyframework/log.h"
#include "guyframework/graphics/graphics.h"
#include "guyframework/audio/emyl.h"

#include <memory>

boost::shared_ptr<Guy::SpriteAnimData> Player::s_sprData;
boost::shared_ptr<emyl::sound> Player::s_sndHdl;
boost::shared_ptr<Guy::ParticleEmitter> Player::s_runEmitter;
boost::shared_ptr<Guy::ParticleEmitter> Player::s_airJumpEmitter;
ALuint Player::s_sndJump = 0;
ALuint Player::s_sndAirJump = 0;
ALuint Player::s_sndDash = 0;
ALuint Player::s_sndRun = 0;

Player::Player(Tilemap &parent)
	: TilemapCharacter(parent), hasFailed(NULL), hasJumped(NULL),
	  hasAirJumped(NULL), hasDashed(NULL), m_runEmitter()
{
	m_jumpTimeLeft = 0.0f;
	m_init = false;
	m_dashing = false;
	m_failed = false;
	m_grounded = false;
	this->setScale(0.375f/1.0f);
}

void Player::load() {
	if (s_sprData == NULL) {
		s_sprData = boost::shared_ptr < Guy::SpriteAnimData > (new Guy::SpriteAnimData());
		if (!s_sprData->load("data/scripts/runner128.anim")) {
			s_sprData.reset();
			//std::cout << "CRITICAL: data/scripts/mario01.anim doesn't exist." << std::endl;
		}

	}

	if (s_runEmitter == NULL) {
		s_runEmitter = boost::shared_ptr < Guy::ParticleEmitter
			       > (new Guy::ParticleEmitter());
		if (!s_runEmitter->load("data/scripts/runner_dust.emp")) {
			s_runEmitter.reset();
			//std::cout  << "CRITICAL: data/scripts/runner_dust.emp doesn't exist." << std::endl;
		}
	}

	if (s_airJumpEmitter == NULL) {
		s_airJumpEmitter = boost::shared_ptr < Guy::ParticleEmitter
				   > (new Guy::ParticleEmitter());
		if (!s_airJumpEmitter->load("data/scripts/runner_airjump.emp")) {
			s_airJumpEmitter.reset();
			//std::cout  << "CRITICAL: data/scripts/runner_airjump.emp doesn't exist." << std::endl;
		}
	}

	if (s_sndHdl == NULL) {
		s_sndHdl = boost::shared_ptr < emyl::sound > (new emyl::sound());
		s_sndHdl->set_source();
	}

	emyl::manager* audiomng = emyl::manager::get_instance();
	if (s_sndJump == 0)
		s_sndJump = audiomng->get_buffer("data/sound/jump.ogg");
	if (s_sndAirJump == 0)
		s_sndAirJump = audiomng->get_buffer("data/sound/airjump.ogg");
	if (s_sndRun == 0)
		s_sndRun = audiomng->get_buffer("data/sound/running.ogg");
	if (s_sndDash == 0)
		s_sndDash = audiomng->get_buffer("data/sound/flying.ogg");
}

bool Player::loaded() {
	return s_sprData != NULL;
}

void Player::unload() {
	s_sprData.reset();
	s_sprData.reset();
	s_sndHdl.reset();

	s_runEmitter.reset();
	s_airJumpEmitter.reset();
}

void Player::update(float deltaTime) {
	static const math::vec2f gra_acc(0, -1800);
	static const math::vec2f vel_run(450, 1200);
	static const math::vec2f vel_jmp(0, 450);
	static const float jump_time = 0.2f;
	static const float dash_time = 0.5f;

	if (!m_init)
		reset();

	if (m_failed) {
		ensureAnim("Death");
		s_sndHdl->stop();
		m_fri = math::vec2f(1000, 0);
		m_acc = gra_acc;
		m_velLim = vel_run;
		if(m_vel.y > 0) m_vel.y = 0;
	} else {
		const Actions& state = *Actions::instance()[0];

		if (m_grounded) {
			m_airJumpLeft = 1;
			m_groundedTime += deltaTime;
			m_groundedDash = state.isPressed(ActionsRunner::Dash);
			m_dashTimeLeft = dash_time;
			m_dashing = false;

			if (ensureAnim("Run") || !s_sndHdl->playing())
				s_sndHdl->play_buffer(s_sndRun, 1);
		} else {
			m_groundedDash = false;
			m_groundedTime = 0;

			if (m_dashing) {
				ensureAnim("Dash");
			} else {
				if (m_vel.y > 0)
					ensureAnim("Jump");
				else
					ensureAnim("Fall");
			}
		}

		m_acc = math::vec2f(0, 0);
		m_fri = math::vec2f(0, 0);
		m_velLim = vel_run;
		m_vel.x = vel_run.x;

		if (state.isDown(ActionsRunner::Jump)) {
			if (m_grounded) {
				s_sndHdl->play_buffer(s_sndJump, 0);
				m_jumpTimeLeft = jump_time;
				m_dashTimeLeft = dash_time;

				if (hasJumped) hasJumped();

			} else if (m_airJumpLeft > 0) {
				s_sndHdl->play_buffer(s_sndAirJump, 0);
				m_airJumpLeft--;
				m_jumpTimeLeft = jump_time;
				m_dashTimeLeft = dash_time;
				m_dashing = false;

				m_airJumpEmitter->restart();
				if(hasAirJumped) hasAirJumped();
			}
		}

		if (state.isDown(ActionsRunner::Dash) && !m_grounded) {
			s_sndHdl->play_buffer(s_sndDash, 1);
			m_dashing = true;
			if (hasDashed) hasDashed();
		}
		if (!state.isPressed(ActionsRunner::Dash) || m_dashTimeLeft <= 0) {
			if (m_dashing)
				s_sndHdl->stop();
			m_dashing = false;
		}

		if (m_dashing) {
			m_vel.y = 0;
			if (m_dashTimeLeft <= 0)
				m_acc += gra_acc;
			else
				m_dashTimeLeft -= deltaTime;
		} else {
			if (state.isPressed(ActionsRunner::Jump) && m_jumpTimeLeft > 0)
				m_vel.y = vel_jmp.y;
			else
				m_jumpTimeLeft = 0;

			if (m_jumpTimeLeft <= 0)
				m_acc += gra_acc;
			else
				m_jumpTimeLeft -= deltaTime;
		}
	}
	TilemapCharacter::update(deltaTime);

	if (m_almostFail) {
		m_almostFailTime += deltaTime;
		if (m_almostFailTime > 0.1) {
			if(hasFailed) hasFailed();
			m_failed = true;
		}
	}

	if (m_runEmitter == NULL) {
		if (s_runEmitter != NULL)
			m_runEmitter = boost::shared_ptr < Guy::ParticleEmitter
				       > (new Guy::ParticleEmitter(*s_runEmitter));
	} else {
		m_runEmitter->update(deltaTime);
	}

	if (m_runEmitter != NULL) {
		m_runEmitter->setPosition(m_pos - math::vec2f(8, 0));
		if (m_lastAnim == "Run" && m_groundedTime > 0.1f)
			m_runEmitter->setParticleNumber(-1);
		else
			m_runEmitter->setParticleNumber(0);

		m_runEmitter->update(deltaTime);
	}

	if (m_airJumpEmitter != NULL) {
		m_airJumpEmitter->update(deltaTime);
	}
}

void Player::preDrawing() {
	TilemapCharacter::preDrawing();
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}

void Player::postDrawing() {
	TilemapCharacter::postDrawing();

	if (m_runEmitter != NULL)
		m_runEmitter->draw();

	glPushMatrix();
	glMultMatrixf(math::mat4f::fromTranslate(m_pos).v);
	if (m_airJumpEmitter != NULL)
		m_airJumpEmitter->draw();
	glPopMatrix();
}

void Player::noLeftCollision() {

}
void Player::noRightCollision() {
	m_almostFailTime = 0;
	m_almostFail = false;
}

void Player::noUpCollision() {
	m_grounded = false;
}

void Player::noDownCollision() {
	m_dashing = m_groundedDash;
	m_grounded = false;
}

bool Player::onLeftCollision(int x, int j) {
	m_vel.x = 0;
	return true;
}

bool Player::onRightCollision(int x, int j) {
	m_almostFail = true;
	m_vel.x = 0;
	return true;
}

bool Player::onUpCollision(int x, int j) {
	m_vel.y = 0;
	m_jumpTimeLeft = 0;
	m_grounded = false;
	return true;
}

bool Player::onDownCollision(int x, int j) {
	m_vel.y = 0;
	m_grounded = true;
	return true;
}

void Player::reset() {
	m_init = true;
	m_pos = math::vec2f(0, 0);
	m_vel = math::vec2f(0, 0);
	m_dashing = false;
	m_groundedTime = 0;
	m_failed = false;

	if (s_sprData != NULL) {
		TilemapCharacter::setAnimData(s_sprData);
		TilemapCharacter::ensureAnim("Run");
	}

	if (m_runEmitter == NULL) {
		if (s_runEmitter != NULL)
			m_runEmitter = boost::shared_ptr < Guy::ParticleEmitter
				       > (new Guy::ParticleEmitter(*s_runEmitter));
	}

	if (m_airJumpEmitter == NULL) {
		if (s_airJumpEmitter != NULL)
			m_airJumpEmitter = boost::shared_ptr < Guy::ParticleEmitter
					   > (new Guy::ParticleEmitter(*s_airJumpEmitter));
	}
}
