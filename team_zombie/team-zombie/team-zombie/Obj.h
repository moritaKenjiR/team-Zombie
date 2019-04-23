#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <string>
#include "VECTOR2.h"

//��Ұ��݁@ID,�@�ϐ�,�@��Ұ��݂̊Ԋu
enum ANIM_TBL
{
	ANIM_START_ID,
	ANIM_FLAME,
	ANIM_WAIT,
	ANIM_MAX
};

class Obj
{
public:
	Obj(const char(&_keyData)[256], const char(&_keyDataOld)[256], VECTOR2 chipOffset);
	~Obj();
	void init(std::string filename, VECTOR2 divSize, VECTOR2 divCnt, VECTOR2 chipOffset, int speed, int animSpeed, int stateAnimDiv);

	const VECTOR2& GetPos(void);
	void SetPos(const VECTOR2& pos);

	virtual bool Update(void);		//�X�V����
	virtual void Draw(void);		//�`��֐�
	void Draw(int id);
	bool SetAnim(std::string _AnimName);
private:
	//���O���::����ڰĖ�<�^> �ϐ���;
	// std::vector<�^> �ϐ���;
	
	//
	std::map<std::string, int[ANIM_MAX]> animMap;
	std::string animName;					// ��Ұ��݂̖��O
	//
protected:
	std::string imageName;
	const char(&keyData)[256];
	const char(&keyDataOld)[256];
	VECTOR2 pos;			//�`��̈ʒu
	int speed;
	VECTOR2 divSize;		//�`�����߂̻���
	VECTOR2 divCnt;			//�`�����߂̐�(�c���j
	VECTOR2 chipOffset;		//�`�����߂̲Ұ��
	VECTOR2 drawOffset;
	int animCnt;
	int animSpeed;
	int stateAnimDiv;

	bool AddAnim(std::string AnimName, int id_x, int id_y, int flame, int waitCnt);
	bool turn;
};

