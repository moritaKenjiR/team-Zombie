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

	////�S�[���m�[�h�̐ݒ�
	dist[(int)goalNum.x + (int)goalNum.y * (int)mapSize.x].cost = 0;

	q.push(dist[(int)goalNum.x + (int)goalNum.y * (int)mapSize.x]);

	while (!dist[(int)startNum.x + (int)startNum.y * (int)mapSize.x].flag)
	{
		//�T���p�m�[�h�̏�����
		std::tie(node) = (Node&)q.top();
		//�T���m�[�h�̔r��
		q.pop();
		//�T���m�[�h���T���ς݂Ȃ�Δ�����
		if (dist[(int)node.pos.x + (int)node.pos.y * (int)mapSize.x].flag)continue;
		//�T���m�[�h��T���ς݂ɕύX����
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

STATE EnemyAI::CheckDist(VECTOR2 &chipPos)
{
	STATE state;

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
	//���݂���}�b�v�`�b�v
	VECTOR2 extMapChip = VECTOR2((int)(enemy.GetPos().x / lpMapCtl.GetChipSize().x),(int)((enemy.GetPos().y + 52) / lpMapCtl.GetChipSize().y));
	//�_�C�N�X�g���o�H�T���@
	Dijkstra(enemy.GetPos(), VECTOR2(player.lock()->GetPos().x, player.lock()->GetPos().y + 52));
	//if (!Dijkstra(enemy.GetPos(), VECTOR2(player.lock()->GetPos().x, player.lock()->GetPos().y + 52))) enemy.ChangeState(STATE::IDLE);
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

void EnemyAI::SetMapListPtr(const VECTOR2 &chipPos)
{
	for (int y = -1; y < 2; ++y)
	{
		for (int x = -1; x < 2; ++x)
		{
			if (((int)(chipPos.x + x) < 0) || ((int)(chipPos.x + x) >= lpMapCtl.GetGameAreaSize().x / lpMapCtl.GetChipSize().x)) continue;							//�z��O�Q�Ɩh�~�[�u
			if (((int)(chipPos.y + y) < 0) || ((int)(chipPos.y + y) >= lpMapCtl.GetGameAreaSize().y / lpMapCtl.GetChipSize().y)) continue;							//��ɓ���
			if ((x == 0) && (y == 0)) continue;																														//�������g�̃m�[�h�ɃG�b�W��L�΂��̋֎~
			if (lpMapCtl.GetChipType(VECTOR2(chipPos.x * lpMapCtl.GetChipSize().x, chipPos.y * lpMapCtl.GetChipSize().y)) == CHIP_FLOOR) continue;					//�������g���ړ��s�u���b�N��������
			if (lpMapCtl.GetChipType(VECTOR2(chipPos.x * lpMapCtl.GetChipSize().x, chipPos.y * lpMapCtl.GetChipSize().y)) == CHIP_BLOCK) continue;					//�G�b�W��L�΂��̋֎~
			if (lpMapCtl.GetChipType(VECTOR2(chipPos.x * lpMapCtl.GetChipSize().x, chipPos.y * lpMapCtl.GetChipSize().y)) == CHIP_FLOORDOWN) continue;				//**
			if ((lpMapCtl.GetChipType(VECTOR2((chipPos.x + x) * lpMapCtl.GetChipSize().x, (chipPos.y + y) * lpMapCtl.GetChipSize().y)) != CHIP_FLOOR)				//�ړ��s�u���b�N�ɃG�b�W��L�΂��̋֎~
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
	VECTOR2 mapSize = VECTOR2(lpMapCtl.GetGameAreaSize().x / lpMapCtl.GetChipSize().x, lpMapCtl.GetGameAreaSize().y / lpMapCtl.GetChipSize().y);
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
}
