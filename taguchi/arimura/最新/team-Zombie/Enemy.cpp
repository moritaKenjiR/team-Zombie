#include "Dxlib.h"
#include "Enemy.h"
#include "MapCtl.h"
#include "EnemyAI.h"



Enemy::Enemy()
{
	pos = { 0,0 };
	animAdd = 0;
	aclCnt = 0;
	count = 0;
	jump = -12.0f;
	grav = 5.0f;
	state = STATE::FDOWN;
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
	lpEnemyAI.CreateMove((*this));
	animAdd = 0;
	(this->*EmodeTbl[(int)state])();
	animAdd = 1;
	animCnt += animAdd;
	return true;
}

void Enemy::Draw(void)
{
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	Obj::Draw();
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	DrawBox(0, 40, 0 + aclCnt, 80, 0x000000, true);
	//DrawString(0, 0, "Enter �ړ��@Space �W�����v�@Up ��~", 0xffffff);
	
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
	
	return 0;
}

int Enemy::stateFDown(void)
{
	pos.x += speed;
	pos.y += grav;
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