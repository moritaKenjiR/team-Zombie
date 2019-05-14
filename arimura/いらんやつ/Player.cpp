#include "Player.h"
#include "Dxlib.h"
#include "GameTask.h"
#include "VECTOR2.h"

Player::Player()
{
	//pos = { 50,700 };
	animAdd = 0;
	jumpFlag = false;
	jump = -9.0f;
	grav = 0.3f;
	locate = { 50,50 };
	Init();
}

//Player::Player(const char(&_keyData)[256], const char(&_keyDataOld)[256], VECTOR2 chipOffset)
//	:Obj(_keyData, _keyDataOld, chipOffset)
//{
//}


Player::~Player()
{
}

int Player::Init()
{
	mouseCtl = std::make_shared<MouseCtl>();
	return 0;
}

bool Player::Update(void)
{
	memcpy(keyDataOld, keyData, sizeof(keyDataOld));
	GetHitKeyStateAll(keyData);
	SetAnim("歩く");
	//pos.x+=speed;
	animAdd = 1;

	WireMove();
	if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE] || (jumpFlag == true))
	{
		jumpFlag = true;
		pos.y += jump;
		jump += grav;
		if ((pos.y == 600) &&(jumpFlag == true))
		{
			jump = -9.0f;
			jumpFlag = false;
		}
		animAdd = 0;
		SetAnim("ジャンプ");
	}
	

	animCnt += animAdd;
	return true;
}

void Player::Draw(void)
{
	Obj::Draw();
}

void Player::WireMove()
{
	bool wireFlag = false;
	(*mouseCtl).Update();
	
	
	if ((mouseCtl->GetBtn()[ST_NOW]) & (~mouseCtl->GetBtn()[ST_OLD]) & MOUSE_INPUT_RIGHT)
	{
		VECTOR2 mpos = mouseCtl->GetPoint();
		VECTOR2 slope;
		
		slope.x = mpos.x - pos.x;
		slope.y = mpos.y - pos.y;
		slope.normalize();

		pos.x += slope.fx * 50;
		pos.y += slope.fy * 50;
	}
}




bool Player::initAnim(void)
{
	AddAnim("歩く", 1, 0, 6, 50);
	AddAnim("ジャンプ", 0, 2, 8, 50);
	return true;
}
