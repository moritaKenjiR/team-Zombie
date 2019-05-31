#pragma once
#include <memory>
#include "VECTOR2.h"
#include "Obj.h"


class Camera
{
public:
	Camera();
	~Camera();
	VECTOR2 &GetPos(void);
	bool SetPos(VECTOR2 pos);
	bool SetPos(float x, float y);
	bool SetTarget(std::weak_ptr<Obj> target);
	
	bool Update(void);
	void Draw(void);
private:
	std::weak_ptr<Obj> target;
	VECTOR2 pos;
	RECT antiMoveRect;
};