#include "DxLib.h"
#include <string>
#include "MouseCtl.h"
#include "GameTask.h"

MouseCtl::MouseCtl()
{
}


MouseCtl::~MouseCtl()
{
}

MOUSE_INT MouseCtl::GetBtn()const
{
	return mouseBtn;
}

VECTOR2 MouseCtl::GetPoint()const
{
	return pos;
}

//void MouseCtl::Update(std::weak_ptr<GameBoard> gameboard)
//{
//	bool EndFlag = false;
//	mouseBtn[ST_OLD] = 0;
//
//
//	mouseBtn[ST_OLD] = mouseBtn[ST_NOW];
//	mouseBtn[ST_NOW] = oprt_state->GetMouseData().data;
//	pos = oprt_state->GetMouseData().pos;
//
//}

void MouseCtl::Update()
{
	int x, y;
	mouseBtn[ST_OLD] = mouseBtn[ST_NOW];
	mouseBtn[ST_NOW] = GetMouseInput();
	GetMousePoint(&x, &y);
	pos.x = x;
	pos.y = y;
}