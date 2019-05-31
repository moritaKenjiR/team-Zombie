#pragma once
#include "Obj.h"

constexpr int ACCELMAX = 60;
constexpr int SPEED_MAX = 12;

enum class STATE{
	IDLE,
	RUN,
	JUMP,
	UPJUMP,
	FDOWN,
	DAMAGE,
	ATTACK,
	MAX
};



class Enemy :
	public Obj
{
public:
	Enemy();
	~Enemy();

	bool Update(void);
	void Draw(void);

	void ChangeState(STATE state)
	{
		this->state = state;
	};
	STATE GetState(void) { return state; }

private:
	STATE state;
	VECTOR2 mov;
	int animAdd;
	float jump;
	float aclCnt;
	int count;
	int grav;
	bool changeJump;

	bool initAnim(void);

	int (Enemy::*EmodeTbl[(int)STATE::MAX])(void);

	int stateIdle(void);
	int stateRun(void);
	int stateJump(void);
	int stateUpJump(void);
	int stateFDown(void);
	int stateDamage(void);
	int stateAttack(void);

};

