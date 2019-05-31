#pragma once
#include <vector>
#include "BaseScene.h"
class ResultScene :
	public BaseScene
{
public:
	ResultScene();
	~ResultScene();
	virtual int Init();
	virtual BASE Update(BASE &_this, const std::shared_ptr<MouseCtl> _mouseCtl);
	bool LoadProcess(void);
	bool EndProcess(void);

	std::shared_ptr<MouseCtl>mouseCtl;
private:
	void Draw(VECTOR2 mPos);
	void Move(void);
	bool CheckBox(VECTOR2 mPos, VECTOR2 boxLT, VECTOR2 boxRD);

	double extRate;
	bool extFlag;
	bool moveEndFlag;
	bool logoFlag;
	bool rankingFlag;
	double LogoRate;
	double rankingRate;
	int FontHandle,FontHandleMei;
	int rankFont[5];
	int coin, time, score;
	int coincnt;
	int clearType;

	int cnt;
	bool loadEndFlag, retryFlag,sceneEndFlag;

	std::vector<int> rankList;
	VECTOR2 rankingPos;
	
};

