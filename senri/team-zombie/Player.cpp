#include "Player.h"




Player::Player()
{
	pos = { 50,700 };
	animAdd = 0;
	jump = -12.0f;
	Jumpflag = false;
	Wireflag = false;
	mc = std::make_shared<MouseCtl>();
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
	Wire();
	SetMove();
	return true;
}

void Player::SetMove(void)
{
	memcpy(keyDataOld, keyData, sizeof(keyDataOld));
	GetHitKeyStateAll(keyData);
	animAdd = 0;
	if (Jumpflag == false && Wireflag == false)
	{
		SetAnim("歩く");
	}
	pos.x += speed;
	animAdd = 1;

	if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE] || (Jumpflag == true))
	{
		Jumpflag = true;
		pos.y += jump;
		pos.x += 1;
		jump += 0.3f;
		animAdd = 0;
		SetAnim("ジャンプ");
		animAdd = 1;
		if (Jumpflag == true && pos.y >= 600)
		{
			jump = -12.0f;
			Jumpflag = false;
		}
	}
	animCnt += animAdd;

	if (pos.x >= 1024)
	{
		pos.x = 0;
	}
}

void Player::Draw(void)
{
	Obj::Draw();
}

bool Player::Wire(void)
{
	(*mc).Update();
	VECTOR2 mPos = mc->GetPoint();
	if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT || (Wireflag == true))
	{
		Wireflag = true;
		VECTOR2 vec;
		vec.Normalize();
		vec = mPos - pos;
		pos.x += vec.x;
		pos.y +=  vec.y;
		animAdd = 0;
		//SetAnim("ジャンプ");
		animAdd = 1;
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_RIGHT)
		{
			Wireflag = false;
			pos.y -= 2;
		}
	}
	if (Wireflag == true && pos.y >= 600)
	{
		pos.y == 600;
		Wireflag = false;
	}

	return true;
}

bool Player::initAnim(void)
{
	AddAnim("歩く", 1, 0, 6, 7);
	AddAnim("ジャンプ", 0, 2, 8, 12);
	return true;
}
