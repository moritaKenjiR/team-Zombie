#include <random>
#include "GameScene.h"
#include "ResultScene.h"
#include "GameTask.h"
#include "MapCtl.h"
#include "Effect.h"
#include "Shaker.h"
#include "ImageMng.h"
#include "EnemyAI.h"

GameScene::GameScene()
{
	Init();
}

GameScene::~GameScene()
{
	lpEffect.SetGameFlag(false);
}

int GameScene::Init()
{
	//SetDrawScreen(ShakeGH);
	lpEffect.DeleteEffectList();
	lpEffect.SetGameFlag(true);
	lpSoundCtl.AddSoundList("sound/AFF.mp3", loop, 80);
	lpEffect.AddEffectList("Effect/effect2.png", VECTOR2(1024, 768), VECTOR2(5, 1), VECTOR2(0, 0), 5, 5, VECTOR2(0, 0));
	ImageMng::GetInstance().GetID("Image/startNumber.png", {128,160}, {3,1}, {0,0});
	MakePlayer();
	if (lpMapCtl.GetMapType() != 0)
	{
		MakeEnemy();
	}
	lpMapCtl.Init();

	cnt = 600;
	startCnt = 0;
	loadEndFlag = false;
	sceneEndFlag = false;
	startFlag = false;
	startRate = 0;
	startPos = {512,384};
	
	camera->Update();
	lpMapCtl.SetDrawOffset(camera->GetPos());
	lpEnemyAI.CreateShortestMap();
	lpEnemyAI.CreateTopograMap();
	
	return 0;
}

obj_List::iterator GameScene::AddObjList(obj_ptr && obj)
{
	objList.push_back(obj);
	auto itr = objList.end();
	itr--;
	return itr;
}


void GameScene::MakePlayer(void)
{
	std::list<obj_ptr>::iterator player;
	player = AddObjList(std::make_shared<Player>());
	(*player)->init("Image/char.png", { 64, 64 }, { 4,1 }, { 0,0},8, 10, 4);
	(*player)->SetPos(VECTOR2(50, 500));

	lpEnemyAI.SetTarget((*player));
	camera = std::make_unique<Camera>();
	camera->SetTarget((*player));
	camera->SetPos(0, 0);
	lpMapCtl.TimerStart();
}

void GameScene::MakeEnemy(void)
{
	std::list<obj_ptr>::iterator enemy;
	enemy = AddObjList(std::make_shared<Enemy>());
	(*enemy)->init("Image/Enemy.png", { 128,128 }, { 8,2 }, { 0,0 }, 7, 10, 6);
	(*enemy)->SetPos(VECTOR2(32 * 0, 32 * 0));

	camera = std::make_unique<Camera>();
	camera->SetTarget((*enemy));
	camera->SetPos(0, 0);
	lpMapCtl.TimerStart();
}



BASE GameScene::Update(BASE & _this, const std::shared_ptr<MouseCtl>_mouseCtl)
{
	ClsDrawScreen();
	DrawString(0, 0, "gamemain", GetColor(0xff, 0xff, 0xff), true);
	mouseCtl = _mouseCtl;
	(*mouseCtl).Update();
	mouseBtn = mouseCtl->GetBtn();

	if (lpMapCtl.CheckGameEnd())
	{
		lpSoundCtl.SoundDel();
		sceneEndFlag = true;
	}

	if (loadEndFlag && !sceneEndFlag && startFlag && lpMapCtl.GetUpdateFlag())
	{
		for (auto itr = objList.begin(); itr != objList.end(); itr++)
		{
			(*itr)->Update();
		}
		
	}
	camera->Update();
	lpMapCtl.SetDrawOffset(camera->GetPos());
	lpMapCtl.MapDraw(camera->GetPos());

	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		(*itr)->Draw();
	}
	lpEffect.EffectDraw();
	lpMapCtl.MapNearDraw(camera->GetPos());
	lpShaker.ShakeDraw();
	
	
	if (sceneEndFlag)
	{
		if (EndProcess())
		{
			return std::move(std::make_unique <ResultScene>());
		}
	}
	else if(!loadEndFlag)
	{
		if (LoadProcess())
		{
			loadEndFlag = true;
		}
	}
	else if (!startFlag)
	{
		if (StartProcess())
		{
			startFlag = true;
			lpMapCtl.TimerStart();
		}
	}
	else
	{
		if (startPos.y > -256)
		{
			startPos.y -= 25;
			DrawRotaGraph(startPos.x, startPos.y, startRate, 0, IMAGE_ID("Image/startFrame.png")[0], true, false);
			DrawRotaGraph(startPos.x, startPos.y, startRate, 0, IMAGE_ID("Image/Start.png")[0], true, false);
		}
	}
	DrawGraph(_mouseCtl->GetPoint().x, _mouseCtl->GetPoint().y, IMAGE_ID("Image/mouseCursor.png")[0], true);
	ScreenFlip();

	return std::move(_this);
}

bool GameScene::LoadProcess(void)
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

bool GameScene::EndProcess(void)
{
	int coin,time,type;
	lpMapCtl.GetCoinTime(coin, time, type);
	cnt += 10;
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, cnt / 2);
	DrawBox(0, 0, 1024, 768, (type == 1) ? 0xff3030:0xffffff, true);

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	if (cnt >= 600)
	{
		return true;
	}
	return false;;
}

bool GameScene::StartProcess(void)
{
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 100);
	DrawBox(0, 0, 1024, 768, 0x000000, true);
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 255);
	lpMapCtl.TimerStart();

	startCnt++;
	DrawRotaGraph(startPos.x, startPos.y, startRate, 0, IMAGE_ID("Image/startFrame.png")[0], true, false);
	if (startCnt < 100)
	{
		if (startRate < 1)
		{
			startRate += 0.05;
		}
		DrawRotaGraph(startPos.x, startPos.y, startRate, 0, IMAGE_ID("Image/Ready.png")[0], true, false);
	}
	else if (startCnt < 280)
	{
		DrawRotaGraph(startPos.x, startPos.y, startRate, 0, IMAGE_ID("Image/startNumber.png")[(startCnt-100)/60], true, false);
	}
	else if (startCnt <= 300)
	{
		DrawRotaGraph(startPos.x, startPos.y, startRate, 0, IMAGE_ID("Image/Start.png")[0], true, false);
		return true;
	}
	return false;
}
