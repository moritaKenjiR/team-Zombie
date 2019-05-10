#include "TitleScene.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "GameTask.h"

TitleScene::~TitleScene()
{
	Init();
}

int TitleScene::Init()
{
	mouseCtl = std::make_shared<MouseCtl>();
	return 0;
}

BASE TitleScene::Update(BASE & _this, const std::shared_ptr<MouseCtl> _mouseCtl)
{
	ClsDrawScreen();
	DrawString(0, 0, "Title", GetColor(0xff, 0xff, 0xff), true);
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
