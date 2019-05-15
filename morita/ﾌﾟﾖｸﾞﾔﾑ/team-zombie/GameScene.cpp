#include "GameScene.h"
#include "ResultScene.h"
#include "GameTask.h"
#include "MapCtl.h"
#include "Effect.h"

GameScene::GameScene()
{
	Init();
}

GameScene::~GameScene()
{
}

int GameScene::Init()
{
	lpEffect.AddEffectList("Effect/effect2.png", VECTOR2(1024, 768), VECTOR2(5, 1), VECTOR2(0, 0), 5,5,VECTOR2(0,0));
	MakePlayer();

	camera->Update();
	lpMapCtl.SetDrawOffset(camera->GetPos());
	//objList.push_back(std::make_shared<Player>());

	/*auto itr = objList.end();
	itr--;
	(*itr)->init("image/1.png", VECTOR2(64, 64), VECTOR2(1, 1), VECTOR2(0, 0), 2, 0, 0);*/
	//objList.clear();

	//player = std::make_shared<Player>();
	//player->init("image/1.png", VECTOR2(64,64), VECTOR2(1,1),VECTOR2(0,0), 2,0,0);
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
	(*player)->SetPos(VECTOR2(50, 400));

	camera = std::make_unique<Camera>();
	camera->SetTarget((*player));
	camera->SetPos(0, 0);
}



BASE GameScene::Update(BASE & _this, const std::shared_ptr<MouseCtl>_mouseCtl)
{
	ClsDrawScreen();
	DrawString(0, 0, "gamemain", GetColor(0xff, 0xff, 0xff), true);

	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		(*itr)->Update();
	}
	camera->Update();
	lpMapCtl.SetDrawOffset(camera->GetPos());

	lpMapCtl.MapDraw(camera->GetPos());
	//player->Draw();
	for (auto itr = objList.begin(); itr != objList.end(); itr++)
	{
		(*itr)->Draw();
	}
	lpEffect.EffectDraw();
	lpMapCtl.MapNearDraw(camera->GetPos());
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