#pragma once
#include <memory>
#include <array>
#include "DxLib.h"
#include "VECTOR2.h"

enum ST_TYPE
{
	ST_NOW,
	ST_OLD,	
	ST_MAX
};


struct MOUSE_DATA
{
	int data;
	VECTOR2 pos;
};

class MouseCtl;
using MOUSE_INT = std::array<int, ST_MAX>;

class Oprt_State
{
public:
	~Oprt_State();
	virtual void Update(int & data, VECTOR2 & pos) = 0;
	MOUSE_INT mouseBtn;
};

