#include "Dxlib.h"
#include <vector>
#include "VECTOR2.h"
#include "MouseCtl.h"
#include <queue>
#include <tuple>


constexpr int SCREEN_SIZE_X = 1024;
constexpr int SCREEN_SIZE_Y = 768;
constexpr int chipSize = 32;
//constexpr int INF = 10000000;

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

//最短経路探索用map
//std::vector<Edge> shortestPathMap;
//std::vector<Node> dist;
//std::vector<Node> scanList;
//std::vector<Node> scanListNext;

MouseCtl *mc;

int SysInit();
int Init();
int Update();
int Draw();
int Delete();
bool Dijkstra(const VECTOR2&, const VECTOR2&);
//void CreateShortestMap(void);
//void SetMapListPtr(const VECTOR2 &);
void dijkstra(int);

VECTOR2 startPos;
VECTOR2 goalPos;
VECTOR2 mapSize;
char keyData[256];
char keyDataOld[256];
bool flag;
bool searchFlag;
bool drawFlag;


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
	//CreateShortestMap();
	mc = new MouseCtl();
	mapSize = { SCREEN_SIZE_X / chipSize, SCREEN_SIZE_Y / chipSize };
	flag = true;
	searchFlag = false;
	return 0;
}
int Update(void)
{
	mc->Update();
	memcpy(keyDataOld, keyData, sizeof(keyDataOld));
	GetHitKeyStateAll(keyData);

	if (keyData[KEY_INPUT_SPACE] & !keyDataOld[KEY_INPUT_SPACE]){
		if (flag != false)	flag = false;
		else flag = true;
	}

	if (keyData[KEY_INPUT_NUMPADENTER] & !keyDataOld[KEY_INPUT_NUMPADENTER]) {
		searchFlag = true;
	}

	if (((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT) && flag == true) {
		startPos = mc->GetPoint();
	}
	else if (((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)) {
		goalPos = mc->GetPoint();
	}

	if (searchFlag == true) {
		dijkstra(startPos.x);
		//Dijkstra(startPos, goalPos);
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
	}
	for (int y = 0; y <= mapSize.y; ++y){
		DrawLine(0, y * chipSize, SCREEN_SIZE_X, y * chipSize , 0xffffff);
	}
	SetFontSize(32);
	DrawString(startDrawPos.x,startDrawPos.y, "S", 0xffffff);
	DrawString(goalDrawPos.x, goalDrawPos.y, "G", 0xffffff);

	if (drawFlag == true)
	{
		//for (auto itr : dist)
		//{
		//	DrawFormatString(itr.pos.x * chipSize, itr.pos.y * chipSize, 0xffff00, "%d", (int)itr.cost);
		//}
		//for (int y = 0; y < mapSize.y; ++y)
		//{
		//	for (int x = 0; x < mapSize.x; ++x)
		//	{
		//		if (dist[y + x * chipSize].cost != INF)
		//		{
		//			DrawFormatString(dist[y + x * chipSize].pos.x * chipSize, dist[y + x * chipSize].pos.y * chipSize, 0xffff00, "%d", (int)dist[y][x].cost);
		//			//DrawBox(/*pos.x, pos.y, pos.x + chipSize, pos.y + chipSize*/0, 0, 32, 32, 0xff0000, true);
		//		}
		//	}
		//}
	}

	return 0;
}

int Delete(void)
{
	if (mc != nullptr) delete mc;

	//shortestPathMap.clear();
	//dist.clear();
	//shortestPathMap.shrink_to_fit();
	//dist.shrink_to_fit();

	//scanList.clear();
	//scanListNext.clear();
	//scanList.shrink_to_fit();
	//scanListNext.shrink_to_fit();
	
	return 0;
}

bool Dijkstra(const VECTOR2& start, const VECTOR2& goal)
{
	//VECTOR2 mapSize = VECTOR2(SCREEN_SIZE_X / chipSize, SCREEN_SIZE_Y / chipSize);
	//VECTOR2 goalNum = VECTOR2(goal.x / chipSize, goal.y / chipSize);
	//VECTOR2 startNum = VECTOR2(start.x / chipSize, start.y / chipSize);
	//dist.clear();
	//dist.resize(mapSize.y * mapSize.x);

	//for (int y = 0; y < mapSize.y; ++y){
	//	for (int x = 0; x < mapSize.x; ++x){
	//		dist[y * mapSize.y + x] = Node(VECTOR2(x, y), INF);
	//	}
	//}
	//std::priority_queue<Node, std::vector<Node>, std::greater<Node>> q;
	////ゴールノードの設定
	//dist[goalNum.y + goalNum.x * chipSize].cost = 0;
	//dist[goalNum.y + goalNum.x * chipSize].flag = true;

	//q.push(dist[goalNum.y + goalNum.x * chipSize]);

	//scanList.clear();
	//scanList.push_back(dist[goalNum.y + goalNum.x * chipSize]);
	//scanListNext.clear();

	//while (!q.empty()) {
	//	std::tie(dist[goalNum.y * mapSize.y + goalNum.x]) = q.top();
	//	q.pop();

	//	if (dist[goalNum.y + goalNum.x * chipSize].flag) continue;
	//	dist[goalNum.y + goalNum.x * chipSize].flag = true;
	//	dist[goalNum.y + goalNum.x * chipSize].cost = shortestPathMap[goalNum.y + goalNum.x * chipSize].cost;
	//	for (auto e : shortestPathMap)
	//	{
	//		if (dist[e.to].cost <= e.cost + dist[goalNum.y + goalNum.x * chipSize].cost) continue;
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

//void CreateShortestMap(void)
//{
//	VECTOR2 mapSize = VECTOR2(SCREEN_SIZE_X / chipSize, SCREEN_SIZE_Y / chipSize);
//	VECTOR2 mapPos;
//
//	shortestPathMap.clear();
//	shortestPathMap.resize(mapSize.y * mapSize.x);
//
//	for (int y = 0; y < mapSize.y; ++y){
//		for (int x = 0; x < mapSize.x; ++x){
//			SetMapListPtr(VECTOR2(x, y));
//		}
//	}
//
//}
//
//void SetMapListPtr(const VECTOR2 &chipPos)
//{
//	for (int y = -1; y < 2; ++y){
//		for (int x = -1; x < 2; ++x){
//			if (((int)(chipPos.x + x) < 0) || ((int)(chipPos.x + x) >= SCREEN_SIZE_X / chipSize)) continue;
//			if (((int)(chipPos.y + y) < 0) || ((int)(chipPos.y + y) >= SCREEN_SIZE_Y / chipSize)) continue;
//			if ((x == 0) && (y == 0)) continue;
//			if (y == -1) shortestPathMap.push_back(Edge(chipPos.x + x + chipPos.y + y * mapSize.y, 1));
//			else if (y == 0) shortestPathMap.push_back(Edge(chipPos.x + x + chipPos.y + y * mapSize.y, 1));
//			else shortestPathMap.push_back(Edge(chipPos.x + x + chipPos.y + y * mapSize.y, 1));
//		}
//	}
//}

using namespace std;

const int V = 100;
const int INF = 1 << 28;
using P = pair<int, int>;

vector<P> G[V];			//pair<辺の距離,行き先の頂点>
int 	  dist[V];		//dist[i]はsから頂点iへの最短距離が入る
bool 	  used[V];

void dijkstra(int s)		//s:始点
{
	fill_n(dist, V, INF);
	fill_n(used, V, false);

	std::priority_queue<P, vector<P>, greater<P>> q;

	q.push(P(1, s));

	while (!q.empty())
	{
		int d;		//d:sからの距離
		int t;		//t:行き先

		tie(d, t) = q.top();
		q.pop();

		if (used[t]) continue;	//もうすでに探索済みか

		used[t] = true;
		dist[t] = d;

		for (P e : G[t])
		{
			if (dist[e.second] < d + e.first) continue;
			q.push(P(d + e.first, e.second));
		}
	}
}