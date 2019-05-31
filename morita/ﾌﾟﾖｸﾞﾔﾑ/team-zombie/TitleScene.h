#pragma once
#include "BaseScene.h"
class TitleScene :
	public BaseScene
{
public:
	TitleScene();
	~TitleScene();
	virtual int Init();
	virtual BASE Update(BASE &_this, const std::shared_ptr<MouseCtl> _mouseCtl);
	void Move(void);
	void Draw(void);
	bool LoadProcess(void);
	bool EndProcess(void);

	std::shared_ptr<MouseCtl>mouseCtl;

private:
	bool sceneEndFlag;
	bool loadEndFlag;
	bool cntFlag;
	int cnt;
	int animCnt;
	int FontHandle;
	VECTOR2 effPos[2];
};

