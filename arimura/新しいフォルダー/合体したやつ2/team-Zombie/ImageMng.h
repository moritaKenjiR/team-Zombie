#pragma once
#include <map>
#include <memory>
#include <vector>
#include "VECTOR2.h"

using VEC_INT = std::vector<int>;

#define IMAGE_ID(X) (ImageMng::GetInstance().GetID(X))

class ImageMng
{
public:
	static ImageMng &GetInstance();
	
	const VEC_INT& GetID(std::string f_name);
	const VEC_INT& GetID(std::string f_name, VECTOR2 divSize, VECTOR2 divCnt, VECTOR2 chipOffset);
private:
	ImageMng();
	~ImageMng();
	struct ImageMngDeleter
	{
		void operator()(ImageMng* imagemng) const
		{
			delete imagemng;
		}
	};
	static std::unique_ptr<ImageMng, ImageMngDeleter> s_Instance;
	std::map<std::string, VEC_INT> imgMap;
};
