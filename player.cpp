#include "player.h"

Player::Player() : data(nullptr)
{

}

bool Player::Load()
{
	if (data == nullptr) {
		data = new SpriteAnimData();
		if (!data->Load("data/scripts/runner.anim")) {
			delete data;
			data = nullptr;
		}
		else {
			SpriteAnim::setAnimData(data);
			SpriteAnim::SelectAnim("Run");
		}
	}

	return Loaded();
}

bool Player::Loaded()
{
	return data != nullptr;
}

void Player::Update(float GameTime)
{
	SpriteAnim::Update(GameTime);
}

void Player::preDrawing()
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
}
