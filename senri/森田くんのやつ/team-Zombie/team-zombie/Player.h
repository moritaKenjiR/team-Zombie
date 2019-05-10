#pragma once
#include "Obj.h"
#include "MouseCtl.h"


struct WIRE {
	VECTOR2 pos;
};

class Camera;

class Player :
	public Obj
{
public:
	Player(std::unique_ptr<Camera> camera);
	//Player(const char(&_keyData)[256], const char(&_keyDataOld)[256], VECTOR2 chipOffset);
	~Player();

	bool Update(void);
	void SetMove(void);
	void Draw(void);
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
	float jump;
	float wireSpeed;

	std::shared_ptr<MouseCtl> mc;
	std::unique_ptr<Camera> camera;
};

