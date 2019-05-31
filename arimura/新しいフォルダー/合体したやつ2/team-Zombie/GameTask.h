#pragma once
#include <memory>
#include "BaseScene.h"

#define SCREEN_SIZE_X 1024
#define SCREEN_SIZE_Y 768
#define lpGameTask GameTask::GetInstance()

class GameTask
{
public:
	static GameTask &GetInstance();
	//static GameTask & GetInstance()
	//{
	//	return *s_Instance;
	//}

	void Run();
	
private:
	GameTask();
	~GameTask();

	struct GameTaskDeleter
	{
		void operator()(GameTask* gametask) const
		{
			delete gametask;
		}
	};
	static std::unique_ptr<GameTask, GameTaskDeleter> s_Instance;

	int SysInit();

	std::shared_ptr<MouseCtl>mouseCtl;

	BASE ActiveScene;

};

