#pragma once
#include "Obj.h"
#include "MouseCtl.h"

#define WIRE_CNT 5		//���C���[�g�p�\�̎���
#define MAX_SPARK 10000

struct WIRE {
	VECTOR2 pos;
};

struct IMAGE {
	VECTOR2 pos;
};

typedef struct tagSPARK
{
	int Valid;	// ���̃f�[�^���g�p�����A�t���O
	int X, Y;	// �ΉԂ̈ʒu
	int Sx, Sy;	// �ΉԂ̈ړ���
	int G;	// �ΉԂ̏d��
	int Bright;	// �ΉԂ̖��邳
} SPARK;

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
	bool Wire(void);
private:
	SPARK Spark[MAX_SPARK];	// �Ήԃf�[�^
	WIRE wire;
	bool initAnim(void);
	int animAdd;
	char keyData[256];
	char keyDataOld[256];
	bool Jumpflag;		//�W�����v���t���O
	bool Readyflag;		//���C���[�����t���O
	bool Wireflag;		//���C���[���t���O
	bool DownFlag;		//���C���[���̗����t���O
	void CreateSpark(int x, int y);	// �ΉԂ��o������
	void MoveSpark(void);	// �ΉԈړ�����
	//bool Wireflag;
	
	VECTOR2 mPos;
	int wireCnt;	//�킢��[���[�邽����
	int wireTime;	//���C���[�̃N�[���^�C��
	float jump;
	float wireSpeed;
	IMAGE Image;

	std::shared_ptr<MouseCtl> mc;
};

