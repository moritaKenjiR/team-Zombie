#include "SoundMng.h"
#include <DxLib.h>


//std::unique_ptr<SoundMng, SoundMng::SoundMngDeleter>SoundMng::s_Instance(new SoundMng());


void SoundMng::AddSoundList(std::string s_name)
{
	GetID(s_name);
	soundList.push_back(SoundMap[s_name]);
}


void SoundMng::SoundPlay(void)
{
	for (auto itr : soundList)
	{
		PlaySoundMem(itr, DX_PLAYTYPE_BACK, false);
	}


	std::vector<int> ::const_iterator it = soundList.begin();
	while (it != soundList.end())
	{
		//soundData sound = (*it);
		if (!CheckSoundMem(*it))
		{
			it = soundList.erase(it);
		}
		else
		{
			it++;
		}
	}
 }
void SoundMng::SoundDel(std::string s_name)
{
	
	//soundList.clear();
	std::vector<int> ::const_iterator it = soundList.begin();
	while (it != soundList.end())
	{
		StopSoundMem(*it);
		it = soundList.erase(it);
		it++;
	}
}

const int & SoundMng::GetID(std::string s_name)
{
	if (SoundMap.find(s_name) == SoundMap.end())
	{
		SoundMap[s_name] = LoadSoundMem(s_name.c_str());
	}
	return SoundMap[s_name];
}

SoundMng::SoundMng()
{
	if (true);
}


SoundMng::~SoundMng()
{
}
