#include "Player.h"
#include "MapCtl.h"
#include "ImageMng.h"
#include "GameTask.h"
#include "Effect.h"

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
	cnt = 0;

	for (int i = 0; i < MAX_SPARK; i++)
	{
		Spark[i].Valid = 0;
	}

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
	MoveSpark();
	(this->*StateTbl[(int)state_p])();
	animAdd = 1;
	animCnt += animAdd;

	return true;
}

void Player::SetMove(void)
{
	cnt++;
	lpMapCtl.IfMove(pos);
	if (cnt % 3)
	{
		oldPos = pos;
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

	//ﾌﾟﾚｲﾔｰの状態移動
	if (lpMapCtl.CheckFloor(pos) && (state_p != STATE_P::SET_WIRE) && (state_p != STATE_P::WIRE))
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

	//草ﾌﾞﾛｯｸにおいてのｽﾋﾟｰﾄﾞ減少
	if (lpMapCtl.GetChipType(pos + VECTOR2(32, 33)) == CHIP_TYPE::CHIP_GRASS2)
	{
		speed = 4.0f;
	}
	else
	{
		speed = 8.0f;
	}

	//残像ｴﾌｪｸﾄ
	if ((cnt % 20) == 0)
	{
		lpEffect.AddEffectList("Effect/dash.png", VECTOR2(240, 240), VECTOR2(4, 1), VECTOR2(0, 0), 4, 5, VECTOR2(pos.x - 32, pos.y - 120));
	}
	else
	{
		lpEffect.SetEffPos("Effect/dash.png", VECTOR2(pos.x - 32, pos.y - 120));
	}

	if (state_p == STATE_P::WIRE && (cnt % 10) == 0)
	{
		lpEffect.AddEffectList("a/衝撃32.png", VECTOR2(192, 192), VECTOR2(5, 4), VECTOR2(0, 0), 18, 3, VECTOR2(pos.x - 50, pos.y - 100));
	}
	else
	{
		lpEffect.SetEffPos("a/衝撃32.png", VECTOR2(pos.x - 50, pos.y - 100));

	}

	lpMapCtl.IfMove(pos);
	lpMapCtl.SetPlayerPos(pos);
}

void Player::Draw(void)
{
		//残像
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, 150);
	DrawGraph(oldPos.x + lpMapCtl.GameDrawOffset().x, oldPos.y + lpMapCtl.GameDrawOffset().y, IMAGE_ID("Image/char.png")[1], true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 255);
	
	Obj::Draw();

	int a = GetRand(20);
	int b = GetRand(20);
	int c = GetRand(30);

	//for (int j = 0; j < MAX_SPARK; j++)
	//{
	//	// 火花データが有効な時のみ描画
	//	if (Spark[j].Valid == 1)
	//	{
	//		DrawPixel(Spark[j].X / 100 - 2, Spark[j].Y / 100 + 10, GetColor(0, 255, 255));
	//		DrawPixel(Spark[j].X / 100 - 1, Spark[j].Y / 100 + 10, GetColor(0, 0, 0));
	//		DrawPixel(Spark[j].X / 100 + 1, Spark[j].Y / 100 + 10, GetColor(0, 255, 127));
	//		DrawPixel(Spark[j].X / 100 + 2, Spark[j].Y / 100 + 10, GetColor(255, 255, 255));
	//		//DrawBox(Spark[j].X / 100, Spark[j].Y / 100, Spark[j].X / 100+ a, Spark[j].Y / 100 + b,GetColor(Spark[j].Bright, Spark[j].Bright, Spark[j].Bright), true);
	//	}

	//}

	DrawFormatString(900, 650, 0x000000, "wireCnt:%d", wireCnt);
	//秒数表示
	DrawString(0, SCREEN_SIZE_Y - 20, (std::to_string(lpGameTask.GetPrgTime()) + "秒").c_str(), 0xffffff);
	if (state_p == STATE_P::SET_WIRE || state_p == STATE_P::WIRE)
	{
		DrawLine(pos.x + lpMapCtl.GameDrawOffset().x + 32, pos.y + lpMapCtl.GameDrawOffset().y + 42, wire.pos.x + lpMapCtl.GameDrawOffset().x, wire.pos.y, GetColor(0,0,0));
	}
}

bool Player::initAnim(void)
{
	AddAnim("歩く", 0, 0, 4, 4);
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
		Spark[i].Sx = -GetRand(2000) - 500;
		Spark[i].Sy = GetRand((500));

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
		Spark[i].Bright -= 5;

		// 火花の明るさが０以下になったら火花データを無効にする
		if (Spark[i].Bright < 0) Spark[i].Valid = 0;
	}
}

int Player::StateRun(void)
{
	if (!lpMapCtl.CheckWall(pos + VECTOR2(65, 32)))
	{
		pos.x += speed;
	}
	jump = -11.5f;
	SetAnim("歩く");
	wireSpeed = -7.0f;
	return 0;
}

int Player::StateJump(void)
{
	if (lpMapCtl.CheckWall(pos + VECTOR2(65, 32)))				//ｼﾞｬﾝﾌﾟする際壁抜けたからその処置
	{
		speed = 0;
	}
	if (lpMapCtl.CheckUpBlock(pos) && jump < 0.0f)				//posの上にﾌﾞﾛｯｸがあったらそれ以上上にいかない
	{
		state_p = STATE_P::FDOWN;
	}
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
	lpEffect.SetEffPos("a/光5.png", VECTOR2(pos.x - 32, pos.y-16));

	if (state_p == STATE_P::SET_WIRE)
	{
		if ((lpMapCtl.GetChipType(pos + VECTOR2(0, divSize.y)) == CHIP_TYPE::CHIP_BLANK)
			&& (lpMapCtl.GetChipType(pos + divSize) == CHIP_TYPE::CHIP_BLANK))
		{
			state_p = STATE_P::FDOWN;
		}
		if ((mc->GetBtn()[ST_NOW]) & (~mc->GetBtn()[ST_OLD]) & MOUSE_INPUT_RIGHT)
		{
			lpEffect.AddEffectList("a/光5.png", VECTOR2(192, 192), VECTOR2(5, 3), VECTOR2(0, 0), 15, 3, VECTOR2(pos.x - 64, pos.y-16));
			state_p = STATE_P::WIRE;
		}
		if (wire.pos.x <= (pos.x + 75) || (mPos.x <= 600))
		{
			state_p = STATE_P::RUN;
		}
	}

	return 0;
}

int Player::StateWire(void)
{
	int R = GetRand(500);
	for (int j = 0; j < R; j++)
	{
		CreateSpark(pos.x + lpMapCtl.GameDrawOffset().x + 32, pos.y + lpMapCtl.GameDrawOffset().y + 42);
	}

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

