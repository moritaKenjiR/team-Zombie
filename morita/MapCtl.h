#pragma once
#include <vector>
#include <memory>
#include "VECTOR2.h"


enum CHIP_TYPE {
	CHIP_BLANK,		// 空白
	CHIP_FLOOR,		// 床
	CHIP_BLOCK,		// ﾌﾞﾛｯｸ
};


#define GET_MAP_ID 
#define lpMapCtl MapCtl::GetInstance()
#define DATA_BIT_LEN	(8)
#define CHIP_CNT_X		(32)	//マップの縦チップ数
#define CHIP_CNT_Y		(24)	//マップの横チップ数

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
	CHIP_TYPE GetChipType(VECTOR2 pos);	// 受け取ったposの所のﾁｯﾌﾟﾀｲﾌﾟを返す
	void MapDraw(void);					// ﾏｯﾌﾟの描画関数
	bool MapLoad(void);					// ﾏｯﾌﾟのﾛｰﾄﾞ関数
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
};

