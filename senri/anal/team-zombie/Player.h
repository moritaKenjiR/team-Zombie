#pragma once
#include "Obj.h"
#include "MouseCtl.h"


#define WIRE_CNT 5		//���C���[�g�p�\�̎���
#define MAX_SPARK 10000

struct WIRE {
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

enum class STATE_P
{
	RUN,
	JUMP,
	FDOWN,
	SET_WIRE,
	WIRE,
	WIRE_DOWN,
	MAX
};


class Player :
	public Obj
{
public:
	Player();
	~Player();

	bool Update(void);
	void SetMove(void);
	void Draw(void);

	void ChangeState_P(STATE_P state_p)
	{
		this->state_p = state_p;
	};
	STATE_P GetState(void) { return state_p; }
private:
	SPARK Spark[MAX_SPARK];	// �Ήԃf�[�^
	WIRE wire;
	STATE_P state_p;
	bool initAnim(void);
	void CreateSpark(int x, int y);	// �ΉԂ��o������
	void MoveSpark(void);	// �ΉԈړ�����

	int animAdd;
	int wireCnt;	//�킢��[���[�邽����
	int wireTime;	//���C���[�̃N�[���^�C��
	char keyData[256];
	char keyDataOld[256];
	float jump;
	float grav;
	float wireSpeed;
	
	VECTOR2 mPos;
	VECTOR2 oldPos;
	int cnt;

	std::shared_ptr<MouseCtl> mc;

	int (Player::*StateTbl[(int)STATE_P::MAX])(void);
	//��ڲ԰�̃��[�u��Ԃ̕ێ�
	int StateRun(void);
	int StateJump(void);
	int StateFdown(void);
	int StateSetWire(void);
	int StateWire(void);
	int StateWireDown(void);};
