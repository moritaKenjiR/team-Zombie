#include "MenuScene.h"
#include "GameScene.h"
#include "TitleScene.h"
#include "MapCtl.h"
#include "ImageMng.h"



MenuScene::MenuScene()
{
	selectStage = 1; 
	moveTime = 0;
	cnt = 600;
	sceneEndFlag = false;
	loadEndFlag = false;
	animCnt = 0;
	MoveEndFlag = true;
	extFlag = false;
	upFlag = true;
	backPos = { 0,0};
	FontHandle = CreateFontToHandle("メイリオ",32,4, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	stageList.push_back(movingObj{VECTOR2(1280,200),"Image/tmp2.png",POS_C,MAP_2});
	stageList.push_back(movingObj{VECTOR2(256, 200),"Image/tmp1.png",POS_L,MAP_1});
	stageList.push_back(movingObj{VECTOR2(-768, 200),"Image/tmp3.png",POS_R,MAP_3});
	SetMouseDispFlag(TRUE);
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
	mouseCtl = _mouseCtl;

	(*mouseCtl).Update();
	mouseBtn = mouseCtl->GetBtn();
	Move();
	Draw(_mouseCtl->GetPoint());
	
	if (loadEndFlag && !sceneEndFlag)
	{
		if (MoveEndFlag)
		{
			if (!CheckFlag)
			{
				if (((_mouseCtl->GetBtn()[ST_NOW]) & (~_mouseCtl->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT) &&
					CheckBox(_mouseCtl->GetPoint(), VECTOR2(800, 300), VECTOR2(890, 396)))
				{
					moveID = MOVE_L;
					MoveEndFlag = false;
					moveTime = 50;
				}
				else if (((_mouseCtl->GetBtn()[ST_NOW]) & (~_mouseCtl->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT) &&
					CheckBox(_mouseCtl->GetPoint(), VECTOR2(134, 300), VECTOR2(220, 396)))
				{
					moveID = MOVE_R;
					MoveEndFlag = false;
					moveTime = 50;
				}

				if ((_mouseCtl->GetBtn()[ST_NOW]) & (~_mouseCtl->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT
					&& _mouseCtl->GetPoint() > VECTOR2(256, 200) && _mouseCtl->GetPoint() < VECTOR2(768, 512))
				{
					CheckFlag = true;

				}
				else if ((_mouseCtl->GetBtn()[ST_NOW]) & (~_mouseCtl->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT
					&& _mouseCtl->GetPoint() > VECTOR2(10, 10) && _mouseCtl->GetPoint() < VECTOR2(266, 74))
				{
					return std::move(std::make_unique <TitleScene>());
				}
			}
			else
			{
				DrawGraph(320, 250, IMAGE_ID("Image/playCheck.png")[0], true);
				if (_mouseCtl->GetPoint() > VECTOR2(370, 420) && _mouseCtl->GetPoint() < VECTOR2(460, 470))
				{
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 70);
					DrawBox(370, 420, 460, 470, 0xffffff, true);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					if ((_mouseCtl->GetBtn()[ST_NOW]) & (~_mouseCtl->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
					{
						lpMapCtl.SetMapType(selectStage);
						sceneEndFlag = true;
					}
				}
				else if (_mouseCtl->GetPoint() > VECTOR2(530, 420) && _mouseCtl->GetPoint() < VECTOR2(670, 470))
				{
					SetDrawBlendMode(DX_BLENDMODE_ALPHA, 70);
					DrawBox(530, 420, 670, 470, 0xffffff, true);
					SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
					if ((_mouseCtl->GetBtn()[ST_NOW]) & (~_mouseCtl->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
					{
						CheckFlag = false;
					}
				}
			}
		}
	}
	else if (sceneEndFlag)
	{
		//シーン終わりの演出
		if (EndProcess())
		{
			return std::move(std::make_unique <GameScene>());
		}
	}
	else
	{
		//シーン始めの演出
		if (LoadProcess())
		{
			loadEndFlag = true;
		}
	}
	DrawGraph(_mouseCtl->GetPoint().x, _mouseCtl->GetPoint().y, IMAGE_ID("Image/mouseCursor.png")[0], true);
	ScreenFlip();
	

	return std::move(_this);
}

bool MenuScene::LoadProcess(void)
{
	cnt -= 10;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cnt);
	DrawCircle(0, 0, cnt * 2, 0x000000, true);
	DrawCircle(1024, 0, cnt * 2, 0x000000, true);
	DrawCircle(0, 768, cnt * 2, 0x000000, true);
	DrawCircle(1024, 768, cnt * 2, 0x000000, true);
	
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	if (cnt <= 0)
	{
		return true;
	}
	return false;
}

bool MenuScene::EndProcess(void)
{
	cnt += 10;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cnt);
	DrawCircle(0, 0, cnt * 2, 0x000000, true);
	DrawCircle(1024, 0, cnt * 2, 0x000000, true);
	DrawCircle(0, 768, cnt * 2, 0x000000, true);
	DrawCircle(1024, 768, cnt * 2, 0x000000, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	if (cnt >= 600)
	{
		return true;
	}
	return false;
}

void MenuScene::Draw(VECTOR2 mPos)
{
	if (extFlag)
	{
		if (extRate > 1.05)
		{
			
		}
		else
		{
			extRate+= 0.02;
		}
	}
	else extRate = 1;
	extFlag = false;
	backPos.x--;
	if (backPos.x <= -1024) backPos.x = 0;
	DrawGraph(backPos.x, backPos.y, IMAGE_ID("Image/selectBack.png")[0], true);
	DrawGraph(backPos.x + 1024, backPos.y, IMAGE_ID("Image/selectBack.png")[0], true);

	if (CheckBox(mPos, VECTOR2(10,10), VECTOR2(266,74)))
	{
		extFlag = true;
		DrawRotaGraph(138, 42,extRate,0,IMAGE_ID("Image/titleReturn.png")[0], true,false);
	}
	else 
	{
		DrawRotaGraph(138, 42,1,0, IMAGE_ID("Image/titleReturn.png")[0], true,false);
	}
	if (MoveEndFlag)
	{
		if (CheckBox(mPos, VECTOR2(800, 300), VECTOR2(890, 396)))
		{
			extFlag = true;
			DrawRotaGraph(842, 348,extRate * 1.2,0, IMAGE_ID("Image/selectR.png")[0], true,false);
			DrawRotaGraph(182, 348, 1, 0, IMAGE_ID("Image/selectR.png")[0], true, true);
		}
		else if (CheckBox(mPos, VECTOR2(134, 300), VECTOR2(220, 396)))
		{
			extFlag = true;
			DrawRotaGraph(182, 348,extRate * 1.2,0, IMAGE_ID("Image/selectR.png")[0], true,true);
			DrawRotaGraph(842, 348, 1, 0, IMAGE_ID("Image/selectR.png")[0], true, false);
		}
		else
		{
			DrawRotaGraph(842, 348,1,0,IMAGE_ID("Image/selectR.png")[0], true,false);
			DrawRotaGraph(182, 348,1,0,IMAGE_ID("Image/selectR.png")[0], true,true);
		}
		DrawStringToHandle(134, 270, "左へ", 0x000000, FontHandle, GetColor(255, 190, 50));
		DrawStringToHandle(816, 270, "右へ", 0x000000, FontHandle, GetColor(255, 190, 50));
	}
	DrawGraph(600, 600, IMAGE_ID("Image/diffFrame.png")[0], true);
	int starCnt;
	if (selectStage == 0)
	{
		starCnt = 1;
		stageTitle = "チュートリアル";
	}
	else if (selectStage == 1)
	{
		starCnt = 3;
		stageTitle = "むず";
	}
	else if (selectStage == 2)
	{
		starCnt = 5;
		stageTitle = "ちょーむず";
	}

	for (int i = 0; i < 5; i++)
	{
		if (i < starCnt)
		{
			DrawGraph(732 + (i * 48), 608, IMAGE_ID("Image/diffStar.png")[0], true);
		}
		else
		{
			DrawGraph(732 + (i * 48), 608 , IMAGE_ID("Image/diffStarFrame.png")[0], true);
		}
	}
	DrawGraph(50, 600, IMAGE_ID("Image/nameFrame.png")[0], true);
	DrawStringToHandle(60, 616, "ステージ:", 0x000000, FontHandle, 0xffffff);
	DrawStringToHandle(250, 616, stageTitle.c_str(), 0x000000, FontHandle,0xffffff);

	for (auto itr : stageList)
	{
		if (CheckBox(mPos, VECTOR2(256, 200), VECTOR2(768, 515)) && MoveEndFlag)
		{
			extFlag = true;
			DrawRotaGraph(itr.Pos.x + 256, itr.Pos.y + 156,extRate,0, IMAGE_ID(itr.GHandle)[0], true , false);
			DrawRotaGraph(itr.Pos.x + 256, itr.Pos.y + 156,extRate,0, IMAGE_ID("Image/stageFrame1.png")[0], true, false);
		}
		else
		{
			DrawRotaGraph(itr.Pos.x + 256, itr.Pos.y + 156,1,0, IMAGE_ID(itr.GHandle)[0], true,false);
			DrawRotaGraph(itr.Pos.x + 256, itr.Pos.y + 156,1,0, IMAGE_ID("Image/stageFrame1.png")[0], true,false);
		}
	}
	
	DrawCircleAA(452, 130, 25, 100,(selectStage == MAP_1) ? 0xff8d30 : 0x808080, true, true);
	DrawCircleAA(512, 130, 25, 100,(selectStage == MAP_2) ? 0xff8d30 : 0x808080, true, true);
	DrawCircleAA(572, 130, 25, 100,(selectStage == MAP_3) ? 0xff8d30 : 0x808080, true, true);
	DrawStringToHandle(220, 700, "右クリックで遊ぶステージを選択しよう！", 0x000000, FontHandle);
}

void MenuScene::Move(void)
{
	if (moveTime <= 0) MoveEndFlag = true;
	if (MoveEndFlag)
	{
		for (int i = 0; i < stageList.size(); i++)
		{
			if (stageList[i].Pos.x < 0)			stageList[i].id = POS_L;
			else if (stageList[i].Pos.x > 1024) stageList[i].id = POS_R;
			else
			{
				stageList[i].id = POS_C;
				selectStage = (int)stageList[i].mapID;
			}
		}
	}
	else
	{
		moveTime--;
		for (int i = 0; i < stageList.size(); i++)
		{
			switch (stageList[i].id)
			{
			case POS_L:
				if (moveID == MOVE_L)
				{
					stageList[i].Pos = VECTOR2(1280, 200);
				}
				else
				{
					stageList[i].Pos += VECTOR2(20.48f, 0);
				}
				break;
			case POS_R:
				if (moveID == MOVE_L)
				{
					stageList[i].Pos -= VECTOR2(20.48f, 0);
				}
				else
				{
					stageList[i].Pos = VECTOR2(-768, 200);
				}
				break;
			case POS_C:
				if (moveID == MOVE_L)
				{
					stageList[i].Pos -= VECTOR2(20.48f, 0);
				}
				else
				{
					stageList[i].Pos += VECTOR2(20.48f, 0);
				}
				break;
			default:
				break;
			}
		}
	}
}

bool MenuScene::CheckBox(VECTOR2 mPos, VECTOR2 boxLT, VECTOR2 boxRD)
{
	if (boxLT <= mPos && mPos <= boxRD)
	{
		return true;
	}
	return false;
}
