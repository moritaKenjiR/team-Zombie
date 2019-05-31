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
	bool LoadProcess(void);
	bool EndProcess(void);

	std::shared_ptr<MouseCtl> mouseCtl;
private:
	void Draw(VECTOR2 mPos);
	void Move(void);
	bool CheckBox(VECTOR2 mPos, VECTOR2 boxLT, VECTOR2 boxRD);

	bool MoveEndFlag;
	bool CheckFlag;
	int selectStage;
	int moveTime;
	int animCnt;
	int cnt;
	bool loadEndFlag, sceneEndFlag;
	int FontHandle;
	std::string stageTitle;
	MOVE_TYPE moveID;
	std::vector<movingObj> stageList;
	double extRate, backRate;
	bool extFlag, upFlag;

	VECTOR2 backPos;
};

