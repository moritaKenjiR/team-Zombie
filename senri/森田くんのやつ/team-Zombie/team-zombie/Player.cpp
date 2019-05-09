#include "Player.h"
#include "MapCtl.h"
#include "Camera.h"



Player::Player(std::unique_ptr<Camera>camera)
{
	pos = { 50,700 };
	animAdd = 0;
	jump = -12.0f;
	Jumpflag = false;
	Readyflag = false;
	Wireflag = false;
	mc = std::make_shared<MouseCtl>();
	this->camera = std::move(camera);
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
	//デバッグ用
	if (pos.y < 0)
	{
		pos.y = 0;
	}
	if (pos.x < 0)
	{
		pos.x = 0;
	}
	if (pos.y >= 600)
	{
		pos.y = 600;
	}
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
	else if (Wireflag)
	{
		if ((int)pos.y <= (int)mPos.y+10)
		{
			PlayerDown();
		}
	}
	else if (Jumpflag)
	{
		DrawString(0, 100, "ジャンプ中", GetColor(0xff, 0xff, 0xff), true);
		pos.y += jump;
		pos.x += speed;
		jump += 0.3f;
		animAdd = 0;
		SetAnim("ジャンプ");
		animAdd = 1;
	}
	else
	{
		animAdd = 0;
		SetAnim("ジャンプ");
		if (wire.pos.x < pos.x)
		pos.x += 5;
		pos.y -=5;
		pos.y -= jump;
	}
	animCnt += animAdd;
}

void Player::Draw(void)
{
	Obj::Draw();
	VECTOR2 vec;
	vec.x = wire.pos.x - pos.x;
	vec.y = wire.pos.y - pos.y;
	vec.Normalize();
	VECTOR2 drawOffset = lpMapCtl.GameDrawOffset();
	DrawLine(pos.x + drawOffset.x + 32, pos.y + drawOffset.y + 42,mPos.x,mPos.y, 0xffffff);
	DrawCircle(pos.x + drawOffset.x + 32, pos.y + drawOffset.y + 42,10, 0xffffff);
	DrawFormatString(50, 0, 0x000000, "playerposx:%f", pos.x);
	DrawFormatString(50, 50, 0x000000, "mouseposx:%f", mPos.x);
	DrawFormatString(50, 100, 0x000000, "playerposy:%f", pos.y);
	DrawFormatString(50, 150, 0x000000, "mouseposy:%f", mPos.y);
	DrawFormatString(50, 300, 0x000000, "wireposx:%f", wire.pos.x);
	DrawFormatString(50, 350, 0x000000, "wireposy:%f", wire.pos.y);
	DrawFormatString(50, 400, 0x000000, "drawOffsetx:%f", drawOffset.x);
	DrawFormatString(pos.x + drawOffset.x,pos.y + drawOffset.y,0x000000,"vecx:%f", vec.x);
}

bool Player::Wire(void)
{
	(*mc).Update();
	if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
	{
		mPos = mc->GetPoint();
		wire.pos = mPos;
		wire.pos.x = (int)mPos.x + (int)pos.x - 512;
		Readyflag = true;
		animAdd = 1;
	}
	if (Readyflag)
	{
		DrawString(0, 150, "ワイヤー準備", GetColor(0xff, 0xff, 0xff), true);
	}

	if ((((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_RIGHT) && (Readyflag == true)) || (Wireflag == true))
	{
		Wireflag = true;
		Readyflag = false;

		VECTOR2 vec;
		vec.x = wire.pos.x- pos.x;
		vec.y = wire.pos.y - pos.y;
		vec.Normalize();
		DrawString(0, 100, "ワイヤー", GetColor(0xff, 0xff, 0xff), true);
		pos.x += vec.fx * 20;
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

void Player::PlayerDown(void)
{
	Wireflag = false;
	if (pos.y >= 600)
	{
		return;
	}
}

bool Player::initAnim(void)
{
	AddAnim("歩く", 1, 0, 6, 7);
	AddAnim("ジャンプ", 0, 2, 8, 11);
	return true;
}
