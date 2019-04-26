#include "Player.h"
#include "Dxlib.h"




Player::Player()
{
	pos = { 50,700 };
	animAdd = 0;
	jump = -12.0f;
	Jumpflag = false;
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
	animAdd = 0;
	if (Jumpflag == false)
	{
		SetAnim("歩く");
	}
	pos.x += speed;
	animAdd = 1;

	if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE] || (Jumpflag == true))
	{
		Jumpflag = true;
		pos.y += jump;
		jump += 0.3f;
		animAdd = 0;
		SetAnim("ジャンプ");
		animAdd = 1;
		if (Jumpflag == true && pos.y == 600)
		{
			jump = -12.0f;
			Jumpflag = false;
		}
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
	AddAnim("ジャンプ", 0, 2, 8, 11);
	return true;
}
