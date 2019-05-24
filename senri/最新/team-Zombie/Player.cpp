#include "Player.h"
#include "MapCtl.h"
#include "ImageMng.h"
#include "Effect.h"
#include "Shaker.h"

Player::Player()
{
	pos = { 50,700 };
	animAdd = 0;
	jump = -1.5f;
	grav = 5.0f;
	wireSpeed = -5.0f;
	jumpLimit = false;
	mc = std::make_shared<MouseCtl>();

	cnt = 0;

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
	animAdd = 0;
	cnt++;
	lpMapCtl.IfMove(pos);
	if (cnt % 3)
	{
		oldPos = pos;
	}
	if (lpMapCtl.CheckFloor(pos) && (state_p != STATE_P::SET_WIRE) && (state_p != STATE_P::WIRE))
	{
		state_p = STATE_P::RUN;
		if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])
		{
			lpEffect.AddEffectList("Effect/jump.png", VECTOR2(240, 240), VECTOR2(6, 1), VECTOR2(0, 0), 6, 5, VECTOR2(pos.x - 96, pos.y - 120));
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
	if (lpMapCtl.GetChipType(pos + VECTOR2(32, 32)) == CHIP_FIRE)
	{
		lpMapCtl.SetEndFlag(true);
	}
	lpMapCtl.SetPlayerPos(pos);
}

void Player::Draw(void)
{
	//�c��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawGraph(oldPos.x + lpMapCtl.GameDrawOffset().x, oldPos.y + lpMapCtl.GameDrawOffset().y, IMAGE_ID("Image/char.png")[1], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	
	if (state_p == STATE_P::WIRE && (cnt % 10) == 0)
	{
		//���C���[�G�t�F�N�g(��)
		lpEffect.AddEffectList("wireEff/wEf2.png", VECTOR2(192, 192), VECTOR2(5, 4), VECTOR2(0, 0), 18, 3, VECTOR2(pos.x - 80, pos.y - 70));
	}

	Obj::Draw();

	//////////////�f�o�b�O�\��
	if (state_p == STATE_P::SET_WIRE || state_p == STATE_P::WIRE)
	{
		DrawLine(pos.x + lpMapCtl.GameDrawOffset().x + 32, pos.y + lpMapCtl.GameDrawOffset().y + 42, wire.pos.x + lpMapCtl.GameDrawOffset().x + 100, wire.pos.y, GetColor(0, 0, 0));
	}
}

int Player::StateRun(void)
{
	if (!lpMapCtl.CheckWall(pos + VECTOR2(65, 32)))
	{
		pos.x += speed;
	}

	if (lpMapCtl.GetChipType(pos + VECTOR2(32, 33)) == CHIP_TYPE::CHIP_GRASS2)
	{
		speed = 4.0f;
	}
	else
	{
		speed = 8.0f;
	}

	if ((cnt % 20) == 0)
	{
		lpEffect.AddEffectList("Effect/dash.png", VECTOR2(240, 240), VECTOR2(4, 1), VECTOR2(0, 0), 4, 5, VECTOR2(pos.x - 32, pos.y - 120));
	}
	else
	{
		//lpEffect.SetEffPos("Effect/dash.png", VECTOR2(pos.x - 32, pos.y - 120));
	}
	jump = -1.5f;
	jumpLimit = false;
	SetAnim("����");
	wireSpeed = -7.0f;
	return 0;
}

int Player::StateJump(void)
{
	if (!jumpLimit)
	{
		if (keyData[KEY_INPUT_SPACE] && keyDataOld[KEY_INPUT_SPACE] && jump > -10.0f)
		{
			jump -= 1.5f;
		}
		else if (jump < -10.0f || (!keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE]))
		{
			jumpLimit = true;
		}
	}

	if (lpMapCtl.CheckUpBlock(pos) && jump < 0.0f)				//pos�̏����ۯ����������炻��ȏ��ɂ����Ȃ�
	{
		//JumpLimit = true;
		state_p = STATE_P::FDOWN;
	}
	else
	{
		pos.y += jump;
	}
	//SetAnim("�W�����v");

	if (!lpMapCtl.CheckWall(pos + VECTOR2(65, 32)))
	{
		pos.x += speed;
	}
	jump += 0.3f;

	return 0;
}

int Player::StateFdown(void)
{
	pos.x += speed;
	pos.y += grav;
	//SetAnim("�W�����v");

	return 0;
}

int Player::StateSetWire(void)
{
	//���C���[����
	if (mPos.x > (1024 / 2))
	{
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
		{
			mPos = mc->GetPoint();
			wire.pos = mPos;
			wire.pos.x = (int)mPos.x + (int)pos.x - 512;
			lpEffect.AddEffectList("wireEff/����3.png", VECTOR2(192, 192), VECTOR2(5, 3), VECTOR2(0, 0), 15, 2, VECTOR2(pos.x - 50, pos.y - 64));
		}
	}
	pos.x += speed;
	if (lpMapCtl.GetChipType(pos + VECTOR2(32, 33)) == CHIP_TYPE::CHIP_GRASS2)
	{
		pos.x -= 3.0f;
	}
	else
	{
		pos.x -= 7.0f;
	}

	if (state_p == STATE_P::SET_WIRE)
	{
		if ((lpMapCtl.GetChipType(pos + VECTOR2(0, divSize.y)) == CHIP_TYPE::CHIP_BLANK)
			&& (lpMapCtl.GetChipType(pos + divSize) == CHIP_TYPE::CHIP_BLANK))
		{
			state_p = STATE_P::FDOWN;
		}
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_RIGHT)
		{
			//���C���[�G�t�F�N�g(��)
			lpEffect.AddEffectList("wireEff/wEf1.png", VECTOR2(192, 192), VECTOR2(5, 2), VECTOR2(0, 0), 10, 3, VECTOR2(pos.x - 64, pos.y - 120));
			state_p = STATE_P::WIRE;
		}
		if (wire.pos.x <= (pos.x + 75) || (mPos.x <= 600))
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
bool Player::initAnim(void)
{
	AddAnim("����", 0, 0, 4, 4);
	return true;
}
