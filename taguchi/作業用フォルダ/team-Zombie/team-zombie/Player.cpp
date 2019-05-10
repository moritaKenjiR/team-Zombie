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
	if (lpMapCtl.CheckFloor(pos + VECTOR2(0, 50 )) && (!Wireflag))
	{
		Jumpflag = false;
		if (Jumpflag == false && Wireflag == false)
		{
			/*if (keyData[KEY_INPUT_LEFT])
			{
				pos.x -= speed;
			}
			else if (keyData[KEY_INPUT_RIGHT])
			{
				pos.x += speed;
			}*/
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
		DrawString(0, 100, "ワイヤー", GetColor(0xff, 0xff, 0xff), true);
		pos.x += wire.vec.fx * 15;
		pos.y += wire.vec.fy * 15;
		animAdd = 0;
		SetAnim("ジャンプ");
		if (pos.y <= mPos.y)
		{
			wire.vec.fx = 0.0f;
			wire.vec.fy = 0.0f;
			Wireflag = false;
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
		//pos.y -= jump;
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
	VECTOR2 vec;
	vec.x = /*(mPos.x + (int)(pos.x / 1024) * 1024) - pos.x*/mPos.x - ((int)pos.x % (int)lpMapCtl.GetViewAreaSize().x);
	vec.y = mPos.y - pos.y;
	vec.Normalize();
	VECTOR2 drawOffset = lpMapCtl.GameDrawOffset();
	DrawLine(pos.x + divSize.x / 2 + drawOffset.x, pos.y + divSize.y / 2 + drawOffset.y, pos.x + drawOffset.x + vec.fx * 100, pos.y + drawOffset.y + vec.fy * 100, 0xffffff);
	DrawFormatString(50, 0, 0x000000, "playerposx:%f", pos.x);
	DrawFormatString(50, 50, 0x000000, "playerposy:%f", pos.y);
	DrawFormatString(50, 100, 0x000000, "mouseposx:%f", mPos.x); 
	DrawFormatString(50, 150, 0x000000, "mouseposy:%f", mPos.y);
	DrawFormatString(50, 400, 0x000000, "wireposx:%f", (mPos.x + (int)(pos.x / lpMapCtl.GetViewAreaSize().x) * lpMapCtl.GetViewAreaSize().x));
}

bool Player::Wire(void)
{
	(*mc).Update();
	if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT /*&& (Readyflag == false)*/)
	{	
		Readyflag = true;
		mPos = mc->GetPoint();
		DrawString(0, 150, "ワイヤー準備", GetColor(0xff, 0xff, 0xff), true);
		//vec = mPos - pos;
		animAdd = 1;
	}

	if ((((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_RIGHT) && (Readyflag == true)))
	{
		Wireflag = true;
		Readyflag = false;
		if (mPos.x >= lpMapCtl.GetViewAreaSize().x / 2)
		{
			wire.pos.x = pos.x + divSize.x / 2 + mPos.x - lpMapCtl.GetViewAreaSize().x / 2;
			wire.pos.y = pos.y + divSize.y / 2 + mPos.y - lpMapCtl.GetViewAreaSize().y / 2;
		}
		else
		{
			wire.pos.x = pos.x + divSize.x / 2 - mPos.x - lpMapCtl.GetViewAreaSize().x / 2;
			wire.pos.y = pos.y + divSize.y / 2 + mPos.y - lpMapCtl.GetViewAreaSize().y / 2;
		}
		wire.vec.x = wire.pos.x - pos.x/*(mPos.x + (int)(pos.x / lpMapCtl.GetViewAreaSize().x) * lpMapCtl.GetViewAreaSize().x)*//*mPos.x - ((int)pos.x % (int)lpMapCtl.GetViewAreaSize().x)*/;
		wire.vec.y = mPos.y - pos.y;
		wire.vec.Normalize();
		/*VECTOR2 vec;
		vec.x = (mPos.x + (int)(pos.x / lpMapCtl.GetViewAreaSize().x) * lpMapCtl.GetViewAreaSize().x) - pos.x;
		vec.y = mPos.y - pos.y;
		vec.Normalize();*/
		/*DrawString(0, 100, "ワイヤー", GetColor(0xff, 0xff, 0xff), true);
		pos.x += vec.fx * 15;
		pos.y += vec.fy * 15;
		animAdd = 0;
		SetAnim("ジャンプ");*/
	}

	/*if (Wireflag == true && pos.y > 600)
	{
		pos.y == 600;
		Wireflag = false;
	}*/

	return true;
}

bool Player::initAnim(void)
{
	AddAnim("歩く", 1, 0, 6, 7);
	AddAnim("ジャンプ", 0, 2, 8, 12);
	return true;
}
