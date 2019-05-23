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
	//デバッグ用
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

	//うまくいってない
	if (lpMapCtl.CheckFloor(pos + VECTOR2(0, 50 )))
	{
		Jumpflag = false;
		if (!(Jumpflag) && !(Wireflag))
		{

			pos.x += speed;
			SetAnim("歩く");
			DrawString(0, 100, "走り中", GetColor(0xff, 0xff, 0xff), true);
			jump = -12.0f;
			wireSpeed = -5.0f;
			DownFlag = false;

		}
		animAdd = 1;


		if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])
		{
			Jumpflag = true;
			DrawString(0, 100, "ジャンプ中", GetColor(0xff, 0xff, 0xff), true);
			pos.y += jump;
			pos.x += speed;
			jump += 0.3f;
			animAdd = 0;
			SetAnim("ジャンプ");
			animAdd = 1;
		}
	}
	//ワイヤー中の時
	else if (Wireflag)
	{
		if ((int)pos.y <= (int)mPos.y+96)	//マウス座標にプレイヤー座標が当たったら (ちょっとplayer座標ずらしてる)
		{
			wireTime = 1;
			wireCnt = WIRE_CNT;
			lpGameTask.EndPrgTime();
			Wireflag = false;
		}
	}
	//ジャンプの落下処理
	else if (Jumpflag && !(DownFlag))
	{
		DrawString(0, 100, "ジャンプ中", GetColor(0xff, 0xff, 0xff), true);
		pos.y += jump;
		pos.x += speed;
		jump += 0.3f;
		animAdd = 0;
		SetAnim("ジャンプ");
		animAdd = 1;
	}
	else
	{
		//ジャンプ、ワイヤー以外の落下処理
		if (!DownFlag)
		{
			animAdd = 0;
			SetAnim("ジャンプ");
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
		// 火花データが有効な時のみ描画
		if (Spark[j].Valid == 1)
		{
			DrawPixel(Spark[j].X / 100-30, Spark[j].Y / 100 + 10, GetColor(0, 255, 255));
			DrawPixel(Spark[j].X / 100- 10, Spark[j].Y / 100 + 10, GetColor(0, 0, 0));
			DrawPixel(Spark[j].X / 100+ 10, Spark[j].Y / 100 + 10, GetColor(255, 215, 0));
			DrawPixel(Spark[j].X / 100+30, Spark[j].Y / 100+10, GetColor(255, 255, 255));
		}

	}
	/////////////デバッグ表示
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
		DrawString(850, 750, "ワイヤー使用可能", true);
		//ワイヤー準備
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT && (Wireflag == false))
		{
			mPos = mc->GetPoint();
			wire.pos = mPos;
			wire.pos.x = (int)mPos.x + (int)pos.x - 512;
			Readyflag = true;
			animAdd = 1;
		}
	}

	//ワイヤー準備時、プレイヤーのスピードを下げる (いるかわからん)
	if (Readyflag)
	{
		DrawString(0, 150, "ワイヤー準備", GetColor(0xff, 0xff, 0xff), true);
		pos.x -= 7;
	}

	//プレイヤーより後ろ、下にワイヤーで飛べないようにする処理
	if ((mPos.x < 600) && (Readyflag) || wire.pos.x <= pos.x + 50 || wire.pos.y >= pos.y - 50)
	{
		Readyflag = false;
	}

	///////////////デバッグ表示
	if (Readyflag || Wireflag || DownFlag)
	{
		DrawLine(pos.x + lpMapCtl.GameDrawOffset().x + 32, pos.y + lpMapCtl.GameDrawOffset().y + 42, wire.pos.x + lpMapCtl.GameDrawOffset().x, wire.pos.y, 0xffffff);
	}

	//ワイヤー処理
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
			SetAnim("ジャンプ");
		}
	}
	//ワイヤー中の落下時の処理
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
	AddAnim("歩く", 1, 0, 6, 7);
	AddAnim("ジャンプ", 0, 2, 8, 11);
	return true;
}

void Player::CreateSpark(int x, int y)
{
	int i;

	// 使われていない火花データを探す
	for (i = 0; i < MAX_SPARK; i++)
	{
		if (Spark[i].Valid == 0) break;
	}
	if (i != MAX_SPARK)
	{
		// 火花の位置を設定
		Spark[i].X = x * 100;
		Spark[i].Y = y * 100;

		// 移動力を設定
		Spark[i].Sx = -GetRand(2000) - 300;
		Spark[i].Sy = GetRand((800));

		// 火花の重さをセット
		Spark[i].G = GetRand(1);

		// 火花の明るさセット
		Spark[i].Bright = 255;

		// 火花データを使用中にセット
		Spark[i].Valid = 1;
	}
}

void Player::MoveSpark(void)
{
	int i;

	// 火花の移動処理
	for (i = 0; i < MAX_SPARK; i++)
	{
		// 火花データが無効だったらスキップ
		if (Spark[i].Valid == 0) continue;

		// 位置を移動力に応じてずらす
		Spark[i].Y += Spark[i].Sy;
		Spark[i].X += Spark[i].Sx;

		// 移動力を変更
		Spark[i].Sy += Spark[i].G;

		// 火花の明るさを下げる
		Spark[i].Bright -= 2;

		// 火花の明るさが０以下になったら火花データを無効にする
		if (Spark[i].Bright < 0) Spark[i].Valid = 0;
	}
}
