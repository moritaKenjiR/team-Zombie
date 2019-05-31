#include "Obj.h"
#include "GameTask.h"
#include "ImageMng.h"
#include "DxLib.h"
#include"MapCtl.h"

Obj::Obj()
{
}

Obj::~Obj()
{
}

void Obj::init(std::string filename, VECTOR2 divSize, VECTOR2 divCnt, VECTOR2 chipOffset, int speed, int animSpeed, int stateAnimDiv)
{
	ImageMng::GetInstance().GetID(filename, divSize, divCnt, chipOffset);
	imageName			= filename;
	this->divSize		= divSize;
	this->divCnt		= divCnt;
	this->chipOffset	= chipOffset;
	this->speed			= speed;
	this->animSpeed		= animSpeed;
	this->stateAnimDiv	= stateAnimDiv;
	animCnt				= 0;
	initAnim();
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

	VECTOR2 drawOffset = lpMapCtl.GameDrawOffset();
	DrawGraph(pos.x + drawOffset.x, pos.y + drawOffset.y, IMAGE_ID(imageName)[id], true);

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