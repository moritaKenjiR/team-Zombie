#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "VECTOR2.h"

//ｱﾆﾒｰｼｮﾝ　ID,　ｺﾏ数,　ｱﾆﾒｰｼｮﾝの間隔
enum ANIM_TBL
{
	ANIM_START_ID,
	ANIM_FLAME,
	ANIM_WAIT,
	ANIM_MAX
};

class Obj
{
public:
	Obj();
	//Obj(const char(&_keyData)[256], const char(&_keyDataOld)[256], VECTOR2 chipOffset);
	~Obj();
	void init(std::string filename, VECTOR2 divSize, VECTOR2 divCnt, VECTOR2 chipOffset, int speed, int animSpeed, int stateAnimDiv);

	const VECTOR2& GetPos(void);
	void SetPos(const VECTOR2& pos);

	virtual bool Update(void);		//更新処理
	virtual void Draw(void);		//描画関数
	void Draw(int id);
	bool SetAnim(std::string _AnimName);
private:
	virtual bool initAnim(void);
	//名前空間::ﾃﾝﾌﾟﾚｰﾄ名<型> 変数名;
	// std::vector<型> 変数名;
	
	//
	std::map<std::string, int[ANIM_MAX]> animMap;
	std::string animName;					// ｱﾆﾒｰｼｮﾝの名前
	//
protected:
	std::string imageName;
	//const char(&keyData)[256];
	//const char(&keyDataOld)[256];
	VECTOR2 pos;			//描画の位置
	int speed;
	VECTOR2 divSize;		//描画ﾁｯﾌﾟのｻｲｽﾞ
	VECTOR2 divCnt;			//描画ﾁｯﾌﾟの数(縦横）
	VECTOR2 chipOffset;		//描画ﾁｯﾌﾟのｲﾒｰｼﾞ
	VECTOR2 drawOffset;
	int animCnt;
	int animSpeed;
	int stateAnimDiv;

	bool AddAnim(std::string AnimName, int id_x, int id_y, int flame, int waitCnt);
	bool turn;
	VECTOR2 oldPos;
};

