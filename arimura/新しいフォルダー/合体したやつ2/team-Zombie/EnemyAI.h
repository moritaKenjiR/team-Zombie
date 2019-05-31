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
	bool flag = false;
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
	~EnemyAI();
	static EnemyAI &GetInstance();

	//ダイクストラ経路探索
	bool Dijkstra(const VECTOR2&,const VECTOR2&, const std::vector<std::vector<Edge>>&);
	bool NormalizeList(std::vector<std::vector<Node>>&, const int&);

	STATE CheckDist(VECTOR2&, Enemy&);
	bool SetTarget(std::weak_ptr<Obj>);
	//状況からエネミーの動き方を決定する関数
	void CreateMove(Enemy&);
	//shortestPathMapの初期化用関数
	void CreateShortestMap(void);
	bool CreateTopograMap(void);
	void SetMapListPtr(const VECTOR2 &);
	void Draw(Enemy&);

private:
	EnemyAI();
	EnemyAI(const EnemyAI&) {};
	EnemyAI &operator=(const EnemyAI&) {};


	//最短経路探索用map
	std::vector<std::vector<Edge>> shortestPathMapL;		//左へエッジを伸ばすマップ
	std::vector<std::vector<Edge>> shortestPathMapR;		//右へエッジを伸ばすマップ

	std::vector<Node> dist;
	std::vector<std::pair<float, bool>> topograMap;
	Node node;
	std::weak_ptr<Obj> player;
	VECTOR2 mapSize;
	int searchChipSize;
	VECTOR2 extMapChip;										//現在いるマップチップ

	//デバック用
	bool groundflag;
	VECTOR2 targetP;

};

