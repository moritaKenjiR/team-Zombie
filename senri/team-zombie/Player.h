#pragma once
#include "Obj.h"
class Player :
	public Obj
{
public:
	Player();
	//Player(const char(&_keyData)[256], const char(&_keyDataOld)[256], VECTOR2 chipOffset);
	~Player();

	bool Update(void);
	void Draw(void);
private:
	bool initAnim(void);
	int animAdd;
	char keyData[256];
	char keyDataOld[256];
	bool Jumpflag;
	float jump;
};

