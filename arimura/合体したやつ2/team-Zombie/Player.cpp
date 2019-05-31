#include "Player.h"
#include "MapCtl.h"
#include "ImageMng.h"
#include "Effect.h"
#include "Shaker.h"

Player::Player()
{
	pos = { 50, 500 };
	animAdd = 0;

	jump = -1.5f;
	gravity = 5.0f;
	offset = 171;	//DrawOffset��-����Ă����l��pos��+����Ă����l�̍���
	wireSpeed = -5.0f;
	jumpLimit = false;

	mc = std::make_shared<MouseCtl>();

	//ð��ق�����Ă��ꂼ��̊֐�����ڲ԰�̏�Ԃ�ێ������Ă�
	state_p = STATE_P::RUN;
	StateTbl[(int)STATE_P::RUN] = &Player::StateRun;
	StateTbl[(int)STATE_P::JUMP] = &Player::StateJump;
	StateTbl[(int)STATE_P::FDOWN] = &Player::StateFdown;
	StateTbl[(int)STATE_P::SET_WIRE] = &Player::StateSetWire;
	StateTbl[(int)STATE_P::JUMP_WIRE] = &Player::StateJumpWire;
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

	/////////////////////////////������
	if (lpMapCtl.CheckFloor(pos) && (state_p != STATE_P::SET_WIRE) && (state_p != STATE_P::WIRE))
	{
		animAdd = 1;
		state_p = STATE_P::RUN;

		if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])
		{
			lpEffect.AddEffectList("Effect/jump.png", VECTOR2(240, 240), VECTOR2(6, 1), VECTOR2(0, 0), 6, 5, VECTOR2(pos.x - 96, pos.y - 120));
			state_p = STATE_P::JUMP;										//�ެ��ߏ���
		}
		else if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
		{
			mPos = mc->GetPoint();
			if (lpMapCtl.GetChipType(mPos + VECTOR2((int)pos.x - 512 + 300, 0)) >= CHIP_BLOCK1)
			{
				gravity += 0.1f;
				state_p = STATE_P::SET_WIRE;								//ܲ԰��������
			}
		}
	}
	else if ((lpMapCtl.GetChipType(pos + VECTOR2(0, divSize.y/2)) == (CHIP_TYPE::CHIP_BLANK && CHIP_TYPE::CHIP_FIRE))
		&& (lpMapCtl.GetChipType(pos + divSize/2) == (CHIP_TYPE::CHIP_BLANK && CHIP_TYPE::CHIP_FIRE)) && (state_p == STATE_P::RUN))
	{
		state_p = STATE_P::FDOWN;											//�ެ��߂̗�������
	}

	//�����̎��̽�߰�ނ̕ω�
	if ((lpMapCtl.GetChipType(pos + VECTOR2(32, 33)) == CHIP_TYPE::CHIP_GRASS2))
	{
		speed = 4.0f;
	}
	else
	{
		speed = 7.0f;
	}

	//���������ɏI��
	if (lpMapCtl.GetChipType(pos + VECTOR2(32, 32)) == CHIP_FIRE)
	{
		lpMapCtl.SetEndFlag(true,1);
	}

	//�Q�[���N���A
	if (pos.x >= lpMapCtl.GetGameAreaSize().x)
	{
		lpMapCtl.SetEndFlag(true, 0);
	}

	lpMapCtl.SetPlayerPos(pos);
}

void Player::Draw(void)
{
	//�c��
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawGraph(oldPos.x + lpMapCtl.GameDrawOffset().x, oldPos.y + lpMapCtl.GameDrawOffset().y, IMAGE_ID("Image/char.png")[1], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);

	Obj::Draw();

	if (state_p == STATE_P::SET_WIRE || state_p == STATE_P::WIRE || state_p == STATE_P::JUMP_WIRE)	//�f�o�b�O�\��
	{
		DrawLine(pos.x + lpMapCtl.GameDrawOffset().x + 32, pos.y + lpMapCtl.GameDrawOffset().y + 42, wire.pos.x + lpMapCtl.GameDrawOffset().x + 300, wire.pos.y,0xdcdcdc);
	}
}

//////////////////////////////����
int Player::StateRun(void)
{
	if (lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, divSize.y / 2)) && (!lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, 0))))
	{
		pos.y -= 10;
	}
	else if (lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, divSize.y / 2)) || (lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, 0))))
	{
		speed = 0;
	}

	if ((cnt % 20) == 0)
	{
		lpEffect.AddEffectList("Effect/dash.png", VECTOR2(240, 240), VECTOR2(4, 1), VECTOR2(0, 0), 4, 5, VECTOR2(pos.x - 32, pos.y - 120));
	}
	pos.x += speed;
	jump = -1.5f;
	jumpLimit = false;
	SetAnim("����");
	wireSpeed = -7.0f;
	return 0;
}

/////////////////////////////�ެ��ߏ���
int Player::StateJump(void)
{
	animAdd = 1;
	//�ެ��߂̌��E�Ə��ެ�
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

	jump += 0.4f;

	if ((lpMapCtl.CheckUpBlock(pos + VECTOR2(0, 0)) && jump <= 0.0f) ||
		(lpMapCtl.CheckUpBlock(pos + VECTOR2(0, -divSize.y/2)) && jump <= 0.0f))				//pos�̏����ۯ����������炻��ȏ��ɂ����Ȃ�
	{
    	state_p = STATE_P::FDOWN;
	}
	else
	{
		pos.y += jump;
	}

	if (!lpMapCtl.CheckWall(pos + VECTOR2(65, 32)))
	{
		pos.x += speed;
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
		{
			mPos = mc->GetPoint();
			if (lpMapCtl.GetChipType(mPos + VECTOR2((int)pos.x - 512 + 300, 0)) >= CHIP_BLOCK1)
			{
				wire.pos = mPos;
				wire.pos.x = (int)mPos.x + (int)pos.x - 512;
				state_p = STATE_P::JUMP_WIRE;								//ܲ԰��������
			}
		}
	}
	

	return 0;
}

////////////////////////��������
int Player::StateFdown(void)
{

	if (lpMapCtl.CheckWall(pos + VECTOR2(divSize.x / 2, divSize.y / 2)) && (!lpMapCtl.CheckWall(pos + VECTOR2(divSize.x / 2, 0))))
	{
		pos.y -= 10;
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
		{
			mPos = mc->GetPoint();
			if (lpMapCtl.GetChipType(mPos + VECTOR2((int)pos.x - 512 + 300, 0)) >= CHIP_BLOCK1)
			{
				wire.pos = mPos;
				wire.pos.x = (int)mPos.x + (int)pos.x - 512;
				state_p = STATE_P::JUMP_WIRE;								//ܲ԰��������
			}
		}
	}

	if (!lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, 0)))
	{
		pos.x += speed;
	}
	jump += 0.6f;
	pos.y += jump;
	pos.y += gravity;
	animAdd = 0;
	return 0;
}

//////////////////////////////ܲ԰��������
int Player::StateSetWire(void)
{
	int wireOffset = (lpMapCtl.GetViewAreaSize().x / 3);
	if (lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, divSize.y / 2)))
	{
		speed = 0;
		state_p = STATE_P::RUN;
		return 0;
	}

	if (mPos.x > wireOffset)
	{
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)	//���C���[����
		{
			mPos = mc->GetPoint();
			if (lpMapCtl.GetChipType(mPos + VECTOR2((int)pos.x - 512 + 300, 0)) == CHIP_BLOCK1)
			{
				wire.pos = mPos;
				wire.pos.x = (int)mPos.x + (int)pos.x - 512;
			}
		}
	}
	else
	{
		state_p = STATE_P::RUN;
	}

	if (!(lpMapCtl.GetChipType(pos + VECTOR2(32, 33)) == CHIP_TYPE::CHIP_GRASS2))
	{
		pos.x -= 5.0;
	}
	else
	{
		pos.x -= 3.0f;
	}

	pos.x += speed;

	if ((cnt % 20) == 0)	//�̪��
	{
		lpEffect.AddEffectList("wireEff/shadow.png", VECTOR2(192, 192), VECTOR2(5, 5), VECTOR2(0, 0), 25, 2, VECTOR2(pos.x - 30, pos.y - 64));
	}


	if (state_p == STATE_P::SET_WIRE)
	{
		if ((lpMapCtl.GetChipType(pos + VECTOR2(divSize.x / 2, divSize.y)) == CHIP_TYPE::CHIP_BLANK))
		{
			state_p = STATE_P::FDOWN;
		}
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_RIGHT)
		{
			lpEffect.AddEffectList("wireEff/wire1.png", VECTOR2(192, 192), VECTOR2(5, 3), VECTOR2(0, 0), 15, 3, VECTOR2(pos.x, pos.y - 70));	//���C���[�G�t�F�N�g(��)
			lpShaker.SetShaker(true, 40, SHAKE_TYPE::SHAKE_XY);
			state_p = STATE_P::WIRE;
		}
		if (wire.pos.x <= (pos.x - wireOffset + offset))
		{
			state_p = STATE_P::RUN;
		}
	}
	animAdd = 1;
	return 0;
}

////////////////////////////////////�ެ��ߒ���ܲ԰����
int Player::StateJumpWire(void)
{
	int wireOffset = (lpMapCtl.GetViewAreaSize().x / 3);
	if (lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, divSize.y / 2)))
	{
		speed = 0;
		state_p = STATE_P::RUN;
		return 0;
	}

	if (mPos.x > wireOffset)
	{
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)	//���C���[����
		{
			mPos = mc->GetPoint();
			if (lpMapCtl.GetChipType(mPos + VECTOR2((int)pos.x - 512 + 300, 0)) == CHIP_BLOCK1)
			{
				wire.pos = mPos;
				wire.pos.x = (int)mPos.x + (int)pos.x - 512;
			}
		}
	}
	else
	{
		state_p = STATE_P::RUN;
	}

	if (!(lpMapCtl.GetChipType(pos + VECTOR2(32, 33)) == CHIP_TYPE::CHIP_GRASS2))
	{
		pos.x -= 5.0;
	}
	else
	{
		pos.x -= 3.0f;
	}

	pos.x += speed;
	pos.y += gravity;

	if ((cnt % 20) == 0)	//�̪��
	{
		lpEffect.AddEffectList("wireEff/shadow.png", VECTOR2(192, 192), VECTOR2(5, 5), VECTOR2(0, 0), 25, 2, VECTOR2(pos.x - 30, pos.y - 64));
	}


	if (state_p == STATE_P::JUMP_WIRE)
	{
		if ((lpMapCtl.GetChipType(pos + VECTOR2(divSize.x / 2, divSize.y)) == CHIP_TYPE::CHIP_BLANK)
			&& lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, divSize.y / 2)))
		{
			pos.y += gravity;
		}
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_RIGHT)
		{
			lpEffect.AddEffectList("wireEff/wire1.png", VECTOR2(192, 192), VECTOR2(5, 3), VECTOR2(0, 0), 15, 3, VECTOR2(pos.x, pos.y - 70));	//���C���[�G�t�F�N�g(��)
			lpShaker.SetShaker(true, 40, SHAKE_TYPE::SHAKE_XY);
			state_p = STATE_P::WIRE;
		}
		if (wire.pos.x <= (pos.x - wireOffset + offset))
		{
			state_p = STATE_P::RUN;
		}
	}
	animAdd = 1;
	return 0;
}

/////////////////////////////////ܲ԰����
int Player::StateWire(void)
{
	vec.x = wire.pos.x + offset - pos.x;
	vec.y = wire.pos.y - pos.y;
	vec.Normalize();

	if (!lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, divSize.y / 2)))
	{
		//�޸�قƈړ���
		pos.x += vec.fx * 16;
		pos.y += vec.fy * 14;
	}
	else
	{
		state_p = STATE_P::WIRE_DOWN;
	}

	pos.y += gravity;

	if (!lpMapCtl.CheckFloor(pos + VECTOR2(0, 50)))
	{
		if (((int)pos.y <= (int)mPos.y + 96) && (state_p == STATE_P::WIRE))		//�}�E�X���W�Ƀv���C���[���W������������ (�������player���W���炵�Ă�)
		{
			state_p = STATE_P::WIRE_DOWN;
		}
		if (lpMapCtl.CheckUpBlock(pos) && ((vec.fy) <= pos.y))
		{
			state_p = STATE_P::FDOWN;
		}
	}

	if (cnt % 10 == 0)		//ܲ԰�̪��(��)
	{
		lpEffect.AddEffectList("wireEff/wire2.png", VECTOR2(192, 192), VECTOR2(5, 4), VECTOR2(0, 0), 20, 3, VECTOR2(pos.x + lpMapCtl.GameDrawOffset().x - 15, pos.y + 5));
	}

	animAdd = 1;
	return 0;
}

///////////////////////////////ܲ԰�g�p��̗�������
int Player::StateWireDown(void)
{
	if (lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, divSize.y)) || (state_p == STATE_P::WIRE))
	{
		speed = 0;
	}

	if (lpMapCtl.CheckUpBlock(pos) && ((vec.fy) <= pos.y))
	{
		state_p = STATE_P::FDOWN;
	}
	else if (lpMapCtl.CheckWall(pos + VECTOR2(divSize.x/2, 0)))
	{
		state_p = STATE_P::FDOWN;
	}
	else if (lpMapCtl.CheckUpBlock(pos + VECTOR2(divSize.x/2, divSize.y/2)) && ((vec.fy) >= pos.y))
	{
		state_p = STATE_P::FDOWN;
	}

	pos.y += wireSpeed;
	pos.x += speed;
	animAdd = 0;
	wireSpeed += 0.25f;

	return 0;
}

bool Player::initAnim(void)
{
	AddAnim("����", 0, 0, 4, 4);
	return true;
}