#pragma once

#include "VECTOR2.h"
#include <map>
#include <vector>

using VEC_INT = std::vector<int>;

#define IMAGE_ID(X) (ImageMng::GetInstance()->GetID(X))

class ImageMng
{
public:
	static void Create();
	static void Destroy();
	static ImageMng *GetInstance()
	{
		Create();
		return s_Instance;
	}
	const VEC_INT& GetID(std::string f_name);
	const VEC_INT& GetID(std::string f_name, VECTOR2 divSize, VECTOR2 divCnt, VECTOR2 chipOffset);

private:
	ImageMng();
	~ImageMng();
	static ImageMng *s_Instance;
	//std::map<ｷｰの型, 連想配列の型> 連想配列名;
	std::map<std::string, VEC_INT> imgMap;
};

