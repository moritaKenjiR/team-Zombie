#include "Dxlib.h"
#include <vector>
#include "VECTOR2.h"
#include "MouseCtl.h"
#include <queue>
#include <tuple>


constexpr int SCREEN_SIZE_X = 1024;
constexpr int SCREEN_SIZE_Y = 768;
constexpr int chipSize = 32;
constexpr int INF = 10000000;

VECTOR2 mapSize = VECTOR2(SCREEN_SIZE_X / chipSize, SCREEN_SIZE_Y / chipSize);

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
	Node(VECTOR2 pos, float cost) :pos(pos), cost(cost) {};
};

enum MODE{
	START,
	END,
	BREAK,
	MAX
};

//最短経路探索用map
std::vector<std::vector<Edge>> shortestPathMap;
std::vector<Node> dist;
Node node;
MODE mode;
//std::vector<Node> scanList;
//std::vector<Node> scanListNext;

MouseCtl *mc;

int SysInit();
int Init();
int Update();
int Draw();
int Delete();
bool Dijkstra(const VECTOR2&, const VECTOR2&);
void CreateShortestMap(void);
void SetMapListPtr(const VECTOR2 &);
bool BreakMapChip(const VECTOR2&);
//void dijkstra(int);

VECTOR2 startPos;
VECTOR2 goalPos;
char keyData[256];
char keyDataOld[256];
bool searchFlag;
bool drawFlag;


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

int WINAPI WinMain(HINSTANCE hinstance, HINSTANCE hPrevInstace, LPSTR lpCmdLine, int nCmdShow)
{
	SysInit();
	Init();
	while (ProcessMessage() == 0 && CheckHitKey(KEY_INPUT_ESCAPE) == 0){
		ClsDrawScreen();

		Update();
		Draw();

		ScreenFlip();
	}
	Delete();
	DxLib_End();
	return 0;
}

int SysInit()
{
	SetWindowText("Dijkstra");
	SetGraphMode(SCREEN_SIZE_X, SCREEN_SIZE_Y, 16);
	ChangeWindowMode(true);
	if (DxLib_Init() == -1){
		return -1;
	}
	SetDrawScreen(DX_SCREEN_BACK);
	return 0;
}

int Init(void)
{
	CreateShortestMap();
	mc = new MouseCtl();
	mapSize = { SCREEN_SIZE_X / chipSize, SCREEN_SIZE_Y / chipSize };
	mode = START;
	searchFlag = false;
	return 0;
}
int Update(void)
{
	mc->Update();
	memcpy(keyDataOld, keyData, sizeof(keyDataOld));
	GetHitKeyStateAll(keyData);

	if (keyData[KEY_INPUT_SPACE] & !keyDataOld[KEY_INPUT_SPACE]){
		if (mode == START) mode = END;
		else if (mode == END) mode = BREAK;
		else mode = START;
	}

	if (keyData[KEY_INPUT_NUMPADENTER] & !keyDataOld[KEY_INPUT_NUMPADENTER]) {
		searchFlag = true;
	}

	if (((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT) && mode == START) {
		startPos = mc->GetPoint();
	}
	else if (((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT) && mode == END){
		goalPos = mc->GetPoint();
	}
	else if(((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT) && mode == BREAK){
		//ブロック破壊処理
		BreakMapChip(mc->GetPoint());
	}

	if (searchFlag == true) {
		Dijkstra(startPos, goalPos);
		searchFlag = false;
		drawFlag = true;
	}
	return 0;
}

int Draw(void)
{
	VECTOR2 startDrawPos = VECTOR2((int)(startPos.x / chipSize) * chipSize, (int)(startPos.y / chipSize) * chipSize );
	VECTOR2 goalDrawPos = VECTOR2((int)(goalPos.x / chipSize) * chipSize, (int)(goalPos.y / chipSize) * chipSize );

	for (int x = 0; x <= mapSize.x; ++x){
		DrawLine(x * chipSize, 0, x * chipSize, SCREEN_SIZE_Y, 0xffffff);
		DrawFormatString(x * chipSize, 0, 0xffffff, "%d", x);
	}
	for (int y = 0; y <= mapSize.y; ++y){
		DrawLine(0, y * chipSize, SCREEN_SIZE_X, y * chipSize , 0xffffff);
		DrawFormatString(0, y * chipSize, 0xffffff, "%d", y);
	}
	SetFontSize(32);
	DrawString(startDrawPos.x,startDrawPos.y, "S", 0xffffff);
	DrawString(goalDrawPos.x, goalDrawPos.y, "G", 0xffffff);
	
	//DrawFormatString(startDrawPos.x + 32, startDrawPos.y, 0xffff00, "%d,%d", startPos.x, startPos.y);
	//DrawFormatString(goalDrawPos.x + 32, goalDrawPos.y, 0xffff00, "%d,%d", goalPos.x, goalPos.y);

	if (drawFlag == true)
	{
		for (auto itr : dist)
		{
			if (itr.cost == INF) continue;
			DrawFormatString(itr.pos.x * chipSize, itr.pos.y * chipSize, 0xffff00, "%d", (int)itr.cost);
		}
	}

	return 0;
}

int Delete(void)
{
	if (mc != nullptr) delete mc;

	
	dist.clear();
	dist.shrink_to_fit();

	for (auto edge : shortestPathMap)
	{
		edge.clear();
		edge.shrink_to_fit();
	}
	shortestPathMap.clear();
	shortestPathMap.shrink_to_fit();


	//scanList.clear();
	//scanListNext.clear();
	//scanList.shrink_to_fit();
	//scanListNext.shrink_to_fit();
	
	return 0;
}

bool Dijkstra(const VECTOR2& start, const VECTOR2& goal)
{
	
	VECTOR2 goalNum = VECTOR2(goal.x / chipSize, goal.y / chipSize);
	VECTOR2 startNum = VECTOR2(start.x / chipSize, start.y / chipSize);

	dist.clear();
	dist.resize(mapSize.x * mapSize.y);

	for (int y = 0; y < mapSize.y; ++y){
		for (int x = 0; x < mapSize.x; ++x){
			dist[x + y * mapSize.x] = Node(VECTOR2(x, y), INF);
		}
	}
	std::priority_queue<Node, std::vector<Node>, std::less<std::vector<Node>::value_type>> q;


	////ゴールノードの設定
	dist[goalNum.x + goalNum.y * mapSize.x].cost = 0;
	//dist[goalNum.x + goalNum.y * chipSize].flag = true;

	q.push(dist[goalNum.x + goalNum.y * mapSize.x]);

	//scanList.clear();
	//scanList.push_back(dist[goalNum.y + goalNum.x * chipSize]);
	//scanListNext.clear();

	while (!dist[startNum.x + startNum.y * mapSize.x].flag/*!q.empty()*/)
	{
		//探索用ノードの初期化
		std::tie(node) = (Node&)q.top();
		//探索ノードの排除
		q.pop();
		//探索ノードが探索済みならば抜ける
		if (dist[node.pos.x + node.pos.y * mapSize.x].flag)continue;
		//探索ノードを探索済みに変更する
		dist[node.pos.x + node.pos.y * mapSize.x].flag = true;
		for (auto edge : shortestPathMap[node.pos.x + node.pos.y * mapSize.x])
		{
			if (dist[edge.to].cost <= edge.cost + dist[node.pos.x + node.pos.y * mapSize.x].cost) continue;
			dist[edge.to].cost = edge.cost + dist[node.pos.x + node.pos.y * mapSize.x].cost;
			q.push(dist[edge.to]);
		}

	}
	//while (!q.empty()) {
	//	//std::tie(dist[goalNum.y * mapSize.y + goalNum.x]) = q.top();
	//	q.pop();

	//	if (dist[goalNum.x + goalNum.y * chipSize].flag) continue;
	//	dist[goalNum.x + goalNum.y * chipSize].flag = true;
	//	dist[goalNum.x + goalNum.y * chipSize].cost = shortestPathMap[goalNum.x + goalNum.y * chipSize].cost;
	//	for (auto e : shortestPathMap)
	//	{
	//		if (dist[e.to].cost <= e.cost + dist[goalNum.x + goalNum.y * chipSize].cost) continue;
	//		q.push(dist[e.to]);
	//	}
	//}

	//while (!dist[startNum.y + startNum.x * chipSize].flag) {

	//	for (auto list = scanList.begin(); list != scanList.end(); ++list){
	//		for (auto itr = shortestPathMap[(*list).pos.y][(*list).pos.x].begin(); itr != shortestPathMap[(*list).pos.y][(*list).pos.x].end(); ++itr){
	//			if ((!dist[(*itr).to.y][(*itr).to.x].flag) && (dist[(*itr).to.y][(*itr).to.x].cost > dist[(*list).pos.y][(*list).pos.x].cost + (*itr).cost)){
	//				dist[(*itr).to.y][(*itr).to.x].cost = dist[(*list).pos.y][(*list).pos.x].cost + (*itr).cost;
	//				//dist[(*itr).to.y][(*itr).to.x].cost = min(dist[(*itr).to.y][(*itr).to.x].cost, dist[(*list).pos.y][(*list).pos.x].cost + (*itr).cost);
	//				if (scanListNext.empty()){
	//					scanListNext.push_back(dist[(*itr).to.y][(*itr).to.x]);
	//				}
	//				else if (scanListNext.front().cost == dist[(*itr).to.y][(*itr).to.x].cost){
	//					scanListNext.push_back(dist[(*itr).to.y][(*itr).to.x]);
	//				}
	//				else{
	//					scanListNext.clear();
	//					scanListNext.push_back(dist[(*itr).to.y][(*itr).to.x]);
	//				}
	//			}
	//		}
	//		//検索ノードのコスト値決定処理
	//		dist[(*list).pos.y][(*list).pos.x].flag = true;
	//		//dist[(*list).pos.y][(*list).pos.x].cost = dist[(*list).pos.y][(*list).pos.x].cost / INF;
	//	}
	//	scanList.clear();
	//	scanList.resize(scanListNext.size());
	//	scanList = scanListNext;
	//	scanList.shrink_to_fit();
	//	scanListNext.clear();
	//	scanListNext.shrink_to_fit();

	//	//プレイヤーがもし移動不可位置に移動してしまったとき
	//	if (scanList.empty()){
	//		return false;
	//	}
	//}

	return true;

}

void CreateShortestMap(void)
{
	for (auto edge : shortestPathMap)
	{
		edge.clear();
		edge.shrink_to_fit();
	}
	shortestPathMap.clear();
	shortestPathMap.resize(mapSize.y * mapSize.x);

	for (int y = 0; y < mapSize.y; ++y){
		for (int x = 0; x < mapSize.x; ++x){
			SetMapListPtr(VECTOR2(x, y));
		}
	}

}

void SetMapListPtr(const VECTOR2 &chipPos)
{
	for (int y = -1; y < 2; ++y){
		for (int x = -1; x < 2; ++x){
			if (((int)(chipPos.x + x) < 0) || ((int)(chipPos.x + x) >= SCREEN_SIZE_X / chipSize)) continue;
			if (((int)(chipPos.y + y) < 0) || ((int)(chipPos.y + y) >= SCREEN_SIZE_Y / chipSize)) continue;
			if ((x == 0) && (y == 0)) continue;
			if (y == -1) shortestPathMap[chipPos.x + chipPos.y * mapSize.x].push_back(Edge((chipPos.x + chipPos.y * mapSize.x) + (x + y * mapSize.x), 1));
			else if (y == 0) shortestPathMap[chipPos.x + chipPos.y * mapSize.x].push_back(Edge((chipPos.x + chipPos.y * mapSize.x) + (x + y * mapSize.x), 1));
			else shortestPathMap[chipPos.x + chipPos.y * mapSize.x].push_back(Edge((chipPos.x + chipPos.y * mapSize.x) + (x + y * mapSize.x), 1));
		}
	}
}

bool BreakMapChip(const VECTOR2& pos)
{
	VECTOR2 chipPos = VECTOR2(pos.x / chipSize, pos.y / chipSize);
	if (shortestPathMap[chipPos.x + chipPos.y * mapSize.x].size() == 0) return false;
	int size = 0;

	while (shortestPathMap[chipPos.x + chipPos.y * mapSize.x].size() != 0)
	{
		for (auto edge : shortestPathMap[chipPos.x + chipPos.y * mapSize.x])
		{
			for (auto edge2 : shortestPathMap[edge.to])
			{
				//distが生成されてないから×
				if (edge2.to == dist[chipPos.x + chipPos.y * mapSize.x].pos.x + dist[chipPos.x + chipPos.y * mapSize.x].pos.y * mapSize.x)
				{
					shortestPathMap[edge.to].erase(shortestPathMap[edge.to].begin() + size);
					shortestPathMap[edge.to].shrink_to_fit();
					continue;
				}
				size++;
			}
			size = 0;
		}
		shortestPathMap[chipPos.x + chipPos.y * mapSize.x].clear();
		shortestPathMap[chipPos.x + chipPos.y * mapSize.x].shrink_to_fit();
	}
	return true;
}

//using namespace std;
//
//const int V = 100;
//const int INF = 1 << 28;
//using P = pair<int, int>;
//
//vector<P> G[V];			//pair<辺の距離,行き先の頂点>
//int 	  dist[V];		//dist[i]はsから頂点iへの最短距離が入る
//bool 	  used[V];
//
//void dijkstra(int s)		//s:始点
//{
//	//全マップのINF初期化とフラグの切り替え
//	fill_n(dist, V, INF);
//	fill_n(used, V, false);
//
//	//優先度付きキューの生成
//	std::priority_queue<P, vector<P>, greater<P>> q;
//
//	//キューにスタートの値を入れる
//	q.push(P(1, s));
//
//	//キューが空になった時点で終了
//	while (!q.empty())
//	{
//		int d;		//d:sからの距離
//		int t;		//t:行き先
//
//		tie(d, t) = q.top();
//		q.pop();
//
//		if (used[t]) continue;	//もうすでに探索済みか
//
//		used[t] = true;
//		dist[t] = d;
//
//		for (P e : G[t])
//		{
//			if (dist[e.second] < d + e.first) continue;
//			q.push(P(d + e.first, e.second));
//		}
//	}
//}