#pragma once
#include <vector>
#include "BaseScene.h"

enum ID {
	POS_L,
	POS_C,
	POS_R,
	POS_MAX,
};

enum MOVE_TYPE {
	MOVE_L,
	MOVE_R,
	MOVE_MAX,
};

enum MAP_ID {
	MAP_1,
	MAP_2,
	MAP_3,
	MAP_MAX,
};

struct movingObj {
	VECTOR2 Pos;
	std::string GHandle;
	ID id;
	MAP_ID mapID;
};



class MenuScene :
	public BaseScene
{
public:
	MenuScene();
	~MenuScene();

	virtual int Init();
	virtual BASE Update(BASE &_this, const std::shared_ptr<MouseCtl> _mouseCtl);

	std::shared_ptr<MouseCtl> mouseCtl;
private:
	void Draw(void);
	void Move(void);

	bool MoveEndFlag;
	bool CheckFlag;
	int selectStage;
	int moveTime;
	MOVE_TYPE moveID;
	std::vector<movingObj> stageList;
};

