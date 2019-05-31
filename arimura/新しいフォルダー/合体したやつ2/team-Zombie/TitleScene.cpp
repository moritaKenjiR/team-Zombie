#include "TitleScene.h"
#include "GameScene.h"
#include "MenuScene.h"
#include "GameTask.h"
#include "ImageMng.h"

TitleScene::TitleScene()
{
	loadEndFlag = false;
	cnt = 600;
	FontHandle = CreateFontToHandle("メイリオ", 48, 4, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	effPos[0] = { 0,0 };
	effPos[1] = { 1024,0 };
	SetMouseDispFlag(FALSE);
}

TitleScene::~TitleScene()
{
	
}

int TitleScene::Init()
{
	mouseCtl = std::make_shared<MouseCtl>();
	lpSoundCtl.AddSoundList("sound/pastel.mp3", loop, 50);
	return 0;
}

BASE TitleScene::Update(BASE & _this, const std::shared_ptr<MouseCtl> _mouseCtl)
{
	ClsDrawScreen();
	Move();
	Draw();

	mouseCtl = _mouseCtl;
	(*mouseCtl).Update();
	mouseBtn = mouseCtl->GetBtn();

	if (sceneEndFlag)
	{
		if (EndProcess())
		{
			lpSoundCtl.SoundDel();
			DeleteFontToHandle(FontHandle);
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


void TitleScene::Move(void)
{
	if (cntFlag)
	{
		animCnt++;
		if (animCnt >= 255)
		{
			cntFlag = false;
		}
	}
	else
	{
		animCnt--;
		if (animCnt <= 0)
		{
			cntFlag = true;
		}
	}
	for (int i = 0; i < 2; i++)
	{
		effPos[i].x--;
		if (effPos[i].x <= -1024)
		{
			effPos[i].x = 1024;
		}
	}
}

void TitleScene::Draw(void)
{
	DrawGraph(0, 0, IMAGE_ID("Image/titleImg.png")[0], true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, animCnt);
	DrawGraph(effPos[0].x, effPos[0].y, IMAGE_ID("Image/titleEff.png")[0], true);
	DrawTurnGraph(effPos[1].x, effPos[1].y, IMAGE_ID("Image/titleEff.png")[0], true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	if ((animCnt / 30) % 2)
	{
		DrawStringToHandle(256, 500, "右クリックでスタート！", 0x000000, FontHandle, 0x00ffff);
	}
}

bool TitleScene::LoadProcess(void)
{
	cnt -= 10;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cnt / 2);
	DrawBox(0, 0, 1024, 768, 0x000000, true);

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
	if (cnt >= 2000)
	{
		return true;
	}
	return false;
}

