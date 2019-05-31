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

bool EnemyAI::Dijkstra(const VECTOR2& start, const VECTOR2& goal, const std::vector<std::vector<Edge>>& map)
{
	//スタートとゴールのマップチップが移動可能範囲外チップにいるとき抜ける
	if (!((Range(lpMapCtl.GetChipType(start), CHIP_GRASS1, CHIP_GRASS3) || lpMapCtl.GetChipType(start) == CHIP_BLANK)
		&& ((Range(lpMapCtl.GetChipType(goal), CHIP_GRASS1, CHIP_GRASS3) || lpMapCtl.GetChipType(goal) == CHIP_BLANK)))) return false;

	VECTOR2 goalNum = VECTOR2(goal.x / lpMapCtl.GetChipSize().x, goal.y / lpMapCtl.GetChipSize().y);
	VECTOR2 startNum = VECTOR2(start.x / lpMapCtl.GetChipSize().x, start.y / lpMapCtl.GetChipSize().y);


	//配列外参照防止処置
	if (!Range(goalNum.x,0,mapSize.x - 1) || (!Range(goalNum.y, 0 ,mapSize.y - 1))) return false;
	if (!Range(startNum.x, 0, mapSize.x - 1) || (!Range(startNum.y, 0, mapSize.y - 1))) return false;

	//最短距離のデータが入る配列の初期化
	dist.clear();
	dist.shrink_to_fit();
	dist.resize(mapSize.x * mapSize.y);
	for (int y = 0; y < mapSize.y; ++y) {
		for (int x = 0; x < mapSize.x; ++x) {
			dist[x + y * mapSize.x] = Node(VECTOR2(x, y), INF);
		}
	}

	//優先度付きキューの生成
	std::priority_queue<Node, std::vector<Node>, std::less<std::vector<Node>::value_type>> q;

	//ゴールノードの設定
	dist[(int)goalNum.x + (int)goalNum.y * (int)mapSize.x].cost = 0;

	//ゴールノードをキューに入れる
	q.push(dist[(int)goalNum.x + (int)goalNum.y * (int)mapSize.x]);

	while (!dist[(int)startNum.x + (int)startNum.y * (int)mapSize.x].flag)
	{
		//キューの中身が空になったら抜ける（例外時の処理）
		if (q.empty()) {
			return false;
		}

		//探索用ノードの初期化
		std::tie(node) = (Node&)q.top();

		//探索ノードの排除
		q.pop();

		//探索ノードが探索済みならば抜ける
		if (dist[(int)node.pos.x + (int)node.pos.y * (int)mapSize.x].flag)continue;

		//探索ノードを探索済みに変更する
		dist[(int)node.pos.x + (int)node.pos.y * (int)mapSize.x].flag = true;

		//探索ノードから延びているエッジの先のノードの値を書き換えてキューに入れる
		for (auto edge : map/*shortestPathMap*/[(int)node.pos.x + (int)node.pos.y *(int)mapSize.x])
		{
			//探索ノード＋エッジの値が次の探索ノードの値より大きければ抜ける
			if (dist[edge.to].cost <= edge.cost + dist[(int)node.pos.x + (int)node.pos.y * (int)mapSize.x].cost) continue;

			//次の探索ノードの値を書き換えてキューに入れる
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
	
	if (enemy.GetState() == STATE::JUMP || enemy.GetState() == STATE::UPJUMP) {
		state = STATE::RUN;
		return state;
	}

	if (dist[(chipPos.x ) + (chipPos.y - 1) * mapSize.x].cost <= dist[(chipPos.x + 1) + (chipPos.y - 1) * mapSize.x].cost)
	{
        state = STATE::UPJUMP;
	}
	else if (dist[(chipPos.x + 1) + (chipPos.y - 1) * mapSize.x].cost < dist[(chipPos.x + 1) + chipPos.y * mapSize.x].cost)
	{
		state = STATE::JUMP;
	}
	else if (dist[(chipPos.x + 1) + chipPos.y * mapSize.x].cost <= dist[(chipPos.x + 1) + (chipPos.y + 1) * mapSize.x].cost)
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
	extMapChip = VECTOR2((int)((enemy.GetPos().x + lpMapCtl.GetChipSize().x * 2) / lpMapCtl.GetChipSize().x), (int)((enemy.GetPos().y + lpMapCtl.GetChipSize().y * 3) / lpMapCtl.GetChipSize().y));
	if (dist.size() == 0)
	{
		Dijkstra(VECTOR2(enemy.GetPos().x + lpMapCtl.GetChipSize().x * 2, enemy.GetPos().y + lpMapCtl.GetChipSize().y * 3), VECTOR2(VECTOR2(player.lock()->GetPos().x + 32, player.lock()->GetPos().y + 32)), shortestPathMapL);
	}

	if (lpMapCtl.CheckFloor(enemy.GetPos() + VECTOR2(lpMapCtl.GetChipSize().x, lpMapCtl.GetChipSize().y * 2)))
	{
		//プレイヤーを到達地点としたときの経路探索
		if (Range(player.lock()->GetPos().x, enemy.GetPos().x, enemy.GetPos().x + lpMapCtl.GetChipSize().x * searchChipSize))		//右への索敵範囲内にプレイヤーがいるかどうか
		{
			Dijkstra(VECTOR2(enemy.GetPos().x + lpMapCtl.GetChipSize().x * 2, enemy.GetPos().y + lpMapCtl.GetChipSize().y * 3), VECTOR2(VECTOR2(player.lock()->GetPos().x + 32, player.lock()->GetPos().y + 32)), shortestPathMapL);
		}
		else if (Range(player.lock()->GetPos().x, enemy.GetPos().x, enemy.GetPos().x - lpMapCtl.GetChipSize().x * searchChipSize))	//左の索敵範囲内にプレイヤーがいるかどうか
		{
			Dijkstra(VECTOR2(enemy.GetPos().x + lpMapCtl.GetChipSize().x * 2, enemy.GetPos().y + lpMapCtl.GetChipSize().y * 3), VECTOR2(VECTOR2(player.lock()->GetPos().x + 32, player.lock()->GetPos().y + 32)), shortestPathMapR);
		}
		else
		{
			bool checkFlag = false;
			for (int x = searchChipSize; x >= 0; --x)
			{
				for (int y = mapSize.y - 1; y >= 0; --y)
				{
					if ((checkFlag == true)
						&& ((Range(lpMapCtl.GetChipType(VECTOR2(enemy.GetPos().x + lpMapCtl.GetChipSize().x * x, y * lpMapCtl.GetChipSize().y)), CHIP_GRASS1, CHIP_GRASS3)
							|| lpMapCtl.GetChipType(VECTOR2(enemy.GetPos().x + lpMapCtl.GetChipSize().x * x, y * lpMapCtl.GetChipSize().y)) == CHIP_BLANK)))
					{
						Dijkstra(VECTOR2(enemy.GetPos().x + lpMapCtl.GetChipSize().x * 2, enemy.GetPos().y + lpMapCtl.GetChipSize().y * 3), VECTOR2(enemy.GetPos().x + lpMapCtl.GetChipSize().x * x, y * lpMapCtl.GetChipSize().y), shortestPathMapL);
						targetP = VECTOR2(enemy.GetPos().x + lpMapCtl.GetChipSize().x * x, y * lpMapCtl.GetChipSize().y);
						goto END;
					}
					if (!Range(lpMapCtl.GetChipType(VECTOR2(enemy.GetPos().x + lpMapCtl.GetChipSize().x * x, y * lpMapCtl.GetChipSize().y)), CHIP_BLOCK1, CHIP_BLOCK8)
						&& !Range(lpMapCtl.GetChipType(VECTOR2(enemy.GetPos().x + lpMapCtl.GetChipSize().x * x, y * lpMapCtl.GetChipSize().y)), CHIP_FLOOR1, CHIP_FLOOR8))
					{
						checkFlag = false;
						continue;
					}

					checkFlag = true;
				}
			}
		}

	}
	END:

	//NormalizeList(dist, INF);
	groundflag = false;
		//エネミーの行動決定処理（空中にいるときは行動を変更しない）
	if (lpMapCtl.CheckFloor(enemy.GetPos() + VECTOR2(lpMapCtl.GetChipSize().x, lpMapCtl.GetChipSize().y * 2)))
	{
		groundflag = true;
		for (int y = extMapChip.y - 1; y <= extMapChip.y + 1; ++y)
		{
			for (int x = extMapChip.x; x <= extMapChip.x + 1; ++x)
			{
				dist[x + y * mapSize.x].cost += topograMap[x + y * mapSize.x].first;
			}
		}
		enemy.ChangeState(CheckDist(extMapChip, enemy));
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
			CHIP_TYPE chipType = lpMapCtl.GetChipType(VECTOR2(x * lpMapCtl.GetChipSize().x, y * lpMapCtl.GetChipSize().y));
			if (chipType >= CHIP_FLOOR1 && chipType <= CHIP_FLOOR8) continue;
			if (chipType >= CHIP_BLOCK1 && chipType <= CHIP_BLOCK8) continue;

			if ((y == mapSize.y - 1) && (chipType == CHIP_BLANK)) {
				topograMap[x + y * mapSize.x].first = 1.0f;
				topograMap[x + y * mapSize.x].second = true;
				continue;
			}
			else if (chipType == CHIP_FIRE) {
				topograMap[x + y * mapSize.x].first = 1.0f;
				topograMap[x + y * mapSize.x].second = true;
				continue;
			}
			else if ((Range(lpMapCtl.GetChipType(VECTOR2((x + 1) * lpMapCtl.GetChipSize().x, y * lpMapCtl.GetChipSize().y)), CHIP_FLOOR1, CHIP_FLOOR8) 
				|| Range(lpMapCtl.GetChipType(VECTOR2((x + 1) * lpMapCtl.GetChipSize().x, y * lpMapCtl.GetChipSize().y)), CHIP_BLOCK1, CHIP_BLOCK8))
				&& (Range(lpMapCtl.GetChipType(VECTOR2(x * lpMapCtl.GetChipSize().x, (y + 1) * lpMapCtl.GetChipSize().y)), CHIP_FLOOR1, CHIP_FLOOR8)
				|| Range(lpMapCtl.GetChipType(VECTOR2(x * lpMapCtl.GetChipSize().x, (y + 1) * lpMapCtl.GetChipSize().y)), CHIP_BLOCK1, CHIP_BLOCK8))) {
				topograMap[x + y * mapSize.x].first = 1.0f;
				topograMap[x + y * mapSize.x].second = true;
				continue;
			}
			else{
				topograMap[x + y * mapSize.x].first = 1.0f;
			}

			if (y >= mapSize.y - 1) continue;

			if (topograMap[x + (y + 1) * mapSize.x].second == true) {
				topograMap[x + y * mapSize.x].first = topograMap[x + (y + 1) * mapSize.x].first * 0.8f;
				topograMap[x + y * mapSize.x].second = true;
			}
			else if(topograMap[x + (y + 1) * mapSize.x].first != 0.0f){
				topograMap[x + y * mapSize.x].first = topograMap[x + (y + 1) * mapSize.x].first * 1.2f;
			}
		}
	}

	return 0;
}

void EnemyAI::CreateShortestMap(void)
{
	mapSize = VECTOR2(lpMapCtl.GetGameAreaSize().x / lpMapCtl.GetChipSize().x, lpMapCtl.GetGameAreaSize().y / lpMapCtl.GetChipSize().y);

	for (auto edge : shortestPathMapR)
	{
		edge.clear();
		edge.shrink_to_fit();
	}
	shortestPathMapR.clear();
	shortestPathMapR.resize(mapSize.y * mapSize.x);

	for (auto edge : shortestPathMapL)
	{
		edge.clear();
		edge.shrink_to_fit();
	}
	shortestPathMapL.clear();
	shortestPathMapL.resize(mapSize.y * mapSize.x);

	for (int y = 0; y < mapSize.y; ++y) {
		for (int x = 0; x < mapSize.x; ++x) {
			SetMapListPtr(VECTOR2(x, y));
		}
	}

}



void EnemyAI::SetMapListPtr(const VECTOR2 &chipPos)
{
	for (int y = -1; y < 2; ++y)
	{
		for (int x = 0; x < 2; ++x)
		{
			CHIP_TYPE chipType = lpMapCtl.GetChipType(VECTOR2(chipPos.x * lpMapCtl.GetChipSize().x, chipPos.y * lpMapCtl.GetChipSize().y));
			CHIP_TYPE araChipType = lpMapCtl.GetChipType(VECTOR2((chipPos.x + x) * lpMapCtl.GetChipSize().x, (chipPos.y + y) * lpMapCtl.GetChipSize().y));

			if (((int)(chipPos.x + x) < 0) || ((int)(chipPos.x + x) >= lpMapCtl.GetGameAreaSize().x / lpMapCtl.GetChipSize().x)) continue;							//配列外参照防止措置
			if (((int)(chipPos.y + y) < 0) || ((int)(chipPos.y + y) >= lpMapCtl.GetGameAreaSize().y / lpMapCtl.GetChipSize().y)) continue;							//上に同じ
			if ((x == 0) && (y == 0)) continue;																														//そいつ自身のノードにエッジを伸ばすの禁止
			if (Range(chipType,CHIP_FLOOR1,CHIP_FLOOR8)) continue;																						//そいつ自身が移動不可ブロックだった時
			if (Range(chipType, CHIP_BLOCK1, CHIP_BLOCK8)) continue;																						//そいつがエッジを伸ばすの禁止
			if (Range(araChipType, CHIP_FLOOR1, CHIP_FLOOR8)) continue;																					//移動不可ブロックにエッジを伸ばすの禁止
			if (Range(araChipType, CHIP_BLOCK1, CHIP_BLOCK8)) continue;
			if (Range(araChipType, CHIP_GRASS1, CHIP_GRASS3)) {
				shortestPathMapR[chipPos.x + chipPos.y * mapSize.x].push_back(Edge((chipPos.x + chipPos.y * mapSize.x) + (x + y * mapSize.x), 2));
				break;
			}
			shortestPathMapR[chipPos.x + chipPos.y * mapSize.x].push_back(Edge((chipPos.x + chipPos.y * mapSize.x) + (x + y * mapSize.x), 1));
		}
	}

	for (int y = -1; y < 2; ++y)
	{
		for (int x = -1; x < 1; ++x)
		{
			CHIP_TYPE chipType = lpMapCtl.GetChipType(VECTOR2(chipPos.x * lpMapCtl.GetChipSize().x, chipPos.y * lpMapCtl.GetChipSize().y));
			CHIP_TYPE araChipType = lpMapCtl.GetChipType(VECTOR2((chipPos.x + x) * lpMapCtl.GetChipSize().x, (chipPos.y + y) * lpMapCtl.GetChipSize().y));

			if (((int)(chipPos.x + x) < 0) || ((int)(chipPos.x + x) >= lpMapCtl.GetGameAreaSize().x / lpMapCtl.GetChipSize().x)) continue;							//配列外参照防止措置
			if (((int)(chipPos.y + y) < 0) || ((int)(chipPos.y + y) >= lpMapCtl.GetGameAreaSize().y / lpMapCtl.GetChipSize().y)) continue;							//上に同じ
			if ((x == 0) && (y == 0)) continue;																														//そいつ自身のノードにエッジを伸ばすの禁止
			if (Range(chipType, CHIP_FLOOR1, CHIP_FLOOR8)) continue;																						//そいつ自身が移動不可ブロックだった時
			if (Range(chipType, CHIP_BLOCK1, CHIP_BLOCK8)) continue;																						//そいつがエッジを伸ばすの禁止
			if (Range(araChipType, CHIP_FLOOR1, CHIP_FLOOR8)) continue;																					//移動不可ブロックにエッジを伸ばすの禁止
			if (Range(araChipType, CHIP_BLOCK1, CHIP_BLOCK8)) continue;
			if (Range(araChipType, CHIP_GRASS1, CHIP_GRASS3)) {
				shortestPathMapL[chipPos.x + chipPos.y * mapSize.x].push_back(Edge((chipPos.x + chipPos.y * mapSize.x) + (x + y * mapSize.x), 2));
				break;
			}
			shortestPathMapL[chipPos.x + chipPos.y * mapSize.x].push_back(Edge((chipPos.x + chipPos.y * mapSize.x) + (x + y * mapSize.x), 1));
		}
	}
}

//デバック用
void EnemyAI::Draw(Enemy& enemy)
{
	SetFontSize(32);

	if (!Range(enemy.GetPos().x, 0, lpMapCtl.GetGameAreaSize().x) || !Range(enemy.GetPos().y, 0, lpMapCtl.GetGameAreaSize().y)) return;

	int size = dist[(int)(enemy.GetPos().x / 32 + 2) + (int)(enemy.GetPos().y / 32 + 3) * mapSize.x].cost;
	if(size != INF) DrawFormatString((enemy.GetPos().x + 32 * 2) + lpMapCtl.GameDrawOffset().x, (enemy.GetPos().y + 32 * 3) + lpMapCtl.GameDrawOffset().y , 0xffffff, "%d", size);
	

	size = dist[(int)(enemy.GetPos().x / 32 + 3) + (int)(enemy.GetPos().y / 32 + 2) * mapSize.x].cost;
	if (size != INF) {
		DrawFormatString((enemy.GetPos().x + 32 * 3) + lpMapCtl.GameDrawOffset().x, (enemy.GetPos().y + 32 * 2) + lpMapCtl.GameDrawOffset().y, 0xffffff, "%d", size);
	}

	size = dist[(int)(enemy.GetPos().x / 32 + 3) + (int)(enemy.GetPos().y / 32 + 3) * mapSize.x].cost;
	if (size != INF)DrawFormatString((enemy.GetPos().x + 32 * 3) + lpMapCtl.GameDrawOffset().x, (enemy.GetPos().y + 32 * 3) + lpMapCtl.GameDrawOffset().y, 0xffffff, "%d",size);

	size = dist[(int)(enemy.GetPos().x / 32 + 3) + (int)(enemy.GetPos().y / 32 + 4) * mapSize.x].cost;
	if (size != INF) {
		DrawFormatString((enemy.GetPos().x + 32 * 3) + lpMapCtl.GameDrawOffset().x, (enemy.GetPos().y + 32 * 4) + lpMapCtl.GameDrawOffset().y, 0xffffff, "%d", size);
	}

	size = dist[(int)(enemy.GetPos().x / 32 + 2) + (int)(enemy.GetPos().y / 32 + 2) * mapSize.x].cost;
	if (size != INF) {
		DrawFormatString((enemy.GetPos().x + 32 * 2) + lpMapCtl.GameDrawOffset().x, (enemy.GetPos().y + 32 * 2) + lpMapCtl.GameDrawOffset().y, 0xffffff, "%d", size);
	}

	DrawBox(targetP.x + lpMapCtl.GameDrawOffset().x, targetP.y + lpMapCtl.GameDrawOffset().y, targetP.x + 32 + lpMapCtl.GameDrawOffset().x, targetP.y + 32 + lpMapCtl.GameDrawOffset().y, 0xffff00, true);

	if (groundflag) {
		DrawString(0, 60, "flag = true", 0x000000);
	}
	else {
		DrawString(0, 60, "flag = false", 0x000000);
	}
}

EnemyAI::EnemyAI()
{
	searchChipSize = 24;
	targetP = VECTOR2(0, 0);
}


EnemyAI::~EnemyAI()
{
	dist.clear();
	dist.shrink_to_fit();

	for (auto edge : shortestPathMapR)
	{
		edge.clear();
		edge.shrink_to_fit();
	}
	shortestPathMapR.clear();
	shortestPathMapR.shrink_to_fit();

	for (auto edge : shortestPathMapL)
	{
		edge.clear();
		edge.shrink_to_fit();
	}
	shortestPathMapL.clear();
	shortestPathMapL.shrink_to_fit();
	topograMap.clear();
	topograMap.shrink_to_fit();
}
