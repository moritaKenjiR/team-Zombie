#include "GameTask.h"
#include "TitleScene.h"
#include "DxLib.h"

std::unique_ptr<GameTask, GameTask::GameTaskDeleter> GameTask::s_Instance(new GameTask());



GameTask::GameTask()
{
	SysInit();
}


GameTask::~GameTask()
{
}

void GameTask::Run()
{
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0)
	{
		ActiveScene = std::move(ActiveScene->Update(ActiveScene, mouseCtl));
	}
}

void GameTask::StartPrgTime(void)
{
	tp[static_cast<int>(TIME_ST::START)] = chrono_clock::now();
}

void GameTask::EndPrgTime(void)
{
	tp[static_cast<int>(TIME_ST::END)] = chrono_clock::now();
}

__int64 GameTask::GetPrgTime(void)
{
	return std::chrono::duration_cast<std::chrono::seconds>(
		tp[static_cast<int>(TIME_ST::END)] - tp[static_cast<int>(TIME_ST::START)]).count();
}

int GameTask::SysInit()
{
	SetWindowText("team-Zombie");
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1)
	{
		return -1;
	}
	ActiveScene = std::make_unique<TitleScene>();
	mouseCtl = std::make_unique<MouseCtl>();
	SetDrawScreen(DX_SCREEN_BACK);
}
