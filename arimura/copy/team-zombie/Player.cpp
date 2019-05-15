#include "Player.h"
#include "MapCtl.h"
#include "Camera.h"
#include "GameTask.h"



Player::Player()
{
	pos = { 0,0 };
	animAdd = 0;
	jump = -12.0f;
	grav = 5.0f;
	wireSpeed = -10.0f;
	wireCnt = 0;
	wireTime = 0;

	state_p = STATE_P::RUN;
	StateTbl[(int)STATE_P::RUN]			= &Player::StateRun;
	StateTbl[(int)STATE_P::JUMP]		= &Player::StateJump;
	StateTbl[(int)STATE_P::FDOWN]		= &Player::StateFdown;
	StateTbl[(int)STATE_P::SET_WIRE]	= &Player::StateSetWire;
	StateTbl[(int)STATE_P::WIRE]		= &Player::StateWire;

	mc = std::make_shared<MouseCtl>();
}


Player::~Player()
{
}

bool Player::Update(void)
{
	memcpy(keyDataOld, keyData, sizeof(keyDataOld));
	GetHitKeyStateAll(keyData);
	(*mc).Update();
	mPos = mc->GetPoint();
	lpGameTask.StartPrgTime();
	SetMove();
	(this->*StateTbl[(int)state_p])();
	animAdd = 1;
	animCnt += animAdd;
	
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

	animAdd = 0;
	
	
	if (lpMapCtl.CheckFloor(pos + VECTOR2(0, 50)))
	{
		if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])
		{
			state_p = STATE_P::JUMP;		//ｼﾞｬﾝﾌﾟ処理
		}
		else
		{
			state_p = STATE_P::RUN;			//走る処理
		}

		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
		{
			state_p = STATE_P::SET_WIRE;

		}
	}
	else if ((lpMapCtl.GetChipType(pos + VECTOR2(0, divSize.y)) == CHIP_TYPE::CHIP_BLANK) && (lpMapCtl.GetChipType(pos + divSize) == CHIP_TYPE::CHIP_BLANK) && (state_p != STATE_P::JUMP))
	{
 		state_p = STATE_P::FDOWN;			//ｼﾞｬﾝﾌﾟの落下処理
	}

	
	//ワイヤー中の時
	//else if (Wireflag)
	//{
	//	if ((int)pos.y <= (int)mPos.y+96)	//マウス座標にプレイヤー座標が当たったら (ちょっとplayer座標ずらしてる)
	//	{
	//		wireTime = 1;
	//		wireCnt = WIRE_CNT;
	//		lpGameTask.EndPrgTime();
	//		Wireflag = false;
	//	}
	//}
	//else
	//{
	//	//ジャンプ、ワイヤー以外の落下処理
	//	if (!DownFlag)
	//	{
	//		animAdd = 0;
	//		SetAnim("ジャンプ");
	//		pos.y -= jump;
	//	}
	//}
	
}

void Player::Draw(void)
{
	Obj::Draw();

	/////////////デバッグ表示
	
	/*vec.x = wire.pos.x - pos.x;
	vec.y = wire.pos.y - pos.y;
	vec.Normalize();*/
	VECTOR2 drawOffset = lpMapCtl.GameDrawOffset();
	DrawFormatString(100, 50, 0x000000, "playerposx:%f", pos.x);
	DrawFormatString(50, 200, 0x000000, "Drawposx:%f", pos.x + drawOffset.x);
	/*DrawFormatString(50, 50, 0x000000, "mouseposx:%f", mPos.x);
	DrawFormatString(50, 100, 0x000000, "playerposy:%f", pos.y);
	DrawFormatString(50, 150, 0x000000, "mouseposy:%f", mPos.y);
	DrawFormatString(50, 300, 0x000000, "wireposx:%f", wire.pos.x);
	DrawFormatString(50, 350, 0x000000, "wireposy:%f", wire.pos.y);
	DrawFormatString(50, 400, 0x000000, "drawOffsetx:%f", drawOffset.x);
	DrawFormatString(50, 400, 0x000000, "drawOffsetx:%f", drawOffset.x);*/
	DrawFormatString(900, 650, 0x000000, "wireCnt:%d", wireCnt);

}

bool Player::initAnim(void)
{
	AddAnim("歩く", 1, 0, 6, 7);
	AddAnim("ジャンプ", 0, 2, 8, 11);
	return true;
}

int Player::StateRun(void)
{
	pos.x += speed;
	jump = -12.0f;
	SetAnim("歩く");
	//wireSpeed = -5.0f;
	
	return 0;
}

int Player::StateJump(void)
{
	pos.y += jump;
	pos.x += speed;
	jump += 0.3f;
	animAdd = 0;
	SetAnim("ジャンプ");
	animAdd = 1;

	if (lpMapCtl.CheckFloor(pos + VECTOR2(0, 50)))
	{
		state_p = STATE_P::RUN;
	}

	return 0;
}


int Player::StateFdown(void)
{
	pos.x += speed;
	pos.y += grav;
	SetAnim("ジャンプ");
	animAdd = 1;
	return 0;
}

int Player::StateSetWire(void)
{
	
	if (wireTime == 1)
	{
		wireCnt = WIRE_CNT + lpGameTask.GetPrgTime();
	}
	if (wireCnt <= 0)
	{
		wire.pos = mPos;
		wireCnt = 0;
		DrawString(850, 750, "ワイヤー使用可能", true);
		//ワイヤー準備
		if (mPos.x > (SCREEN_SIZE_X / 2))
		{
			
			wire.pos.x = (int)mPos.x + (int)pos.x - 512;
			animAdd = 1;
		}
	}
	//else		//ワイヤー準備時
	//{
	//	DrawString(0, 150, "ワイヤー準備", GetColor(0xff, 0xff, 0xff), true);
	//	pos.x -= 7;
	//}
	//else//////////////デバッグ表示
	//{
	//	DrawLine(pos.x + lpMapCtl.GameDrawOffset().x + 32, pos.y + lpMapCtl.GameDrawOffset().y + 42, wire.pos.x + lpMapCtl.GameDrawOffset().x, wire.pos.y, 0xffffff);
	//}
	return 0;
}

int Player::StateWire(void)
{
	////ワイヤー処理
	vec.x = wire.pos.x - pos.x;
	vec.y = wire.pos.y - pos.y;
	vec.Normalize();
	if (mPos.x > (SCREEN_SIZE_X / 2))
	{
		DrawString(0, 100, "ワイヤー", GetColor(0xff, 0xff, 0xff), true);
		pos.x += vec.fx * 16;
		pos.y += vec.fy * 16;
		pos.y -= -3.0f;
		animAdd = 0;
		SetAnim("ジャンプ");
	}
	
	//ワイヤー中の落下時の処理
	/*if (!(lpMapCtl.CheckFloor(pos + VECTOR2(0, 50))))
	{
		pos.y += wireSpeed;
		pos.x += 8;
		wireSpeed += 0.2f;
	}*/
	return 0;
}
