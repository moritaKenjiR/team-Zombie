#include "main.h"
#include "GameTask.h"
#include "DxLib.h"

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstace, LPSTR lpCmdLine, int nCmdShow)
{
	lpGameTask.Run();
	DxLib_End();
	return 0;
}