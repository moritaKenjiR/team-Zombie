#pragma once
#include "BaseScene.h"
class TitleScene :
	public BaseScene
{
public:
	~TitleScene();
	virtual int Init();
	virtual BASE Update(BASE &_this, const std::shared_ptr<MouseCtl> _mouseCtl);

	std::shared_ptr<MouseCtl>mouseCtl;
};

