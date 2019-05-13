#include "Player.h"
#include "MapCtl.h"
#include "ImageMng.h"
#include "Effect.h"

Player::Player()
{
	pos = { 50,700 };
	animAdd = 0;
	jump = -12.0f;
	Jumpflag = false;
	Readyflag = false;
	Wireflag = false;
	JumpLimit = false;
	mc = std::make_shared<MouseCtl>();

	cnt = 0;
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
	cnt++;
	lpMapCtl.IfMove(pos);
	if (cnt % 3)
	{
		oldPos = pos;
	}
	//���܂������ĂȂ�
	if (lpMapCtl.CheckFloor(pos))
	{
		Jumpflag = false;
		JumpLimit = false;
		if (Jumpflag == false && Wireflag == false)
		{
			//pos.x += speed;
			SetAnim("����");
			DrawString(0, 100, "���蒆", GetColor(0xff, 0xff, 0xff), true);
			jump = -1.0f;

			if (lpMapCtl.GetChipType(pos + VECTOR2(32,33)) == CHIP_TYPE::CHIP_FLOOR10)
			{
				speed = 4.0f;
			}
			else
			{
				speed = 8.0f;
			}

			if ((cnt%20) == 0)
			{
				lpEffect.AddEffectList("Effect/dash.png", VECTOR2(240, 240), VECTOR2(4, 1), VECTOR2(0, 0), 4,5, VECTOR2(pos.x - 32, pos.y - 120));
			}
			else
			{
				//lpEffect.SetEffPos("Effect/dash.png", VECTOR2(pos.x - 32, pos.y - 120));
			}
			///////////
			if (keyData[KEY_INPUT_RIGHT])
			{
				if (!lpMapCtl.CheckWall(pos + VECTOR2(65, 32)))
				{
					pos.x += speed;
				}
			}
			else if(keyData[KEY_INPUT_LEFT])
			{
				if (!lpMapCtl.CheckWall(pos + VECTOR2(-1, 32)))
				{
					pos.x -= speed;
				}
			}

		}
		animAdd = 1;

		
		if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])
		{
			Jumpflag = true;
			DrawString(0, 100, "�W�����v��", GetColor(0xff, 0xff, 0xff), true);
			pos.y += jump;
			if (!lpMapCtl.CheckWall(pos + VECTOR2(65, 32)))
			{
				pos.x += speed;
			}
			jump -= 1.0f;
			animAdd = 0;
			//SetAnim("�W�����v");
			animAdd = 1;
			lpEffect.AddEffectList("Effect/jump.png", VECTOR2(240, 240), VECTOR2(6, 1), VECTOR2(0, 0), 6,5, VECTOR2(pos.x-96 , pos.y -120));
		}
		lpMapCtl.IfMove(pos);
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
		if (jump > -12.0f && !JumpLimit)
		{
			if (keyData[KEY_INPUT_SPACE] && keyDataOld[KEY_INPUT_SPACE])
			{
				jump -= 1.5f;
			}
			else if (!keyDataOld[KEY_INPUT_SPACE])
			{
				JumpLimit = true;
			}
		}
		else if (jump <= -12.0f)
		{
   			JumpLimit = true;
		}
		DrawString(0, 100, "�W�����v��", GetColor(0xff, 0xff, 0xff), true);
		pos.y += jump;
		if (!lpMapCtl.CheckWall(pos + VECTOR2(65, 32)))
		{
			pos.x += speed;
		}
		jump += 0.5f;
		animAdd = 0;
		//SetAnim("�W�����v");
		animAdd = 1;
	}
	else
	{
		jump += 0.5f;
		pos.y += jump;
		if (!lpMapCtl.CheckWall(pos + VECTOR2(65, 32)))
		{
			pos.x += speed;
		}
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
		//�c��
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
		DrawGraph(oldPos.x + lpMapCtl.GameDrawOffset().x, oldPos.y + lpMapCtl.GameDrawOffset().y, IMAGE_ID("Image/char.png")[1], true);
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	
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
		//SetAnim("�W�����v");
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
	AddAnim("����", 0, 0, 4, 4);
	return true;
}
