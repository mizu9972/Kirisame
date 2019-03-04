//-----------------------------------------------
//敵キャラクターの処理
//-----------------------------------------------
#include "main.h"
#include "Enemy.h"
#include "Asset.h"
#include "config.h"
#include "TexLoad.h"
#include "XAudio2.h"

//攻撃したときに画面全体にエフェクトをかける
//複数の攻撃でエフェクトが重複しないようにグローバルで定義
bool AttackEffectFlag = false;
int AttackEffectCount = 0;

extern int DeathEnemyNum;

Enemy::Enemy(void) {
	//コンストラクタ
	MoveFlagXRight = true;
	MoveFlagYUp = true;
	MoveFlagXLeft = true;
	MoveFlagYDown = true;

	isFalling = false;
	AttackMode = false;
	Attack = false;
	isAlive = true;
}

Enemy::~Enemy(void) {
	//デストラクタ

}

void Enemy::Init(COORD GetCoord) {
	//初期化

	//受け取った座標情報を反映する
	Coord.X = GetCoord.X;
	Coord.Y = GetCoord.Y;
	
	//どのマスにいるか
	BlockCoord.X = ((GetCoord.X - (STAGEPOS_YOKO - (MASUWIDTH / 2))) / MASUWIDTH) - 1;
	BlockCoord.Y = ((GetCoord.Y - (STAGEPOS_TATE - (MASUHEIGHT / 2))) / MASUHEIGHT) - 1;

	Size = ENEMY_SIZE;
	isAlive = true;
	  MoveFlagXRight = true;
	  MoveFlagYUp = true;
	  MoveFlagXLeft = true;
	  MoveFlagYDown = true;

	  isFalling = false;
	  AttackMode = false;
	  Attack = false;

	  MoveMode = RIGHT_MOVE_MODE;//移動パターン
	  MoveCount = 0;
	  Animation = 0;//アニメーション用変数
	  AnimeCount = 0;//アニメーション速度
	  DrawCount = (int)(ENEMY_MOVESPEED * 2 * FPS);

	  Texture = TexOp->BrownBearTex;
	  AttackInfo.Texture = TexOp->EnemyAttackTex;
	  DiveTexture = TexOp->EnemySunabokoriTex;

	  MoveFlagXRight = true;
	  MoveFlagYUp = true;
	  MoveFlagXLeft = true;
	  MoveFlagYDown = true;

	  isFalling = false;
	  AttackMode = false;
	  Attack = false;
	  isAlive = true;

}

bool Enemy::Update(COORD PlayerCoord,bool Diveflag,bool Attackflag,COORD ToCoord) {
	//更新処理
	bool ReturnBool = false;

	//アニメーション進行
	if (AnimeCount > 0.5f * FPS) {//アニメーション速度
		if (Animation > 4) {//アニメーションが再生しきったらリセット
			Animation = 0;
		}
		else {
			Animation += 1;
		}
		AnimeCount = 0;
	}
	else {
		AnimeCount += 1;
	}
	
	//攻撃可能なら攻撃準備に入る
	if (AttackMode == false && Attackflag == true && Diveflag == 1) {
		AttackMode = true;//攻撃準備フラグ
		memcpy(&AttackInfo.Coord, &PlayerCoord, sizeof(COORD));//攻撃する座標をセット
	}
	//初期位置からのプレイヤーの距離計算------------------------------------------------
	COORD SubCoord;

	SubCoord.X = (short)sqrt((ToCoord.X - PlayerCoord.X) * (ToCoord.X - PlayerCoord.X));
	SubCoord.Y = (short)sqrt((ToCoord.Y - PlayerCoord.Y) * (ToCoord.Y - PlayerCoord.Y));
	//----------------------------------------------------------------------------------

	//敵キャラクターとプレイヤーの距離計算--------------------------------------------------
	COORD SubPersonalCoord;

	SubPersonalCoord.X = (short)sqrt((Coord.X - PlayerCoord.X) * (Coord.X - PlayerCoord.X));
	SubPersonalCoord.Y = (short)sqrt((Coord.Y - PlayerCoord.Y) * (Coord.Y - PlayerCoord.Y));
	//--------------------------------------------------------------------------------------

	if (Diveflag) {//潜っているか
		//プレイヤーの位置によって移動する目標地点を調整する
		if ((SubCoord.X < ENEMY_SEARCH_RAD * MASUWIDTH) && (SubCoord.Y < ENEMY_SEARCH_RAD * MASUHEIGHT)) {
			//プレイヤーがテリトリーに近づいたら

			//プレイヤーの位置を移動目標にする
			MokuhyoCoord.X = PlayerCoord.X;
			MokuhyoCoord.Y = PlayerCoord.Y;
		}
		else {
			//違うなら初期位置を目標に
			MokuhyoCoord.X = ToCoord.X;
			MokuhyoCoord.Y = ToCoord.Y;
		}

		if ((SubPersonalCoord.X < ENEMY_PERSONAL_SPACE * MASUWIDTH) && (SubPersonalCoord.Y < ENEMY_PERSONAL_SPACE * MASUWIDTH)) {
			//プレイヤーがかなり近くにいるなら追い続ける
			MokuhyoCoord.X = PlayerCoord.X;
			MokuhyoCoord.Y = PlayerCoord.Y;
		}
	}
	else {
		//プレイヤーが潜っているなら初期位置を目標に
		MokuhyoCoord.X = ToCoord.X;
		MokuhyoCoord.Y = ToCoord.Y;
	}
	if (isFalling == false) {
		if (ENEMY_MOVESPEED * FPS < MoveCount) {//一定時間ごとに処理
			if (AttackMode == true && Diveflag == 1) {
				AttackInfo.Flag = true;//攻撃する
				if ((Coord.X - PlayerCoord.X) * (Coord.X - PlayerCoord.X) + (Coord.Y - PlayerCoord.Y) * (Coord.Y - PlayerCoord.Y) < (MASUWIDTH / 2 * 1.5) * (MASUWIDTH / 2 * 1.5)) {
					//命中
					AttackEffectFlag = true;//エフェクト描画フラグをtrueに
					ReturnBool = AttackMove();//攻撃処理
				}
				else {
					//外れ
					AttackMode = false;
					if (PollSound(AttackSE) == false) {
						PlaySound(MissAttackSE);//攻撃失敗SE
					}
				}
			}
			else if (AttackMode == false || Diveflag == 0){
				AttackInfo.Flag = false;//攻撃しない
				Move();//移動
			}
			MoveCount = 0;//行動したらリセット
		}
		MoveCount += 1;
	}

	return ReturnBool;//攻撃に成功したかどうかを返す
}

void Enemy::Move(void) {
	bool MoveFlag = true;//移動可能か判定用

	if (MokuhyoCoord.X == Coord.X && MokuhyoCoord.Y == Coord.Y) {
		return;
	}
	//目標座標と自分の座標の差を計算--------
	COORD SubCoord;

	SubCoord.X = MokuhyoCoord.X - Coord.X;
	SubCoord.Y = MokuhyoCoord.Y - Coord.Y;
	//--------------------------------------

		//X軸とY軸のより目標との距離があるほうを優先して移動する
	if ((SubCoord.X) * (SubCoord.X) > (SubCoord.Y) * (SubCoord.Y)) {
		//X座標の方が離れている場合---------------------------------
		if (MoveFlag) {
			//移動可能なら目標座標目指して移動
			if (SubCoord.X > 0) {
				//右移動
				if (MoveFlagXRight) {
					MoveMode = RIGHT_MOVE_MODE;
					Coord.X += ENEMY_SPEED * MASUWIDTH;
					BlockCoord.X += 1;
					MoveFlag = false;
				}
			}
			else if (SubCoord.X < 0) {
				//左移動
				if (MoveFlagXLeft) {
					MoveMode = LEFT_MOVE_MODE;
					Coord.X -= ENEMY_SPEED * MASUWIDTH;
					BlockCoord.X -= 1;
					MoveFlag = false;
				}
			}
			else {
				//何もしない
			}
		}

		//X軸の方が離れているが、通れなかった時---------------------
		//Y軸方面へ移動(Y軸移動と同じ処理)
		if (MoveFlag == true && (MoveFlagXRight == false || MoveFlagXLeft == false)) {
			//移動可能なら目標座標目指して移動
			if (SubCoord.Y > 0) {
				//下移動
				if (MoveFlagYDown) {
					MoveMode = DOWN_MOVE_MODE;
					Coord.Y += ENEMY_SPEED * MASUHEIGHT;
					BlockCoord.Y += 1;
					MoveFlag = false;
				}
			}
			else if (SubCoord.Y <= 0) {
				//上移動
				if (MoveFlagYUp) {
					MoveMode = UP_MOVE_MODE;
					Coord.Y -= ENEMY_SPEED * MASUHEIGHT;
					BlockCoord.Y -= 1;
					MoveFlag = false;
				}
			}
			else {
				//何もしない
			}
		}
		//----------------------------------------------------------

	}
	else{
		//Y軸の方が離れている場合-----------------------------------
		if (MoveFlag ) {
			//移動可能なら目標座標目指して移動
			if (SubCoord.Y > 0) {
				//下移動
				if (MoveFlagYDown) {
					MoveMode = DOWN_MOVE_MODE;
					Coord.Y += ENEMY_SPEED * MASUHEIGHT;
					BlockCoord.Y += 1;
					MoveFlag = false;
				}
			}
			else if (SubCoord.Y < 0) {
				//上移動
				if (MoveFlagYUp) {
					MoveMode = UP_MOVE_MODE;
					Coord.Y -= ENEMY_SPEED * MASUHEIGHT;
					BlockCoord.Y -= 1;
					MoveFlag = false;
				}
			}
			else{
				//何もしない
			}
		}

		//----------------------------------------------------------


		//Y軸の方が離れているが、通れなかった時---------------------
		//X軸方面へ移動(X軸移動と同じ処理)
		if (MoveFlag == true && (MoveFlagYUp == false || MoveFlagYDown == false)) {
			//移動可能なら目標座標目指して移動
			if (SubCoord.X > 0) {
				//右移動
				if (MoveFlagXRight) {
					MoveMode = RIGHT_MOVE_MODE;
					Coord.X += ENEMY_SPEED * MASUWIDTH;
					BlockCoord.X += 1;
					MoveFlag = false;
				}
			}
			else if (SubCoord.X <= 0) {
				//左移動
				if (MoveFlagXLeft) {
					MoveMode = LEFT_MOVE_MODE;
					Coord.X -= ENEMY_SPEED * MASUWIDTH;
					BlockCoord.X -= 1;
					MoveFlag = false;
				}
			}
			else {
				//何もしない
			}
		}

		//----------------------------------------------------------

	}
	//画面外にいかないように(X軸)
	while (Coord.X < STAGEPOS_YOKO) {
		Coord.X += ENEMY_SPEED * MASUWIDTH;
		BlockCoord.X += 1;
	}
	while (Coord.X > STAGEPOS_YOKO + YOKOMASU_NUM * MASUWIDTH) {
		Coord.X -= ENEMY_SPEED * MASUWIDTH;
		BlockCoord.X -= 1;
	}
	//画面外にいかないように(Y軸)
	while (Coord.Y < STAGEPOS_TATE) {
		Coord.Y += ENEMY_SPEED * MASUHEIGHT;
		BlockCoord.Y += 1;
	}
	while (Coord.Y >= STAGEPOS_TATE + TATEMASU_NUM * MASUHEIGHT) {
		Coord.Y -= ENEMY_SPEED * MASUWIDTH;
		BlockCoord.Y -= 1;
	}
}

bool Enemy::AttackMove(void) {
	//攻撃動作処理
	AttackMode = false;
	PlaySound(AttackSE);
	return true;
}

void Enemy::AvoidHole(PieceT RightBlock, PieceT UpBlock, PieceT LeftBlock, PieceT DownBlock) {
	//穴を避けて通る処理
	//上下左右に穴があるなら通れないようにする

	//右
	if (RightBlock.isActive) {
		if (RightBlock.isCut == true || RightBlock.isPassagable == false) {
			MoveFlagXRight = false;
		}
		else {
			MoveFlagXRight = true;
		}
	}
	//上
	if (UpBlock.isActive) {
		if (UpBlock.isCut == true || UpBlock.isPassagable == false) {
			MoveFlagYUp = false;
		}
		else {
			MoveFlagYUp = true;
		}
	}
	//左
	if (LeftBlock.isActive) {
		if (LeftBlock.isCut == true || LeftBlock.isPassagable == false) {
			MoveFlagXLeft = false;
		}
		else {
			MoveFlagXLeft = true;
		}
	}
	//下
	if (DownBlock.isActive) {
		if (DownBlock.isCut == true || DownBlock.isPassagable == false) {
			MoveFlagYDown = false;
		}
		else {
			MoveFlagYDown = true;
		}
	}
}

void Enemy::AvoidAnotherEnemy(COORD AnotherCoord) {
	//隣のマスにほかの敵キャラがいる場合その方向への移動をできなくする
	//右
	if (BlockCoord.X + 1 == AnotherCoord.X && BlockCoord.Y == AnotherCoord.Y) {
		MoveFlagXRight = false;
	}
	//左
	if (BlockCoord.X - 1 == AnotherCoord.X && BlockCoord.Y == AnotherCoord.Y) {
		MoveFlagXLeft = false;
	}
	//下
	if (BlockCoord.Y + 1 == AnotherCoord.Y && BlockCoord.X == AnotherCoord.X) {
		MoveFlagYDown = false;
	}
	//上
	if (BlockCoord.Y - 1 == AnotherCoord.Y && BlockCoord.X == AnotherCoord.X) {
		MoveFlagYUp = false;
	}
}
void Enemy::FallintoHole(void) {
	//穴に落ちる処理
	isFalling = true;
	if (Size <= 0) {
		isAlive = false;
		DeathEnemyNum += 1;
	}
	Size -= 1;
}
void Enemy::Draw(bool DiveFlag) {
	//描画処理

	if (DiveFlag) {
		//敵キャラクター描画
		Draw2dPolygon(Coord.X - Size / 2, Coord.Y - Size / 2, Size, Size, D3DCOLOR_ARGB(255, 255, 255, 255), Texture, 0.25f * Animation, 0.25f * MoveMode, 0.25f, 0.25f);
	}
	else {
		if (ENEMY_MOVESPEED * 3 * FPS < DrawCount) {

			memcpy(&SunabokoriCoord, &Coord, sizeof(COORD));
			//砂ぼこり描画
			Draw2dPolygon(SunabokoriCoord.X - Size / 2, SunabokoriCoord.Y - Size / 2, Size, Size, D3DCOLOR_ARGB(255, 255, 255, 255), DiveTexture, 0, 0, 1, 1);
			DrawCount = 0;
		}

		DrawCount += 1;

	}
	if (AttackInfo.Flag) {
		if (AttackInfo.Count < 0.2f * FPS) {
			//爪痕描画
			Draw2dPolygon(AttackInfo.Coord.X - MASUWIDTH / 2.0f, AttackInfo.Coord.Y - MASUHEIGHT / 2.0f, MASUWIDTH, MASUHEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), AttackInfo.Texture, 0, 0, 1.0f, 1.0f);
			AttackInfo.Count += 1;
		}
		else {
			AttackInfo.Count = 0;
			AttackInfo.Flag = false;
		}
	}
	if (AttackEffectFlag) {
		//攻撃エフェクト描画
		if (AttackEffectCount < 0.1f * FPS) {
			Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(128, 200, 0, 0));//赤っぽいエフェクト
			AttackEffectCount += 1;
		}
		else {
			AttackEffectCount = 0;
			AttackEffectFlag = false;
		}

	}
}

COORD Enemy::OutBlockCoord(void) {
	return BlockCoord;
}

COORD Enemy::OutPos(void) {
	return Coord;
}

BrownBear::BrownBear(void) {
	//コンストラクタ
	Texture = TexOp->BrownBearTex;
	AttackInfo.Texture = TexOp->EnemyAttackTex;
	DiveTexture = TexOp->EnemySunabokoriTex;
}

BrownBear::~BrownBear(void) {
	//デストラクタ
}