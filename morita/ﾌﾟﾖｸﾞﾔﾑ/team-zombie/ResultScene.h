#pragma once
#include "BaseScene.h"
class ResultScene :
	public BaseScene
{
public:
	ResultScene();
	~ResultScene();
	virtual int Init();
	virtual BASE Update(BASE &_this, const std::shared_ptr<MouseCtl> _mouseCtl);

	std::shared_ptr<MouseCtl>mouseCtl;
};

