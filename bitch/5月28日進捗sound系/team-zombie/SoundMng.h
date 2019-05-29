#pragma once
#include <map>
#include <memory>
#include <vector>
#include "VECTOR2.h"


#define Sound_ID(X)(SoundMng::GetInstance().GetID(X))
#define lpSoundCtl SoundMng::GetInstance()

struct soundData
{
	int SHandle;
	int timer;
	bool active;
};


class SoundMng
{
public:

	static SoundMng &GetInstance()
	{
		return *s_Instance;
	}
	void AddSoundList(std::string s_name);
	void SoundPlay(void);

	void SoundDel(std::string s_name);

	const int & GetID(std::string s_name);
private:
	SoundMng();
	~SoundMng();
	struct SoundMngDeleter
	{
		void operator()(SoundMng* soundmng)const
		{
			delete soundmng;
		}		
	};
	static std::unique_ptr<SoundMng, SoundMngDeleter> s_Instance;
	std::map<std::string, int> SoundMap;
	std::vector<int> soundList;
	
};

