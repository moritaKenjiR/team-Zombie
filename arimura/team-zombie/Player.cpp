#include "Player.h"
#include "MapCtl.h"
#include "ImageMng.h"
#include "GameTask.h"
#include "Effect.h"

#include <string>

Player::Player()
{
	pos = { 0,0 };
	animAdd = 0;
	jump = -12.0f;
	grav = 5.0f;
	wireSpeed = -5.0f;
	cnt = 0;

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
	(*mc).Update();
	SetMove();
	(this->*StateTbl[(int)state_p])();
	animAdd = 1;
	animCnt += animAdd;

	return true;
}

void Player::SetMove(void)
{
	cnt++;
	lpMapCtl.IfMove(pos);
	if (cnt % 3)
	{
		oldPos = pos;
	}

	//��ڲ԰�̏�Ԉړ�
	if (lpMapCtl.CheckFloor(pos) && (state_p != STATE_P::SET_WIRE) && (state_p != STATE_P::WIRE))
	{
		state_p = STATE_P::RUN;
		if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])
		{
			state_p = STATE_P::JUMP;										//�ެ��ߏ���
		}
		else if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
		{
			mPos = mc->GetPoint();
			state_p = STATE_P::SET_WIRE;								//ܲ԰��������
		}
	}
	else if ((lpMapCtl.GetChipType(pos + VECTOR2(0, divSize.y)) == CHIP_TYPE::CHIP_BLANK)
		&& (lpMapCtl.GetChipType(pos + divSize) == CHIP_TYPE::CHIP_BLANK) && (state_p == STATE_P::RUN))
	{
		state_p = STATE_P::FDOWN;											//�ެ��߂̗�������
	}

	//����ۯ��ɂ����Ă̽�߰�ތ���
	if (lpMapCtl.GetChipType(pos + VECTOR2(32, 33)) == CHIP_TYPE::CHIP_GRASS2)
	{
		speed = 4.0f;
	}
	else
	{
		speed = 10.0f;
	}

	//�c���̪��
	if ((cnt % 20) == 0)
	{
		lpEffect.AddEffectList("Effect/dash.png", VECTOR2(240, 240), VECTOR2(4, 1), VECTOR2(0, 0), 4, 5, VECTOR2(pos.x - 32, pos.y - 120));
	}
	else
	{
		lpEffect.SetEffPos("Effect/dash.png", VECTOR2(pos.x - 32, pos.y - 120));
	}

	lpMapCtl.IfMove(pos);
	lpMapCtl.SetPlayerPos(pos);
}

void Player::Draw(void)
{
		//�c��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawGraph(oldPos.x + lpMapCtl.GameDrawOffset().x, oldPos.y + lpMapCtl.GameDrawOffset().y, IMAGE_ID("Image/char.png")[1], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	Obj::Draw();

	//////////////�f�o�b�O�\��
	if ((state_p == STATE_P::SET_WIRE) || (state_p== STATE_P::WIRE))
	{
		DrawLine(pos.x + lpMapCtl.GameDrawOffset().x + 32, pos.y + lpMapCtl.GameDrawOffset().y + 42, wire.pos.x + lpMapCtl.GameDrawOffset().x + (lpMapCtl.GetViewAreaSize().x / 3), wire.pos.y, 0xffffff);
	}
	DrawFormatString(0, 350, 0x000000, "pos:%f", pos.x /*+ lpMapCtl.GameDrawOffset().x*/);
	DrawFormatString(0,300, 0x000000, "wire:%f",  lpMapCtl.GameDrawOffset().x);

}

bool Player::initAnim(void)
{
	AddAnim("����", 0, 0, 4, 4);
	return true;
}

int Player::StateRun(void)
{
	if (!lpMapCtl.CheckWall(pos + VECTOR2(65, 32)))
	{
		pos.x += speed;
	}
	
	jump = -11.5f;
	SetAnim("����");
	wireSpeed = -7.0f;
	return 0;
}

int Player::StateJump(void)
{
	if (lpMapCtl.CheckWall(pos + VECTOR2(65, 32)))				//�ެ��߂���ەǔ��������炻�̏��u
	{
		speed = 0;
	}
	if (lpMapCtl.CheckUpBlock(pos) && jump < 0.0f)				//pos�̏����ۯ����������炻��ȏ��ɂ����Ȃ�
	{
		state_p = STATE_P::FDOWN;
	}
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
	int wireOffset = (lpMapCtl.GetViewAreaSize().x / 3);
	//���C���[����
	if (mPos.x > (lpMapCtl.GetViewAreaSize().x / 6))
	{
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
		{
			mPos = mc->GetPoint();
			wire.pos = mPos;
			wire.pos.x = (int)mPos.x + (int)pos.x - 512;
		}
	}
	pos.x += speed;

	if (state_p == STATE_P::SET_WIRE)
	{
		if ((lpMapCtl.GetChipType(pos + VECTOR2(0, divSize.y)) == CHIP_TYPE::CHIP_BLANK)
			&& (lpMapCtl.GetChipType(pos + divSize) == CHIP_TYPE::CHIP_BLANK))
		{
			state_p = STATE_P::FDOWN;
		}
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_RIGHT)
		{
			state_p = STATE_P::WIRE;
		}
		if (wire.pos.x <= (pos.x - wireOffset + 171))
		{
			state_p = STATE_P::RUN;
		}
	}
	return 0;
}

int Player::StateWire(void)
{
	DrawString(0, 100, "���C���[", GetColor(0xff, 0xff, 0xff), true);
	//���C���[����
	VECTOR2 vec;
	vec.x = wire.pos.x + 171 - pos.x;
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
			state_p = STATE_P::WIRE_DOWN;
		}
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

