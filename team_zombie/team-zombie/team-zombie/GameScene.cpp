#include "GameScene.h"
#include "ResultScene.h"

GameScene::GameScene()
{
	Init();
}

GameScene::~GameScene()
{
}

int GameScene::Init()
{
	return 0;
}

BASE GameScene::Update(BASE & _this, const std::shared_ptr<MouseCtl>_mouseCtl)
{
	ClsDrawScreen();
	DrawString(0, 0, "gamemain", GetColor(0xff, 0xff, 0xff), true);
	ScreenFlip();
	mouseCtl = _mouseCtl;
	(*mouseCtl).Update();
	mouseBtn = mouseCtl->GetBtn();
	if ((_mouseCtl->GetBtn()[ST_NOW]) & (~_mouseCtl->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
	{
		return 	std::move(std::make_unique <ResultScene>());
	}
	return std::move(_this);
}
