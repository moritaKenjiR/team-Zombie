#include "DxLib.h"
#include "ImageMng.h"

std::unique_ptr<ImageMng, ImageMng::ImageMngDeleter> ImageMng::s_Instance(new ImageMng());

const VEC_INT & ImageMng::GetID(std::string f_name)
{
	if (imgMap.find(f_name) == imgMap.end())	
	{
		//f_nameの画像IDを読み込み、ﾊﾝﾄﾞﾙを作成する
		imgMap[f_name].resize(1);
		imgMap[f_name][0] = LoadGraph(f_name.c_str());
	}
	return imgMap[f_name];		//f_nameの画像IDのﾊﾝﾄﾞﾙ
}

const VEC_INT & ImageMng::GetID(std::string f_name, VECTOR2 divSize, VECTOR2 divCnt, VECTOR2 chipOffset)
{
	//f_nameの画像IDのﾊﾝﾄﾞﾙがあるかどうか
	if (imgMap.find(f_name) == imgMap.end())	
	{
		//f_nameの画像IDを読み込み、ﾊﾝﾄﾞﾙを作成する
		imgMap[f_name].resize(divCnt.x * divCnt.y);
		LoadDivGraph(f_name.c_str(), divCnt.x * divCnt.y, divCnt.x, divCnt.y, divSize.x, divSize.y, &imgMap[f_name][0], true);
	}

	return imgMap[f_name];	//f_nameの画像IDのﾊﾝﾄﾞﾙ
}

ImageMng::ImageMng()
{
}


ImageMng::~ImageMng()
{
}
