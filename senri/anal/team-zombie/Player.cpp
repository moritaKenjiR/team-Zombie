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
	wireCnt = 0;
	wireTime = 0;
	cnt = 0;

	for (int i = 0; i < MAX_SPARK; i++)
	{
		Spark[i].Valid = 0;
	}

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
	MoveSpark();
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

	//ܲ԰�̸����ъ֌W
	if (wireTime == 1)
	{
		wireCnt = WIRE_CNT + lpGameTask.GetPrgTime();
	}
	if (wireCnt <= 0)
	{
		wireCnt = 0;
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

	//����ۯ��ɂ����Ă̽�߰�ތ���
	if (lpMapCtl.GetChipType(pos + VECTOR2(32, 33)) == CHIP_TYPE::CHIP_GRASS2)
	{
		speed = 4.0f;
	}
	else
	{
		speed = 8.0f;
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

	if (state_p == STATE_P::WIRE && (cnt % 10) == 0)
	{
		lpEffect.AddEffectList("a/�Ռ�32.png", VECTOR2(192, 192), VECTOR2(5, 4), VECTOR2(0, 0), 18, 3, VECTOR2(pos.x - 50, pos.y - 100));
	}
	else
	{
		lpEffect.SetEffPos("a/�Ռ�32.png", VECTOR2(pos.x - 50, pos.y - 100));

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

	int a = GetRand(20);
	int b = GetRand(20);
	int c = GetRand(30);

	//for (int j = 0; j < MAX_SPARK; j++)
	//{
	//	// �Ήԃf�[�^���L���Ȏ��̂ݕ`��
	//	if (Spark[j].Valid == 1)
	//	{
	//		DrawPixel(Spark[j].X / 100 - 2, Spark[j].Y / 100 + 10, GetColor(0, 255, 255));
	//		DrawPixel(Spark[j].X / 100 - 1, Spark[j].Y / 100 + 10, GetColor(0, 0, 0));
	//		DrawPixel(Spark[j].X / 100 + 1, Spark[j].Y / 100 + 10, GetColor(0, 255, 127));
	//		DrawPixel(Spark[j].X / 100 + 2, Spark[j].Y / 100 + 10, GetColor(255, 255, 255));
	//		//DrawBox(Spark[j].X / 100, Spark[j].Y / 100, Spark[j].X / 100+ a, Spark[j].Y / 100 + b,GetColor(Spark[j].Bright, Spark[j].Bright, Spark[j].Bright), true);
	//	}

	//}

	DrawFormatString(900, 650, 0x000000, "wireCnt:%d", wireCnt);
	//�b���\��
	DrawString(0, SCREEN_SIZE_Y - 20, (std::to_string(lpGameTask.GetPrgTime()) + "�b").c_str(), 0xffffff);
	if (state_p == STATE_P::SET_WIRE || state_p == STATE_P::WIRE)
	{
		DrawLine(pos.x + lpMapCtl.GameDrawOffset().x + 32, pos.y + lpMapCtl.GameDrawOffset().y + 42, wire.pos.x + lpMapCtl.GameDrawOffset().x, wire.pos.y, GetColor(0,0,0));
	}
}

bool Player::initAnim(void)
{
	AddAnim("����", 0, 0, 4, 4);
	return true;
}

void Player::CreateSpark(int x, int y)
{
	int i;

	// �g���Ă��Ȃ��Ήԃf�[�^��T��
	for (i = 0; i < MAX_SPARK; i++)
	{
		if (Spark[i].Valid == 0) break;
	}
	if (i != MAX_SPARK)
	{
		// �ΉԂ̈ʒu��ݒ�
		Spark[i].X = x * 100;
		Spark[i].Y = y * 100;

		// �ړ��͂�ݒ�
		Spark[i].Sx = -GetRand(2000) - 500;
		Spark[i].Sy = GetRand((500));

		// �ΉԂ̏d�����Z�b�g
		Spark[i].G = GetRand(1);

		// �ΉԂ̖��邳�Z�b�g
		Spark[i].Bright = 255;

		// �Ήԃf�[�^���g�p���ɃZ�b�g
		Spark[i].Valid = 1;
	}
}

void Player::MoveSpark(void)
{
	int i;

	// �ΉԂ̈ړ�����
	for (i = 0; i < MAX_SPARK; i++)
	{
		// �Ήԃf�[�^��������������X�L�b�v
		if (Spark[i].Valid == 0) continue;

		// �ʒu���ړ��͂ɉ����Ă��炷
		Spark[i].Y += Spark[i].Sy;
		Spark[i].X += Spark[i].Sx;

		// �ړ��͂�ύX
		Spark[i].Sy += Spark[i].G;

		// �ΉԂ̖��邳��������
		Spark[i].Bright -= 5;

		// �ΉԂ̖��邳���O�ȉ��ɂȂ�����Ήԃf�[�^�𖳌��ɂ���
		if (Spark[i].Bright < 0) Spark[i].Valid = 0;
	}
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
	lpEffect.SetEffPos("a/��5.png", VECTOR2(pos.x - 32, pos.y-16));

	if (state_p == STATE_P::SET_WIRE)
	{
		if ((lpMapCtl.GetChipType(pos + VECTOR2(0, divSize.y)) == CHIP_TYPE::CHIP_BLANK)
			&& (lpMapCtl.GetChipType(pos + divSize) == CHIP_TYPE::CHIP_BLANK))
		{
			state_p = STATE_P::FDOWN;
		}
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_RIGHT)
		{
			lpEffect.AddEffectList("a/��5.png", VECTOR2(192, 192), VECTOR2(5, 3), VECTOR2(0, 0), 15, 3, VECTOR2(pos.x - 64, pos.y-16));
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
	int R = GetRand(500);
	for (int j = 0; j < R; j++)
	{
		CreateSpark(pos.x + lpMapCtl.GameDrawOffset().x + 32, pos.y + lpMapCtl.GameDrawOffset().y + 42);
	}

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

