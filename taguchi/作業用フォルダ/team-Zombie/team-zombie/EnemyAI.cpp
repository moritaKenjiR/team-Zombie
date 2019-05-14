#include "EnemyAI.h"
#include "Enemy.h"
#include "MapCtl.h"
#include "Obj.h"

void EnemyAI::Dijkstra(const VECTOR2& start, const VECTOR2& goal)
{

}

bool EnemyAI::SetTarget(std::weak_ptr<Obj> player)
{
	if (player.expired()) return false;

	this->player = player;

	return true;
}

void EnemyAI::CreateMove(Enemy &enemy)
{
	VECTOR2 tmpPos;
	if (!player.expired())
	{
		tmpPos = player.lock()->GetPos();
	}

	//床の確認処理
	if (lpMapCtl.CheckFloor(enemy.GetPos() + VECTOR2(0,50))) 
	{
		if (lpMapCtl.GetChipType(enemy.GetPos() + VECTOR2(72/*エネミーのサイズ*/, 84 - lpMapCtl.GetChipSize().y)) == CHIP_TYPE::CHIP_BLOCK)
		{
			enemy.ChangeState(STATE::JUMP);
		}
		else
		{
			enemy.ChangeState(STATE::RUN);
		}
	}
	//前方下に床があるかどうか
	else if (//(lpMapCtl.GetChipType(enemy.GetPos() + VECTOR2(0, 84/*エネミーのサイズ*/)) == CHIP_TYPE::CHIP_BLANK) && 
		(lpMapCtl.GetChipType(enemy.GetPos() + VECTOR2(72/*エネミーのサイズ*/, 84)) == CHIP_TYPE::CHIP_BLANK) && (enemy.GetState() != STATE::JUMP))
	{
		//下に床があるのかどうか　
		for (int i = enemy.GetPos().y / 32 + 1; i < CHIP_CNT_Y; ++i)
		{
			if ((lpMapCtl.GetChipType(VECTOR2(enemy.GetPos().x, i * lpMapCtl.GetChipSize().y)) != CHIP_TYPE::CHIP_BLANK)
				 && (tmpPos.y > enemy.GetPos().y))
			{
				//前方の下に床がなくてその下に床があるとき
				enemy.ChangeState(STATE::FDOWN);
				break;
			}
			else if (i == CHIP_CNT_Y - 1)
			{
				//前方の下に床がなく空白の状態のとき
				enemy.ChangeState(STATE::JUMP);
			}
		}
	}
}

void EnemyAI::CreateShotestMap(void)
{
	VECTOR2 mapSize = VECTOR2(lpMapCtl.GetGameAreaSize().x / lpMapCtl.GetChipSize().x, lpMapCtl.GetGameAreaSize().y / lpMapCtl.GetChipSize().y);
	VECTOR2 mapPos;

	shortestPathMap.clear();
	shortestPathMap.resize(mapSize.y);
	for (int y = 0; y < shortestPathMap.size(); ++y)
	{
		shortestPathMap[y].resize(mapSize.x);
	}

	for (int y = 0; y < mapSize.y; ++y)
	{
		for (int x = 0; x < mapSize.x; ++x)
		{
			//mapPos = VECTOR2(lpMapCtl.GetChipSize().x * x, lpMapCtl.GetChipSize().y * y);
			if (SetMapListPtr(VECTOR2(x, y))) break;
		}
	}

}

bool EnemyAI::SetMapListPtr(const VECTOR2 &chipPos)
{
	//停止条件::マップチップから伸びるすべてのEdgeの走査
}

EnemyAI::EnemyAI()
{
}


EnemyAI::~EnemyAI()
{
}
