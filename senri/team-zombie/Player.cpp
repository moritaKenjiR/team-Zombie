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
	animAdd = 0;
	SetAnim("����");
	pos.x+=speed;
	animAdd = 1;
	if (CheckHitKey(KEY_INPUT_RIGHT))
	{
		animAdd = 0;
		SetAnim("�W�����v");
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
	AddAnim("����", 0, 0, 4, 8);
	AddAnim("�W�����v", 0, 1, 4, 8);
	return true;
}
