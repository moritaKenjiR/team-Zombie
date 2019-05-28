#include "ResultScene.h"
#include "MenuScene.h"

ResultScene::ResultScene()
{
}

ResultScene::~ResultScene()
{
}

int ResultScene::Init()
{
	
	return 0;
}

BASE ResultScene::Update(BASE & _this, const std::shared_ptr<MouseCtl> _mouseCtl)
{
	ClsDrawScreen();
	DrawString(0, 0, "result", GetColor(0xff, 0xff, 0xff), true);
	ScreenFlip();
	
	mouseCtl = _mouseCtl;
	(*mouseCtl).Update();
	mouseBtn = mouseCtl->GetBtn();
	if ((_mouseCtl->GetBtn()[ST_NOW]) & (~_mouseCtl->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
	{
		return std::move(std::make_unique <MenuScene>());
	}

	return std::move(_this);
}
