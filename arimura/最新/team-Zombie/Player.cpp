#include "Player.h"
#include "MapCtl.h"
#include "ImageMng.h"
#include "Effect.h"
#include "Shaker.h"

Player::Player()
{
	pos = { 0,0 };
	animAdd = 0;
	jump = -1.5f;
	grav = 5.0f;
	offset = 171;
	wireSpeed = -5.0f;
	jumpLimit = false;
	mc = std::make_shared<MouseCtl>();

	cnt = 0;
	timer = 300;

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
	(*mc).Update();
	SetMove();
	(this->*StateTbl[(int)state_p])();
	animAdd = 1;
	animCnt += animAdd;
	return true;
}

void Player::SetMove(void)
{
	animAdd = 0;
	cnt++;
	lpMapCtl.IfMove(pos);
	if (cnt % 3)
	{
		oldPos = pos;
	}
	if (lpMapCtl.CheckFloor(pos) && (state_p != STATE_P::SET_WIRE) && (state_p != STATE_P::WIRE))
	{
		state_p = STATE_P::RUN;
		if (keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE])
		{
			lpEffect.AddEffectList("Effect/jump.png", VECTOR2(240, 240), VECTOR2(6, 1), VECTOR2(0, 0), 6, 5, VECTOR2(pos.x - 96, pos.y - 120));
			state_p = STATE_P::JUMP;										//ｼﾞｬﾝﾌﾟ処理
		}
		else if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
		{
			mPos = mc->GetPoint();
			state_p = STATE_P::SET_WIRE;								//ﾜｲﾔｰ準備処理
		}
	}
	else if ((lpMapCtl.GetChipType(pos + VECTOR2(0, divSize.y)) == CHIP_TYPE::CHIP_BLANK)
		&& (lpMapCtl.GetChipType(pos + divSize) == CHIP_TYPE::CHIP_BLANK) && (state_p == STATE_P::RUN))
	{
		state_p = STATE_P::FDOWN;											//ｼﾞｬﾝﾌﾟの落下処理
	}

	//草村の時のｽﾋﾟｰﾄﾞの変化
	if ((lpMapCtl.GetChipType(pos + VECTOR2(32, 33)) == CHIP_TYPE::CHIP_GRASS2) ||((state_p == STATE_P::RUN) && (state_p == STATE_P::SET_WIRE)))
	{
		speed = 4.0f;
	}
	else
	{
		speed = 7.0f;
	}
	//落ちた時に終了
	if (lpMapCtl.GetChipType(pos + VECTOR2(32, 32)) == CHIP_FIRE)
	{
		lpMapCtl.SetEndFlag(true);
	}
	lpMapCtl.SetPlayerPos(pos);
}

void Player::Draw(void)
{
	//残像
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawGraph(oldPos.x + lpMapCtl.GameDrawOffset().x, oldPos.y + lpMapCtl.GameDrawOffset().y, IMAGE_ID("Image/char.png")[1], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	
	if (state_p == STATE_P::WIRE && (cnt % 10) == 0)
	{
		//ワイヤーエフェクト(仮)
		lpEffect.AddEffectList("wireEff/wEf2.png", VECTOR2(192, 192), VECTOR2(5, 4), VECTOR2(0, 0), 18, 3, VECTOR2(pos.x - 80, pos.y - 70));
	}

	Obj::Draw();

	//////////////デバッグ表示
	if (state_p == STATE_P::SET_WIRE || state_p == STATE_P::WIRE)
	{
		DrawLine(pos.x + lpMapCtl.GameDrawOffset().x + 32, pos.y + lpMapCtl.GameDrawOffset().y + 42, wire.pos.x + lpMapCtl.GameDrawOffset().x + 300, wire.pos.y, GetColor(0, 0, 0));
	}
	
	DrawFormatString(0,100,0xffffff, "timer:%d" ,timer);
}

int Player::StateRun(void)
{
	if (lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, divSize.y / 2)) && (!lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, 0)))) 
	{
		pos.y -= 10;
	}
	else if (lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, divSize.y / 2)) || (lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, 0))))
	{
		speed = 0;
	}

	if ((cnt % 20) == 0)
	{
		lpEffect.AddEffectList("Effect/dash.png", VECTOR2(240, 240), VECTOR2(4, 1), VECTOR2(0, 0), 4, 5, VECTOR2(pos.x - 32, pos.y - 120));
	}
	pos.x += speed;
	jump = -1.5f;
	jumpLimit = false;
	SetAnim("歩く");
	wireSpeed = -7.0f;
	return 0;
}

int Player::StateJump(void)
{
	//ｼﾞｬﾝﾌﾟの限界と小ｼﾞｬﾝ
	if (!jumpLimit)
	{
		if (keyData[KEY_INPUT_SPACE] && keyDataOld[KEY_INPUT_SPACE] && jump > -10.0f)
		{
			jump -= 1.5f;
		}
		else if (jump < -10.0f || (!keyData[KEY_INPUT_SPACE] && !keyDataOld[KEY_INPUT_SPACE]))
		{
			jumpLimit = true;
		}
	}

	if (lpMapCtl.CheckUpBlock(pos) && jump < 0.0f)				//posの上にﾌﾞﾛｯｸがあったらそれ以上上にいかない
	{
		state_p = STATE_P::FDOWN;
	}
	else
	{
		pos.y += jump;
	}

	if (!lpMapCtl.CheckWall(pos + VECTOR2(65, 32)))
	{
		pos.x += speed;
	}
	jump += 0.3f;

	return 0;
}

int Player::StateFdown(void)
{
	if (lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, divSize.y / 2)) && (!lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, 0))))
	{
		pos.y -= 10;
	}
	//pos.x += speed;
	pos.y += grav;

	return 0;
}

int Player::StateSetWire(void)
{
	int wireOffset = (lpMapCtl.GetViewAreaSize().x / 3);
	if (lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, divSize.y / 2)))
	{
		speed = 0;
		state_p = STATE_P::RUN;
		return 0;
	}
	//ワイヤー準備
	//if (mPos.x > wireOffset)
	{
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_LEFT)
		{
				mPos = mc->GetPoint();
				wire.pos = mPos;
				wire.pos.x = (int)mPos.x + (int)pos.x - 512;
				lpEffect.AddEffectList("wireEff/溜め3.png", VECTOR2(192, 192), VECTOR2(5, 3), VECTOR2(0, 0), 15, 2, VECTOR2(pos.x - 50, pos.y - 64));
		}
	}
	
	pos.x += speed;

	if (state_p == STATE_P::SET_WIRE)
	{
		if ((lpMapCtl.GetChipType(pos + VECTOR2(0, divSize.y)) == CHIP_TYPE::CHIP_BLANK))
		{
			state_p = STATE_P::FDOWN;
		}
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_RIGHT)
		{
			//ワイヤーエフェクト(仮)
			lpEffect.AddEffectList("wireEff/wEf1.png", VECTOR2(192, 192), VECTOR2(5, 2), VECTOR2(0, 0), 10, 3, VECTOR2(pos.x - 64, pos.y - 120));
			state_p = STATE_P::WIRE;
		}
		if (wire.pos.x <= (pos.x - wireOffset + offset))
		{
			state_p = STATE_P::RUN;
		}
	}

	return 0;
}

int Player::StateWire(void)
{
	vec.x = wire.pos.x + offset - pos.x;
	vec.y = wire.pos.y - pos.y;
	vec.Normalize();
	if (!lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, divSize.y / 2)))
	{
		pos.x += vec.fx * 16;
		pos.y += vec.fy * 16;
	}
	else
	{
		state_p = STATE_P::WIRE_DOWN;
	}
	pos.y += grav;
	SetAnim("ジャンプ");

	if (!lpMapCtl.CheckFloor(pos + VECTOR2(0, 50)))
	{
		if (((int)pos.y <= (int)mPos.y ) && (state_p == STATE_P::WIRE))		//マウス座標にプレイヤー座標が当たったら (ちょっとplayer座標ずらしてる)
		{
			state_p = STATE_P::WIRE_DOWN;
		}
		if (lpMapCtl.CheckUpBlock(pos) && ((vec.fy) <= pos.y))				//posの上にﾌﾞﾛｯｸがあったらそれ以上上にいかない
		{
			state_p = STATE_P::FDOWN;
		}
	}
	return 0;
}

int Player::StateWireDown(void)
{
	if (lpMapCtl.CheckWall(pos + VECTOR2(divSize.x, divSize.y / 2)))
	{
		speed = 0;
	}
	if (lpMapCtl.CheckUpBlock(pos) && ((vec.fy) <= pos.y))				//posの上にﾌﾞﾛｯｸがあったらそれ以上上にいかない
	{
		state_p = STATE_P::FDOWN;
	}
	pos.y += wireSpeed;
	pos.x += speed;
	wireSpeed += 0.25f;

	return 0;
}

bool Player::initAnim(void)
{
	AddAnim("歩く", 0, 0, 4, 4);
	return true;
}