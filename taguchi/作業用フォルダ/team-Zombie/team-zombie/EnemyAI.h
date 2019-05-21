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

	//ダイクストラ経路探索
	bool Dijkstra(const VECTOR2&,const VECTOR2&);
	bool NormalizeList(std::vector<std::vector<Node>>&, const int&);

	STATE CheckDist(VECTOR2&);
	bool SetTarget(std::weak_ptr<Obj>);
	//状況からエネミーの動き方を決定する関数
	void CreateMove(Enemy&);
	//shortestPathMapの初期化用関数
	void CreateShortestMap(void);
	void SetMapListPtr(const VECTOR2 &);
	void Draw(void);

private:
	EnemyAI();
	EnemyAI(const EnemyAI&) {};
	EnemyAI &operator=(const EnemyAI&) {};
	~EnemyAI();

	//最短経路探索用map
	std::vector<std::vector<Edge>> shortestPathMap;
	std::vector<Node> dist;
	Node node;
	std::weak_ptr<Obj> player;
	VECTOR2 mapSize;
	int count = 0;

};

