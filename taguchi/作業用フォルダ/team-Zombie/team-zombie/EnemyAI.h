#pragma once
#include <memory>
#include <vector>
#include "VECTOR2.h"

#define lpEnemyAI EnemyAI::GetInstance()
constexpr int INF = 1000000000;

class Obj;
class Enemy;

struct Edge {
	VECTOR2 to;
	float cost = 1;
	Edge() {};
	Edge(VECTOR2 to, float cost) :to(to), cost(cost) {};
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

	bool Dijkstra(const VECTOR2&,const VECTOR2&);
	bool SetTarget(std::weak_ptr<Obj>);
	//状況からエネミーの動き方を決定する関数
	void CreateMove(Enemy&);
	//shortestPathMapの初期化用関数
	void CreateShotestMap(void);
	void SetMapListPtr(const VECTOR2 &);
	void Draw(void);

private:
	EnemyAI();
	EnemyAI(const EnemyAI&) {};
	EnemyAI &operator=(const EnemyAI&) {};
	~EnemyAI();

	//最短経路探索用map
	std::vector<std::vector<std::vector <Edge>>> shortestPathMap;
	std::vector<std::vector<Node>> dist;
	std::vector<Node> scanList;
	std::vector<Node> scanListNext;
	std::weak_ptr<Obj> player;
	int count = 0;

};

