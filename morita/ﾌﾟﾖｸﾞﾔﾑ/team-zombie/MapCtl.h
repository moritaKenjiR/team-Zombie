#pragma once
#include <vector>
#include <memory>
#include <chrono>
#include "VECTOR2.h"


enum CHIP_TYPE {
	CHIP_BLANK,		// 空白
	CHIP_FLOOR1,		// 床
	CHIP_FLOOR2,		// 床
	CHIP_FLOOR3,		// 床
	CHIP_FLOOR4,		// 床
	CHIP_FLOOR5,		// 床
	CHIP_FLOOR6,		// 床
	CHIP_FLOOR7,		// 床
	CHIP_FLOOR8,		// 床
	CHIP_GRASS1,
	CHIP_GRASS2,
	CHIP_GRASS3,
	CHIP_BLOCK1,		// ﾌﾞﾛｯｸ
	CHIP_BLOCK2,		// ﾌﾞﾛｯｸ
	CHIP_BLOCK3,		// ﾌﾞﾛｯｸ
	CHIP_BLOCK4,		// ﾌﾞﾛｯｸ
	CHIP_BLOCK5,		// ﾌﾞﾛｯｸ
	CHIP_BLOCK6,		// ﾌﾞﾛｯｸ
	CHIP_BLOCK7,		// ﾌﾞﾛｯｸ
	CHIP_BLOCK8,		// ﾌﾞﾛｯｸ
	CHIP_FIRE,			//炎
	CHIP_THORN,			//棘
	CHIP_FLAG_OBJ3,
	CHIP_COIN,		//ｺｲﾝ
	CHIP_OBJ1,
	CHIP_OBJ2,
	CHIP_OBJ3,
};

struct GetCoin {
	VECTOR2 pos;
	bool active;
	VECTOR2 moveVec;
	float moveRate;
};

#define GET_MAP_ID 
#define lpMapCtl MapCtl::GetInstance()
#define DATA_BIT_LEN	(8)
#define CHIP_CNT_X		(700)	//マップの縦チップ数
#define VIEW_CHIP_CNT_X (32)
#define CHIP_CNT_Y		(24)	//マップの横チップ数

#define FLOOR_START	(CHIP_FLOOR1)
#define FLOOR_LAST	(CHIP_FLOOR8)
#define GRASS_START (CHIP_GRASS1)
#define GRASS_LAST  (CHIP_GRASS3)
#define BLOCK_START (CHIP_BLOCK1)
#define BLOCK_LAST  (CHIP_BLOCK8)

using	VEC_CHIP = std::vector<unsigned int>;
using	VEC_CHIP2 = std::vector<VEC_CHIP>;

class MapCtl
{
public:
	static MapCtl & GetInstance()
	{
		return *s_Instance;
	}

	int MapInit(void);					// ﾏｯﾌﾟの初期化関数
	bool CheckFloor(VECTOR2 pos);		// 受け取ったposの一つ下のﾁｯﾌﾟが立てる場所ならtrue
	bool CheckWall(VECTOR2 pos);
	bool CheckUpBlock(VECTOR2 pos);
	void IfMove(VECTOR2& pos);			// playerの位置から色々な必要な処理をする(埋まり防止など)
	void StepMove(VECTOR2& pos);
	void Ground(VECTOR2& pos);
	void Ceiling(VECTOR2& pos);
	void CheckCoin(VECTOR2 pos);
	void CoinScoreAdd(void);
	CHIP_TYPE GetChipType(VECTOR2 pos);	// 受け取ったposの所のﾁｯﾌﾟﾀｲﾌﾟを返す
	void MapDraw(VECTOR2 camPos);					// ﾏｯﾌﾟの描画関数
	void MapNearDraw(VECTOR2 camPos);
	bool MapLoad(void);					// ﾏｯﾌﾟのﾛｰﾄﾞ関数
	VECTOR2 GetChipSize(void);
	VECTOR2 GetViewAreaSize(void);
	VECTOR2 GetGameAreaSize(void);
	void SetDrawOffset(VECTOR2 Offset);
	VECTOR2 GameDrawOffset(void);
	void SetPlayerPos(VECTOR2 pos);
	void TimerStart(void);
private:
	MapCtl();
	~MapCtl();
	struct MapCtlDeleter
	{
		void operator()(MapCtl* MapCtl) const
		{
			delete MapCtl;
		}
	};
	static std::unique_ptr<MapCtl, MapCtlDeleter> s_Instance;

	VEC_CHIP2 mapID;
	VECTOR2 cOffset;
	VECTOR2 pPos;
	int Score;
	std::vector<GetCoin> coinList;
	int animCnt;
	std::chrono::time_point<std::chrono::system_clock> startTime,nowTime,endTime;
	int Time;
};

