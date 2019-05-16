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

	animAdd = 0;
	
	
	if (lpMapCtl.CheckFloor(pos + VECTOR2(0, 50)) && (state_p != STATE_P::SET_WIRE) && (state_p != STATE_P::WIRE))
	{
		state_p = STATE_P::RUN;												//走る処理

		if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])
		{
			state_p = STATE_P::JUMP;										//ｼﾞｬﾝﾌﾟ処理
		}
		else if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
		{
			state_p = STATE_P::SET_WIRE;
		}
	}
	else if ((lpMapCtl.GetChipType(pos + VECTOR2(0, divSize.y)) == CHIP_TYPE::CHIP_BLANK)		//※stateがJUMP,WIRE,WIRE_DOWNじゃないときに落とす
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
	DrawFormatString(50, 200, 0x000000, "Drawposx:%f", pos.x + drawOffset.x);
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
	wireSpeed = -5.0f;

	return 0;
}

int Player::StateJump(void)
{
	pos.y += jump;
	pos.x += speed;
	jump += 0.3f;
	SetAnim("ジャンプ");
	animAdd = 1;

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

	//ワイヤー準備
	mPos = mc->GetPoint();
	wire.pos = mPos;
	wire.pos.x = (int)mPos.x + (int)pos.x - 512;
	pos.x += speed;
	animAdd = 1;
	pos.x -= 7;

	if (state_p == STATE_P::SET_WIRE)
	{
		wireCnt = 0;
		if ((lpMapCtl.GetChipType(pos + VECTOR2(0, divSize.y)) == CHIP_TYPE::CHIP_BLANK)		//※stateがJUMP,WIRE,WIRE_DOWNじゃないときに落とす
			&& (lpMapCtl.GetChipType(pos + divSize) == CHIP_TYPE::CHIP_BLANK))
		{
			state_p = STATE_P::FDOWN;
		}
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_RIGHT)
		{
			state_p = STATE_P::WIRE;
		}
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
	pos.y += 6.5f;
	SetAnim("ジャンプ");

	if (!lpMapCtl.CheckFloor(pos + VECTOR2(0, 50)))
	{
		if (((int)pos.y <= (int)mPos.y + 96) && (state_p == STATE_P::WIRE))		//マウス座標にプレイヤー座標が当たったら (ちょっとplayer座標ずらしてる)
		{
			mPos = VECTOR2(0, 0);
			state_p = STATE_P::WIRE_DOWN;
		}
	}

	return 0;
}

int Player::StateWireDown(void)
{
	pos.y += wireSpeed;
	pos.x += 8;
	wireSpeed += 0.3f;

	return 0;
}
