#pragma once
#include "Obj.h"
#include "MouseCtl.h"


struct WIRE {
	VECTOR2 pos;
};

enum class STATE_P
{
	RUN,
	JUMP,
	FDOWN,
	SET_WIRE,
	WIRE,
	WIRE_DOWN,
	MAX
};

class Player :
	public Obj
{
public:
	Player();
	~Player();

	bool Update(void);
	void SetMove(void);
	void Draw(void);

	void ChangeState_P(STATE_P state_p)
	{
		this->state_p = state_p;
	};

	STATE_P GetState(void) { return state_p; }

private:
	WIRE wire;
	STATE_P state_p;

	bool initAnim(void);

	int animAdd;
	int offset;
	int cnt;

	char keyData[256];
	char keyDataOld[256];

	float jump;
	float grav;
	float wireSpeed;

	bool jumpLimit;
	
	VECTOR2 mPos;
	VECTOR2 oldPos;


	std::shared_ptr<MouseCtl> mc;

	int (Player::*StateTbl[(int)STATE_P::MAX])(void);
	//��ڲ԰�̃��[�u��Ԃ̕ێ�
	int StateRun(void);
	int StateJump(void);
	int StateFdown(void);
	int StateSetWire(void);
	int StateWire(void);
	int StateWireDown(void);
};

