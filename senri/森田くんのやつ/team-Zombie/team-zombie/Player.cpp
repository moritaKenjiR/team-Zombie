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

	//���܂������ĂȂ�
	if (lpMapCtl.CheckFloor(pos + VECTOR2(0, 50 )))
	{
		Jumpflag = false;
		if (Jumpflag == false && Wireflag == false)
		{
			//pos.x += speed;
			SetAnim("����");
			DrawString(0, 100, "���蒆", GetColor(0xff, 0xff, 0xff), true);
			jump = -12.0f;

		}
		animAdd = 1;


		if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])
		{
			Jumpflag = true;
			DrawString(0, 100, "�W�����v��", GetColor(0xff, 0xff, 0xff), true);
			pos.y += jump;
			pos.x += speed;
			jump += 0.3f;
			animAdd = 0;
			SetAnim("�W�����v");
			animAdd = 1;
		}
	}
	else if (Wireflag)
	{
		if (pos.y <= mPos.y)
		{
			Wireflag = false;
		}
	}
	else if (Jumpflag)
	{
		DrawString(0, 100, "�W�����v��", GetColor(0xff, 0xff, 0xff), true);
		pos.y += jump;
		pos.x += speed;
		jump += 0.3f;
		animAdd = 0;
		SetAnim("�W�����v");
		animAdd = 1;
	}
	else
	{
		pos.y -= jump;
	}
	animCnt += animAdd;

	//���[�v
	/*if (pos.x >= 1024)
	{
		pos.x = 0;
	}*/
}

void Player::Draw(void)
{
	Obj::Draw();
	VECTOR2 vec;
	vec.x = (mPos.x + (int)(pos.x / 1024) * 1024) - pos.x;
	vec.y = mPos.y - pos.y;
	vec.Normalize();
	DrawLine(pos.x, pos.y, pos.x + vec.fx * 100, pos.y + vec.fy * 100, 0xffffff);
	DrawCircle(pos.x, pos.y,10, 0xffffff);
	DrawFormatString(50, 0, 0x000000, "playerposx:%f", pos.x);
	DrawFormatString(50, 50, 0x000000, "mouseposx:%f", mPos.x);
	DrawFormatString(50, 100, 0x000000, "playerposy:%f", pos.y);
	DrawFormatString(50, 150, 0x000000, "mouseposy:%f", mPos.y);
	DrawFormatString(50,300,0x000000,"wireposx:%f", wire.pos.x);
}

bool Player::Wire(void)
{
	(*mc).Update();
	if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT || (Readyflag == true))
	{
		mPos = mc->GetPoint();
		wire.pos = mPos + lpMapCtl.GameDrawOffset();
		Readyflag = true;
		DrawString(0, 150, "���C���[����", GetColor(0xff, 0xff, 0xff), true);
		//vec = mPos - pos;
		animAdd = 1;
	}

	if ((((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_RIGHT) && (Readyflag == true)) || (Wireflag == true))
	{
		Wireflag = true;
		Readyflag = false;
		VECTOR2 vec;
		vec.x = (wire.pos.x + (int)(pos.x / 1024) * 1024) - pos.x;
		vec.y = wire.pos.y - pos.y;
		vec.Normalize();
		DrawString(0, 100, "���C���[", GetColor(0xff, 0xff, 0xff), true);
		pos.x += vec.fx * 15;
		pos.y += vec.fy * 15;
		animAdd = 0;
		SetAnim("�W�����v");
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
	AddAnim("����", 1, 0, 6, 7);
	AddAnim("�W�����v", 0, 2, 8, 12);
	return true;
}
