#include"DxLib.h"
#include "ImageMng.h"

ImageMng *ImageMng::s_Instance = nullptr;

void ImageMng::Create()
{
	if (s_Instance == nullptr)
	{
		s_Instance = new ImageMng();
	}
}

void ImageMng::Destroy()
{
	if (s_Instance)
	{
		delete s_Instance;
	}
	s_Instance = nullptr;
}

const VEC_INT & ImageMng::GetID(std::string f_name)
{
	if (imgMap.find(f_name) == imgMap.end())
	{
		imgMap[f_name].resize(1);
		imgMap[f_name][0] = LoadGraph(f_name.c_str());
	}

	return imgMap[f_name];
}

const VEC_INT & ImageMng::GetID(std::string f_name, VECTOR2 divSize, VECTOR2 divCnt, VECTOR2 chipOffset)
{
	if (imgMap.find(f_name) == imgMap.end())
	{
		imgMap[f_name].resize(divCnt.x * divCnt.y);
		LoadDivGraph(f_name.c_str(), divCnt.x * divCnt.y, divCnt.x, divCnt.y, divSize.x, divSize.y, &imgMap[f_name][0], true);
	}

	return imgMap[f_name];
}

ImageMng::ImageMng()
{
}


ImageMng::~ImageMng()
{
}
