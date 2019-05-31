#include <random>
#include <DxLib.h>
#include "Shaker.h"
#include "ImageMng.h"

std::unique_ptr<Shaker, Shaker::shakerDeleter> Shaker::s_Instance(new Shaker());

void Shaker::ShakeOffsetSet(void)
{
	if (moveType == RAND_SHAKE)
	{
		shakeOffset = { 0,0 };
		std::random_device rndx;
		std::mt19937 mtx(rndx());
		std::random_device rndy;
		std::mt19937 mty(rndy());
		std::uniform_int_distribution<> rand(-10, 10);	// ˆø”‚Å—h‚ê•Žw’è

		switch (shakeType) {
		case SHAKE_TYPE::SHAKE_X:
			shakeOffset.x = rand(mtx);
			break;
		case SHAKE_TYPE::SHAKE_Y:
			shakeOffset.y = rand(mty);
			break;
		case SHAKE_TYPE::SHAKE_XY:
			shakeOffset.x = rand(mtx);
			shakeOffset.y = rand(mty);
			break;
		default:
			break;
		}
	}
	if (moveType == JUMP_SHAKE)
	{
		shakeOffset.y -= 2;
	}
}

void Shaker::SetShaker(bool flag, int timer, SHAKE_TYPE type)
{
	shakeFlag = flag;
	shakeTimer = timer;
	shakeType = type;
}

void Shaker::MoveOffset(std::string moveName)
{
	if (moveName == "ƒWƒƒƒ“ƒv")
	{
		shakeOffset.y++;
	}
}

void Shaker::ShakeDraw()
{
	if (shakeTimer > 0)
	{
		shakeTimer--;
		if (shakeFlag)
		{
			GetDrawScreenGraph(0, 0, 1024, 768, ShakeGH);
			ShakeOffsetSet();
			DrawGraph(0, 0, IMAGE_ID("Image/black.png")[0], false);
			DrawGraph(shakeOffset.x, shakeOffset.y, ShakeGH, false);
		}
	}
	else
	{
		shakeOffset = { 0,0 };
		shakeFlag = false;
	}
}

Shaker::Shaker()
{
	ShakeGH = MakeGraph(1024, 768);
	shakeFlag = false;
	moveType = RAND_SHAKE;
}


Shaker::~Shaker()
{
}
