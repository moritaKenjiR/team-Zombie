#include "DxLib.h"
#include "main.h"
#include "GameTask.h"


int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstace, LPSTR lpCmdLine, int nCmdShow)
{
	lpGameTask.Run();
	DxLib_End();
	return 0;
}