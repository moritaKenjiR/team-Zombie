#include <stdio.h>
#include <sys\stat.h>
#include <algorithm>
#include <DxLib.h>
#include "MapCtl.h"
#include "ImageMng.h"
#include "Effect.h"
#include "VECTOR2.h"

std::unique_ptr<MapCtl, MapCtl::MapCtlDeleter> MapCtl::s_Instance(new MapCtl());

MapCtl & MapCtl::GetInstance()
{
	static MapCtl Instance;
	return Instance;
}

int MapCtl::Init(void)
{
	mapID.clear();
	mapID.resize(ChipCnt.x);
	for (int y = 0; y < ChipCnt.y; y++)
	{
		mapID[y].resize(ChipCnt.x);
	}
	for (unsigned int y = 0; y < mapID.size(); y++)
	{
		for (unsigned int x = 0; x < mapID[y].size(); x++)
		{
			mapID[y][x] = 0;
		}
	}
	MapLoad();
	animCnt = 0;
	Score = 0;
	endFlag = false;
	objUpdateFlag = true;
	messageTimer = 0;
	Time = 0;
	windowRate = 0;
	mFont = CreateFontToHandle("メイリオ", 30, 4, DX_FONTTYPE_ANTIALIASING);
	for (int i = 0; i < 4; i++)
	{
		backPos[i] = { 0,0 };
	}
	pPos = oldPos;
	return 0;
}

bool MapCtl::CheckFloor(VECTOR2 pos)
{
	VECTOR2 tmpPos = (pos + VECTOR2(0, 0)) / 32;
	if (tmpPos.x >= 0 && tmpPos.x + 1 <= ChipCnt.x && tmpPos.y + 2 >= 0 && tmpPos.y + 2 <= ChipCnt.y)
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
	if (tmpPos.x >= 0 && tmpPos.x + 1 <= ChipCnt.x && tmpPos.y >= 0 && tmpPos.y <= ChipCnt.y)
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
	VECTOR2 tmpPos = (pos + VECTOR2(32, -1)) / 32;
	if (tmpPos.x >= 0 && tmpPos.x <= ChipCnt.x && tmpPos.y >= 0 && tmpPos.y <= ChipCnt.y)
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
	//埋まり防止
	VECTOR2 tmpPos = (pos + VECTOR2(32, 63)) / 32;
	if (tmpPos.x >= 0 && tmpPos.x <= ChipCnt.x && tmpPos.y >= 0 && tmpPos.y <= ChipCnt.y)
	{
		for (int i = FLOOR_START; i < FLOOR_LAST; i++)
		{
			if (mapID[(int)tmpPos.y][(int)tmpPos.x] == i)
			{
				pos.y = (tmpPos.y - 2) * 32;
				return;
			}
		}
	}
}

void MapCtl::Ceiling(VECTOR2 & pos)
{
	VECTOR2 tmpPos = (pos + VECTOR2(32, -1)) / 32;
	if (tmpPos.x >= 0 && tmpPos.x <= ChipCnt.x && tmpPos.y >= 0 && tmpPos.y <= ChipCnt.y)
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
	if (tmpPos.x >= 0 && tmpPos.x <= ChipCnt.x && tmpPos.y >= 0 && tmpPos.y + 1 <= ChipCnt.y)
	{
		if (mapID[(int)tmpPos.y][(int)tmpPos.x] == CHIP_TYPE::CHIP_COIN)
		{
			VECTOR2 coinPos = VECTOR2(tmpPos.x * 32, tmpPos.y * 32) - VECTOR2(cOffset.x - (GetViewAreaSize().x / 2), 0);
			mapID[(int)tmpPos.y][(int)tmpPos.x] = CHIP_TYPE::CHIP_BLANK;
			coinList.push_back(GetCoin{ coinPos,true,VECTOR2((VECTOR2(690,50) - coinPos) / 20),0.1f });
			lpEffect.AddEffectList("Effect/coinEff.png", VECTOR2(192, 192), VECTOR2(4, 3), VECTOR2(0, 0), 12, 2, VECTOR2(tmpPos.x * 32, tmpPos.y * 32) + VECTOR2(-84, -96));
		}
		if (mapID[(int)tmpPos.y + 1][(int)tmpPos.x] == CHIP_TYPE::CHIP_COIN)
		{
			VECTOR2 coinPos = VECTOR2(tmpPos.x * 32, (tmpPos.y + 1) * 32) - VECTOR2(cOffset.x - (GetViewAreaSize().x / 2), 0);
			mapID[(int)tmpPos.y + 1][(int)tmpPos.x] = CHIP_TYPE::CHIP_BLANK;
			coinList.push_back(GetCoin{ coinPos,true,VECTOR2((VECTOR2(690,50) - coinPos) / 20),0.1f });
			lpEffect.AddEffectList("Effect/coinEff.png", VECTOR2(192, 192), VECTOR2(4, 3), VECTOR2(0, 0), 12, 1, VECTOR2(tmpPos.x * 32, (tmpPos.y + 1) * 32) + VECTOR2(-84, -96));
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
	if (tmpPos.x >= 0 && tmpPos.x <= ChipCnt.x && tmpPos.y >= 0 && tmpPos.y <= ChipCnt.y)
	{
		return (CHIP_TYPE)mapID[(int)tmpPos.y][(int)tmpPos.x];
	}
	return CHIP_BLANK;
}

void MapCtl::MapDraw(VECTOR2 camPos)
{
	animCnt++;

	nowTime = std::chrono::system_clock::now();
	Time = (int)std::chrono::duration_cast<std::chrono::seconds>(nowTime - startTime).count();
	//DrawGraph(0, 0, IMAGE_ID("Image/back.png")[0], true);
	MapBackDraw();

	lpEffect.WindEffDraw();

	std::string mtStr;
	if (mapType != 2) mtStr = "Image/mt.png";
	else mtStr = "Image/mt2.png";

	int leftX, rightX;
	leftX = (int)((camPos.x / 32) - (VIEW_CHIP_CNT_X / 2) - 1);
	rightX = (int)((camPos.x / 32) + (VIEW_CHIP_CNT_X / 2) + 1);
	for (int y = 0; y < ChipCnt.y; y++)
	{
		if (leftX < 0)leftX = 0;
		if (rightX > ChipCnt.x) rightX = ChipCnt.x;
		for (leftX; leftX < rightX; leftX++)
		{
			if (mapID[y][leftX] != CHIP_GRASS1 && mapID[y][leftX] != CHIP_GRASS2 && mapID[y][leftX] != CHIP_GRASS3)
			{
				if (mapID[y][leftX] == CHIP_COIN)
				{
					DrawGraph(leftX * 32 + GameDrawOffset().x, y * 32 + GameDrawOffset().y, IMAGE_ID("Image/coinAnim2.png")[(animCnt / 10) % 8], true);
				}
				else if (mapID[y][leftX] == CHIP_FIRE)
				{
					if ((GetRand(200) % 200) == 0)
					{
						lpEffect.AddEffectList("Effect/steam.png", VECTOR2(32, 32), VECTOR2(4, 3), VECTOR2(0, 0), 12, 4, VECTOR2(leftX * 32, y * 32) + VECTOR2(0, -16));
					}
					DrawGraph(leftX * 32 + GameDrawOffset().x, y * 32 + GameDrawOffset().y, IMAGE_ID("Image/mgm.png")[((y + animCnt) / 30) % 4], true);

				}
				else if (mapID[y][leftX] == CHIP_THORN)
				{
					DrawGraph((leftX - 2) * 32 + GameDrawOffset().x, (y - 2) * 32 + GameDrawOffset().y, IMAGE_ID("Image/kanban.png")[0], true);
				}
				else
				{
					DrawGraph(leftX * 32 + GameDrawOffset().x, y * 32 + GameDrawOffset().y, IMAGE_ID(mtStr)[mapID[y][leftX]], true);
				}
			}
		}
		leftX = (int)((camPos.x / 32) - (VIEW_CHIP_CNT_X / 2) - 1);
	}

	DrawGraph(0, 720, IMAGE_ID("Image/progress.png")[0], true);
	DrawGraph((pPos.x / GetGameAreaSize().x) * GetViewAreaSize().x - 16, 700, IMAGE_ID("Image/pCursor.png")[0], true);

	DrawGraph(700, 0, IMAGE_ID("Image/coinFrame.png")[0], true);
	DrawGraph(785, 8, IMAGE_ID("Image/number.png")[(Score / 100)], true);
	DrawGraph(820, 8, IMAGE_ID("Image/number.png")[(Score % 100) / 10], true);
	DrawGraph(855, 8, IMAGE_ID("Image/number.png")[Score % 10], true);

	DrawExtendGraph(700, 0, 764, 64, IMAGE_ID("Image/coinAnim2.png")[(animCnt / 10) % 4], true);
	DrawGraph(750, 16, IMAGE_ID("Image/X.png")[0], true);

	DrawGraph(50, 0, IMAGE_ID("Image/timeFrame.png")[0], true);
	DrawGraph(205, 8, IMAGE_ID("Image/number.png")[Time / 60], true);	//10分越えるとエラー伯(11こめをみるので)
	DrawGraph(260, 8, IMAGE_ID("Image/number.png")[(Time % 60) / 10], true);
	DrawGraph(295, 8, IMAGE_ID("Image/number.png")[(Time % 60) % 10], true);

	for (int i = 0; i < coinList.size(); i++)
	{
		if (coinList[i].active)
		{
			coinList[i].pos += (coinList[i].moveVec * coinList[i].moveRate);
			coinList[i].moveRate += 0.1f;
			if (coinList[i].pos.y < 25)
			{
				coinList[i].active = false;
				lpEffect.AddEffectList("Effect/get.png", VECTOR2(192, 192), VECTOR2(5, 3), VECTOR2(0, 0), 15, 2, VECTOR2(634, -66));
				CoinScoreAdd();
			}
		}
	}
	for (auto itr : coinList)
	{
		DrawGraph(itr.pos.x, itr.pos.y, IMAGE_ID("Image/mt.png")[CHIP_COIN], true);
	}





	//処理を終えたﾃﾞｰﾀの削除
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
	oldPos = pPos;
}

void MapCtl::MapBackDraw(void)
{
	if (pPos != oldPos && objUpdateFlag)
	{
		for (int i = 0; i < 4; i++)
		{
			switch (i)
			{
			case 0:
				backPos[i].x -= 0.125;
				break;
			case 1:
				backPos[i].x -= 0.25;
				break;
			case 2:
				backPos[i].x -= 0.5;
				break;
			case 3:
				backPos[i].x--;
				break;
			default:
				backPos[i].x--;;
				break;
			}
			backPos[i].x--;
			if (backPos[i].x < -1024)
			{
				backPos[i].x = 0;
			}
		}
	}
	if (mapType != 2)
	{
		DrawGraph(backPos[0].x, 0, IMAGE_ID("Image/CloudsBack.png")[0], true);
		DrawGraph(backPos[0].x + 1024, 0, IMAGE_ID("Image/CloudsBack.png")[0], true);

		DrawGraph(backPos[1].x, 0, IMAGE_ID("Image/CloudsFront.png")[0], true);
		DrawGraph(backPos[1].x + 1024, 0, IMAGE_ID("Image/CloudsFront.png")[0], true);

		DrawGraph(backPos[2].x, 0, IMAGE_ID("Image/BGBack.png")[0], true);
		DrawGraph(backPos[2].x + 1024, 0, IMAGE_ID("Image/BGBack.png")[0], true);

		DrawGraph(backPos[3].x, 0, IMAGE_ID("Image/BGFront.png")[0], true);
		DrawGraph(backPos[3].x + 1024, 0, IMAGE_ID("Image/BGFront.png")[0], true);
	}
	else
	{
		DrawGraph(backPos[3].x, 0, IMAGE_ID("Image/map2back.png")[0], true);
		DrawGraph(backPos[3].x + 1024, 0, IMAGE_ID("Image/map2back.png")[0], true);
	}

}

void MapCtl::MapNearDraw(VECTOR2 camPos)
{
	std::string mtStr;
	if (mapType != 2) mtStr = "Image/mt.png";
	else mtStr = "Image/mt2.png";

	int leftX, rightX;
	leftX = (int)((camPos.x / 32) - (VIEW_CHIP_CNT_X / 2) - 1);
	rightX = (int)((camPos.x / 32) + (VIEW_CHIP_CNT_X / 2) + 1);
	for (int y = 0; y < ChipCnt.y; y++)
	{
		if (leftX < 0)leftX = 0;
		if (rightX > ChipCnt.x) rightX = ChipCnt.x;
		for (leftX; leftX < rightX; leftX++)
		{
			if (mapID[y][leftX] == CHIP_GRASS1 || mapID[y][leftX] == CHIP_GRASS2 || mapID[y][leftX] == CHIP_GRASS3)
			{
				DrawGraph(leftX * 32 + GameDrawOffset().x, y * 32 + GameDrawOffset().y, IMAGE_ID(mtStr)[mapID[y][leftX]], true);
			}
		}
		leftX = (int)((camPos.x / 32) - (VIEW_CHIP_CNT_X / 2) - 1);
	}
	if (mapType == 0) TutorialMessage();
}

bool MapCtl::MapLoad(void)
{
	FILE *fp;
	std::string fileName;
	if (mapType == 0)fileName = "data/ちゅーとりある.fmf";
	else if (mapType == 1)fileName = "data/ちょいむず.fmf";
	else if (mapType == 2)fileName = "data/なんか.fmf";
	fopen_s(&fp, fileName.c_str(), "rb");
	if (fp == nullptr)
	{
		return false;
	}
	for (int y = 0; y < (int)mapID.size(); y++)
	{
		for (int x = 0; x < (int)mapID[y].size(); x++)
		{
			fread(&mapID[y][x], sizeof(unsigned char), 1, fp);
		}
	}
	return true;
}

VECTOR2 MapCtl::GetChipSize(void)
{
	return VECTOR2(32, 32);
}

VECTOR2 MapCtl::GetViewAreaSize(void)
{
	return VECTOR2(1024, 768);
}

VECTOR2 MapCtl::GetGameAreaSize(void)
{
	return ChipCnt * 32;
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
	oldPos = pPos;
	pPos = pos;
}

void MapCtl::SetEnemyPos(VECTOR2 pos)
{
	ePos = pos;
}

void MapCtl::TimerStart(void)
{
	startTime = std::chrono::system_clock::now();
}

bool MapCtl::CheckGameEnd(void)
{
	return endFlag;
}

void MapCtl::SetEndFlag(bool flag, int resultType)
{
	endFlag = flag;
	clearType = resultType;
	messageTimer = 0;
	backOffset = 0;
	pPos = { 50,0 };
}

void MapCtl::SetMapType(int no)
{
	mapType = no;
	if (mapType == 0)
	{
		ChipCnt = { 320,24 };
	}
	else
	{
		ChipCnt = { 700,24 };
	}
}

void MapCtl::GetCoinTime(int & coin, int & time, int & type)
{
	coin = Score;
	time = Time;
	type = clearType;
}

std::vector<int> MapCtl::GetRankList(void)
{
	return RankList;
}

void MapCtl::AddRanking(int score)
{
	RankList.push_back(score);
	std::sort(RankList.begin(), RankList.end(), std::greater<int>());	//降順ソート
	RankingSave();
}

void MapCtl::TutorialMessage(void)
{
	if (mapType == 0)
	{
		DrawRotaGraph(512, 160, windowRate, 0, IMAGE_ID("Image/mWindow.png")[0], true, false);

		if (pPos.x > 200 && messageTimer < 540)
		{
			messageTimer++;
			if (windowRate < 1)
			{
				windowRate += 0.05;
			}
			else
			{
				if (0 < messageTimer&& messageTimer < 180)
				{
					DrawStringToHandle(270, 110, "チュートリアルへようこそ！", 0xf0f0f0, mFont);
				}
				else if (180 < messageTimer&& messageTimer < 360)
				{
					DrawStringToHandle(270, 110, "ここでは基本的な操作", 0xf0f0f0, mFont);
					DrawStringToHandle(450, 160, "を教えます", 0xf0f0f0, mFont);
				}
				else if (360 < messageTimer&& messageTimer < 540)
				{
					DrawStringToHandle(270, 110, "キャラは自動で走るので", 0xf0f0f0, mFont);
					DrawStringToHandle(270, 160, "とりあえず進んでみましょう！", 0xf0f0f0, mFont);
				}

			}
			objUpdateFlag = false;
		}
		else if (pPos.x > 25 * 32 && messageTimer < 1260)
		{
			messageTimer++;
			if (windowRate < 1)
			{
				windowRate += 0.05;
			}
			else
			{
				if (540 < messageTimer&& messageTimer < 720)
				{
					DrawStringToHandle(270, 110, "先に穴があります 穴には", 0xf0f0f0, mFont);
					DrawStringToHandle(270, 160, "茂みがあり遅くなってしまいます", 0xf0f0f0, mFont);
				}
				else if (720 < messageTimer&& messageTimer < 900)
				{
					DrawStringToHandle(270, 110, "SPACEキーを押すことで", 0xf0f0f0, mFont);
					DrawStringToHandle(270, 160, "ジャンプができます", 0xf0f0f0, mFont);
				}
				else if (900 < messageTimer&& messageTimer < 1080)
				{
					DrawStringToHandle(270, 110, "長く押せばより大きく", 0xf0f0f0, mFont);
					DrawStringToHandle(270, 160, "ジャンプすることができます", 0xf0f0f0, mFont);
				}
				else if (1080 < messageTimer&& messageTimer < 1260)
				{
					DrawStringToHandle(270, 110, "ではやってみましょう！", 0xf0f0f0, mFont);
				}

			}
			objUpdateFlag = false;
		}
		else if (pPos.x > 124 * 32 && messageTimer < 1980)
		{
			messageTimer++;
			if (windowRate < 1)
			{
				windowRate += 0.05;
			}
			else
			{
				if (1260 < messageTimer&& messageTimer < 1440)
				{
					DrawStringToHandle(270, 110, "先ほどにもありましたが", 0xf0f0f0, mFont);
					DrawStringToHandle(270, 160, "いろいろな場所にコインがあります", 0xf0f0f0, mFont);
				}
				else if (1440 < messageTimer&& messageTimer < 1620)
				{
					DrawStringToHandle(270, 110, "これはスコアの要素の1つで", 0xf0f0f0, mFont);
					DrawStringToHandle(270, 160, "コインとタイムでスコアが増えます", 0xf0f0f0, mFont);
				}
				else if (1620 < messageTimer&& messageTimer < 1800)
				{
					DrawStringToHandle(270, 110, "多くのコインを手に入れられる", 0xf0f0f0, mFont);
					DrawStringToHandle(270, 160, "ように動くのが高得点のコツです", 0xf0f0f0, mFont);
				}
				else if (1800 < messageTimer&& messageTimer < 1980)
				{
					DrawStringToHandle(270, 110, "では進みましょう", 0xf0f0f0, mFont);
				}

			}
			objUpdateFlag = false;
		}
		else if (pPos.x > 222 * 32 && messageTimer < 2700)
		{
			messageTimer++;
			if (windowRate < 1)
			{
				windowRate += 0.05;
			}
			else
			{
				if (1980 < messageTimer&& messageTimer < 2160)
				{
					DrawStringToHandle(270, 110, "ステージには右のような", 0xf0f0f0, mFont);
					DrawStringToHandle(270, 160, "ブロックが配置されています", 0xf0f0f0, mFont);
					DrawBox(660, 105, 700, 145, 0xf0f0f0, true);
					DrawGraph(664, 110, IMAGE_ID("Image/mt.png")[CHIP_BLOCK1], true);
				}
				else if (2160 < messageTimer&& messageTimer < 2340)
				{
					DrawStringToHandle(270, 110, "このブロックにはワイヤーを", 0xf0f0f0, mFont);
					DrawStringToHandle(380, 160, "使用することができます", 0xf0f0f0, mFont);
				}
				else if (2340 < messageTimer&& messageTimer < 2520)
				{
					DrawStringToHandle(270, 110, "カーソルをブロックに合わせて", 0xf0f0f0, mFont);
					DrawStringToHandle(270, 160, "左クリックすると準備状態になります", 0xf0f0f0, mFont);
				}
				if (2520 < messageTimer&& messageTimer < 2700)
				{
					DrawStringToHandle(270, 110, "そして右クリックをすると", 0xf0f0f0, mFont);
					DrawStringToHandle(270, 160, "ワイヤーを使用することができます", 0xf0f0f0, mFont);
				}

			}
			objUpdateFlag = false;
		}
		else if (pPos.x > 294 * 32 && messageTimer < 3260)
		{
			messageTimer++;
			if (windowRate < 1)
			{
				windowRate += 0.05;
			}
			else
			{
				if (2700 < messageTimer&& messageTimer < 2880)
				{
					DrawStringToHandle(270, 110, "これでチュートリアルは終了です！", 0xf0f0f0, mFont);
					DrawStringToHandle(270, 160, "お疲れさまでした！", 0xf0f0f0, mFont);
				}
				else if (2880 < messageTimer&& messageTimer < 3060)
				{
					DrawStringToHandle(270, 110, "今回はいませんでしたが", 0xf0f0f0, mFont);
					DrawStringToHandle(270, 160, "別のｽﾃｰｼﾞには逃げるべき敵がいます", 0xf0f0f0, mFont);
				}
				else if (3060 < messageTimer&& messageTimer < 3240)
				{
					DrawStringToHandle(270, 110, "捕まらないように逃げながら", 0xf0f0f0, mFont);
					DrawStringToHandle(270, 160, "高得点を目指して走り抜けましょう！", 0xf0f0f0, mFont);
				}
				else if (messageTimer > 3260)
				{
					endFlag = true;
					clearType = 0;
				}
			}
			objUpdateFlag = false;
		}
		else
		{
			objUpdateFlag = true;
			if (windowRate > 0)
			{
				windowRate -= 0.05;
			}
		}
	}
}

bool MapCtl::GetUpdateFlag(void)
{
	return objUpdateFlag;
}

bool MapCtl::GetMoveFlag(void)
{
	if (pPos != oldPos)
	{
		return true;
	}
	return false;
}

bool MapCtl::RankingLoad(void)
{
	FILE *fp;
	std::string dataName;
	if (mapType == 1) dataName = "data/rankData1.bin";
	else dataName = "data/rankData2.bin";
	fopen_s(&fp, dataName.c_str(), "rb");
	if (fp == nullptr)
	{
		return false;
	}
	for (int i = 0; i < RankList.size(); i++)
	{
		fread(&RankList[i], sizeof(int), 1, fp);
	}
	fclose(fp);
	return false;
}

bool MapCtl::RankingSave(void)
{
	FILE *fp;
	std::string dataName;
	if (mapType == 1) dataName = "data/rankData1.bin";
	else dataName = "data/rankData2.bin";
	fopen_s(&fp, dataName.c_str(), "wb");
	if (fp == nullptr)
	{
		return false;
	}
	for (int i = 0; i < RankList.size(); i++)
	{
		fwrite(&RankList[i], sizeof(int), 1, fp);
	}
	fclose(fp);
	return true;
}

int MapCtl::GetMapType(void)
{
	return mapType;
}


MapCtl::MapCtl()
{
	RankList.resize(200);
	RankingLoad();
	std::sort(RankList.begin(), RankList.end(), std::greater<int>());
	ImageMng::GetInstance().GetID("Image/mt.png", { 32,32 }, { 27,1 }, { 0,0 });
	ImageMng::GetInstance().GetID("Image/mt2.png", { 32,32 }, { 27,1 }, { 0,0 });
	ImageMng::GetInstance().GetID("Image/mgm.png", { 32,32 }, { 4,1 }, { 0,0 });
	ImageMng::GetInstance().GetID("Image/number.png", { 36,48 }, { 10,1 }, { 0,0 });
	ImageMng::GetInstance().GetID("Effect/get.png", { 192,192 }, { 5,3 }, { 0,0 });
	ImageMng::GetInstance().GetID("Image/coinAnim2.png", VECTOR2(32, 32), VECTOR2(8, 1), VECTOR2(0, 0));
	ChipCnt = { 1,1 };
	Init();
}


MapCtl::~MapCtl()
{
}
