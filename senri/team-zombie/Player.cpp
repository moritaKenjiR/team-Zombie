#include "Player.h"
#include "Dxlib.h"




Player::Player()
{
	pos = { 50,700 };
	animAdd = 0;
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
	memcpy(keyDataOld, keyData, sizeof(keyDataOld));
	GetHitKeyStateAll(keyData);

	if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])
	{
		animAdd = 0;
		pos.x += speed;
		SetAnim("ジャンプ");
	}
	else
	{
		animAdd = 0;
		SetAnim("歩く");
		pos.x += speed;
		animAdd = 1;
	}
	animCnt += animAdd;
	return true;
}

void Player::Draw(void)
{
	Obj::Draw();
}

bool Player::initAnim(void)
{
	AddAnim("歩く", 1, 0, 6, 7);
	AddAnim("ジャンプ", 0, 2, 8, 7);
	return true;
}
