#include <DxLib.h>
#include "Effect.h"
#include "ImageMng.h"
#include "MapCtl.h"
#include <list>

//std::unique_ptr<Effect, Effect::effectDeleter> Effect::s_Instance(new Effect());

void Effect::AddEffectList(std::string imgName,VECTOR2 divSize,VECTOR2 divCnt,VECTOR2 chipOffset,int animMaxCnt,int animspeed,VECTOR2 pos)
{
	ImageMng::GetInstance().GetID(imgName, divSize, divCnt, chipOffset);
	//effMap[imgName].animMaxCnt = animMaxCnt;
	effList.push_back(effectData{ imgName ,pos,animMaxCnt ,0,animspeed ,true, });
}

Effect&
Effect::GetInstance() {
	static Effect instance;
	return instance;
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
			else if (itr.GHandle == "Effect/get.png")
			{
				SetDrawBlendMode(DX_BLENDMODE_ALPHA, 180);
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

			if (((effList[i].timer / effList[i].animSpeed) >= effList[i].animMaxCnt) && effList[i].GHandle != "Effect/effect2.png")
			{
				effList[i].active = false;
			}
		}
	}
	//èàóùÇèIÇ¶ÇΩ√ﬁ∞¿ÇÃçÌèú
	std::vector<effectData> ::const_iterator it = effList.begin();
	while (it != effList.end())
	{
		effectData eff = (*it);
		if (eff.active == false) {

			it = effList.erase(it);
		}
		else
		{
			it++;
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

void Effect::SetEffSpeed(std::string imgName, int speed)
{
	for (auto itr : effList)
	{
		if (itr.GHandle == imgName)
		{
			itr.animSpeed = speed;
		}
	}
}

Effect::Effect()
{
}


Effect::~Effect()
{
}
