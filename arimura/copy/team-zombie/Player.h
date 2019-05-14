#pragma once
#include "Obj.h"
#include "MouseCtl.h"

#define WIRE_CNT 5		//���C���[�g�p�\�̎���

struct WIRE {
	VECTOR2 pos;
};

class Camera;

class Player :
	public Obj
{
public:
	Player();
	//Player(const char(&_keyData)[256], const char(&_keyDataOld)[256], VECTOR2 chipOffset);
	~Player();

	bool Update(void);
	void SetMove(void);
	void Draw(void);
	bool SetWire(void);
	bool Wire(void);
private:
	WIRE wire;
	bool initAnim(void);
	int animAdd;
	char keyData[256];
	char keyDataOld[256];
	bool Jumpflag;		//�W�����v���t���O
	bool Readyflag;		//���C���[�����t���O
	bool Wireflag;		//���C���[���t���O
	bool DownFlag;		//���C���[���̗����t���O
	//bool Wireflag;
	
	VECTOR2 mPos;
	int wireCnt;	//�킢��[���[�邽����
	int wireTime;	//���C���[�̃N�[���^�C��
	float jump;
	float wireSpeed;

	std::shared_ptr<MouseCtl> mc;
};

