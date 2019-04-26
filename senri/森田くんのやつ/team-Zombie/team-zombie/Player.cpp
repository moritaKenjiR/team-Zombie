#include "Player.h"
#include "MapCtl.h"



Player::Player()
{
	pos = { 50,700 };
	animAdd = 0;
	jump = -12.0f;
	Jumpflag = false;
	Readyflag = false;
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
	//うまくいってない
	if (lpMapCtl.CheckFloor(pos + VECTOR2(0, 50 )))
	{
		Jumpflag = false;
		if (Jumpflag == false && Wireflag == false)
		{
			pos.x += speed;
			SetAnim("歩く");
			DrawString(0, 100, "走り中", GetColor(0xff, 0xff, 0xff), true);
			jump = -12.0f;

		}
		animAdd = 1;


		if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])
		{
			Jumpflag = true;
			DrawString(0, 100, "ジャンプ中", GetColor(0xff, 0xff, 0xff), true);
			pos.y += jump;
			pos.x += speed;
			jump += 0.3f;
			animAdd = 0;
			SetAnim("ジャンプ");
			animAdd = 1;
		}
	}
	else
	{
		DrawString(0, 100, "ジャンプ中", GetColor(0xff, 0xff, 0xff), true);
		pos.y += jump;
		pos.x += speed;
		jump += 0.3f;
		animAdd = 0;
		SetAnim("ジャンプ");
		animAdd = 1;
	}
	animCnt += animAdd;

	//ループ
	/*if (pos.x >= 1024)
	{
		pos.x = 0;
	}*/
}

void Player::Draw(void)
{
	Obj::Draw();
}

bool Player::Wire(void)
{
	(*mc).Update();
	if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT || (Readyflag == true))
	{
		Readyflag = true;
		DrawString(0, 150, "ワイヤー準備", GetColor(0xff, 0xff, 0xff), true);
		//vec = mPos - pos;
		animAdd = 1;
	}

	if ((((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_RIGHT) && (Readyflag == true)) || (Wireflag == true))
	{
		Wireflag = true;
		Readyflag = false;
		VECTOR2 mPos = mc->GetPoint();
		VECTOR2 vec;
		vec.x = (mPos.x + (int)(pos.x / 1024) * 1024) - pos.x;
		vec.y = mPos.y - pos.y;
		vec.Normalize();
		DrawString(0, 100, "ワイヤー", GetColor(0xff, 0xff, 0xff), true);
		pos.x += vec.fx * 15;
		pos.y += vec.fy * 15;
		animAdd = 0;
		SetAnim("ジャンプ");
	}

	if (Wireflag == true && pos.y > 600)
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
