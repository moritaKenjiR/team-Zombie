#pragma once
#include "Obj.h"
#include "MouseCtl.h"

#define WIRE_CNT 5		//ワイヤー使用可能の時間

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
	MAX
};

class Camera;

class Player :
	public Obj
{
public:
	Player();
	~Player();

	bool Update(void);
	void SetMove(void);
	void Draw(void);
	bool SetWire(void);
	bool Wire(void);

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
	char keyData[256];
	char keyDataOld[256];
	
	VECTOR2 mPos;
	VECTOR2 vec;
	int wireCnt;	//わいやーくーるたいむ
	int wireTime;	//ワイヤーのクールタイム
	float jump;
	float grav;
	float wireSpeed;



	int (Player::*StateTbl[(int)STATE_P::MAX])(void);
	//ﾌﾟﾚｲﾔｰのムーブ状態の保持
	int StateRun(void);
	int StateJump(void);
	int StateFdown(void);
	int StateSetWire(void);
	int StateWire(void);

	std::shared_ptr<MouseCtl> mc;
};

