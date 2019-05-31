#pragma once
#include <memory>
#include "DxLib.h"
#include "MouseCtl.h"

struct BaseScene;
using BASE = std::unique_ptr<BaseScene>;


struct BaseScene
{
public:
	~BaseScene();

	virtual int Init() = 0;
	virtual BASE Update(BASE &_this, const std::shared_ptr<MouseCtl> _mouseCtl) = 0;
	
	std::shared_ptr<MouseCtl>mosueCtl;
	MOUSE_INT mouseBtn;
};

