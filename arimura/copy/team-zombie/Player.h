#pragma once
#include "Obj.h"
#include "MouseCtl.h"

#define WIRE_CNT 5		//ワイヤー使用可能の時間

struct WIRE {
	VECTOR2 pos;
};

class Camera;

class Player :
	public Obj
{
public:
	Player();
	//Player(const char(&_keyData)[256], const char(&_keyDataOld)[256], VECTOR2 chipOffset);
	~Player();

	bool Update(void);
	void SetMove(void);
	void Draw(void);
	bool SetWire(void);
	bool Wire(void);
private:
	WIRE wire;
	bool initAnim(void);
	int animAdd;
	char keyData[256];
	char keyDataOld[256];
	bool Jumpflag;		//ジャンプ中フラグ
	bool Readyflag;		//ワイヤー準備フラグ
	bool Wireflag;		//ワイヤー中フラグ
	bool DownFlag;		//ワイヤー中の落下フラグ
	//bool Wireflag;
	
	VECTOR2 mPos;
	int wireCnt;	//わいやーくーるたいむ
	int wireTime;	//ワイヤーのクールタイム
	float jump;
	float wireSpeed;

	std::shared_ptr<MouseCtl> mc;
};

