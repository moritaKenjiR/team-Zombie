#pragma once
#include "BaseScene.h"
#include <memory>
#include <list>
#include "Player.h"
#include "Obj.h"
#include "Enemy.h"
#include "Camera.h"

typedef std::shared_ptr<Obj> obj_ptr;
typedef std::list<obj_ptr> obj_List;

class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();
	virtual int Init();
	virtual BASE Update(BASE &_this, const std::shared_ptr<MouseCtl> _mouseCtl);
	bool LoadProcess(void);
	bool EndProcess(void);
	bool StartProcess(void);

	void MakePlayer(void);
	void MakeEnemy(void);
	obj_List::iterator AddObjList(obj_ptr && obj);

	

	std::shared_ptr<MouseCtl>mouseCtl;
	std::shared_ptr<Player> player;
	std::shared_ptr <Enemy> enemy;
	std::unique_ptr<Camera> camera;

	obj_List objList;

	int cnt,startCnt;
	double startRate;
	VECTOR2 startPos;
	bool loadEndFlag, sceneEndFlag, startFlag;
	
};

