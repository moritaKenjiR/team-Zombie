#pragma once
#include <memory>
#include <vector>
#include <map>
#include "VECTOR2.h"

using VEC_INT = std::vector<int>;
#define lpEffect Effect::GetInstance()

struct effectData {
	std::string GHandle;
	VECTOR2 pos;
	int animMaxCnt;
	int timer;
	int animSpeed;
	bool active;
};

class Effect
{
public:
	static Effect &GetInstance();
	void AddEffectList(std::string imgName, VECTOR2 divSize, VECTOR2 divCnt, VECTOR2 chipOffset, int animMaxCnt,int animspeed, VECTOR2 pos);
	
	void EffectDraw(void);
	void SetEffPos(std::string imgName,VECTOR2 pos);
	void SetEffSpeed(std::string imgName, int speed);
private:
	Effect();
	~Effect();
	struct effectDeleter
	{
		void operator()(Effect* effect) const
		{
			delete effect;
		}
	};
	static std::unique_ptr<Effect, effectDeleter> s_Instance;
	//std::map<std::string, effectData> effMap;
	std::vector<effectData> effList;
};

