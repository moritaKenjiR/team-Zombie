#pragma once
#include <array>
#include "VECTOR2.h"
#include "Oprt_State.h"

class MouseCtl
{
public:
	MouseCtl();
	~MouseCtl();

	MOUSE_INT GetBtn() const;
	VECTOR2 GetPoint() const;

	void Update();
private:
	std::unique_ptr<Oprt_State>oprt_state;
	VECTOR2 pos;
	int data = 0;
	MOUSE_INT mouseBtn;
	MOUSE_DATA mouse_data;
};



