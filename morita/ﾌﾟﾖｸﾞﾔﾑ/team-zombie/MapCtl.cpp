#include <stdio.h>
#include <algorithm>
#include <DxLib.h>
#include "MapCtl.h"
#include "ImageMng.h"
#include "Effect.h"
#include "VECTOR2.h"

std::unique_ptr<MapCtl, MapCtl::MapCtlDeleter> MapCtl::s_Instance(new MapCtl());

int MapCtl::MapInit(void)
{
	for (unsigned int y = 0; y < mapID.size(); y++)
	{
		for (unsigned int x = 0; x < mapID[y].size(); x++)
		{
			mapID[y][x] = 0;
		}
	}
	return 0;
}

bool MapCtl::CheckFloor(VECTOR2 pos)
{
	VECTOR2 tmpPos = (pos + VECTOR2(0,0)) /32;
	if (tmpPos.x >= 0 && tmpPos.x <= CHIP_CNT_X && tmpPos.y >= 0 && tmpPos.y <= CHIP_CNT_Y)
	{
		for (int i = FLOOR_START; i < FLOOR_LAST; i++)
		{
			if (mapID[(int)tmpPos.y + 2][(int)tmpPos.x] == i)
			{
				return true;
			}
			else if (mapID[(int)tmpPos.y + 2][(int)tmpPos.x + 1] == i)
			{
				return true;
			}
		}
	}
	return false;
}

bool MapCtl::CheckWall(VECTOR2 pos)
{

	VECTOR2 tmpPos = (pos + VECTOR2(0, 0)) / 32;
	if (tmpPos.x >= 0 && tmpPos.x <= CHIP_CNT_X && tmpPos.y >= 0 && tmpPos.y <= CHIP_CNT_Y)
	{
		for (int i = FLOOR_START; i < BLOCK_LAST; i++)
		{
			if (i == GRASS_START)
			{
				i = BLOCK_START;
			}
			if (mapID[(int)tmpPos.y][(int)tmpPos.x] == i)
			{
				return true;
			}
		}
	}
	return false;
}

bool MapCtl::CheckUpBlock(VECTOR2 pos)
{
	VECTOR2 tmpPos = (pos + VECTOR2(32,-1)) / 32;
	if (tmpPos.x >= 0 && tmpPos.x <= CHIP_CNT_X && tmpPos.y >= 0 && tmpPos.y <= CHIP_CNT_Y)
	{
		for (int i = FLOOR_START; i < BLOCK_LAST; i++)
		{
			if (i == GRASS_START)
			{
				i = BLOCK_START;
			}
			if (mapID[(int)tmpPos.y][(int)tmpPos.x] == i)
			{
				return true;
			}
		}
	}
	return false;
}

void MapCtl::IfMove(VECTOR2& pos)
{
	Ground(pos);
	//Ceiling(pos);
	CheckCoin(pos);
}

void MapCtl::StepMove(VECTOR2 & pos)
{
	
}

void MapCtl::Ground(VECTOR2 & pos)
{
	//–„‚Ü‚è–hŽ~
	VECTOR2 tmpPos = (pos + VECTOR2(32, 63)) / 32;
	if (tmpPos.x >= 0 && tmpPos.x <= CHIP_CNT_X && tmpPos.y >= 0 && tmpPos.y <= CHIP_CNT_Y)
	{
		for (int i = FLOOR_START; i < FLOOR_LAST; i++)
		{
			if (mapID[(int)tmpPos.y][(int)tmpPos.x] == i )
			{
				pos.y = (tmpPos.y - 2) * 32;
				return;
			}
		}
	}
}

void MapCtl::Ceiling(VECTOR2 & pos)
{
	VECTOR2 tmpPos = (pos + VECTOR2(32,-1)) / 32;
	if (tmpPos.x >= 0 && tmpPos.x <= CHIP_CNT_X && tmpPos.y >= 0 && tmpPos.y <= CHIP_CNT_Y)
	{
		for (int i = 1; i < 19; i++)
		{
			if (mapID[(int)tmpPos.y][(int)tmpPos.x] == i)
			{
				pos.y = (tmpPos.y + 2) * 32;
				return;
			}
		}
	}
}

void MapCtl::CheckCoin(VECTOR2 pos)
{
	VECTOR2 tmpPos = (pos + VECTOR2(32, 0)) / 32;
	if (tmpPos.x >= 0 && tmpPos.x <= CHIP_CNT_X && tmpPos.y >= 0 && tmpPos.y <= CHIP_CNT_Y)
	{
		if (mapID[(int)tmpPos.y][(int)tmpPos.x] == CHIP_TYPE::CHIP_COIN)
		{
			VECTOR2 coinPos = VECTOR2(tmpPos.x * 32, tmpPos.y * 32) - VECTOR2(cOffset.x - (GetViewAreaSize().x / 2), 0);
			mapID[(int)tmpPos.y][(int)tmpPos.x] = CHIP_TYPE::CHIP_BLANK;
			coinList.push_back(GetCoin{ coinPos,true,VECTOR2((VECTOR2(690,50) - coinPos) /20),0.1f});
		}
		if (mapID[(int)tmpPos.y + 1][(int)tmpPos.x] == CHIP_TYPE::CHIP_COIN)
		{
			VECTOR2 coinPos = VECTOR2(tmpPos.x * 32, (tmpPos.y + 1) * 32) - VECTOR2(cOffset.x - (GetViewAreaSize().x / 2), 0);
			mapID[(int)tmpPos.y + 1][(int)tmpPos.x] = CHIP_TYPE::CHIP_BLANK;
			coinList.push_back(GetCoin{coinPos,true,VECTOR2((VECTOR2(690,50) - coinPos) / 20),0.1f });
		}

	}
	
}

void MapCtl::CoinScoreAdd(void)
{
	Score++;
}

CHIP_TYPE MapCtl::GetChipType(VECTOR2 pos)
{
	VECTOR2 tmpPos = pos / 32;
	return (CHIP_TYPE)mapID[(int)tmpPos.y][(int)tmpPos.x];
}

void MapCtl::MapDraw(VECTOR2 camPos)
{
	animCnt++;
	nowTime = std::chrono::system_clock::now();
	Time = (int)std::chrono::duration_cast<std::chrono::seconds>(nowTime - startTime).count();
	DrawGraph(0, 0, IMAGE_ID("Image/back.png")[0], true);
	int leftX, rightX;
	leftX = (int)((camPos.x / 32) - (VIEW_CHIP_CNT_X / 2) -1);
	rightX = (int)((camPos.x / 32) + (VIEW_CHIP_CNT_X / 2) +1);
	for (int y = 0; y < CHIP_CNT_Y;y++)
	{
		if (leftX < 0)leftX = 0;
		if (rightX > CHIP_CNT_X) rightX = CHIP_CNT_X;
		for (leftX; leftX < rightX; leftX++)
		{
			if (mapID[y][leftX] != CHIP_GRASS1 && mapID[y][leftX] != CHIP_GRASS2 && mapID[y][leftX] != CHIP_GRASS3)
			{
				if (mapID[y][leftX] == CHIP_COIN)
				{
					DrawGraph(leftX * 32 + GameDrawOffset().x, y * 32 + GameDrawOffset().y, IMAGE_ID("Image/coinAnim.png")[(animCnt/10) % 4], true);
				}
				else if (mapID[y][leftX] == CHIP_FIRE)
				{
					DrawGraph(leftX * 32 + GameDrawOffset().x, y * 32 + GameDrawOffset().y, IMAGE_ID("Image/mt.png")[mapID[y][leftX]], true);
				}
				else
				{
					DrawGraph(leftX * 32 + GameDrawOffset().x, y * 32 + GameDrawOffset().y, IMAGE_ID("Image/mt.png")[mapID[y][leftX]], true);
				}
			}
		}
		leftX = (int)((camPos.x / 32) - (VIEW_CHIP_CNT_X / 2) -1);
	}

	DrawGraph(0, 720, IMAGE_ID("Image/progress.png")[0], true);
	DrawGraph((pPos.x/GetGameAreaSize().x) * GetViewAreaSize().x -16,700, IMAGE_ID("Image/pCursor.png")[0], true);

	DrawGraph(700, 0, IMAGE_ID("Image/coinFrame.png")[0], true);
	DrawGraph(800, 8, IMAGE_ID("Image/number.png")[(Score/10)], true);
	DrawGraph(835, 8, IMAGE_ID("Image/number.png")[Score % 10], true);
	
	DrawExtendGraph(700, 0,764,64, IMAGE_ID("Image/coinAnim.png")[(animCnt/10)%4], true);
	DrawGraph(750, 16, IMAGE_ID("Image/X.png")[0], true);

	DrawGraph(50, 0, IMAGE_ID("Image/timeFrame.png")[0], true);
	DrawGraph(205, 8, IMAGE_ID("Image/number.png")[Time / 60], true);	//10•ª‰z‚¦‚é‚ÆƒGƒ‰[”Œ(11‚±‚ß‚ð‚Ý‚é‚Ì‚Å)
	DrawGraph(260, 8, IMAGE_ID("Image/number.png")[(Time%60)/10], true);
	DrawGraph(295, 8, IMAGE_ID("Image/number.png")[(Time%60)%10], true);
	
	for (int i = 0; i < coinList.size(); i++)
	{
		if (coinList[i].active)
		{
			coinList[i].pos += (coinList[i].moveVec * coinList[i].moveRate);
			coinList[i].moveRate += 0.1f;
			if (coinList[i].pos.y < 25)
			{
				coinList[i].active = false;
				lpEffect.AddEffectList("Effect/get.png",VECTOR2(192,192),VECTOR2(5,3),VECTOR2(0,0),15,2,VECTOR2(634,-66));
				CoinScoreAdd();
			}
		}
	}
	for (auto itr : coinList)
	{
		DrawGraph(itr.pos.x, itr.pos.y, IMAGE_ID("Image/mt.png")[CHIP_COIN], true);
	}

	//ˆ—‚ðI‚¦‚½ÃÞ°À‚Ìíœ
	std::vector<GetCoin> ::const_iterator it = coinList.begin();
	while (it != coinList.end())
	{
		GetCoin coin = (*it);
		if (coin.active == false) {

			it = coinList.erase(it);
		}
		else
		{
			it++;
		}
	}
}

void MapCtl::MapNearDraw(VECTOR2 camPos)
{
	int leftX, rightX;
	leftX = (int)((camPos.x / 32) - (VIEW_CHIP_CNT_X / 2) - 1);
	rightX = (int)((camPos.x / 32) + (VIEW_CHIP_CNT_X / 2) + 1);
	for (int y = 0; y < CHIP_CNT_Y; y++)
	{
		if (leftX < 0)leftX = 0;
		if (rightX > CHIP_CNT_X) rightX = CHIP_CNT_X;
		for (leftX; leftX < rightX; leftX++)
		{
			if (mapID[y][leftX] == CHIP_GRASS1 || mapID[y][leftX] == CHIP_GRASS2 || mapID[y][leftX] == CHIP_GRASS3)
			{
				DrawGraph(leftX * 32 + GameDrawOffset().x, y * 32 + GameDrawOffset().y, IMAGE_ID("Image/mt.png")[mapID[y][leftX]], true);
			}
		}
		leftX = (int)((camPos.x / 32) - (VIEW_CHIP_CNT_X / 2) - 1);
	}
}

bool MapCtl::MapLoad(void)
{
	FILE *fp;
	fopen_s(&fp, "data/‚¿‚å‚¢‚Þ‚¸.fmf", "rb");
	if (fp == nullptr)
	{
		return false;
	}
	for (int y = 0; y < (int)mapID.size(); y++)
	{
		for (int x = 0; x < (int)mapID[y].size(); x++)
		{
			fread(&mapID[y][x], sizeof(unsigned char),1, fp);
		}
	}
	return true;
}

VECTOR2 MapCtl::GetChipSize(void)
{
	return VECTOR2(32,32);
}

VECTOR2 MapCtl::GetViewAreaSize(void)
{
	return VECTOR2(1024,768);
}

VECTOR2 MapCtl::GetGameAreaSize(void)
{
	//return VECTOR2(10240,768);
	return VECTOR2(32 * 700, 768);
}

void MapCtl::SetDrawOffset(VECTOR2 Offset)
{
	cOffset = Offset;
}

VECTOR2 MapCtl::GameDrawOffset(void)
{
	return -(cOffset - ((GetViewAreaSize() / 2)));
}

void MapCtl::SetPlayerPos(VECTOR2 pos)
{
	pPos = pos;
}

void MapCtl::TimerStart(void)
{
	startTime = std::chrono::system_clock::now();
}


MapCtl::MapCtl()
{
	mapID.resize(CHIP_CNT_Y);
	for (int y = 0; y < CHIP_CNT_Y; y++)
	{
		mapID[y].resize(CHIP_CNT_X);
	}
	ImageMng::GetInstance().GetID("Image/mt.png", { 32,32 }, { 27,1 }, { 0,0 });
	ImageMng::GetInstance().GetID("Image/fire.png", { 32,32 }, { 6,1 }, { 0,0 });
	ImageMng::GetInstance().GetID("Image/number.png", { 36,48 }, { 10,1 }, { 0,0 });
	ImageMng::GetInstance().GetID("Effect/get.png", { 192,192 }, { 5,3 }, { 0,0 });
	ImageMng::GetInstance().GetID("Image/coinAnim.png", VECTOR2(32, 32), VECTOR2(4, 1), VECTOR2(0, 0));
	MapInit();
	MapLoad();
	animCnt = 0;
	Score = 0;
}


MapCtl::~MapCtl()
{
}
