#include "GameScene.h"
#include "ResultScene.h"
#include "GameTask.h"
#include "MapCtl.h"
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
	lpGameTask.StartPrgTime();
	MakePlayer();
	MakeEnemy();
	camera->Update();
	lpMapCtl.SetDrawOffset(camera->GetPos());
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
	(*player)->init("Image/protPlayer.png", { 72, 84 }, { 4,4 }, { 0,0 },8, 10, 6);
	(*player)->SetPos(VECTOR2(50, 600));

	camera = std::make_unique<Camera>();
	camera->SetTarget((*player));
	camera->SetPos(0, 0);
	lpEnemyAI.SetTarget((*player));
}

void GameScene::MakeEnemy(void)
{
	std::list<obj_ptr>::iterator enemy;
	enemy = AddObjList(std::make_shared<Enemy>());
	(*enemy)->init("Image/protEnemy.png", { 72,84 }, { 4,4 }, { 0,0 }, 8, 10, 6);
	(*enemy)->SetPos(VECTOR2(0, 600));
	/*camera = std::make_unique<Camera>();
	camera->SetTarget((*enemy));
	camera->SetPos(0, 0);*/
}



BASE GameScene::Update(BASE & _this, const std::shared_ptr<MouseCtl>_mouseCtl)
{
	ClsDrawScreen();
	DrawString(0, 0, "gamemain", GetColor(0xff, 0xff, 0xff), true);
	camera->Update();
	lpMapCtl.SetDrawOffset(camera->GetPos());
	lpMapCtl.MapDraw(camera->GetPos());
	//player->Draw();
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		(*itr)->Draw();
	}
	
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		(*itr)->Update();
	}

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