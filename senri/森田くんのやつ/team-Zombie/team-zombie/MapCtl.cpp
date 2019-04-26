#include <stdio.h>
#include <DxLib.h>
#include "MapCtl.h"
#include "ImageMng.h"

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
	if (mapID[(int)tmpPos.y+1][(int)tmpPos.x] == CHIP_TYPE::CHIP_FLOOR || 
			mapID[(int)tmpPos.y+1][(int)tmpPos.x] == CHIP_TYPE::CHIP_BLOCK)
	{
		return true;
	}
	else if (mapID[(int)tmpPos.y+1][(int)tmpPos.x + 1] == CHIP_TYPE::CHIP_FLOOR ||
		mapID[(int)tmpPos.y+1][(int)tmpPos.x + 1] == CHIP_TYPE::CHIP_BLOCK)
	{
		return true;
	}
	return false;
}

CHIP_TYPE MapCtl::GetChipType(VECTOR2 pos)
{
	VECTOR2 tmpPos = pos / 32;
	return (CHIP_TYPE)mapID[(int)tmpPos.y][(int)tmpPos.x];
}

void MapCtl::MapDraw(VECTOR2 camPos)
{
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
			DrawGraph(leftX * 32 + GameDrawOffset().x, y * 32 + GameDrawOffset().y, IMAGE_ID("Image/tiles.png")[mapID[y][leftX]],true);
		}
		leftX = (int)((camPos.x / 32) - (VIEW_CHIP_CNT_X / 2) -1);
	}
}

bool MapCtl::MapLoad(void)
{
	FILE *fp;
	fopen_s(&fp, "data/‚¿‚ã[‚Æ‚è‚ ‚é.fmf", "rb");
	if (fp == nullptr)
	{
		return false;
	}
	for (int y = 0; y < (int)mapID.size(); y++)
	{
		for (int x = 0; x < (int)mapID[y].size(); x++)
		{
			fread(&mapID[y][x], sizeof(char),1, fp);
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
	return VECTOR2(10240,768);
}

void MapCtl::SetDrawOffset(VECTOR2 Offset)
{
	cOffset = Offset;
}

VECTOR2 MapCtl::GameDrawOffset(void)
{
	return -(cOffset - ((GetViewAreaSize() / 2)));
}


MapCtl::MapCtl()
{
	mapID.resize(CHIP_CNT_Y);
	for (int y = 0; y < CHIP_CNT_Y; y++)
	{
		mapID[y].resize(CHIP_CNT_X);
	}
	ImageMng::GetInstance().GetID("Image/tiles.png", { 32,32 }, { 10,1 }, { 0,0 });
	
	MapInit();
	MapLoad();
}


MapCtl::~MapCtl()
{
}
