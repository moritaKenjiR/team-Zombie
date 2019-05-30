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
}

int GameScene::Init()
{
	//SetDrawScreen(ShakeGH);
	lpEffect.AddEffectList("Effect/effect2.png", VECTOR2(1024, 768), VECTOR2(5, 1), VECTOR2(0, 0), 5,5,VECTOR2(0,0));
	//MakePlayer();
	MakeEnemy();
	lpMapCtl.Init();
	
	camera->Update();
	lpMapCtl.SetDrawOffset(camera->GetPos());
	lpMapCtl.TimerStart();

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
	(*player)->SetPos(VECTOR2(32 * 3, 32 * 8));

	lpEnemyAI.SetTarget((*player));
	camera = std::make_unique<Camera>();
	camera->SetTarget((*player));
	camera->SetPos(0, 0);
}

void GameScene::MakeEnemy(void)
{
	std::list<obj_ptr>::iterator enemy;
	enemy = AddObjList(std::make_shared<Enemy>());
	(*enemy)->init("Image/Enemy.png", { 128,128 }, { 8,2 }, { 0,0 }, 10, 10, 6);
	(*enemy)->SetPos(VECTOR2(32 * 1, 32 * 8));

	lpEnemyAI.SetTarget((*enemy));
	camera = std::make_unique<Camera>();
	camera->SetTarget((*enemy));
	camera->SetPos(0, 0);
}



BASE GameScene::Update(BASE & _this, const std::shared_ptr<MouseCtl>_mouseCtl)
{
	ClsDrawScreen();
	DrawString(0, 0, "gamemain", GetColor(0xff, 0xff, 0xff), true);

	if (lpMapCtl.CheckGameEnd())
	{
		lpEffect.EffClear();
		return std::move(std::make_unique <ResultScene>());
	}


	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		(*itr)->Update();
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
	DrawGraph(_mouseCtl->GetPoint().x, _mouseCtl->GetPoint().y, IMAGE_ID("Image/mouseCursor.png")[0], true);
	ScreenFlip();
	mouseCtl = _mouseCtl;
	(*mouseCtl).Update();
	mouseBtn = mouseCtl->GetBtn();
	if ((_mouseCtl->GetBtn()[ST_NOW]) & (~_mouseCtl->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
	{
		//return 	std::move(std::make_unique <ResultScene>());
	}
	return std::move(_this);
}