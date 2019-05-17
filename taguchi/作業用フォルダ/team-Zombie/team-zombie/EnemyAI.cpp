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
	//�S�[���m�[�h�̐ݒ�
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
					dist[(*itr).to.y][(*itr).to.x].cost =  dist[(*list).pos.y][(*list).pos.x].cost + (*itr).cost;
					//dist[(*itr).to.y][(*itr).to.x].cost = min(dist[(*itr).to.y][(*itr).to.x].cost, dist[(*list).pos.y][(*list).pos.x].cost + (*itr).cost);
					if (scanListNext.empty())
					{
						scanListNext.push_back(dist[(*itr).to.y][(*itr).to.x]);
					}
					else if (scanListNext.front().cost == dist[(*itr).to.y][(*itr).to.x].cost)
					{
						scanListNext.push_back(dist[(*itr).to.y][(*itr).to.x]);
					}
					else
					{
						scanListNext.clear();
						scanListNext.push_back(dist[(*itr).to.y][(*itr).to.x]);
					}
				}
			}
			//�����m�[�h�̃R�X�g�l���菈��
			dist[(*list).pos.y][(*list).pos.x].flag = true;
			//dist[(*list).pos.y][(*list).pos.x].cost = dist[(*list).pos.y][(*list).pos.x].cost / INF;
		}
		scanList.clear();
		scanList.resize(scanListNext.size());
		scanList = scanListNext;
		scanList.shrink_to_fit();
		scanListNext.clear();
		scanListNext.shrink_to_fit();

		//�v���C���[�������ړ��s�ʒu�Ɉړ����Ă��܂����Ƃ�
		if (scanList.empty())
		{
			return false;
		}
	}
	
	int size = scanList.size();
	int size2 = scanListNext.size();
	int capa = scanList.capacity();
	int capa2 = scanListNext.capacity();
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

STATE EnemyAI::CheckDist(VECTOR2 &chipPos)
{
	STATE state;
	if (dist[chipPos.y - 1][chipPos.x + 1].cost < dist[chipPos.y][chipPos.x + 1].cost)
	{
		state = STATE::JUMP;
	}
	else if (dist[chipPos.y][chipPos.x + 1].cost < dist[chipPos.y + 1][chipPos.x + 1].cost)
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
	//���݂���}�b�v�`�b�v
	VECTOR2 extMapChip = VECTOR2(enemy.GetPos().x / lpMapCtl.GetChipSize().x, (enemy.GetPos().y + 52) / lpMapCtl.GetChipSize().y);
	//�_�C�N�X�g���o�H�T���@
	if (!Dijkstra(enemy.GetPos(), VECTOR2(player.lock()->GetPos().x, player.lock()->GetPos().y + 52))) enemy.ChangeState(STATE::IDLE);
	//NormalizeList(dist, INF);

	if (lpMapCtl.CheckFloor(enemy.GetPos() + VECTOR2(0, 50)))
	{
		enemy.ChangeState(CheckDist(extMapChip));
	}
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

	////���̊m�F����
	//if (lpMapCtl.CheckFloor(enemy.GetPos() + VECTOR2(0,50))) 
	//{
	//	if (lpMapCtl.GetChipType(enemy.GetPos() + VECTOR2(72/*�G�l�~�[�̃T�C�Y*/, 84 - lpMapCtl.GetChipSize().y)) == CHIP_TYPE::CHIP_BLOCK)
	//	{
	//		enemy.ChangeState(STATE::JUMP);
	//	}
	//	else
	//	{
	//		enemy.ChangeState(STATE::RUN);
	//	}
	//}
	////�O�����ɏ������邩�ǂ���
	//else if (//(lpMapCtl.GetChipType(enemy.GetPos() + VECTOR2(0, 84/*�G�l�~�[�̃T�C�Y*/)) == CHIP_TYPE::CHIP_BLANK) && 
	//	(lpMapCtl.GetChipType(enemy.GetPos() + VECTOR2(72/*�G�l�~�[�̃T�C�Y*/, 84)) == CHIP_TYPE::CHIP_BLANK) && (enemy.GetState() != STATE::JUMP))
	//{
	//	//���ɏ�������̂��ǂ����@
	//	for (int i = enemy.GetPos().y / 32 + 1; i < CHIP_CNT_Y; ++i)
	//	{
	//		if ((lpMapCtl.GetChipType(VECTOR2(enemy.GetPos().x, i * lpMapCtl.GetChipSize().y)) != CHIP_TYPE::CHIP_BLANK)
	//			 && (tmpPos.y > enemy.GetPos().y))
	//		{
	//			//�O���̉��ɏ����Ȃ��Ă��̉��ɏ�������Ƃ�
	//			enemy.ChangeState(STATE::FDOWN);
	//			break;
	//		}
	//		else if (i == CHIP_CNT_Y - 1)
	//		{
	//			//�O���̉��ɏ����Ȃ��󔒂̏�Ԃ̂Ƃ�
	//			enemy.ChangeState(STATE::JUMP);
	//		}
	//	}
	//}
}

void EnemyAI::CreateShortestMap(void)
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
			SetMapListPtr(VECTOR2(x, y));
		}
	}

}

void EnemyAI::SetMapListPtr(const VECTOR2 &chipPos)
{
	for (int y = -1; y < 2; ++y)
	{
		for (int x = -1; x < 1; ++x)
		{
			if (((int)(chipPos.x + x) < 0) || ((int)(chipPos.x + x) >= lpMapCtl.GetGameAreaSize().x / lpMapCtl.GetChipSize().x)) continue;
			if (((int)(chipPos.y + y) < 0) || ((int)(chipPos.y + y) >= lpMapCtl.GetGameAreaSize().y / lpMapCtl.GetChipSize().y)) continue;
			if ((x == 0) && (y == 0)) continue;
			if (lpMapCtl.GetChipType(VECTOR2(chipPos.x * lpMapCtl.GetChipSize().x, chipPos.y * lpMapCtl.GetChipSize().y)) == CHIP_FLOOR) continue;
			if (lpMapCtl.GetChipType(VECTOR2(chipPos.x * lpMapCtl.GetChipSize().x, chipPos.y * lpMapCtl.GetChipSize().y)) == CHIP_BLOCK) continue;
			if (lpMapCtl.GetChipType(VECTOR2(chipPos.x * lpMapCtl.GetChipSize().x, chipPos.y * lpMapCtl.GetChipSize().y)) == CHIP_FLOORDOWN) continue;
			if ((lpMapCtl.GetChipType(VECTOR2((chipPos.x + x) * lpMapCtl.GetChipSize().x, (chipPos.y + y) * lpMapCtl.GetChipSize().y)) != CHIP_FLOOR)
				&&(lpMapCtl.GetChipType(VECTOR2((chipPos.x + x) * lpMapCtl.GetChipSize().x, (chipPos.y + y) * lpMapCtl.GetChipSize().y)) != CHIP_BLOCK)
				&&(lpMapCtl.GetChipType(VECTOR2((chipPos.x + x) * lpMapCtl.GetChipSize().x, (chipPos.y + y) * lpMapCtl.GetChipSize().y)) != CHIP_FLOORDOWN))
			{
				if(y == -1) shortestPathMap[chipPos.y][chipPos.x].push_back(Edge(VECTOR2(chipPos.x + x, chipPos.y + y), 1));
				else if (y == 0) shortestPathMap[chipPos.y][chipPos.x].push_back(Edge(VECTOR2(chipPos.x + x, chipPos.y + y), 1));
				else shortestPathMap[chipPos.y][chipPos.x].push_back(Edge(VECTOR2(chipPos.x + x, chipPos.y + y), 1));
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
			else
			{
				DrawString(x * 32, y * 32, "INF", 0x000000 );
			}
		}
	}
}

EnemyAI::EnemyAI()
{
}


EnemyAI::~EnemyAI()
{
	for (int i = 0; i < lpMapCtl.GetGameAreaSize().y / lpMapCtl.GetChipSize().y; ++i)
	{
		shortestPathMap[i].clear();
		dist[i].clear();
		shortestPathMap[i].shrink_to_fit();
		dist[i].shrink_to_fit();
	}

	shortestPathMap.clear();
	dist.clear();
	shortestPathMap.shrink_to_fit();
	dist.shrink_to_fit();

	scanList.clear();
	scanListNext.clear();
	scanList.shrink_to_fit();
	scanListNext.shrink_to_fit();
}
