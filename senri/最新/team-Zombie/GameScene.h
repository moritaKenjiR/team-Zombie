#pragma once
#include "BaseScene.h"
#include <memory>
#include <list>
#include "Player.h"
#include "Obj.h"
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

	void MakePlayer(void);
	obj_List::iterator AddObjList(obj_ptr && obj);

	

	std::shared_ptr<MouseCtl>mouseCtl;
	std::shared_ptr<Player> player;
	std::unique_ptr<Camera> camera;

	obj_List objList;
	
};

