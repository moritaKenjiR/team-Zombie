#pragma once
#include <vector>
#include <memory>
#include "VECTOR2.h"


enum CHIP_TYPE {
	CHIP_BLANK,		// ��
	CHIP_FLOOR,		// ��
	CHIP_BLOCK,		// ��ۯ�
	CHIP_FLOORDOWN
};


#define GET_MAP_ID 
#define lpMapCtl MapCtl::GetInstance()
#define DATA_BIT_LEN	(8)
#define CHIP_CNT_X		(320)	//�}�b�v�̏c�`�b�v��
#define VIEW_CHIP_CNT_X (32)
#define CHIP_CNT_Y		(24)	//�}�b�v�̉��`�b�v��

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
	bool CheckFloor(VECTOR2 pos);		// �󂯎����pos�̈�������߂����Ă�ꏊ�Ȃ�true
	CHIP_TYPE GetChipType(VECTOR2 pos);	// �󂯎����pos�̏����������߂�Ԃ�
	void MapDraw(VECTOR2 camPos);					// ϯ�߂̕`��֐�
	bool MapLoad(void);					// ϯ�߂�۰�ފ֐�
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

