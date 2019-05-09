#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "VECTOR2.h"

//±ÆÒ°¼®İ@ID,@ºÏ”,@±ÆÒ°¼®İ‚ÌŠÔŠu
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

	virtual bool Update(void);		//XVˆ—
	virtual void Draw(void);		//•`‰æŠÖ”
	void Draw(int id);
	bool SetAnim(std::string _AnimName);
private:
	virtual bool initAnim(void);
	//–¼‘O‹óŠÔ::ÃİÌßÚ°Ä–¼<Œ^> •Ï”–¼;
	// std::vector<Œ^> •Ï”–¼;
	
	//
	std::map<std::string, int[ANIM_MAX]> animMap;
	std::string animName;					// ±ÆÒ°¼®İ‚Ì–¼‘O
	//
protected:
	std::string imageName;
	//const char(&keyData)[256];
	//const char(&keyDataOld)[256];
	VECTOR2 pos;			//•`‰æ‚ÌˆÊ’u
	int speed;
	VECTOR2 divSize;		//•`‰æÁ¯Ìß‚Ì»²½Ş
	VECTOR2 divCnt;			//•`‰æÁ¯Ìß‚Ì”(c‰¡j
	VECTOR2 chipOffset;		//•`‰æÁ¯Ìß‚Ì²Ò°¼Ş
	VECTOR2 drawOffset;
	int animCnt;
	int animSpeed;
	int stateAnimDiv;

	bool AddAnim(std::string AnimName, int id_x, int id_y, int flame, int waitCnt);
	bool turn;
};

