#include "SoundMng.h"


//std::unique_ptr<SoundMng, SoundMng::SoundMngDeleter>SoundMng::s_Instance(new SoundMng());


void SoundMng::AddSoundList(std::string s_name,SoundType type)
{
	//push_back�̒��g��SHandle�����������ē���Ă�
	soundList.push_back(soundData{ GetID(s_name),true,type});
}

SoundMng&
SoundMng::GetInstance() {
	static SoundMng instance;
	return instance;
}


void SoundMng::SoundPlay(void)
{
	for (auto itr = soundList.begin(); itr != soundList.end(); ++itr)
	{
		if ((*itr).Soundflag == true)
		{
			PlaySoundMem((*itr).SHandle, (*itr).type , true);
			(*itr).Soundflag = false;
		}
	}
	/*for (auto itr : soundList)
	{
		if (itr.Soundflag == true)
		{
			PlaySoundMem(itr.SHandle, DX_PLAYTYPE_BACK, false);
			itr.Soundflag = false;
		}
	}*/

	std::vector<soundData> ::const_iterator it = soundList.begin();
	while (it != soundList.end())
	{
		//soundData sound = (*it);
		if (!CheckSoundMem((*it).SHandle))
		{
			it = soundList.erase(it);
		}
		else
		{
			it++;
		}
	}
 }
void SoundMng::SoundDel()
{
	
	//soundList.clear();
	std::vector<soundData> ::const_iterator it = soundList.begin();
	while (it != soundList.end())
	{
		StopSoundMem((*it).SHandle);
		it = soundList.erase(it);
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
	
}


SoundMng::~SoundMng()
{
}
