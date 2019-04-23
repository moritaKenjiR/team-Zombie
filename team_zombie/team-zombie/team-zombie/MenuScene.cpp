#include "MenuScene.h"
#include "GameScene.h"



MenuScene::MenuScene()
{
}


MenuScene::~MenuScene()
{
}

int MenuScene::Init()
{
	return 0;
}

BASE MenuScene::Update(BASE & _this, const std::shared_ptr<MouseCtl> _mouseCtl)
{
	ClsDrawScreen();
	DrawString(0, 0, "menu", GetColor(0xff, 0xff, 0xff), true);
	ScreenFlip();
	mouseCtl = _mouseCtl;
	(*mouseCtl).Update();
	mouseBtn = mouseCtl->GetBtn();
	if ((_mouseCtl->GetBtn()[ST_NOW]) & (~_mouseCtl->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
	{
		return std::move(std::make_unique <GameScene>());
	}

	return std::move(_this);
}
