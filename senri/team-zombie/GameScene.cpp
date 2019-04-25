#include "GameScene.h"
#include "ResultScene.h"
#include "GameTask.h"

GameScene::GameScene()
{
	Init();
}

GameScene::~GameScene()
{
}

int GameScene::Init()
{
	MakePlayer();
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
	(*player)->init("image/protPlayer.png", { 72, 84 }, { 4,2 }, { 0,0 }, 1, 10, 4);
	(*player)->SetPos(VECTOR2(50, 600));
}



BASE GameScene::Update(BASE & _this, const std::shared_ptr<MouseCtl>_mouseCtl)
{
	ClsDrawScreen();
	DrawString(0, 0, "gamemain", GetColor(0xff, 0xff, 0xff), true);

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
		return 	std::move(std::make_unique <ResultScene>());
	}
	return std::move(_this);
}