#include "TitleScene.h"
#include "SoundMng.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "GameTask.h"
#include "ImageMng.h"


TitleScene::TitleScene()
{
	Init();
}


TitleScene::~TitleScene()
{
}

int TitleScene::Init()
{
	//mouseCtl = std::make_shared<MouseCtl>();
	lpSoundCtl.AddSoundList("sound/pastel.mp3",loop,80);
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
		lpSoundCtl.SoundDel();
		return std::move(std::make_unique <MenuScene>());
	}

	return std::move(_this);
}
