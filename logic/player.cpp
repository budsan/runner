#include "player.h"

#include <memory>

#include "log.h"
#include "audio/emyl.h"

#include "keys.h"
#include "input/input.h"

std::shared_ptr<SpriteAnimData> Player::s_sprData = nullptr;
std::unique_ptr<emyl::sound> Player::s_sndHdl = nullptr;
ALuint Player::s_sndJump = 0;


Player::Player(Tilemap &parent) : TilemapCharacter(parent)
{
	if (s_sprData == nullptr) load();
	SpriteAnim::setAnimData(s_sprData[player]);
	SpriteAnim::ensureAnim("Run");
	m_jumpTimeLeft = 0.0f;
}

void Player::load()
{
	if (s_sprData == nullptr)
	{
		s_sprData = std::shared_ptr<SpriteAnimData> (new SpriteAnimData());
		if (!s_sprData->load("data/scripts/runner.anim")) {
			delete s_sprData;
			s_sprData = nullptr;
			//LOG << "CRITICAL: data/scripts/mario01.anim doesn't exist." << std::endl;
		}
	}

	if (emitter == nullptr) {
		emitter = std::unique_ptr<ParticleEmitter>(new ParticleEmitter());
		if (!emitter->load("data/scripts/runner_dust.emp")) {
			delete emitter;
			emitter = nullptr;
		}
	}

	if (s_sndHdl == nullptr)
	{
		s_sndHdl = std::unique_ptr<emyl::sound>(new emyl::sound());
		s_sndHdl->set_source();
	}

	emyl::manager* audiomng = emyl::manager::get_instance();
	if (s_sndJump == 0) s_sndJump = audiomng->get_buffer("data/sound/jump.ogg");
}

bool Player::loaded()
{
	return s_sprData != nullptr;
}

void Player::unload()
{
	if (s_sprData[0] != nullptr)
	{
		delete s_sprData[0];
		s_sprData[0] = nullptr;
	}

	if (s_sprData[1] != nullptr)
	{
		delete s_sprData[1];
		s_sprData[1] = nullptr;
	}

	if (s_sndHdl != nullptr)
	{
		delete s_sndHdl;
		s_sndHdl = nullptr;
	}
}

void Player::update(float deltaTime)
{
	InputState state = Input::Instance().getInputState(player);
	const math::vec2f gra_acc(   0 -1200);
	const math::vec2f vel_run( 300,  300);
	const math::vec2f vel_jmp( 300,  300);
	const float jump_time = 0.3f;

	if (m_grounded) ensureAnim("Run");
	else
	{
		if (m_vel.y > 0) ensureAnim("Jump");
		else ensureAnim("Fall");
	}

	m_acc = math::vec2f(0,0);
	m_fri = math::vec2f(0,0);
	m_velLim = vel_run;
	m_vel.x = vel_run.x;

	if ( state.getKeyDown(K_JUMP) && m_grounded)
	{
		s_sndHdl->play_buffer(s_sndJump);
		m_jumpTimeLeft = jump_time;
	}

	if (state.getKeyState(K_JUMP) && m_jumpTimeLeft > 0)
		m_vel.y = vel_jmp.y;
	else m_jumpTimeLeft = 0;

	if (m_jumpTimeLeft <= 0) m_acc += gra_acc; //La gravedad afecta mientras no salte
	else m_jumpTimeLeft -= deltaTime; //Se le resta el tiempo mientras salta

	TilemapCharacter::update(deltaTime);

	if (emitter != nullptr) emitter->update(deltaTime);
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
	if (emitter != nullptr) emitter->draw();	
}

void Player::noLeftCollision()
{

}
void Player::noRightCollision()
{

}

void Player::noUpCollision()
{
	m_grounded = false;
}

void Player::noDownCollision()
{
	m_grounded = false;
}


bool Player::onLeftCollision(int x, int j)
{
	m_vel.x = 0;
	return true;
}

bool Player::onRightCollision(int x, int j)
{
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
	m_vel = math::vec2f(0,0);
}
