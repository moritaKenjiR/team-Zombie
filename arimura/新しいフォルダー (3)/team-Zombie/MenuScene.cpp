#include "MenuScene.h"
#include "GameScene.h"
#include "MapCtl.h"
#include "ImageMng.h"



MenuScene::MenuScene()
{
	selectStage = 1; 
	moveTime = 0;
	MoveEndFlag = true;
	extFlag = false;
	
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
	
	
	if (MoveEndFlag)
	{
		if (!CheckFlag)
		{
			if (((_mouseCtl->GetBtn()[ST_NOW]) & (~_mouseCtl->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT) &&
				_mouseCtl->GetPoint() > VECTOR2(778, 300) && _mouseCtl->GetPoint() < VECTOR2(880, 400))
			{
				moveID = MOVE_L;
				MoveEndFlag = false;
				moveTime = 100;
			}
			else if (((_mouseCtl->GetBtn()[ST_NOW]) & (~_mouseCtl->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT) &&
				_mouseCtl->GetPoint() > VECTOR2(150, 300) && _mouseCtl->GetPoint() < VECTOR2(250, 400))
			{
				moveID = MOVE_R;
				MoveEndFlag = false;
				moveTime = 100;
			}

			if ((_mouseCtl->GetBtn()[ST_NOW]) & (~_mouseCtl->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT
				&& _mouseCtl->GetPoint() > VECTOR2(256, 200) && _mouseCtl->GetPoint() < VECTOR2(768, 512))
			{
				CheckFlag = true;
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
					return std::move(std::make_unique <GameScene>());
				}
			}
			else if (_mouseCtl->GetPoint() > VECTOR2(530, 420) && _mouseCtl->GetPoint() < VECTOR2(670, 470))
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 70);
				DrawBox(530, 420, 670, 470,0xffffff, true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
				if ((_mouseCtl->GetBtn()[ST_NOW]) & (~_mouseCtl->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
				{
					CheckFlag = false;
				}
			}
		}
	}
	DrawGraph(_mouseCtl->GetPoint().x, _mouseCtl->GetPoint().y, IMAGE_ID("Image/mouseCursor.png")[0], true);
	ScreenFlip();
	

	return std::move(_this);
}

void MenuScene::Draw(VECTOR2 mPos)
{
	if (extFlag)
	{
		if (extRate > 1.1)
		{
			
		}
		else
		{
			extRate+= 0.01;
		}
	}
	else extRate = 1;
	extFlag = false;
	DrawGraph(0, 0, IMAGE_ID("Image/selectBack.png")[0], true);

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
		if (CheckBox(mPos, VECTOR2(778, 300), VECTOR2(874, 396)))
		{
			extFlag = true;
			DrawRotaGraph(826, 348,extRate,0, IMAGE_ID("Image/selectR.png")[0], true,false);
			DrawRotaGraph(198, 348, 1, 0, IMAGE_ID("Image/selectR.png")[0], true, true);
		}
		else if (CheckBox(mPos, VECTOR2(150, 300), VECTOR2(246, 396)))
		{
			extFlag = true;
			DrawRotaGraph(198, 348,extRate,0, IMAGE_ID("Image/selectR.png")[0], true,true);
			DrawRotaGraph(826, 348, 1, 0, IMAGE_ID("Image/selectR.png")[0], true, false);
		}
		else
		{
			DrawRotaGraph(826, 348,1,0,IMAGE_ID("Image/selectR.png")[0], true,false);
			DrawRotaGraph(198, 348,1,0,IMAGE_ID("Image/selectR.png")[0], true,true);
		}
	}
	DrawGraph(600, 600, IMAGE_ID("Image/diffFrame.png")[0], true);
	int starCnt;
	if (selectStage == 0)starCnt = 1;
	else if (selectStage == 1)starCnt = 3;
	else if (selectStage == 2)starCnt = 5;

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

	for (auto itr : stageList)
	{
		DrawGraph(itr.Pos.x, itr.Pos.y,IMAGE_ID(itr.GHandle)[0], true);
		DrawGraph(itr.Pos.x - 34, itr.Pos.y -25, IMAGE_ID("Image/stageFrame1.png")[0], true);
	}
	
	DrawCircleAA(452, 130, 25, 100,(selectStage == MAP_1) ? 0xff0000 : 0x000000, true, true);
	DrawCircleAA(512, 130, 25, 100,(selectStage == MAP_2) ? 0xff0000 : 0x000000, true, true);
	DrawCircleAA(572, 130, 25, 100,(selectStage == MAP_3) ? 0xff0000 : 0x000000, true, true);

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
					stageList[i].Pos += VECTOR2(10.24f, 0);
				}
				break;
			case POS_R:
				if (moveID == MOVE_L)
				{
					stageList[i].Pos -= VECTOR2(10.24f, 0);
				}
				else
				{
					stageList[i].Pos = VECTOR2(-768, 200);
				}
				break;
			case POS_C:
				if (moveID == MOVE_L)
				{
					stageList[i].Pos -= VECTOR2(10.24f, 0);
				}
				else
				{
					stageList[i].Pos += VECTOR2(10.24f, 0);
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
