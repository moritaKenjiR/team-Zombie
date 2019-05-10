#include "Dxlib.h"
#include "Enemy.h"
#include "MapCtl.h"



Enemy::Enemy()
{
	pos = { 0,0 };
	animAdd = 0;
	aclCnt = 0;
	count = 0;
	jump = -14.0f;
	state = STATE::RUN;
	EmodeTbl[(int)STATE::IDLE] = &Enemy::stateIdle;
	EmodeTbl[(int)STATE::RUN] = &Enemy::stateRun;
	EmodeTbl[(int)STATE::JUMP] = &Enemy::stateJump;
	EmodeTbl[(int)STATE::FDOWN] = &Enemy::stateFDown;
	EmodeTbl[(int)STATE::DAMAGE] = &Enemy::stateDamage;
	EmodeTbl[(int)STATE::ATTACK] = &Enemy::stateAttack;
}


Enemy::~Enemy()
{
}

bool Enemy::Update(void)
{
	memcpy(keyDataOld, keyData, sizeof(keyDataOld));
	GetHitKeyStateAll(keyData);
	SetMove();
	(this->*EmodeTbl[(int)state])();
	return true;
}

void Enemy::SetMove(void)
{
	animAdd = 0;
	
	if (lpMapCtl.CheckFloor(pos + VECTOR2(0, 50)))
	{
		if (keyData[KEY_INPUT_NUMPADENTER] && !keyDataOld[KEY_INPUT_NUMPADENTER]) state = STATE::RUN;
		//else if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE]) state = STATE::JUMP;
		else if (keyData[KEY_INPUT_RIGHT] && !keyDataOld[KEY_INPUT_RIGHT])state = STATE::RUN;
		//else if(keyData[KEY_INPUT_UP] && !keyDataOld[KEY_INPUT_UP])state = STATE::IDLE;
		//else if (keyData[KEY_INPUT_DOWN] && !keyDataOld[KEY_INPUT_DOWN]) state = STATE::DAMAGE;
		//else if (keyData[KEY_INPUT_RIGHT] && !keyDataOld[KEY_INPUT_RIGHT])state = STATE::ATTACK;

		if (lpMapCtl.GetChipType(pos + VECTOR2(divSize.x, divSize.y - lpMapCtl.GetChipSize().y)) == CHIP_TYPE::CHIP_BLOCK)
		{
			state = STATE::JUMP;
		}
		else
		{
			state = STATE::RUN;
		}
	}
	else if ((lpMapCtl.GetChipType(pos + VECTOR2(0, divSize.y)) == CHIP_TYPE::CHIP_BLANK) && (lpMapCtl.GetChipType(pos + divSize) == CHIP_TYPE::CHIP_BLANK) && (state != STATE::JUMP))
	{
		for (int i = pos.y / 32 + 1; i < CHIP_CNT_Y; ++i)
		{
			if (lpMapCtl.GetChipType(VECTOR2(pos.x, i * lpMapCtl.GetChipSize().y)) != CHIP_TYPE::CHIP_BLANK)
			{
				state = STATE::FDOWN;
				break;
			}
			else if (i == CHIP_CNT_Y - 1)
			{
				state = STATE::JUMP;
			}
		}
	}
	//else if (state != STATE::JUMP)
	//{
	//	state = STATE::JUMP;
 //  		//pos.y -= jump;
	//}

	/*if (state != STATE::DAMAGE)
	{
		aclCnt += 0.3f;
		if (aclCnt >= ACCELMAX)
		{
			if (speed <= SPEED_MAX)
			{
				speed += 1;
			}
			aclCnt = 0;
		}
	}*/

	//if (count > 60)
	//{
	//	count = 0;
	//	state = STATE::RUN;
	//}

	if (pos.x >= 10000)
	{
		state = STATE::IDLE;
	}
	animAdd = 1;
	animCnt += animAdd;
}

void Enemy::Draw(void)
{
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	Obj::Draw();
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	DrawBox(0, 40, 0 + aclCnt, 80, 0x000000, true);
	DrawString(0, 0, "Enter �ړ��@Space �W�����v�@Up ��~", 0xffffff);
	
}

bool Enemy::initAnim(void)
{
	AddAnim("����", 1, 0, 6, 7);
	AddAnim("�W�����v", 0, 2, 8, 12);
	AddAnim("��~", 0, 0, 1, 1);
	AddAnim("�U��", 0, 2, 4, 12);
	return true;
}

int Enemy::stateIdle(void)
{
	SetAnim("��~");
	return 0;
}

int Enemy::stateRun(void)
{
	pos.x += speed;
	jump = -12.0f;
	SetAnim("����");

	return 0;
}

int Enemy::stateJump(void)
{
	pos.y += jump;
	pos.x += speed;
	jump += 0.3f;
	animAdd = 0;
	SetAnim("�W�����v");
	animAdd = 1;

	if (lpMapCtl.CheckFloor(pos + VECTOR2(0, 50)))
	{
		state = STATE::RUN;
	}
	
	return 0;
}

int Enemy::stateFDown(void)
{
	pos.y += 3.0f;
	return 0;
}

int Enemy::stateDamage(void)
{
	//enemy�̓_���[�W��H�炤��60�t���[���ԍs�����~���A���̃t���[���Ԃ�aclCnt�����炳���@
	//��萔aclCnt�����炳���ƃX�s�[�h����������
	aclCnt -= 0.3f;
	if (aclCnt <= 0.0f)
	{
		speed -= 2;
		aclCnt = ACCELMAX;
	}
	SetAnim("��~");
	count++;
	return 0;
}

int Enemy::stateAttack(void)
{

	SetAnim("�U��");
	return 0;
}