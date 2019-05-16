#include "EnemyAI.h"
#include "Enemy.h"
#include "MapCtl.h"
#include "DxLib.h"
#include "Obj.h"

bool EnemyAI::Dijkstra(const VECTOR2& start, const VECTOR2& goal)
{
	VECTOR2 mapSize = VECTOR2(lpMapCtl.GetGameAreaSize().x / lpMapCtl.GetChipSize().x, lpMapCtl.GetGameAreaSize().y / lpMapCtl.GetChipSize().y);
	VECTOR2 goalNum = VECTOR2(goal.x / lpMapCtl.GetChipSize().x, goal.y / lpMapCtl.GetChipSize().y);
	VECTOR2 startNum = VECTOR2(start.x / lpMapCtl.GetChipSize().x, start.y / lpMapCtl.GetChipSize().y);
	dist.clear();
	dist.resize(mapSize.y);
	for (int y = 0; y < dist.size(); ++y)
	{
		dist[y].resize(mapSize.x);
	}

	for (int y = 0; y < mapSize.y; ++y)
	{
		for (int x = 0; x < mapSize.x; ++x)
		{
			dist[y][x] = Node(VECTOR2(x, y), INF);
		}
	}
	//ゴールノードの設定
	dist[goalNum.y][goalNum.x].cost = 0;
	dist[goalNum.y][goalNum.x].flag = true;

	scanList.clear();
	scanList.push_back(dist[goalNum.y][goalNum.x]);
	scanListNext.clear();

	while (!dist[startNum.y][startNum.x].flag) {

		for (auto list = scanList.begin(); list != scanList.end(); ++list)
		{
			for (auto itr = shortestPathMap[(*list).pos.y][(*list).pos.x].begin(); itr != shortestPathMap[(*list).pos.y][(*list).pos.x].end(); ++itr)
			{
				
				if ((!dist[(*itr).to.y][(*itr).to.x].flag) && (dist[(*itr).to.y][(*itr).to.x].cost > dist[(*list).pos.y][(*list).pos.x].cost + (*itr).cost))
				{
					dist[(*itr).to.y][(*itr).to.x].cost = min(dist[(*itr).to.y][(*itr).to.x].cost, dist[(*list).pos.y][(*list).pos.x].cost + (*itr).cost);
					scanListNext.push_back(dist[(*itr).to.y][(*itr).to.x]);
				}
			}
			dist[(*list).pos.y][(*list).pos.x].flag = true;
		}
		scanList.clear();
		scanList.resize(scanListNext.size());
		scanList = scanListNext;	
		scanListNext.clear();

		//プレイヤーがもし移動不可位置に移動してしまったとき
		if (scanList.empty()) break;
	}

	return true;

}

bool EnemyAI::SetTarget(std::weak_ptr<Obj> player)
{
	if (player.expired()) return false;

	this->player = player;

	return true;
}

void EnemyAI::CreateMove(Enemy &enemy)
{
	//ダイクストラ経路探索法
	Dijkstra(enemy.GetPos(), player.lock()->GetPos());
	/*if (count == 100)
	{
		
		count = 0;
	}
	
	count++;*/
	//VECTOR2 tmpPos;
	//if (!player.expired())
	//{
	//	tmpPos = player.lock()->GetPos();
	//}

	////床の確認処理
	//if (lpMapCtl.CheckFloor(enemy.GetPos() + VECTOR2(0,50))) 
	//{
	//	if (lpMapCtl.GetChipType(enemy.GetPos() + VECTOR2(72/*エネミーのサイズ*/, 84 - lpMapCtl.GetChipSize().y)) == CHIP_TYPE::CHIP_BLOCK)
	//	{
	//		enemy.ChangeState(STATE::JUMP);
	//	}
	//	else
	//	{
	//		enemy.ChangeState(STATE::RUN);
	//	}
	//}
	////前方下に床があるかどうか
	//else if (//(lpMapCtl.GetChipType(enemy.GetPos() + VECTOR2(0, 84/*エネミーのサイズ*/)) == CHIP_TYPE::CHIP_BLANK) && 
	//	(lpMapCtl.GetChipType(enemy.GetPos() + VECTOR2(72/*エネミーのサイズ*/, 84)) == CHIP_TYPE::CHIP_BLANK) && (enemy.GetState() != STATE::JUMP))
	//{
	//	//下に床があるのかどうか　
	//	for (int i = enemy.GetPos().y / 32 + 1; i < CHIP_CNT_Y; ++i)
	//	{
	//		if ((lpMapCtl.GetChipType(VECTOR2(enemy.GetPos().x, i * lpMapCtl.GetChipSize().y)) != CHIP_TYPE::CHIP_BLANK)
	//			 && (tmpPos.y > enemy.GetPos().y))
	//		{
	//			//前方の下に床がなくてその下に床があるとき
	//			enemy.ChangeState(STATE::FDOWN);
	//			break;
	//		}
	//		else if (i == CHIP_CNT_Y - 1)
	//		{
	//			//前方の下に床がなく空白の状態のとき
	//			enemy.ChangeState(STATE::JUMP);
	//		}
	//	}
	//}
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
			SetMapListPtr(VECTOR2(x, y));
		}
	}

}

void EnemyAI::SetMapListPtr(const VECTOR2 &chipPos)
{
	for (int y = -1; y < 2; ++y)
	{
		for (int x = -1; x < 2; ++x)
		{
			if (((int)(chipPos.x + x) < 0) || ((int)(chipPos.x + x) >= lpMapCtl.GetGameAreaSize().x / lpMapCtl.GetChipSize().x)) continue;
			if (((int)(chipPos.y + y) < 0) || ((int)(chipPos.y + y) >= lpMapCtl.GetGameAreaSize().y / lpMapCtl.GetChipSize().y)) continue;
			if ((x == 0) && (y == 0)) continue;
			if (lpMapCtl.GetChipType(VECTOR2(chipPos.x * lpMapCtl.GetChipSize().x, chipPos.y * lpMapCtl.GetChipSize().y)) == CHIP_FLOOR) continue;
			if (lpMapCtl.GetChipType(VECTOR2(chipPos.x * lpMapCtl.GetChipSize().x, chipPos.y * lpMapCtl.GetChipSize().y)) == CHIP_BLOCK) continue;
			if (lpMapCtl.GetChipType(VECTOR2(chipPos.x * lpMapCtl.GetChipSize().x, chipPos.y * lpMapCtl.GetChipSize().y)) == CHIP_FLOORDOWN) continue;
			if (lpMapCtl.GetChipType(VECTOR2((chipPos.x + x) * lpMapCtl.GetChipSize().x, (chipPos.y + y) * lpMapCtl.GetChipSize().y)) != CHIP_BLOCK)
			{
				shortestPathMap[chipPos.y][chipPos.x].push_back(Edge(VECTOR2(chipPos.x + x, chipPos.y + y), 1));
			}
		}
	}
}

void EnemyAI::Draw(void)
{
	VECTOR2 mapSize = VECTOR2(lpMapCtl.GetGameAreaSize().x / lpMapCtl.GetChipSize().x, lpMapCtl.GetGameAreaSize().y / lpMapCtl.GetChipSize().y);
	SetFontSize(16);
	for (int y = 0; y < mapSize.y; ++y)
	{
		for (int x = 0; x < mapSize.x; ++x)
		{
			//int size = shortestPathMap[y][x].size();
			int size = dist[y][x].cost;
			if (size != INF) {
			DrawFormatString(x * 32 /*- player.lock()->GetPos().x*/, y * 32, 0x000000, "%d", size);
			}
		}
	}
}

EnemyAI::EnemyAI()
{
}


EnemyAI::~EnemyAI()
{
	shortestPathMap.clear();
	dist.clear();
}
