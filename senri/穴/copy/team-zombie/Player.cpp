#include "Player.h"
#include "MapCtl.h"
#include "Camera.h"
#include "GameTask.h"
#include "ImageMng.h"



Player::Player()
{
	pos = { 50,700 };
	animAdd = 0;
	jump = -12.0f;
	wireSpeed = -10.0f;

	Image.pos = VECTOR2(-1024, 81);

	Jumpflag = false;
	Readyflag = false;
	Wireflag = false;
	DownFlag = false;

	wireCnt = 0;
	wireTime = 0;

	for (int i = 0; i < MAX_SPARK; i++)
	{
		Spark[i].Valid = 0;
	}

	mc = std::make_shared<MouseCtl>();
}


Player::~Player()
{
}

bool Player::Update(void)
{
	lpGameTask.StartPrgTime();
	Wire();
	MoveSpark();
	SetMove();
	return true;
}

void Player::SetMove(void)
{
	//�f�o�b�O�p
	if (pos.y < 0)
	{
		pos.y = 0;
	}
	if (pos.x < 0)
	{
		pos.x = 0;
	}

	memcpy(keyDataOld, keyData, sizeof(keyDataOld));
	GetHitKeyStateAll(keyData);
	animAdd = 0;

	//���܂������ĂȂ�
	if (lpMapCtl.CheckFloor(pos + VECTOR2(0, 50 )))
	{
		Jumpflag = false;
		if (!(Jumpflag) && !(Wireflag))
		{

			pos.x += speed;
			SetAnim("����");
			DrawString(0, 100, "���蒆", GetColor(0xff, 0xff, 0xff), true);
			jump = -12.0f;
			wireSpeed = -5.0f;
			DownFlag = false;

		}
		animAdd = 1;


		if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])
		{
			Jumpflag = true;
			DrawString(0, 100, "�W�����v��", GetColor(0xff, 0xff, 0xff), true);
			pos.y += jump;
			pos.x += speed;
			jump += 0.3f;
			animAdd = 0;
			SetAnim("�W�����v");
			animAdd = 1;
		}
	}
	//���C���[���̎�
	else if (Wireflag)
	{
		if ((int)pos.y <= (int)mPos.y+96)	//�}�E�X���W�Ƀv���C���[���W������������ (�������player���W���炵�Ă�)
		{
			wireTime = 1;
			wireCnt = WIRE_CNT;
			lpGameTask.EndPrgTime();
			Wireflag = false;
		}
	}
	//�W�����v�̗�������
	else if (Jumpflag && !(DownFlag))
	{
		DrawString(0, 100, "�W�����v��", GetColor(0xff, 0xff, 0xff), true);
		pos.y += jump;
		pos.x += speed;
		jump += 0.3f;
		animAdd = 0;
		SetAnim("�W�����v");
		animAdd = 1;
	}
	else
	{
		//�W�����v�A���C���[�ȊO�̗�������
		if (!DownFlag)
		{
			animAdd = 0;
			SetAnim("�W�����v");
			pos.y -= jump;
		}
	}
	animCnt += animAdd;
}

void Player::Draw(void)
{
	Obj::Draw();
	int a = GetRand(255);
	int b = GetRand(255);
	int c = GetRand(255);

	for (int j = 0; j < MAX_SPARK; j++)
	{
		// �Ήԃf�[�^���L���Ȏ��̂ݕ`��
		if (Spark[j].Valid == 1)
		{
			DrawPixel(Spark[j].X / 100-30, Spark[j].Y / 100 + 10, GetColor(0, 255, 255));
			DrawPixel(Spark[j].X / 100- 10, Spark[j].Y / 100 + 10, GetColor(0, 0, 0));
			DrawPixel(Spark[j].X / 100+ 10, Spark[j].Y / 100 + 10, GetColor(255, 215, 0));
			DrawPixel(Spark[j].X / 100+30, Spark[j].Y / 100+10, GetColor(255, 255, 255));
		}

	}
	/////////////�f�o�b�O�\��
	DrawFormatString(900, 650, 0x000000, "wireCnt:%d", wireCnt);
	///////////////////
}

bool Player::Wire(void)
{
	if (wireTime == 1)
	{
		wireCnt = WIRE_CNT + lpGameTask.GetPrgTime();
	}

	(*mc).Update();
	if (wireCnt <= 0)

	{
		wireCnt = 0;
		DrawString(850, 750, "���C���[�g�p�\", true);
		//���C���[����
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT && (Wireflag == false))
		{
			mPos = mc->GetPoint();
			wire.pos = mPos;
			wire.pos.x = (int)mPos.x + (int)pos.x - 512;
			Readyflag = true;
			animAdd = 1;
		}
	}

	//���C���[�������A�v���C���[�̃X�s�[�h�������� (���邩�킩���)
	if (Readyflag)
	{
		DrawString(0, 150, "���C���[����", GetColor(0xff, 0xff, 0xff), true);
		pos.x -= 7;
	}

	//�v���C���[�����A���Ƀ��C���[�Ŕ�ׂȂ��悤�ɂ��鏈��
	if ((mPos.x < 600) && (Readyflag) || wire.pos.x <= pos.x + 50 || wire.pos.y >= pos.y - 50)
	{
		Readyflag = false;
	}

	///////////////�f�o�b�O�\��
	if (Readyflag || Wireflag || DownFlag)
	{
		DrawLine(pos.x + lpMapCtl.GameDrawOffset().x + 32, pos.y + lpMapCtl.GameDrawOffset().y + 42, wire.pos.x + lpMapCtl.GameDrawOffset().x, wire.pos.y, 0xffffff);
	}

	//���C���[����
	if ((((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_RIGHT) && (Readyflag == true)) || (Wireflag == true))
	{
		//DrawGraph(Image.pos.x, Image.pos.y, IMAGE_ID("Image/1.png")[0], true);
		int a = GetRand(3);

		Image.pos.x += 1024;
		if (Image.pos.x == 0)
		{

			int R = GetRand(500);
			for (int j = 0; j < R; j++)
			{
				CreateSpark(pos.x + lpMapCtl.GameDrawOffset().x + 32, pos.y + lpMapCtl.GameDrawOffset().y + 70);
			}
			switch (a)
			{
			case 0:
				Image.pos.x += 9;
				break;
			case 1:
				Image.pos.x -= 9;
				break;
			case 2:
				Image.pos.y += 9;
				break;
			case 3:
				Image.pos.y -= 9;
				break;
			default:
				break;
			}
			//DrawGraph(Image.pos.x, Image.pos.y, IMAGE_ID("Image/1.png")[0], false);
			Image.pos = VECTOR2(-1024, 81);

			Wireflag = true;
			DownFlag = true;
			Readyflag = false;

			VECTOR2 vec;
			vec.x = wire.pos.x - pos.x;
			vec.y = wire.pos.y - pos.y;
			vec.Normalize();
			//DrawString(pos.x + lpMapCtl.GameDrawOffset().x - 100, pos.y + lpMapCtl.GameDrawOffset().y, "", GetColor(0, 0, 0), true);
			pos.x += vec.fx * 16;
			pos.y += vec.fy * 16;
			pos.y -= -3.0f;
			animAdd = 0;
			SetAnim("�W�����v");
		}
	}
	//���C���[���̗������̏���
	if (!(lpMapCtl.CheckFloor(pos + VECTOR2(0, 50))) && (Wireflag == false)&& (DownFlag == true))
	{
		pos.y += wireSpeed;
		pos.x += 8;
		wireSpeed += 0.2f;
	}
	return true;
}

bool Player::initAnim(void)
{
	AddAnim("����", 1, 0, 6, 7);
	AddAnim("�W�����v", 0, 2, 8, 11);
	return true;
}

void Player::CreateSpark(int x, int y)
{
	int i;

	// �g���Ă��Ȃ��Ήԃf�[�^��T��
	for (i = 0; i < MAX_SPARK; i++)
	{
		if (Spark[i].Valid == 0) break;
	}
	if (i != MAX_SPARK)
	{
		// �ΉԂ̈ʒu��ݒ�
		Spark[i].X = x * 100;
		Spark[i].Y = y * 100;

		// �ړ��͂�ݒ�
		Spark[i].Sx = -GetRand(2000) - 300;
		Spark[i].Sy = GetRand((800));

		// �ΉԂ̏d�����Z�b�g
		Spark[i].G = GetRand(1);

		// �ΉԂ̖��邳�Z�b�g
		Spark[i].Bright = 255;

		// �Ήԃf�[�^���g�p���ɃZ�b�g
		Spark[i].Valid = 1;
	}
}

void Player::MoveSpark(void)
{
	int i;

	// �ΉԂ̈ړ�����
	for (i = 0; i < MAX_SPARK; i++)
	{
		// �Ήԃf�[�^��������������X�L�b�v
		if (Spark[i].Valid == 0) continue;

		// �ʒu���ړ��͂ɉ����Ă��炷
		Spark[i].Y += Spark[i].Sy;
		Spark[i].X += Spark[i].Sx;

		// �ړ��͂�ύX
		Spark[i].Sy += Spark[i].G;

		// �ΉԂ̖��邳��������
		Spark[i].Bright -= 2;

		// �ΉԂ̖��邳���O�ȉ��ɂȂ�����Ήԃf�[�^�𖳌��ɂ���
		if (Spark[i].Bright < 0) Spark[i].Valid = 0;
	}
}
