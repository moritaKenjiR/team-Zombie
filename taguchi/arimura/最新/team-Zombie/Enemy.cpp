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
	jump = -10.0f;
	grav = 5.0f;
	mov = { 0,0 };
	changeJump = false;
	state = STATE::UPJUMP;
	EmodeTbl[(int)STATE::IDLE] = &Enemy::stateIdle;
	EmodeTbl[(int)STATE::RUN] = &Enemy::stateRun;
	EmodeTbl[(int)STATE::JUMP] = &Enemy::stateJump;
	EmodeTbl[(int)STATE::UPJUMP] = &Enemy::stateUpJump;
	EmodeTbl[(int)STATE::FDOWN] = &Enemy::stateFDown;
	EmodeTbl[(int)STATE::DAMAGE] = &Enemy::stateDamage;
	EmodeTbl[(int)STATE::ATTACK] = &Enemy::stateAttack;
}


Enemy::~Enemy()
{
}

bool Enemy::Update(void)
{
	lpEnemyAI.CreateMove(*this);
	animAdd = 0;
	mov = { 0,0 };
	(this->*EmodeTbl[(int)state])();
	if (lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, divSize.y / 4 * 2)))
	{
		mov.x = 0;
	}
	pos += mov;
	animAdd = 1;
	animCnt += animAdd;
	return true;
}

void Enemy::Draw(void)
{
	//SetDrawBlendMode(DX_BLENDMODE_ALPHA, 0);
	Obj::Draw();
	//lpEnemyAI.Draw(*this);
	//SetDrawBlendMode(DX_BLENDMODE_NOBLEND,0);
	DrawBox(pos.x + 32 * 1 + lpMapCtl.GameDrawOffset().x, pos.y + 32 * 3 + lpMapCtl.GameDrawOffset().y,
			pos.x + 32 * 2 + lpMapCtl.GameDrawOffset().x, pos.y + 32 * 4 + lpMapCtl.GameDrawOffset().y, 0x000000, false);

	DrawBox(pos.x + 32 * 2 + lpMapCtl.GameDrawOffset().x, pos.y + 32 * 2 + lpMapCtl.GameDrawOffset().y,
			pos.x + 32 * 3 + lpMapCtl.GameDrawOffset().x, pos.y + 32 * 3 + lpMapCtl.GameDrawOffset().y, 0xff0000, false);

	DrawBox(pos.x + 32 * 2 + lpMapCtl.GameDrawOffset().x, pos.y + 32 * 3 + lpMapCtl.GameDrawOffset().y,
			pos.x + 32 * 3 + lpMapCtl.GameDrawOffset().x, pos.y + 32 * 4 + lpMapCtl.GameDrawOffset().y, 0x00ff00, false);

	DrawBox(pos.x + 32 * 2 + lpMapCtl.GameDrawOffset().x, pos.y + 32 * 4 + lpMapCtl.GameDrawOffset().y,
			pos.x + 32 * 3 + lpMapCtl.GameDrawOffset().x, pos.y + 32 * 5 + lpMapCtl.GameDrawOffset().y, 0x0000ff, false);
	//DrawString(0, 0, "Enter �ړ��@Space �W�����v�@Up ��~", 0xffffff);
	
}

bool Enemy::initAnim(void)
{
	AddAnim("����", 0, 1, 8, 5);
	AddAnim("�W�����v", 1,1, 4, 12);
	AddAnim("��~", 0, 0, 5, 5);
	//AddAnim("�U��", 0, 2, 4, 12);
	return true;
}

int Enemy::stateIdle(void)
{
	SetAnim("��~");
	return 0;
}

int Enemy::stateRun(void)
{
	mov.x += speed;
	jump = -10.0f;
	changeJump = false;
	SetAnim("����");

	return 0;
}

int Enemy::stateJump(void)
{
	mov.y += jump;
	mov.x += speed;
	jump += 0.3f;
	SetAnim("�W�����v");
	
	return 0;
}

int Enemy::stateUpJump(void)
{
	if (!changeJump)
	{
		changeJump = true;
		jump = -15.0f;
	}
	mov.y += jump;
	mov.x += speed / 3.0f;
	jump += 0.3f;
	return 0;
}

int Enemy::stateFDown(void)
{
	mov.x += speed;
	mov.y += grav;
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