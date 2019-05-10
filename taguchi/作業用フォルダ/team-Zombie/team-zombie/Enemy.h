#pragma once
#include "Obj.h"

constexpr int ACCELMAX = 60;
constexpr int SPEED_MAX = 12;

enum class STATE{
	IDLE,
	RUN,
	JUMP,
	DAMAGE,
	ATTACK,
	MAX
};

struct WEAPON {
	WEAPON() :pos(0,0),mov(0,0),vec(0,0),size(0,0){}
	WEAPON(VECTOR2 pos, VECTOR2 mov, VECTOR2 vec, VECTOR2 size) :pos(pos), mov(mov), vec(vec), size(size) {}
	~WEAPON() {}
	VECTOR2 pos, mov, vec, size;
};


class Enemy :
	public Obj
{
public:
	Enemy();
	~Enemy();

	bool Update(void);
	void SetMove(void);
	void Draw(void);

private:
	char keyData[256];
	char keyDataOld[256];
	STATE state;
	WEAPON weapon;
	int animAdd;
	float jump;
	float aclCnt;
	int count;

	bool initAnim(void);

	int (Enemy::*EmodeTbl[(int)STATE::MAX])(void);

	int stateIdle(void);
	int stateRun(void);
	int stateJump(void);
	int stateDamage(void);
	int stateAttack(void);

};

