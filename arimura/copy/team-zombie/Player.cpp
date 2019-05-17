#include "Player.h"
#include "MapCtl.h"
#include "Camera.h"
#include "GameTask.h"
#include <string>



Player::Player()
{
	pos = { 0,0 };
	animAdd = 0;
	jump = -12.0f;
	grav = 5.0f;
	wireSpeed = -5.0f;
	wireCnt = 0;
	wireTime = 0;

	mc = std::make_shared<MouseCtl>();

	//ð��ق�����Ă��ꂼ��̊֐�����ڲ԰�̏�Ԃ�ێ������Ă�
	state_p = STATE_P::RUN;
	StateTbl[(int)STATE_P::RUN] = &Player::StateRun;
	StateTbl[(int)STATE_P::JUMP] = &Player::StateJump;
	StateTbl[(int)STATE_P::FDOWN] = &Player::StateFdown;
	StateTbl[(int)STATE_P::SET_WIRE] = &Player::StateSetWire;
	StateTbl[(int)STATE_P::WIRE] = &Player::StateWire;
	StateTbl[(int)STATE_P::WIRE_DOWN] = &Player::StateWireDown;
}

Player::~Player()
{
}

bool Player::Update(void)
{
	memcpy(keyDataOld, keyData, sizeof(keyDataOld));
	GetHitKeyStateAll(keyData);

	lpGameTask.StartPrgTime();
	(*mc).Update();
	SetMove();
	(this->*StateTbl[(int)state_p])();
	animAdd = 1;
	animCnt += animAdd;

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

	//ܲ԰�̸����ъ֌W
	if (wireTime == 1)
	{
		wireCnt = WIRE_CNT + lpGameTask.GetPrgTime();
	}
	if (wireCnt <= 0)
	{
		wireCnt = 0;
	}

	if (lpMapCtl.CheckFloor(pos + VECTOR2(0, 50)) && (state_p != STATE_P::SET_WIRE) && (state_p != STATE_P::WIRE))
	{
		state_p = STATE_P::RUN;
		if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])
		{
			state_p = STATE_P::JUMP;										//�ެ��ߏ���
		}
		else if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
		{
			mPos = mc->GetPoint();
			if (wireCnt <= 0)
			{
				state_p = STATE_P::SET_WIRE;								//ܲ԰��������
			}
		}
	}
	else if ((lpMapCtl.GetChipType(pos + VECTOR2(0, divSize.y)) == CHIP_TYPE::CHIP_BLANK)
		 && (lpMapCtl.GetChipType(pos + divSize) == CHIP_TYPE::CHIP_BLANK) && (state_p == STATE_P::RUN))
	{
		state_p = STATE_P::FDOWN;											//�ެ��߂̗�������
	}
}

void Player::Draw(void)
{
	Obj::Draw();
	/////////////�f�o�b�O�\��
	VECTOR2 drawOffset = lpMapCtl.GameDrawOffset();
	DrawFormatString(100, 50, 0x000000, "playerposx:%f", pos.x);
	DrawFormatString(100, 200, 0x000000, "Drawposx:%f", pos.x + drawOffset.x);
	DrawFormatString(900, 650, 0x000000, "wireCnt:%d", wireCnt);
	//�b���\��
	DrawString(0, SCREEN_SIZE_Y - 20, (std::to_string(lpGameTask.GetPrgTime()) + "�b").c_str(), 0xffffff);

}

bool Player::initAnim(void)
{
	AddAnim("����", 1, 0, 6, 7);
	AddAnim("�W�����v", 0, 2, 8, 11);
	return true;
}

int Player::StateRun(void)
{
	pos.x += speed;
	jump = -12.0f;
	SetAnim("����");
	wireSpeed = -7.0f;

	return 0;
}

int Player::StateJump(void)
{
	pos.y += jump;
	pos.x += speed;
	jump += 0.3f;
	SetAnim("�W�����v");

	return 0;
}

int Player::StateFdown(void)
{
	pos.x += speed;
	pos.y += grav;
	SetAnim("�W�����v");

	return 0;
}

int Player::StateSetWire(void)
{
	//���C���[����
	if (mPos.x > (SCREEN_SIZE_X / 2))
	{
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
		{
			mPos = mc->GetPoint();
			wire.pos = mPos;
			wire.pos.x = (int)mPos.x + (int)pos.x - 512;
		}
	}
	pos.x += speed;
	pos.x -= 7;

	if (state_p == STATE_P::SET_WIRE)
	{
		if ((lpMapCtl.GetChipType(pos + VECTOR2(0, divSize.y)) == CHIP_TYPE::CHIP_BLANK)		//��state��JUMP,WIRE,WIRE_DOWN����Ȃ��Ƃ��ɗ��Ƃ�
			&& (lpMapCtl.GetChipType(pos + divSize) == CHIP_TYPE::CHIP_BLANK))
		{
			state_p = STATE_P::FDOWN;
		}
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_RIGHT)
		{
			state_p = STATE_P::WIRE;
		}
		if (wire.pos.x <= (pos.x + 75) ||(mPos.x <= 600))
		{
			state_p = STATE_P::RUN;
		}
	}

	if (state_p == STATE_P::SET_WIRE)
	{
		DrawLine(pos.x + lpMapCtl.GameDrawOffset().x + 32, pos.y + lpMapCtl.GameDrawOffset().y + 42, wire.pos.x + lpMapCtl.GameDrawOffset().x, wire.pos.y, 0xffffff);
	}

	return 0;
}

int Player::StateWire(void)
{
	DrawString(0, 100, "���C���[", GetColor(0xff, 0xff, 0xff), true);
	//���C���[����
	VECTOR2 vec;
	vec.x = wire.pos.x - pos.x;
	vec.y = wire.pos.y - pos.y;
	vec.Normalize();
	pos.x += vec.fx * 16;
	pos.y += vec.fy * 16;
	pos.y += grav;
	SetAnim("�W�����v");

	if (!lpMapCtl.CheckFloor(pos + VECTOR2(0, 50)))
	{
		if (((int)pos.y <= (int)mPos.y + 96) && (state_p == STATE_P::WIRE))		//�}�E�X���W�Ƀv���C���[���W������������ (�������player���W���炵�Ă�)
		{
			wireTime = 1;
			wireCnt = WIRE_CNT;
			lpGameTask.EndPrgTime();
			state_p = STATE_P::WIRE_DOWN;
		}
	}
	//////////////�f�o�b�O�\��
	if (state_p == STATE_P::WIRE)
	{
		DrawLine(pos.x + lpMapCtl.GameDrawOffset().x + 32, pos.y + lpMapCtl.GameDrawOffset().y + 42, wire.pos.x + lpMapCtl.GameDrawOffset().x, wire.pos.y, 0xffffff);
	}
	
	return 0;
}

int Player::StateWireDown(void)
{
	pos.y += wireSpeed;
	pos.x += speed;
	wireSpeed += 0.25f;

	return 0;
}
