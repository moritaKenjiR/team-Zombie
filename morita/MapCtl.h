#pragma once
#include <vector>
#include <memory>
#include "VECTOR2.h"


enum CHIP_TYPE {
	CHIP_BLANK,		// ��
	CHIP_FLOOR,		// ��
	CHIP_BLOCK,		// ��ۯ�
};


#define GET_MAP_ID 
#define lpMapCtl MapCtl::GetInstance()
#define DATA_BIT_LEN	(8)
#define CHIP_CNT_X		(32)	//�}�b�v�̏c�`�b�v��
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
	void MapDraw(void);					// ϯ�߂̕`��֐�
	bool MapLoad(void);					// ϯ�߂�۰�ފ֐�
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

