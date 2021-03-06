#include "ResultScene.h"
#include "MenuScene.h"
#include "GameScene.h"
#include "ImageMng.h"
#include "Shaker.h"
#include "MapCtl.h"
#include "Effect.h"


ResultScene::ResultScene()
{
	lpEffect.SetGameFlag(false);
	lpEffect.DeleteEffectList();
	coin = 0;
	
	LogoRate = 10;
	coincnt = 0;
	cnt = 600;
	loadEndFlag = false;
	sceneEndFlag = false;
	retryFlag = false;
	rankingRate = 1;
	moveEndFlag = false;
	logoFlag = false;
	rankingFlag = false;
	sameFlag = true;
	rankingPos = {448,-600};
	FontHandle = CreateFontToHandle("Bauhaus 93", 32, 4, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	rankFont[0] = CreateFontToHandle("メイリオ", 48, 4, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	rankFont[1] = CreateFontToHandle("メイリオ", 40, 4, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	rankFont[2] = CreateFontToHandle("メイリオ", 36, 4, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	rankFont[3] = CreateFontToHandle("メイリオ", 32, 4, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
	rankFont[4] = CreateFontToHandle("メイリオ", 32, 4, DX_FONTTYPE_ANTIALIASING_EDGE_4X4);
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
	
	Move();
	Draw(_mouseCtl->GetPoint());
	
	
	mouseCtl = _mouseCtl;
	(*mouseCtl).Update();
	mouseBtn = mouseCtl->GetBtn();
	if (!moveEndFlag && logoFlag && (_mouseCtl->GetBtn()[ST_NOW]) & (~_mouseCtl->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
	{
		if (coin > 0)
		{
			score += coin;
			coin = 0;
		}
		else if (time > 0)
		{
			score += tmpTime;
			time = 0;
			moveEndFlag = true;
			if (lpMapCtl.GetMapType() != 0)
			{
				lpMapCtl.AddRanking(score);
			}
		}
	}

	if (loadEndFlag && !retryFlag && !sceneEndFlag && moveEndFlag)
	{
		if ((_mouseCtl->GetBtn()[ST_NOW]) & (~_mouseCtl->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
		{
			if (CheckBox(_mouseCtl->GetPoint(), VECTOR2(30, 400), VECTOR2(350, 464)))
			{
				retryFlag = true;
			}
			else if (CheckBox(_mouseCtl->GetPoint(), VECTOR2(30, 500), VECTOR2(350, 564)) && lpMapCtl.GetMapType() != 0)
			{
				rankingPos = { 448,-600 };
				rankingFlag = true;
				rankList = lpMapCtl.GetRankList();
				std::sort(rankList.begin(), rankList.end(), std::greater<int>());
			}
			else if (CheckBox(_mouseCtl->GetPoint(), VECTOR2(30, 600), VECTOR2(350, 664)))
			{
				sceneEndFlag = true;
				
			}
			else
			{
				rankingFlag = false;
			}
		}
	}
	else if (retryFlag)
	{
		if (EndProcess())
		{
			return std::move(std::make_unique <GameScene>());
		}
	}
	else if (sceneEndFlag)
	{
		if (EndProcess())
		{
			return std::move(std::make_unique <MenuScene>());
		}
	}
	else if(!loadEndFlag)
	{
		if (LoadProcess())
		{
			loadEndFlag = true;
		}
	}
	lpShaker.ShakeDraw();
	DrawGraph(_mouseCtl->GetPoint().x, _mouseCtl->GetPoint().y, IMAGE_ID("Image/mouseCursor.png")[0], true);
	ScreenFlip();
	return std::move(_this);
}

bool ResultScene::LoadProcess(void)
{
	int coin, time, type;
	lpMapCtl.GetCoinTime(coin, time, type);
	cnt -= 10;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cnt / 2);
	//DrawCircle(0, 0, cnt * 2, 0x000000, true);
	//DrawCircle(1024, 0, cnt * 2, 0x000000, true);
	//DrawCircle(0, 768, cnt * 2, 0x000000, true);
	//DrawCircle(1024, 768, cnt * 2, 0x000000, true);
	DrawBox(0, 0, 1024, 768, (type == 1) ? 0xff3030 : 0xffffff, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	if (cnt <= 0)
	{
		return true;
	}
	return false;
}

bool ResultScene::EndProcess(void)
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
		for (int i = 0; i < 5; i++)
		{
			DeleteFontToHandle(rankFont[i]);
		}
		DeleteFontToHandle(FontHandle);
		return true;
	}
	return false;
}

void ResultScene::Draw(VECTOR2 mPos)
{
	if (clearType == 0)
	{
		DrawGraph(0, 0, IMAGE_ID("Image/clearBack.png")[0], true);
	}
	else DrawGraph(0, 0, IMAGE_ID("Image/gameOverBack.png")[0], true);


	if (moveEndFlag)
	{
		if (CheckBox(mPos, VECTOR2(30, 400), VECTOR2(350, 464)))
		{
			extFlag = true;
			DrawRotaGraph(190, 432, extRate, 0, IMAGE_ID("Image/retry.png")[0], true);

		}
		else
		{
			DrawRotaGraph(190, 432, 1, 0, IMAGE_ID("Image/retry.png")[0], true);
		}

		
		if (CheckBox(mPos, VECTOR2(30, 500), VECTOR2(350, 564)) && lpMapCtl.GetMapType() != 0)
		{
			extFlag = true;
			DrawRotaGraph(190, 532, extRate, 0, IMAGE_ID("Image/rankingButton.png")[0], true);
		}
		else if(lpMapCtl.GetMapType() != 0)
		{
			DrawRotaGraph(190, 532, 1, 0, IMAGE_ID("Image/rankingButton.png")[0], true);
		}

		if (CheckBox(mPos, VECTOR2(30, 600), VECTOR2(350, 664)))
		{
			extFlag = true;
			DrawRotaGraph(190, 632, extRate, 0, IMAGE_ID("Image/selectButton.png")[0], true);
		}
		else
		{
			DrawRotaGraph(190, 632, 1, 0, IMAGE_ID("Image/selectButton.png")[0], true);
		}
	}



	lpEffect.EffectDraw();
	if (clearType == 0)
	{
		DrawRotaGraph(512, 150, LogoRate, 0, IMAGE_ID("Image/clearLogo.png")[0], true, false);
	}
	else
	{
		DrawRotaGraph(512, 150, LogoRate, 0, IMAGE_ID("Image/gameOverLogo.png")[0], true, false);
	}
	DrawFormatStringToHandle(400, 300, 0xffffff, FontHandle, "coin: %d", coin);
	DrawFormatStringToHandle(700, 300, 0xffffff, FontHandle, "time: %d", time);
	DrawFormatStringToHandle(450, 450, 0xffffff, FontHandle, "score: %d", score);

	if (rankingRate > 0)
	{
		DrawRotaGraph(rankingPos.x + 256, rankingPos.y + 300,rankingRate,0,IMAGE_ID("Image/rankingBoard.png")[0], true);
	}
	if (rankingFlag && rankingPos.y >= 64)
	{
		for (int i = 0; i < 5; i++)
		{
			DrawFormatStringToHandle(672, 196 + i * 68,(score == rankList[i]) ? 0xffff70 : 0xffffff, rankFont[i], "%d",rankList[i]);
			if (score == rankList[i] && !sameFlag)
			{
				DrawRotaGraph(880, 196 + i * 68, 1, 0, IMAGE_ID("Image/rankin.png")[0], true, false);
				sameFlag = true;
			}
		}
		if (score < rankList[4])
		{
			DrawRotaGraph(750, 550, 1, 0, IMAGE_ID("Image/rankgai.png")[0], true);
		}
	}
}

void ResultScene::Move(void)
{
	coincnt++;

	if (rankingFlag)
	{
		rankingRate = 1;
		if (rankingPos.y < 64)
		{
			rankingPos.y += 6;
		}
	}
	else
	{

		rankingRate -= 0.05;
	}
	

	if (!moveEndFlag && logoFlag && (coincnt / 8) == 1)
	{
		if (coin != 0)
		{
			coin--;
			score++;
			lpEffect.AddEffectList("Effect/Exchange1.png", { 192,192 }, { 5,2 }, { 0,0 }, 10, 3, { 450,325 });
		}
		else
		{
			effCnt++;
			if (time != 0)
			{
				time--;
				if ((effCnt % 3) == 1)
				{
					if (clearType == 0)
					{
						lpEffect.AddEffectList("Effect/charge.png", { 192,192 }, { 5,3 }, { 0,0 }, 12, 2, { 670,230 });
					}
				}
				if (time == 0)
				{
					effCnt = 0;
				}
			}
			else
			{
				if (clearType == 0)
				{
					if (effCnt == 3)
					{
						lpEffect.AddEffectList("Effect/Exchange2.png", { 256,256 }, { 5,2 }, { 0,0 }, 10, 4, { 530,350 });
					}
					else if (effCnt == 7)
					{
						lpEffect.AddEffectList("Effect/get.png", { 192,192 }, { 5,3 }, { 0,0 }, 12, 3, { 520,360 });
						score += tmpTime;
						moveEndFlag = true;
						lpMapCtl.AddRanking(score);
					}
				}
				else
				{
					moveEndFlag = true;
					lpMapCtl.AddRanking(score);
				}
			}
		}
	}
	if (coincnt == 8) coincnt = 0;

	if (extFlag)
	{
		if (extRate > 1.1)
		{

		}
		else
		{
			extRate += 0.02;
		}
	}
	else extRate = 1;
	extFlag = false;

	if (LogoRate >= 1 && (!logoFlag))
	{
		LogoRate -= 0.25;
		lpMapCtl.GetCoinTime(coin, time,clearType);
		if (lpMapCtl.GetMapType() != 0)
		{
			tmpTime = (30 / (time - 44)) * 5;
		}
		else
		{
			tmpTime = (30 / (time - 65)) * 3;
		}
		if (clearType == 1) tmpTime = 0;
	}
	else
	{
		if (!logoFlag)
		{
			lpShaker.SetShaker(true, 10, SHAKE_TYPE::SHAKE_XY);
		}
		logoFlag = true;
	}
}

bool ResultScene::CheckBox(VECTOR2 mPos, VECTOR2 boxLT, VECTOR2 boxRD)
{
	if (boxLT <= mPos && mPos <= boxRD)
	{
		return true;
	}
	return false;
}
