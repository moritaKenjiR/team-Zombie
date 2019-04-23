#pragma once
#include "BaseScene.h"
#include "Player.h"
class GameScene :
	public BaseScene
{
public:
	GameScene();
	~GameScene();
	virtual int Init();
	virtual BASE Update(BASE &_this, const std::shared_ptr<MouseCtl> _mouseCtl);

	std::shared_ptr<MouseCtl>mouseCtl;
	std::shared_ptr<Player> player;
};

