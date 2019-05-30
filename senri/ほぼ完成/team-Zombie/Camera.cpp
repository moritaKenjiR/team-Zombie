#include "DxLib.h"
#include "GameTask.h"
#include "MapCtl.h"
#include "ImageMng.h"
#include "Camera.h"

Camera::Camera()
{
	pos = { 0,0 };
}

Camera::~Camera()
{
}

VECTOR2 &Camera::GetPos(void)
{
	return pos;
}

bool Camera::SetPos(VECTOR2 pos)
{
	this->pos = pos;
	return true;
}

bool Camera::SetPos(float x, float y)
{
	pos = { x ,y };
	return true;
}

bool Camera::SetTarget(std::weak_ptr<Obj> target)
{
	if (target.expired())
	{
		return false;
	}
	this->target = target;
	VECTOR2 tmpPos = (lpMapCtl.GetViewAreaSize() / 2);
	antiMoveRect.left = tmpPos.x;
	antiMoveRect.top = tmpPos.y;
	
	tmpPos = (lpMapCtl.GetGameAreaSize() - (lpMapCtl.GetViewAreaSize()/2));
	antiMoveRect.right = tmpPos.x;
	antiMoveRect.bottom = tmpPos.y;

	if (antiMoveRect.top > antiMoveRect.bottom)
	{
		antiMoveRect.top = ((lpMapCtl.GetGameAreaSize().y / 2));
		antiMoveRect.bottom = antiMoveRect.top;
	}
	pos.x = target.lock()->GetPos().x;
	pos.y = antiMoveRect.top;

	return true;
}

bool Camera::Update(void)
{
	if (target.expired())
	{
		return false;
	}
	pos.y = lpMapCtl.GetViewAreaSize().y /2;
	VECTOR2 tmpPos = target.lock()->GetPos();
	if (tmpPos.x +300 >= antiMoveRect.left 
		&& tmpPos.x  + 300 < antiMoveRect.right)
	{
			pos.x = tmpPos.x +300;
		
	}
	else if (pos.x <= lpMapCtl.GetViewAreaSize().x /2)
	{
		pos.x = lpMapCtl.GetViewAreaSize().x / 2;
	}
	else if (pos.x > lpMapCtl.GetGameAreaSize().x - lpMapCtl.GetViewAreaSize().x / 2)
	{
		pos.x = lpMapCtl.GetGameAreaSize().x - lpMapCtl.GetViewAreaSize().x / 2;
	}
	//if (tmpPos.y >= antiMoveRect.top
	//	&& tmpPos.y < antiMoveRect.bottom)
	//{
	//	//pos.y = tmpPos.y;
	//} 
	return true;
}

void Camera::Draw(void)
{
	/*for (int x = 0; x < VIEW_CHIP_CNT_X; x++)
	{
		DrawLine(pos.x + x * 32, 0, pos.x + x * 32, lpMapCtl.GetViewAreaSize().y, 0xffffff);
	}
	for (int y = 0; y < VIEW_CHIP_CNT_X; y++)
	{
		DrawLine(0, y * 32, lpMapCtl.GetViewAreaSize().x, y * 32, 0xffffff);
	}*/
	//VECTOR2 drawOffset = GameTask::GetInstance().GetGameAreaOffset();
	//DrawGraph(pos.x+ drawOffset.x,pos.y + drawOffset.y, IMAGE_ID("image/tileset.png")[7], true);
}
