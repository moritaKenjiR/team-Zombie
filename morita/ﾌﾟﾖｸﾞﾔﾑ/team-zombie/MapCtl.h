#pragma once
#include <vector>
#include <memory>
#include "VECTOR2.h"


enum CHIP_TYPE {
	CHIP_BLANK,		// ��
	CHIP_FLOOR1,		// ��
	CHIP_FLOOR2,		// ��
	CHIP_FLOOR3,		// ��
	CHIP_FLOOR4,		// ��
	CHIP_FLOOR5,		// ��
	CHIP_FLOOR6,		// ��
	CHIP_FLOOR7,		// ��
	CHIP_FLOOR8,		// ��
	CHIP_GRASS1,
	CHIP_GRASS2,
	CHIP_GRASS3,
	CHIP_BLOCK1,		// ��ۯ�
	CHIP_BLOCK2,		// ��ۯ�
	CHIP_BLOCK3,		// ��ۯ�
	CHIP_BLOCK4,		// ��ۯ�
	CHIP_BLOCK5,		// ��ۯ�
	CHIP_BLOCK6,		// ��ۯ�
	CHIP_BLOCK7,		// ��ۯ�
	CHIP_BLOCK8,		// ��ۯ�
	CHIP_FIRE,			//��
	CHIP_THORN,			//��
	CHIP_FLAG_OBJ3,
	CHIP_COIN,		//���
	CHIP_OBJ1,
	CHIP_OBJ2,
	CHIP_OBJ3,
};


#define GET_MAP_ID 
#define lpMapCtl MapCtl::GetInstance()
#define DATA_BIT_LEN	(8)
#define CHIP_CNT_X		(700)	//�}�b�v�̏c�`�b�v��
#define VIEW_CHIP_CNT_X (32)
#define CHIP_CNT_Y		(24)	//�}�b�v�̉��`�b�v��

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

	int MapInit(void);					// ϯ�߂̏������֐�
	bool CheckFloor(VECTOR2& pos);		// �󂯎����pos�̈�������߂����Ă�ꏊ�Ȃ�true
	bool CheckWall(VECTOR2 pos);
	bool CheckUpBlock(VECTOR2 pos);
	void IfMove(VECTOR2& pos);			// player�̈ʒu����F�X�ȕK�v�ȏ���������(���܂�h�~�Ȃ�)
	void StepMove(VECTOR2& pos);
	void Ground(VECTOR2& pos);
	void Ceiling(VECTOR2& pos);
	void CheckCoin(VECTOR2 pos);
	void CoinScoreAdd(void);
	CHIP_TYPE GetChipType(VECTOR2 pos);	// �󂯎����pos�̏����������߂�Ԃ�
	void MapDraw(VECTOR2 camPos);					// ϯ�߂̕`��֐�
	void MapNearDraw(VECTOR2 camPos);
	bool MapLoad(void);					// ϯ�߂�۰�ފ֐�
	VECTOR2 GetChipSize(void);
	VECTOR2 GetViewAreaSize(void);
	VECTOR2 GetGameAreaSize(void);
	void SetDrawOffset(VECTOR2 Offset);
	VECTOR2 GameDrawOffset(void);
	void SetPlayerPos(VECTOR2 pos);
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
};

