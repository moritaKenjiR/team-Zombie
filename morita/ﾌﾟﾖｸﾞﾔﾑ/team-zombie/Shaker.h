#pragma once
#include <memory>
#include "VECTOR2.h"

enum SHAKE_TYPE {
	SHAKE_X,	//���h��
	SHAKE_Y,	//�c�h��
	SHAKE_XY,	//�c���h��
	SHAKE_MAX,
};
enum SMOVE_TYPE {
	JUMP_SHAKE,	//�ެ��ߎ�
	LAND_SHAKE,	//���n��
	RAND_SHAKE,	//����їh��
};

#define lpShaker Shaker::GetInstance()

class Shaker
{
public:
	static Shaker &GetInstance()
	{
		return *s_Instance;
	}
	void ShakeOffsetSet(void);
	void SetShaker(bool flag,int timer,SHAKE_TYPE type);	//�h����׸ނ�Ă���(timer�͗h�炷����)
	void MoveOffset(std::string moveName);
	void ShakeDraw();
private:
	Shaker();
	~Shaker();
	struct shakerDeleter
	{
		void operator()(Shaker* shaker) const
		{
			delete shaker;
		}
	};
	static std::unique_ptr<Shaker, shakerDeleter> s_Instance;

	int ShakeGH;
	int shakeTimer;
	SHAKE_TYPE shakeType;
	SMOVE_TYPE moveType;
	bool shakeFlag;
	VECTOR2 shakeOffset;
};

