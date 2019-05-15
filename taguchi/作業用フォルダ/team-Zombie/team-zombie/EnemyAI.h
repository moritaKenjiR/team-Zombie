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
	float cost = INF;
	bool flag = false;
	Node() {};
	Node(float cost) :cost(cost) {};
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
	//�󋵂���G�l�~�[�̓����������肷��֐�
	void CreateMove(Enemy&);
	//shortestPathMap�̏������p�֐�
	void CreateShotestMap(void);
	void SetMapListPtr(const VECTOR2 &);
	void Draw(void);

private:
	EnemyAI();
	EnemyAI(const EnemyAI&) {};
	EnemyAI &operator=(const EnemyAI&) {};
	~EnemyAI();

	//�ŒZ�o�H�T���pmap
	std::vector<std::vector<std::vector <Edge>>> shortestPathMap;
	std::vector<std::vector<Node>> dist;
	std::weak_ptr<Obj> player;

};

