#include "Player.h"
#include "MapCtl.h"
#include "Camera.h"
#include "GameTask.h"
#include <string>



Player::Player()
{
	pos = { 0,0 };
	animAdd = 0;
	jump = -12.0f;
	grav = 5.0f;
	wireSpeed = -5.0f;
	wireCnt = 0;
	wireTime = 0;

	mc = std::make_shared<MouseCtl>();

	//ﾃｰﾌﾞﾙを作ってそれぞれの関数にﾌﾟﾚｲﾔｰの状態を保持させてる
	state_p = STATE_P::RUN;
	StateTbl[(int)STATE_P::RUN] = &Player::StateRun;
	StateTbl[(int)STATE_P::JUMP] = &Player::StateJump;
	StateTbl[(int)STATE_P::FDOWN] = &Player::StateFdown;
	StateTbl[(int)STATE_P::SET_WIRE] = &Player::StateSetWire;
	StateTbl[(int)STATE_P::WIRE] = &Player::StateWire;
	StateTbl[(int)STATE_P::WIRE_DOWN] = &Player::StateWireDown;
}

Player::~Player()
{
}

bool Player::Update(void)
{
	memcpy(keyDataOld, keyData, sizeof(keyDataOld));
	GetHitKeyStateAll(keyData);

	lpGameTask.StartPrgTime();
	(*mc).Update();
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

	//ﾜｲﾔｰのｸｰﾙﾀｲﾑ関係
	if (wireTime == 1)
	{
		wireCnt = WIRE_CNT + lpGameTask.GetPrgTime();
	}
	if (wireCnt <= 0)
	{
		wireCnt = 0;
	}

	if (lpMapCtl.CheckFloor(pos + VECTOR2(0, 50)) && (state_p != STATE_P::SET_WIRE) && (state_p != STATE_P::WIRE))
	{
		state_p = STATE_P::RUN;
		if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])
		{
			state_p = STATE_P::JUMP;										//ｼﾞｬﾝﾌﾟ処理
		}
		else if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
		{
			mPos = mc->GetPoint();
			if (wireCnt <= 0)
			{
				state_p = STATE_P::SET_WIRE;								//ﾜｲﾔｰ準備処理
			}
		}
	}
	else if ((lpMapCtl.GetChipType(pos + VECTOR2(0, divSize.y)) == CHIP_TYPE::CHIP_BLANK)
		 && (lpMapCtl.GetChipType(pos + divSize) == CHIP_TYPE::CHIP_BLANK) && (state_p == STATE_P::RUN))
	{
		state_p = STATE_P::FDOWN;											//ｼﾞｬﾝﾌﾟの落下処理
	}
}

void Player::Draw(void)
{
	Obj::Draw();
	/////////////デバッグ表示
	VECTOR2 drawOffset = lpMapCtl.GameDrawOffset();
	DrawFormatString(100, 50, 0x000000, "playerposx:%f", pos.x);
	DrawFormatString(100, 200, 0x000000, "Drawposx:%f", pos.x + drawOffset.x);
	DrawFormatString(900, 650, 0x000000, "wireCnt:%d", wireCnt);
	//秒数表示
	DrawString(0, SCREEN_SIZE_Y - 20, (std::to_string(lpGameTask.GetPrgTime()) + "秒").c_str(), 0xffffff);

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
	wireSpeed = -7.0f;

	return 0;
}

int Player::StateJump(void)
{
	pos.y += jump;
	pos.x += speed;
	jump += 0.3f;
	SetAnim("ジャンプ");

	return 0;
}

int Player::StateFdown(void)
{
	pos.x += speed;
	pos.y += grav;
	SetAnim("ジャンプ");

	return 0;
}

int Player::StateSetWire(void)
{
	//ワイヤー準備
	if (mPos.x > (SCREEN_SIZE_X / 2))
	{
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
		{
			mPos = mc->GetPoint();
			wire.pos = mPos;
			wire.pos.x = (int)mPos.x + (int)pos.x - 512;
		}
	}
	pos.x += speed;
	pos.x -= 7;

	if (state_p == STATE_P::SET_WIRE)
	{
		if ((lpMapCtl.GetChipType(pos + VECTOR2(0, divSize.y)) == CHIP_TYPE::CHIP_BLANK)		//※stateがJUMP,WIRE,WIRE_DOWNじゃないときに落とす
			&& (lpMapCtl.GetChipType(pos + divSize) == CHIP_TYPE::CHIP_BLANK))
		{
			state_p = STATE_P::FDOWN;
		}
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_RIGHT)
		{
			state_p = STATE_P::WIRE;
		}
		if (wire.pos.x <= (pos.x + 75) ||(mPos.x <= 600))
		{
			state_p = STATE_P::RUN;
		}
	}

	if (state_p == STATE_P::SET_WIRE)
	{
		DrawLine(pos.x + lpMapCtl.GameDrawOffset().x + 32, pos.y + lpMapCtl.GameDrawOffset().y + 42, wire.pos.x + lpMapCtl.GameDrawOffset().x, wire.pos.y, 0xffffff);
	}

	return 0;
}

int Player::StateWire(void)
{
	DrawString(0, 100, "ワイヤー", GetColor(0xff, 0xff, 0xff), true);
	//ワイヤー処理
	VECTOR2 vec;
	vec.x = wire.pos.x - pos.x;
	vec.y = wire.pos.y - pos.y;
	vec.Normalize();
	pos.x += vec.fx * 16;
	pos.y += vec.fy * 16;
	pos.y += grav;
	SetAnim("ジャンプ");

	if (!lpMapCtl.CheckFloor(pos + VECTOR2(0, 50)))
	{
		if (((int)pos.y <= (int)mPos.y + 96) && (state_p == STATE_P::WIRE))		//マウス座標にプレイヤー座標が当たったら (ちょっとplayer座標ずらしてる)
		{
			wireTime = 1;
			wireCnt = WIRE_CNT;
			lpGameTask.EndPrgTime();
			state_p = STATE_P::WIRE_DOWN;
		}
	}
	//////////////デバッグ表示
	if (state_p == STATE_P::WIRE)
	{
		DrawLine(pos.x + lpMapCtl.GameDrawOffset().x + 32, pos.y + lpMapCtl.GameDrawOffset().y + 42, wire.pos.x + lpMapCtl.GameDrawOffset().x, wire.pos.y, 0xffffff);
	}
	
	return 0;
}

int Player::StateWireDown(void)
{
	pos.y += wireSpeed;
	pos.x += speed;
	wireSpeed += 0.25f;

	return 0;
}
