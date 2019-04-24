#include "Player.h"
#include "Dxlib.h"




Player::Player()
{
	pos = { 100,100 };
}

//Player::Player(const char(&_keyData)[256], const char(&_keyDataOld)[256], VECTOR2 chipOffset)
//	:Obj(_keyData, _keyDataOld, chipOffset)
//{
//}


Player::~Player()
{
}

bool Player::Update(void)
{
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		pos.x+=speed;
	}
	return true;
}

void Player::Draw(void)
{
	Obj::Draw(0);
}
