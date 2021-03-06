#include "EnemyAI.h"
#include "Enemy.h"
#include "MapCtl.h"
#include "DxLib.h"
#include "Obj.h"

//Overload the < operator.
bool operator< (const Node& node1, const Node &node2)
{
	return node1.cost > node2.cost;
}

//Overload the > operator.
bool operator> (const Node& node1, const Node &node2)
{
	return node1.cost < node2.cost;
}

bool EnemyAI::Dijkstra(const VECTOR2& start, const VECTOR2& goal)
{
	if (lpMapCtl.GetChipType(start) != CHIP_BLANK || lpMapCtl.GetChipType(goal) != CHIP_BLANK) return false;

	VECTOR2 goalNum = VECTOR2(goal.x / lpMapCtl.GetChipSize().x, goal.y / lpMapCtl.GetChipSize().y);
	VECTOR2 startNum = VECTOR2(start.x / lpMapCtl.GetChipSize().x, start.y / lpMapCtl.GetChipSize().y);

	dist.clear();
	dist.resize((lpMapCtl.GetGameAreaSize().x / lpMapCtl.GetChipSize().x) * (lpMapCtl.GetGameAreaSize().y / lpMapCtl.GetChipSize().y));

	for (int y = 0; y < mapSize.y; ++y) {
		for (int x = 0; x < mapSize.x; ++x) {
			dist[x + y * mapSize.x] = Node(VECTOR2(x, y), INF);
		}
	}
	std::priority_queue<Node, std::vector<Node>, std::less<std::vector<Node>::value_type>> q;

	////ゴールノードの設定
	dist[(int)goalNum.x + (int)goalNum.y * (int)mapSize.x].cost = 0;

	q.push(dist[(int)goalNum.x + (int)goalNum.y * (int)mapSize.x]);

	while (!dist[(int)startNum.x + (int)startNum.y * (int)mapSize.x].flag)
	{
		//探索用ノードの初期化
		std::tie(node) = (Node&)q.top();
		//探索ノードの排除
		q.pop();
		//探索ノードが探索済みならば抜ける
		if (dist[(int)node.pos.x + (int)node.pos.y * (int)mapSize.x].flag)continue;
		//探索ノードを探索済みに変更する
		dist[(int)node.pos.x + (int)node.pos.y * (int)mapSize.x].flag = true;
		for (auto edge : shortestPathMap[(int)node.pos.x + (int)node.pos.y *(int)mapSize.x])
		{
			if (dist[edge.to].cost <= edge.cost + dist[(int)node.pos.x + (int)node.pos.y * (int)mapSize.x].cost) continue;
			dist[edge.to].cost = edge.cost + dist[(int)node.pos.x + (int)node.pos.y * (int)mapSize.x].cost;
			q.push(dist[edge.to]);
		}

	}
	return true;

}

bool EnemyAI::NormalizeList(std::vector<std::vector<Node>>& list, const int &max)
{
	for (int y = 0; y < list.size(); ++y)
	{
		for (int x = 0; x < list[y].size(); ++x)
		{
			list[y][x].cost /= max;
			if (list[y][x].cost > 1)
			{
				return false;
			}
		}
	}
	return true;
}

STATE EnemyAI::CheckDist(VECTOR2 &chipPos, Enemy &enemy)
{
	STATE state;
	
	if (enemy.GetState() == STATE::JUMP) {
		state = STATE::RUN;
		return state;
	}

	if (dist[(chipPos.x + 1) + (chipPos.y - 1) * mapSize.x]/*[chipPos.y - 1][chipPos.x + 1]*/.cost < dist[(chipPos.x + 1) + chipPos.y * mapSize.x]/*[chipPos.y][chipPos.x + 1]*/.cost)
	{
		state = STATE::JUMP;
	}
	else if (dist[(chipPos.x + 1) + chipPos.y * mapSize.x]/*[chipPos.y][chipPos.x + 1]*/.cost < dist[(chipPos.x + 1) + (chipPos.y + 1) * mapSize.x]/*[chipPos.y + 1][chipPos.x + 1]*/.cost)
	{
		state = STATE::RUN;
	}
	else
	{
		state = STATE::FDOWN;
	}

	return state;
}

bool EnemyAI::SetTarget(std::weak_ptr<Obj> player)
{
	if (player.expired()) return false;

	this->player = player;

	return true;
}

void EnemyAI::CreateMove(Enemy &enemy)
{
	//現在いるマップチップ
	VECTOR2 extMapChip = VECTOR2((int)(enemy.GetPos().x / lpMapCtl.GetChipSize().x),(int)((enemy.GetPos().y + 52) / lpMapCtl.GetChipSize().y));
	//ダイクストラ経路探索法
	//Dijkstra(enemy.GetPos(), VECTOR2(player.lock()->GetPos().x, player.lock()->GetPos().y + 52));
	//Dijkstra(enemy.GetPos(), VECTOR2(lpMapCtl.GetChipSize().x * (mapSize.x - 1),lpMapCtl.GetChipSize().y * (mapSize.y - 4)));

	//一番奥を到達地点としたときの経路探索
	//if (enemy.GetPos().x <= lpMapCtl.GetChipSize().x * (mapSize.x - 1) - lpMapCtl.GetChipSize().x * searchChipSize)
	//{
	//	for (int y = mapSize.y - 1; y >= 0; --y)
	//	{
	//		if (lpMapCtl.GetChipType(VECTOR2(enemy.GetPos().x + lpMapCtl.GetChipSize().x * searchChipSize, y * lpMapCtl.GetChipSize().y)) != CHIP_BLANK) continue;
	//		Dijkstra(enemy.GetPos(), VECTOR2(enemy.GetPos().x + lpMapCtl.GetChipSize().x * searchChipSize, y * lpMapCtl.GetChipSize().y));
	//		break;
	//	}
	//}
	//else
	//{
	//	Dijkstra(enemy.GetPos(), VECTOR2(lpMapCtl.GetChipSize().x * (mapSize.x - 1), lpMapCtl.GetChipSize().y * (mapSize.y - 4)));
	//}
	//プレイヤーを到達地点としたときの経路探索
	if (enemy.GetPos().x <= player.lock()->GetPos().x - lpMapCtl.GetChipSize().x * searchChipSize)
	{
		for (int y = mapSize.y - 1; y >= 0; --y)
		{
			if (lpMapCtl.GetChipType(VECTOR2(enemy.GetPos().x + lpMapCtl.GetChipSize().x * searchChipSize, y * lpMapCtl.GetChipSize().y)) != CHIP_BLANK) continue;
			Dijkstra(enemy.GetPos(), VECTOR2(enemy.GetPos().x + lpMapCtl.GetChipSize().x * searchChipSize, y * lpMapCtl.GetChipSize().y));
			break;
		}
	}
	else
	{
		Dijkstra(enemy.GetPos(), VECTOR2(VECTOR2(player.lock()->GetPos().x, player.lock()->GetPos().y + 52)));
	}

	//NormalizeList(dist, INF);

	if (lpMapCtl.CheckFloor(enemy.GetPos() + VECTOR2(0, 52)))
	{
		for (int y = extMapChip.y - 1; y <= extMapChip.y + 1; ++y)
		{
			for (int x = extMapChip.x - 1; x <= extMapChip.x + 1; ++x)
			{
				dist[x + y * mapSize.x].cost += topograMap[x + y * mapSize.x].first;
			}
		}
		enemy.ChangeState(CheckDist(extMapChip, enemy));
	}

}

void EnemyAI::CreateShortestMap(void)
{
	for (auto edge : shortestPathMap)
	{
		edge.clear();
		edge.shrink_to_fit();
	}
	shortestPathMap.clear();
	shortestPathMap.resize(mapSize.y * mapSize.x);

	for (int y = 0; y < mapSize.y; ++y) {
		for (int x = 0; x < mapSize.x; ++x) {
			SetMapListPtr(VECTOR2(x, y));
		}
	}

}

bool EnemyAI::CreateTopograMap(void)
{
	topograMap.clear();
	topograMap.shrink_to_fit();

	topograMap.resize(mapSize.y * mapSize.x);
	for (auto map : topograMap)
	{
		map.first = 0.0f;
		map.second = false;
	}

	for (int x = 0; x < mapSize.x; ++x) {
		for (int y = mapSize.y - 1; y >= 0; --y) {
			if (lpMapCtl.GetChipType(VECTOR2(x * lpMapCtl.GetChipSize().x, y * lpMapCtl.GetChipSize().y)) != CHIP_BLANK) continue;
			if ((y == mapSize.y - 1) && (lpMapCtl.GetChipType(VECTOR2(x * lpMapCtl.GetChipSize().x, y * lpMapCtl.GetChipSize().y)) == CHIP_BLANK)) {
				topograMap[x + y * mapSize.x].first = 1.0f;
				topograMap[x + y * mapSize.x].second = true;
			}
			else if (topograMap[x + (y + 1) * mapSize.x].second == true) {
				topograMap[x + y * mapSize.x].first = topograMap[x + (y + 1) * mapSize.x].first - 0.05f;
				topograMap[x + y * mapSize.x].second = true;
			}
			else {
				topograMap[x + y * mapSize.x].first = topograMap[x + (y + 1) * mapSize.x].first + 0.05f;
			}
		}
	}

	return 0;
}

void EnemyAI::SetMapListPtr(const VECTOR2 &chipPos)
{
	for (int y = -1; y < 2; ++y)
	{
		for (int x = -1; x < 2; ++x)
		{
			if (((int)(chipPos.x + x) < 0) || ((int)(chipPos.x + x) >= lpMapCtl.GetGameAreaSize().x / lpMapCtl.GetChipSize().x)) continue;							//配列外参照防止措置
			if (((int)(chipPos.y + y) < 0) || ((int)(chipPos.y + y) >= lpMapCtl.GetGameAreaSize().y / lpMapCtl.GetChipSize().y)) continue;							//上に同じ
			if ((x == 0) && (y == 0)) continue;																														//そいつ自身のノードにエッジを伸ばすの禁止
			if (lpMapCtl.GetChipType(VECTOR2(chipPos.x * lpMapCtl.GetChipSize().x, chipPos.y * lpMapCtl.GetChipSize().y)) == CHIP_FLOOR) continue;					//そいつ自身が移動不可ブロックだった時
			if (lpMapCtl.GetChipType(VECTOR2(chipPos.x * lpMapCtl.GetChipSize().x, chipPos.y * lpMapCtl.GetChipSize().y)) == CHIP_BLOCK) continue;					//エッジを伸ばすの禁止
			if (lpMapCtl.GetChipType(VECTOR2(chipPos.x * lpMapCtl.GetChipSize().x, chipPos.y * lpMapCtl.GetChipSize().y)) == CHIP_FLOORDOWN) continue;				//**
			if ((lpMapCtl.GetChipType(VECTOR2((chipPos.x + x) * lpMapCtl.GetChipSize().x, (chipPos.y + y) * lpMapCtl.GetChipSize().y)) != CHIP_FLOOR)				//移動不可ブロックにエッジを伸ばすの禁止
				&&(lpMapCtl.GetChipType(VECTOR2((chipPos.x + x) * lpMapCtl.GetChipSize().x, (chipPos.y + y) * lpMapCtl.GetChipSize().y)) != CHIP_BLOCK)
				&&(lpMapCtl.GetChipType(VECTOR2((chipPos.x + x) * lpMapCtl.GetChipSize().x, (chipPos.y + y) * lpMapCtl.GetChipSize().y)) != CHIP_FLOORDOWN))
			{
				if(y == -1) shortestPathMap[chipPos.x + chipPos.y * mapSize.x].push_back(Edge((chipPos.x + chipPos.y * mapSize.x) + (x + y * mapSize.x), 1));
				else if (y == 0) shortestPathMap[chipPos.x + chipPos.y * mapSize.x].push_back(Edge((chipPos.x + chipPos.y * mapSize.x) + (x + y * mapSize.x), 1));
				else shortestPathMap[chipPos.x + chipPos.y * mapSize.x].push_back(Edge((chipPos.x + chipPos.y * mapSize.x) + (x + y * mapSize.x), 1));
			}
		}
	}
}

void EnemyAI::Draw(void)
{
	SetFontSize(16);
	for (int y = 0; y < mapSize.y; ++y)
	{
		for (int x = 0; x < mapSize.x; ++x)
		{
			//int size = shortestPathMap[y][x].size();
			int size = dist[x + y * mapSize.x].cost;
			if (size != INF) {
			DrawFormatString(x * 32 /*- player.lock()->GetPos().x*/, y * 32, 0x000000, "%d", size);
			}
			else
			{
				DrawString(x * 32, y * 32, "INF", 0x000000 );
			}
		}
	}
}

EnemyAI::EnemyAI()
{
	mapSize = VECTOR2(lpMapCtl.GetGameAreaSize().x / lpMapCtl.GetChipSize().x, lpMapCtl.GetGameAreaSize().y / lpMapCtl.GetChipSize().y);
	searchChipSize = 32;
}


EnemyAI::~EnemyAI()
{
	dist.clear();
	dist.shrink_to_fit();

	for (auto edge : shortestPathMap)
	{
		edge.clear();
		edge.shrink_to_fit();
	}
	shortestPathMap.clear();
	shortestPathMap.shrink_to_fit();
	topograMap.clear();
	topograMap.shrink_to_fit();
}
