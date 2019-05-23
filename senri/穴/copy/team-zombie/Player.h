#pragma once
#include "Obj.h"
#include "MouseCtl.h"

#define WIRE_CNT 5		//ワイヤー使用可能の時間
#define MAX_SPARK 10000

struct WIRE {
	VECTOR2 pos;
};

struct IMAGE {
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
	bool Wire(void);
private:
	SPARK Spark[MAX_SPARK];	// 火花データ
	WIRE wire;
	bool initAnim(void);
	int animAdd;
	char keyData[256];
	char keyDataOld[256];
	bool Jumpflag;		//ジャンプ中フラグ
	bool Readyflag;		//ワイヤー準備フラグ
	bool Wireflag;		//ワイヤー中フラグ
	bool DownFlag;		//ワイヤー中の落下フラグ
	void CreateSpark(int x, int y);	// 火花を出す処理
	void MoveSpark(void);	// 火花移動処理
	//bool Wireflag;
	
	VECTOR2 mPos;
	int wireCnt;	//わいやーくーるたいむ
	int wireTime;	//ワイヤーのクールタイム
	float jump;
	float wireSpeed;
	IMAGE Image;

	std::shared_ptr<MouseCtl> mc;
};

