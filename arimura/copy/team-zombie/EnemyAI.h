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

class EnemyAI
{
public:
	static EnemyAI &GetInstance(void)
	{
		static EnemyAI getInstance;
		return getInstance;
	}

	void Dijkstra(const VECTOR2&,const VECTOR2&);
	bool SetTarget(std::weak_ptr<Obj>);
	//�󋵂���G�l�~�[�̓����������肷��֐�
	void CreateMove(Enemy&);
	//shortestPathMap�̏������p�֐�
	void CreateShotestMap(void);
	bool SetMapListPtr(const VECTOR2 &);

private:
	EnemyAI();
	EnemyAI(const EnemyAI&) {};
	EnemyAI &operator=(const EnemyAI&) {};
	~EnemyAI();

	//�ŒZ�o�H�T���pmap
	std::vector<std::vector<std::vector <Edge>>> shortestPathMap;
	std::weak_ptr<Obj> player;

};

