#include "Player.h"
#include "MapCtl.h"
#include "Camera.h"
#include "GameTask.h"



Player::Player(std::unique_ptr<Camera>camera)
{
	pos = { 50,700 };
	animAdd = 0;
	jump = -12.0f;
	wireSpeed = -10.0f;

	Jumpflag = false;
	Readyflag = false;
	Wireflag = false;
	DownFlag = false;

	wireCnt = 3;

	mc = std::make_shared<MouseCtl>();
	this->camera = std::move(camera);
}


Player::~Player()
{
}

bool Player::Update(void)
{
	lpGameTask.StartPrgTime();
	Wire();
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
		if ((int)pos.y <= (int)mPos.y+96)	//マウス座標のにプレイヤー座標が当たったら (ちょっとplayer座標ずらしてる)
		{
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

	/////////////デバッグ表示
	VECTOR2 vec;
	vec.x = wire.pos.x - pos.x;
	vec.y = wire.pos.y - pos.y;
	vec.Normalize();
	VECTOR2 drawOffset = lpMapCtl.GameDrawOffset();
	DrawCircle(pos.x + drawOffset.x + 32, pos.y + drawOffset.y + 42,10, 0xffffff);
	DrawFormatString(50, 0, 0x000000, "playerposx:%f", pos.x);
	DrawFormatString(50, 50, 0x000000, "mouseposx:%f", mPos.x);
	DrawFormatString(50, 100, 0x000000, "playerposy:%f", pos.y);
	DrawFormatString(50, 150, 0x000000, "mouseposy:%f", mPos.y);
	DrawFormatString(50, 300, 0x000000, "wireposx:%f", wire.pos.x);
	DrawFormatString(50, 350, 0x000000, "wireposy:%f", wire.pos.y);
	DrawFormatString(50, 400, 0x000000, "drawOffsetx:%f", drawOffset.x);
	DrawFormatString(50, 400, 0x000000, "drawOffsetx:%f", drawOffset.x);
	DrawFormatString(900, 650, 0x000000, "wireCnt:%d", wireCnt);
	///////////////////
}

bool Player::Wire(void)
{
	wireCnt = -lpGameTask.GetPrgTime();
	(*mc).Update();
	if (wireCnt >= 3)
	{
		wireCnt = 3;
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
	///////////////デバッグ表示
	if (Readyflag || Wireflag || DownFlag)
	{
		DrawLine(pos.x + lpMapCtl.GameDrawOffset().x + 32, pos.y + lpMapCtl.GameDrawOffset().y + 42, wire.pos.x + lpMapCtl.GameDrawOffset().x, wire.pos.y, 0xffffff);
	}
	//ワイヤー処理
	if ((((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_RIGHT) && (Readyflag == true)) || (Wireflag == true))
	{
		wireCnt = 3;
		Wireflag = true;
		DownFlag = true;
		Readyflag = false;

		VECTOR2 vec;
		vec.x = wire.pos.x- pos.x;
		vec.y = wire.pos.y - pos.y;
		vec.Normalize();
		DrawString(0, 100, "ワイヤー", GetColor(0xff, 0xff, 0xff), true);
		pos.x += vec.fx * 18;
		pos.y += vec.fy * 20;
		pos.y -=-2.0f;
		animAdd = 0;
		SetAnim("ジャンプ");
	}
	//ワイヤー中の落下時の処理
	if (!(lpMapCtl.CheckFloor(pos + VECTOR2(0, 50))) && (Wireflag == false)&& (DownFlag == true))
	{
		pos.y += wireSpeed;
		pos.x += 7;
		wireSpeed += 0.15f;
	}

	return true;
}

bool Player::initAnim(void)
{
	AddAnim("歩く", 1, 0, 6, 7);
	AddAnim("ジャンプ", 0, 2, 8, 11);
	return true;
}
