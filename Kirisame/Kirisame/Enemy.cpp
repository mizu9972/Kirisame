//-----------------------------------------------
//敵キャラクターの処琁E
//-----------------------------------------------
#include "main.h"
#include "Enemy.h"
#include "Asset.h"
#include "config.h"
#include "TexLoad.h"
#include "XAudio2.h"

//攻撁E��たときに画面全体にエフェクトをかけめE
//褁E��の攻撁E��エフェクトが重褁E��なぁE��ぁE��グローバルで定義
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
	//チE��トラクタ

}

void Enemy::Init(COORD GetCoord) {
	//初期匁E

	//受け取った座標情報を反映する
	Coord.X = GetCoord.X;
	Coord.Y = GetCoord.Y;

	//どのマスにぁE��ぁE
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

bool Enemy::Update(COORD PlayerCoord, bool Diveflag, bool Attackflag, COORD ToCoord) {
	//更新処琁E
	bool ReturnBool = false;

	//アニメーション進衁E
	if (AnimeCount > 0.5f * FPS) {//アニメーション速度
		if (Animation > 4) {//アニメーションが�E生しきったらリセチE��
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

	//攻撁E��能なら攻撁E��備に入めE
	if (AttackMode == false && Attackflag == true && Diveflag == 1) {
		AttackMode = true;//攻撁E��備フラグ
		memcpy(&AttackInfo.Coord, &PlayerCoord, sizeof(COORD));//攻撁E��る座標をセチE��
	}
	//初期位置からのプレイヤーの距離計箁E-----------------------------------------------
	COORD SubCoord;

	SubCoord.X = (short)sqrt((ToCoord.X - PlayerCoord.X) * (ToCoord.X - PlayerCoord.X));
	SubCoord.Y = (short)sqrt((ToCoord.Y - PlayerCoord.Y) * (ToCoord.Y - PlayerCoord.Y));
	//----------------------------------------------------------------------------------

	//敵キャラクターとプレイヤーの距離計箁E-------------------------------------------------
	COORD SubPersonalCoord;

	SubPersonalCoord.X = (short)sqrt((Coord.X - PlayerCoord.X) * (Coord.X - PlayerCoord.X));
	SubPersonalCoord.Y = (short)sqrt((Coord.Y - PlayerCoord.Y) * (Coord.Y - PlayerCoord.Y));
	//--------------------------------------------------------------------------------------

	if (Diveflag) {//潜ってぁE��ぁE
				   //プレイヤーの位置によって移動する目標地点を調整する
		if ((SubCoord.X < ENEMY_SEARCH_RAD * MASUWIDTH) && (SubCoord.Y < ENEMY_SEARCH_RAD * MASUHEIGHT)) {
			//プレイヤーがテリトリーに近づぁE��めE

			//プレイヤーの位置を移動目標にする
			MokuhyoCoord.X = PlayerCoord.X;
			MokuhyoCoord.Y = PlayerCoord.Y;
		}
		else {
			//違うなら�E期位置を目標に
			MokuhyoCoord.X = ToCoord.X;
			MokuhyoCoord.Y = ToCoord.Y;
		}

		if ((SubPersonalCoord.X < ENEMY_PERSONAL_SPACE * MASUWIDTH) && (SubPersonalCoord.Y < ENEMY_PERSONAL_SPACE * MASUWIDTH)) {
			//プレイヤーがかなり近くにぁE��なら追ぁE��けめE
			MokuhyoCoord.X = PlayerCoord.X;
			MokuhyoCoord.Y = PlayerCoord.Y;
		}
	}
	else {
		//プレイヤーが潜ってぁE��なら�E期位置を目標に
		MokuhyoCoord.X = ToCoord.X;
		MokuhyoCoord.Y = ToCoord.Y;
	}
	if (isFalling == false) {
		if (ENEMY_MOVESPEED * FPS < MoveCount) {//一定時間ごとに処琁E
			if (AttackMode == true && Diveflag == 1) {
				AttackInfo.Flag = true;//攻撁E��めE
				if ((Coord.X - PlayerCoord.X) * (Coord.X - PlayerCoord.X) + (Coord.Y - PlayerCoord.Y) * (Coord.Y - PlayerCoord.Y) < (MASUWIDTH / 2 * 1.5) * (MASUWIDTH / 2 * 1.5)) {
					//命中
					AttackEffectFlag = true;//エフェクト描画フラグをtrueに
					ReturnBool = AttackMove();//攻撁E�E琁E
				}
				else {
					//外れ
					AttackMode = false;
					if (PollSound(AttackSE) == false) {
						PlaySound(MissAttackSE);//攻撁E��敗SE
					}
				}
			}
			else if (AttackMode == false || Diveflag == 0) {
				AttackInfo.Flag = false;//攻撁E��なぁE
				Move();//移勁E
			}
			MoveCount = 0;//行動したらリセチE��
		}
		MoveCount += 1;
	}

	return ReturnBool;//攻撁E��成功したかどぁE��を返す
}

void Enemy::Move(void) {
	bool MoveFlag = true;//移動可能か判定用

	if (MokuhyoCoord.X == Coord.X && MokuhyoCoord.Y == Coord.Y) {
		return;
	}
	//目標座標と自刁E�E座標�E差を計箁E-------
	COORD SubCoord;

	SubCoord.X = MokuhyoCoord.X - Coord.X;
	SubCoord.Y = MokuhyoCoord.Y - Coord.Y;
	//--------------------------------------

	//X軸とY軸のより目標との距離があるほぁE��優先して移動すめE
	if ((SubCoord.X) * (SubCoord.X) > (SubCoord.Y) * (SubCoord.Y)) {
		//X座標�E方が離れてぁE��場吁E--------------------------------
		if (MoveFlag) {
			//移動可能なら目標座標目持E��て移勁E
			if (SubCoord.X > 0) {
				//右移勁E
				if (MoveFlagXRight) {
					MoveMode = RIGHT_MOVE_MODE;
					Coord.X += ENEMY_SPEED * MASUWIDTH;
					BlockCoord.X += 1;
					MoveFlag = false;
				}
			}
			else if (SubCoord.X < 0) {
				//左移勁E
				if (MoveFlagXLeft) {
					MoveMode = LEFT_MOVE_MODE;
					Coord.X -= ENEMY_SPEED * MASUWIDTH;
					BlockCoord.X -= 1;
					MoveFlag = false;
				}
			}
			else {
				//何もしなぁE
			}
		}

		//X軸の方が離れてぁE��が、E��れなかった時---------------------
		//Y軸方面へ移勁EY軸移動と同じ処琁E
		if (MoveFlag == true && (MoveFlagXRight == false || MoveFlagXLeft == false)) {
			//移動可能なら目標座標目持E��て移勁E
			if (SubCoord.Y > 0) {
				//下移勁E
				if (MoveFlagYDown) {
					MoveMode = DOWN_MOVE_MODE;
					Coord.Y += ENEMY_SPEED * MASUHEIGHT;
					BlockCoord.Y += 1;
					MoveFlag = false;
				}
			}
			else if (SubCoord.Y <= 0) {
				//上移勁E
				if (MoveFlagYUp) {
					MoveMode = UP_MOVE_MODE;
					Coord.Y -= ENEMY_SPEED * MASUHEIGHT;
					BlockCoord.Y -= 1;
					MoveFlag = false;
				}
			}
			else {
				//何もしなぁE
			}
		}
		//----------------------------------------------------------

	}
	else {
		//Y軸の方が離れてぁE��場吁E----------------------------------
		if (MoveFlag) {
			//移動可能なら目標座標目持E��て移勁E
			if (SubCoord.Y > 0) {
				//下移勁E
				if (MoveFlagYDown) {
					MoveMode = DOWN_MOVE_MODE;
					Coord.Y += ENEMY_SPEED * MASUHEIGHT;
					BlockCoord.Y += 1;
					MoveFlag = false;
				}
			}
			else if (SubCoord.Y < 0) {
				//上移勁E
				if (MoveFlagYUp) {
					MoveMode = UP_MOVE_MODE;
					Coord.Y -= ENEMY_SPEED * MASUHEIGHT;
					BlockCoord.Y -= 1;
					MoveFlag = false;
				}
			}
			else {
				//何もしなぁE
			}
		}

		//----------------------------------------------------------


		//Y軸の方が離れてぁE��が、E��れなかった時---------------------
		//X軸方面へ移勁EX軸移動と同じ処琁E
		if (MoveFlag == true && (MoveFlagYUp == false || MoveFlagYDown == false)) {
			//移動可能なら目標座標目持E��て移勁E
			if (SubCoord.X > 0) {
				//右移勁E
				if (MoveFlagXRight) {
					MoveMode = RIGHT_MOVE_MODE;
					Coord.X += ENEMY_SPEED * MASUWIDTH;
					BlockCoord.X += 1;
					MoveFlag = false;
				}
			}
			else if (SubCoord.X <= 0) {
				//左移勁E
				if (MoveFlagXLeft) {
					MoveMode = LEFT_MOVE_MODE;
					Coord.X -= ENEMY_SPEED * MASUWIDTH;
					BlockCoord.X -= 1;
					MoveFlag = false;
				}
			}
			else {
				//何もしなぁE
			}
		}

		//----------------------------------------------------------

	}
	//画面外にぁE��なぁE��ぁE��(X軸)
	while (Coord.X < STAGEPOS_YOKO) {
		Coord.X += ENEMY_SPEED * MASUWIDTH;
		BlockCoord.X += 1;
	}
	while (Coord.X > STAGEPOS_YOKO + YOKOMASU_NUM * MASUWIDTH) {
		Coord.X -= ENEMY_SPEED * MASUWIDTH;
		BlockCoord.X -= 1;
	}
	//画面外にぁE��なぁE��ぁE��(Y軸)
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
	//攻撁E��作�E琁E
	AttackMode = false;
	PlaySound(AttackSE);
	return true;
}

void Enemy::AvoidHole(PieceT RightBlock, PieceT UpBlock, PieceT LeftBlock, PieceT DownBlock) {
	//穴を避けて通る処琁E
	//上下左右に穴があるなら通れなぁE��ぁE��する

	//右
	if (RightBlock.isActive) {
		if (RightBlock.isCut == true || RightBlock.isPassagable == false) {
			MoveFlagXRight = false;
		}
		else {
			MoveFlagXRight = true;
		}
	}
	//丁E
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
	//丁E
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
	//隣のマスにほか�E敵キャラがいる場合その方向への移動をできなくすめE
	//右
	if (BlockCoord.X + 1 == AnotherCoord.X && BlockCoord.Y == AnotherCoord.Y) {
		MoveFlagXRight = false;
	}
	//左
	if (BlockCoord.X - 1 == AnotherCoord.X && BlockCoord.Y == AnotherCoord.Y) {
		MoveFlagXLeft = false;
	}
	//丁E
	if (BlockCoord.Y + 1 == AnotherCoord.Y && BlockCoord.X == AnotherCoord.X) {
		MoveFlagYDown = false;
	}
	//丁E
	if (BlockCoord.Y - 1 == AnotherCoord.Y && BlockCoord.X == AnotherCoord.X) {
		MoveFlagYUp = false;
	}
}
void Enemy::FallintoHole(void) {
	//穴に落ちる�E琁E
	isFalling = true;
	if (Size <= 0) {
		isAlive = false;
		DeathEnemyNum += 1;
	}
	Size -= 1;
}
void Enemy::Draw(bool DiveFlag) {
	//描画処琁E

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
		//攻撁E��フェクト描画
		if (AttackEffectCount < 0.1f * FPS) {
			Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(128, 200, 0, 0));//赤っぽぁE��フェクチE
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
	//チE��トラクタ
}