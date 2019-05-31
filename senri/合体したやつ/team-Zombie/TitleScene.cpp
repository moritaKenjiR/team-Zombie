#include "TitleScene.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "GameTask.h"
#include "ImageMng.h"

TitleScene::TitleScene()
{
	loadEndFlag = false;
	cnt = 600;
}

TitleScene::~TitleScene()
{
	
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
	DrawGraph(0, 0, IMAGE_ID("Image/title.png")[0],true);
	
	mouseCtl = _mouseCtl;
	(*mouseCtl).Update();
	mouseBtn = mouseCtl->GetBtn();

	if (sceneEndFlag)
	{
		if (EndProcess())
		{
			return std::move(std::make_unique <MenuScene>());
		}
	}
	else if (!loadEndFlag)
	{
		if (LoadProcess())
		{
			loadEndFlag = true;
		}
		
	}

	if ((_mouseCtl->GetBtn()[ST_NOW]) & (~_mouseCtl->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
	{
		sceneEndFlag = true;
	}
	DrawGraph(_mouseCtl->GetPoint().x, _mouseCtl->GetPoint().y, IMAGE_ID("Image/mouseCursor.png")[0], true);
	ScreenFlip();
	return std::move(_this);
}

bool TitleScene::LoadProcess(void)
{
	cnt -= 10;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cnt/2);
	//DrawCircle(0, 0, cnt * 2, 0x000000, true);
	//DrawCircle(1024, 0, cnt * 2, 0x000000, true);
	//DrawCircle(0, 768, cnt * 2, 0x000000, true);
	//DrawCircle(1024, 768, cnt * 2, 0x000000, true);
	DrawBox(0,0,1024,768,0x000000,true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	if (cnt <= 0)
	{
		return true;
	}
	return false;
}

bool TitleScene::EndProcess(void)
{
	cnt += 10;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cnt);
	DrawCircle(0, 0, cnt * 2, 0x000000, true);
	DrawCircle(1024, 0, cnt * 2, 0x000000, true);
	DrawCircle(0, 768, cnt * 2, 0x000000, true);
	DrawCircle(1024, 768, cnt * 2, 0x000000, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	if (cnt >= 400)
	{
		DrawGraph(128, 128, IMAGE_ID("Image/Desc.png")[0], true);
	}
	if (cnt >=2000)
	{
		return true;
	}
	return false;
}
