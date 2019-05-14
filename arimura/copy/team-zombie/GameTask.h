#pragma once
#include <mutex>
#include <memory>
#include <chrono>
#include "BaseScene.h"

#define SCREEN_SIZE_X 1024
#define SCREEN_SIZE_Y 768
#define lpGameTask GameTask::GetInstance()

enum class TIME_ST {
	START,
	END,
	MAX
};

using chrono_clock = std::chrono::system_clock;
using TimePoint = chrono_clock::time_point;
using TimePointArray = std::array<chrono_clock::time_point, static_cast<size_t>(TIME_ST::MAX)>;



class GameTask
{
public:
	static GameTask & GetInstance()
	{
		return *s_Instance;
	}

	void Run();

	void StartPrgTime(void);				//ˆ—n‚ß‚ÌŠÔ
	void EndPrgTime(void);					//ˆ—I‚í‚è‚ÌŠÔ
	__int64 GetPrgTime(void);				//ˆ—ŠÔ
	
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
	TimePointArray tp;
};

