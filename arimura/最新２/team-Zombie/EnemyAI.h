#pragma once
#include <memory>
#include <vector>
#include <queue>
#include <tuple>
#include "VECTOR2.h"


#define lpEnemyAI EnemyAI::GetInstance()
constexpr int INF = 10000000;

class Obj;
class Enemy;
enum class STATE;

struct Edge {
	int to;
	float cost = 1;
	Edge() {};
	Edge(int to, float cost) :to(to), cost(cost) {};
};

struct Node {
	VECTOR2 pos;
	float cost;
	bool flag = false;
	Node() {};
	Node(VECTOR2 pos, float cost) :pos(pos),cost(cost) {};
};

class EnemyAI
{
public:
	static EnemyAI &GetInstance(void)
	{
		static EnemyAI getInstance;
		return getInstance;
	}

	//�_�C�N�X�g���o�H�T��
	bool Dijkstra(const VECTOR2&,const VECTOR2&);
	bool NormalizeList(std::vector<std::vector<Node>>&, const int&);

	STATE CheckDist(VECTOR2&, Enemy&);
	bool SetTarget(std::weak_ptr<Obj>);
	//�󋵂���G�l�~�[�̓����������肷��֐�
	void CreateMove(Enemy&);
	//shortestPathMap�̏������p�֐�
	void CreateShortestMap(void);
	bool CreateTopograMap(void);
	void SetMapListPtr(const VECTOR2 &);
	void Draw();

private:
	EnemyAI();
	EnemyAI(const EnemyAI&) {};
	EnemyAI &operator=(const EnemyAI&) {};
	~EnemyAI();

	//�ŒZ�o�H�T���pmap
	std::vector<std::vector<Edge>> shortestPathMap;
	std::vector<Node> dist;
	std::vector<std::pair<float, bool>> topograMap;
	Node node;
	std::weak_ptr<Obj> player;
	VECTOR2 mapSize;
	int searchChipSize;
	VECTOR2 extMapChip;										//���݂���}�b�v�`�b�v
	VECTOR2 arrivalP;										//���B�n�_

	std::shared_ptr<Enemy> ene;

};

