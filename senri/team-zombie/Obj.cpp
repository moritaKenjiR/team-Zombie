#include "Obj.h"
#include "GameTask.h"
#include "ImageMng.h"
#include "DxLib.h"

Obj::Obj()
{
}

//weak_ptrを使ってObjとCamreaを連結させる
//Obj::Obj(const char(&_keyData)[256], const char(&_keyDataOld)[256], VECTOR2 chipOffset) 
//	:keyData(_keyData), keyDataOld(_keyDataOld)
//{
//	pos		= { 32,32 };
//	divSize = { 0,0 };
//	divCnt  = { 0,0 };
//	turn = false;
//}

Obj::~Obj()
{
	//delete imageID;
	//delete ﾒﾓﾘのﾌﾞﾛｯｸ開放
}

void Obj::init(std::string filename, VECTOR2 divSize, VECTOR2 divCnt, VECTOR2 chipOffset, int speed, int animSpeed, int stateAnimDiv)
{
	ImageMng::GetInstance()->GetID(filename, divSize, divCnt, chipOffset);
	imageName			= filename;
	this->divSize		= divSize;
	this->divCnt		= divCnt;
	this->chipOffset	= chipOffset;
	this->speed			= speed;
	//this->animCnt		= animCnt;
	this->animSpeed		= animSpeed;
	this->stateAnimDiv	= stateAnimDiv;
	animCnt				= 0;
	initAnim();
	//int flag;
	//imageID = new(divCnt.x * divCnt.y);
	//imageID.resize(divCnt.x * divCnt.y);	
	//resizeで確保して中に入れることができる
	//resize前にデータが残っていた場合、それらが破棄される
	//LoadDivGraph(filename.c_str(), divCnt.x * divCnt.y, divCnt.x, divCnt.y, divSize.x, divSize.y, &imageID[0]);
}

const VECTOR2 & Obj::GetPos(void)
{
	return this->pos;
}

void Obj::SetPos(const VECTOR2 & pos)
{
	this->pos = pos;
}

bool Obj::Update(void)
{
	return true;
}

void Obj::Draw(void)
{
	if (imageName.length() == 0)
	{
		return;
	}
	int id = 0;

	if (animMap.find(animName) != animMap.end())
	{
		id = animMap[animName][ANIM_START_ID] + (animCnt / animMap[animName][ANIM_WAIT]) % animMap[animName][ANIM_FLAME];
	}

	//VECTOR2 drawOffset = VECTOR2(GameTask::GetInstance().GetDrawOffset()) + VECTOR2(lpGameTask.DrawOffset());
	//DrawGraph(pos.x, pos.y, IMAGE_ID(imageName)[chipOffset.x + divCnt.x * chipOffset.y], true);
	DrawGraph(pos.x, pos.y, IMAGE_ID(imageName)[id], true);
}

void Obj::Draw(int id)
{	if (imageName.length() == 0)
	{
		return;
	}
	if (imageName.size() <= id)
	{
		return;
	}

	DrawGraph(pos.x , pos.y, IMAGE_ID(imageName)[id], true);
}

//
bool Obj::SetAnim(std::string _AnimName)
{
	if (animMap.find(_AnimName) == animMap.end())
	{
		return false;
	}
	if (Obj::animName != _AnimName)
	{
		animCnt = 0;
		Obj::animName = _AnimName;
	}
	return true;
}

bool Obj::initAnim(void)
{
	return true;
}

bool Obj::AddAnim(std::string AnimName, int id_x, int id_y, int flame, int waitCnt)
{
	animMap[AnimName][ANIM_START_ID] = id_x + id_y * divCnt.x;
	animMap[AnimName][ANIM_FLAME] = flame;
	animMap[AnimName][ANIM_WAIT] = waitCnt;

	return true;
}
//

//C++ では変数からのメンバ関数呼び出しはドット演算子、 ポインタからのメンバ関数呼び出しはアロー演算子

/*EditCarsolの継承ｸﾗｽを作る
一回押したら３６ﾄﾞｯﾄ動く、長押しは反応しない
*/