#pragma once
#include <map>
#include <memory>
#include <vector>
#include "VECTOR2.h"
#include <DxLib.h>

#define lpSoundCtl SoundMng::GetInstance()

enum SoundType
{
	back = DX_PLAYTYPE_BACK,
	loop = DX_PLAYTYPE_LOOP
};

struct soundData
{
	int SHandle;
	bool Soundflag;
	SoundType type;
};



class SoundMng
{
public:

	static SoundMng &GetInstance();
	/*static SoundMng &GetInstance()
	{
		return *s_Instance;
	}*/
	void AddSoundList(std::string s_name, SoundType type , int volume = 100);
	void SoundPlay(void);

	void SoundDel();


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
	//static std::unique_ptr<SoundMng, SoundMngDeleter> s_Instance;
	std::map<std::string, int> SoundMap;
	std::vector<soundData> soundList;

	//int count = 180;
	
};

