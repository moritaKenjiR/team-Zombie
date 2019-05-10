#include "Player.h"
#include "MapCtl.h"
#include "Camera.h"
#include "GameTask.h"



Player::Player(std::unique_ptr<Camera>camera)
{
	pos = { 50,700 };
	animAdd = 0;
	jump = -12.0f;
	wireSpeed = -10.0f;
	Jumpflag = false;
	Readyflag = false;
	Wireflag = false;
	DownFlag = false;
	wireCnt = 3;
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
	lpGameTask.StartPrgTime();
	Wire();
	SetMove();
	return true;
}

void Player::SetMove(void)
{
	//�f�o�b�O�p
	if (pos.y < 0)
	{
		pos.y = 0;
	}
	if (pos.x < 0)
	{
		pos.x = 0;
	}
	memcpy(keyDataOld, keyData, sizeof(keyDataOld));
	GetHitKeyStateAll(keyData);
	animAdd = 0;
	//���܂������ĂȂ�
	if (lpMapCtl.CheckFloor(pos + VECTOR2(0, 50 )))
	{
		Jumpflag = false;
		if (Jumpflag == false && Wireflag == false)
		{
			pos.x += speed;
			SetAnim("����");
			DrawString(0, 100, "���蒆", GetColor(0xff, 0xff, 0xff), true);
			jump = -12.0f;
			wireSpeed = -5.0f;
			DownFlag = false;

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
		if ((int)pos.y <= (int)mPos.y+96)
		{
			lpGameTask.EndPrgTime();
			Wireflag = false;
		}
	}
	else if (Jumpflag && !(DownFlag))
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
		if (!DownFlag)
		{
			animAdd = 0;
			SetAnim("�W�����v");
			pos.y -= jump;
		}
	}
	animCnt += animAdd;
}

void Player::Draw(void)
{
	Obj::Draw();

	/////////////�f�o�b�O�\��
	VECTOR2 vec;
	vec.x = wire.pos.x - pos.x;
	vec.y = wire.pos.y - pos.y;
	vec.Normalize();
	VECTOR2 drawOffset = lpMapCtl.GameDrawOffset();
	//DrawLine(pos.x + drawOffset.x + 32, pos.y + drawOffset.y + 42, wire.pos.x + drawOffset.x, wire.pos.y, 0xffffff);
	DrawCircle(pos.x + drawOffset.x + 32, pos.y + drawOffset.y + 42,10, 0xffffff);
	DrawFormatString(50, 0, 0x000000, "playerposx:%f", pos.x);
	DrawFormatString(50, 50, 0x000000, "mouseposx:%f", mPos.x);
	DrawFormatString(50, 100, 0x000000, "playerposy:%f", pos.y);
	DrawFormatString(50, 150, 0x000000, "mouseposy:%f", mPos.y);
	DrawFormatString(50, 300, 0x000000, "wireposx:%f", wire.pos.x);
	DrawFormatString(50, 350, 0x000000, "wireposy:%f", wire.pos.y);
	DrawFormatString(50, 400, 0x000000, "drawOffsetx:%f", drawOffset.x);
	DrawFormatString(50, 400, 0x000000, "drawOffsetx:%f", drawOffset.x);
	DrawFormatString(900, 650, 0x000000, "wireCnt:%d", wireCnt);
	DrawFormatString(pos.x + drawOffset.x,pos.y + drawOffset.y,0x000000,"vecx:%f", vec.x);
	///////////////////
}

bool Player::Wire(void)
{
	wireCnt = -lpGameTask.GetPrgTime();
	(*mc).Update();
	if (wireCnt >= 3)
	{
		wireCnt = 3;
		DrawString(850, 750, "���C���[�g�p�\", true);
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT && (Wireflag == false))
		{
			mPos = mc->GetPoint();
			wire.pos = mPos;
			wire.pos.x = (int)mPos.x + (int)pos.x - 512;
			Readyflag = true;
			animAdd = 1;
		}
	}
	if (Readyflag)
	{
		DrawString(0, 150, "���C���[����", GetColor(0xff, 0xff, 0xff), true);
		pos.x -= 7;
	}
	///////////////�f�o�b�O�\��
	if (Readyflag || Wireflag || DownFlag)
	{
		DrawLine(pos.x + lpMapCtl.GameDrawOffset().x + 32, pos.y + lpMapCtl.GameDrawOffset().y + 42, wire.pos.x + lpMapCtl.GameDrawOffset().x, wire.pos.y, 0xffffff);
	}

	if ((((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_RIGHT) && (Readyflag == true)) || (Wireflag == true))
	{
		wireCnt = 3;
		Wireflag = true;
		DownFlag = true;
		Readyflag = false;

		VECTOR2 vec;
		vec.x = wire.pos.x- pos.x;
		vec.y = wire.pos.y - pos.y;
		vec.Normalize();
		DrawString(0, 100, "���C���[", GetColor(0xff, 0xff, 0xff), true);
		pos.x += vec.fx * 18;
		pos.y += vec.fy * 20;
		pos.y -=-2.0f;
		animAdd = 0;
		SetAnim("�W�����v");
	}

	if (!(lpMapCtl.CheckFloor(pos + VECTOR2(0, 50))) && (Wireflag == false)&& (DownFlag == true))
	{
		pos.y += wireSpeed;
		pos.x += 4;
		wireSpeed += 0.15f;
	}

	return true;
}

bool Player::initAnim(void)
{
	AddAnim("����", 1, 0, 6, 7);
	AddAnim("�W�����v", 0, 2, 8, 11);
	return true;
}
