#pragma once
#include "Obj.h"
#include "MouseCtl.h"
#include <memory>

class Player :
	public Obj
{
public:
	Player();
	//Player(const char(&_keyData)[256], const char(&_keyDataOld)[256], VECTOR2 chipOffset);
	~Player();

	bool Update(void);
	void Draw(void);
	void WireMove();
	int Init();
private:
	bool initAnim(void);
	
	char keyData[256];
	char keyDataOld[256];
	bool jumpFlag;
	int animAdd;
	float jump;
	float grav;

	std::shared_ptr<MouseCtl>mouseCtl;
	VECTOR2 locate;
};

