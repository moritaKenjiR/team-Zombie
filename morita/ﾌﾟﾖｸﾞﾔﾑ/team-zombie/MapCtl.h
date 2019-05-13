#pragma once
#include <vector>
#include <memory>
#include "VECTOR2.h"


enum CHIP_TYPE {
	CHIP_BLANK,		// 空白
	CHIP_FLOOR,		// 床
	CHIP_FLOOR1,		// 床
	CHIP_FLOOR2,		// 床
	CHIP_FLOOR3,		// 床
	CHIP_FLOOR4,		// 床
	CHIP_FLOOR5,		// 床
	CHIP_FLOOR6,		// 床
	CHIP_FLOOR7,		// 床
	CHIP_FLOOR8,		// 床
	CHIP_FLOOR9,		// 床
	CHIP_FLOOR10,		// 床
	CHIP_FLOOR11,		// 床
	CHIP_BLOCK,		// ﾌﾞﾛｯｸ
	CHIP_BLOCK1,		// ﾌﾞﾛｯｸ
	CHIP_BLOCK2,		// ﾌﾞﾛｯｸ
	CHIP_BLOCK3,		// ﾌﾞﾛｯｸ
	CHIP_BLOCK4,		// ﾌﾞﾛｯｸ
	CHIP_BLOCK5,		// ﾌﾞﾛｯｸ
	CHIP_BLOCK6,		// ﾌﾞﾛｯｸ
	CHIP_FLAG_OBJ,
	CHIP_FLAG_OBJ1,
	CHIP_FLAG_OBJ2,
	CHIP_FLAG_OBJ3,
	CHIP_OBJ,
	CHIP_OBJ1,
	CHIP_OBJ2,
};


#define GET_MAP_ID 
#define lpMapCtl MapCtl::GetInstance()
#define DATA_BIT_LEN	(8)
#define CHIP_CNT_X		(700)	//マップの縦チップ数
#define VIEW_CHIP_CNT_X (32)
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
	bool CheckFloor(VECTOR2& pos);		// 受け取ったposの一つ下のﾁｯﾌﾟが立てる場所ならtrue
	bool CheckWall(VECTOR2 pos);
	void IfMove(VECTOR2& pos);			// playerの位置から色々な必要な処理をする(埋まり防止など)
	void Ground(VECTOR2& pos);
	void CheckObj(VECTOR2 pos);
	CHIP_TYPE GetChipType(VECTOR2 pos);	// 受け取ったposの所のﾁｯﾌﾟﾀｲﾌﾟを返す
	void MapDraw(VECTOR2 camPos);					// ﾏｯﾌﾟの描画関数
	bool MapLoad(void);					// ﾏｯﾌﾟのﾛｰﾄﾞ関数
	VECTOR2 GetChipSize(void);
	VECTOR2 GetViewAreaSize(void);
	VECTOR2 GetGameAreaSize(void);
	void SetDrawOffset(VECTOR2 Offset);
	VECTOR2 GameDrawOffset(void);
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
};

