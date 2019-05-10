#include <DxLib.h>
#include "Effect.h"
#include "ImageMng.h"
#include "MapCtl.h"

std::unique_ptr<Effect, Effect::effectDeleter> Effect::s_Instance(new Effect());

void Effect::AddEffectList(std::string imgName,VECTOR2 divSize,VECTOR2 divCnt,VECTOR2 chipOffset,int animMaxCnt,int animspeed,VECTOR2 pos)
{
	ImageMng::GetInstance().GetID(imgName, divSize, divCnt, chipOffset);
	//effMap[imgName].animMaxCnt = animMaxCnt;
	effList.push_back(effectData{});
	auto itr = effList.end();
	itr--;
	(*itr).GHandle = imgName;
	(*itr).animMaxCnt = animMaxCnt;
	(*itr).pos = pos;
	(*itr).timer = 0;
	(*itr).animSpeed = animspeed;
	(*itr).active = true;
}


void Effect::EffectDraw(void)
{
	VECTOR2 Offset = lpMapCtl.GameDrawOffset();
	for (auto itr : effList)
	{
		if (itr.active)
		{
			if (itr.GHandle == "Effect/effect2.png")
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 30);
				DrawGraph(itr.pos.x, itr.pos.y, IMAGE_ID(itr.GHandle)[(itr.timer / itr.animSpeed) % itr.animMaxCnt], true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			}
			else
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
				DrawGraph(itr.pos.x + Offset.x, itr.pos.y + Offset.y, IMAGE_ID(itr.GHandle)[(itr.timer / itr.animSpeed) % itr.animMaxCnt], true);
				SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
			}
		}
	}
	
	for (int i = 0; i < effList.size(); i++)
	{
		if (effList[i].active)
		{
			effList[i].timer++;

			if (((effList[i].timer / animTime) > effList[i].animMaxCnt) && effList[i].GHandle != "Effect/effect2.png")
			{
				effList[i].active = false;
			}
		}
	}
}

void Effect::SetEffPos(std::string imgName,VECTOR2 pos)
{
	for (auto itr : effList)
	{
		if (itr.GHandle == imgName)
		{
			itr.pos = pos;
		}
	}
}

Effect::Effect()
{
	animTime = 5;
}


Effect::~Effect()
{
}
