#pragma once
#include "Obj.h"
#include "MouseCtl.h"


#define WIRE_CNT 5		//ワイヤー使用可能の時間
#define MAX_SPARK 10000

struct WIRE {
	VECTOR2 pos;
};

typedef struct tagSPARK
{
	int Valid;	// このデータが使用中か、フラグ
	int X, Y;	// 火花の位置
	int Sx, Sy;	// 火花の移動力
	int G;	// 火花の重さ
	int Bright;	// 火花の明るさ
} SPARK;

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
	SPARK Spark[MAX_SPARK];	// 火花データ
	WIRE wire;
	STATE_P state_p;
	bool initAnim(void);
	void CreateSpark(int x, int y);	// 火花を出す処理
	void MoveSpark(void);	// 火花移動処理

	int animAdd;
	int wireCnt;	//わいやーくーるたいむ
	int wireTime;	//ワイヤーのクールタイム
	char keyData[256];
	char keyDataOld[256];
	float jump;
	float grav;
	float wireSpeed;
	
	VECTOR2 mPos;
	VECTOR2 oldPos;
	int cnt;

	std::shared_ptr<MouseCtl> mc;

	int (Player::*StateTbl[(int)STATE_P::MAX])(void);
	//ﾌﾟﾚｲﾔｰのムーブ状態の保持
	int StateRun(void);
	int StateJump(void);
	int StateFdown(void);
	int StateSetWire(void);
	int StateWire(void);
	int StateWireDown(void);};

