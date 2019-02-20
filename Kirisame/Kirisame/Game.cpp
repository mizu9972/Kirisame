#include "Game.h"
#include "XAudio2.h"

extern LPDIRECTINPUTDEVICE8 g_pDIDevGamePad;
extern long JoypadDI_X;
extern long JoypadDI_Y;
int DeathEnemyNum = 0;
Game::Game(void) {
	//コンストラクタ
	character = new Character;
	stageBoardSystem = new StageBoardSystem;
	//S2Brownbear = new BrownBear[ENEMY_STAGE2_NUM];//err
	//S1Brownbear = new BrownBear[ENEMY_STAGE1_NUM];


	ui = new UI;
	//Enemy_num = ENEMY_STAGE1_NUM;//敵の数
}

Game::~Game(void) {
	//デストラクタ
	if (character != NULL) {
		delete character;
	}
	//if (S1Brownbear != NULL) {
	//	S1Brownbear = NULL;
	//	delete[] S1Brownbear;
	//}
	//if (S2Brownbear != NULL) {
	//	S2Brownbear = NULL;
	//	delete[] S2Brownbear;
	//}
	//if (brownBear != NULL) {
	//	delete[] brownBear;
	//}
	
	if (stageBoardSystem != NULL) {
		delete stageBoardSystem;
	}
	if (ui != NULL) {
		delete ui;
	}
}

void Game::Init(void) {
	//初期化
	switch (Scene) {
	case GAME_STAGE1:
		brownBear = S1Brownbear;
		break;

	case GAME_STAGE2:
		brownBear = S2Brownbear;
		break;
	}
	if (brownBear != NULL) {
		EnemyInit();
	}
	if (stageBoardSystem != NULL) {
		stageBoardSystem->InitBoard();
		stageBoardSystem->InitCheckedBoard();
		stageBoardSystem->stage->Init();
	}
	Dive_State = true;//最初は潜ってない状態からスタート
	DeathEnemyNum = 0;
	InitSound();

	PlaySound(TitleBGM);
}

void Game::Edit(void) {
	//直接編集
	stageBoardSystem->stage->Edit();

}

void Game::SetCoord(void) {
	//オブジェクトの座標格納
	stageBoardSystem->stage->SetCoord();

}

void Game::EnemyInit(void) {
	//敵の初期化
	COORD coord[ENEMY_STAGE1_NUM + ENEMY_STAGE2_NUM];
	switch (Scene) {
	case GAME_STAGE1:
		Enemy_MaxNum = ENEMY_STAGE1_NUM;


		coord[0].X = ENEMY1_DEFPOS_X;
		coord[0].Y = ENEMY1_DEFPOS_Y;
		coord[1].X = ENEMY2_DEFPOS_X;
		coord[1].Y = ENEMY2_DEFPOS_Y;
		break;

	case GAME_STAGE2:
		Enemy_MaxNum = ENEMY_STAGE2_NUM;


		coord[0].X = ENEMY3_DEFPOS_X;
		coord[0].Y = ENEMY3_DEFPOS_Y;
		coord[1].X = ENEMY4_DEFPOS_X;
		coord[1].Y = ENEMY4_DEFPOS_Y;
		coord[2].X = ENEMY5_DEFPOS_X;
		coord[2].Y = ENEMY5_DEFPOS_Y;
		coord[3].X = ENEMY6_DEFPOS_X;
		coord[3].Y = ENEMY6_DEFPOS_Y;
		coord[4].X = ENEMY7_DEFPOS_X;
		coord[4].Y = ENEMY7_DEFPOS_Y;
		break;
	}

	if (brownBear != NULL) {
		for (int InitNum = 0; InitNum < Enemy_MaxNum; InitNum++) {
			brownBear[InitNum].Init(coord[InitNum]);
		}
	}
}

void Game::Draw(void) {
	//描画
	//UIの描画
	if (ui != NULL) {
		ui->UIDraw(Dive_State);
	}
	if (Dive_State)//浮上してるとき
	{
		stageBoardSystem->stage->Tu = 0.0f;
		stageBoardSystem->stage->Tv = 0.0f;
	}
	else {//潜っている時
		stageBoardSystem->stage->Tu = 0.5f;
		stageBoardSystem->stage->Tv = 0.0f;
	}
	//ステージの描画
	if (stageBoardSystem != NULL) {
		stageBoardSystem->stage->BlockDraw();
		stageBoardSystem->stage->SideDraw();
		stageBoardSystem->stage->WallDraw();
		stageBoardSystem->stage->RockDraw(Dive_State);
		stageBoardSystem->stage->CakeDraw();
	}
	//プレイヤーの描画
	if (character != NULL) {
		character->Draw();
	}
	//敵キャラクターの描画
	if (brownBear != NULL) {
		for (int DrawNum = 0; DrawNum < Enemy_MaxNum; DrawNum++) {
			if (brownBear[DrawNum].isAlive) {
				brownBear[DrawNum].Draw(Dive_State);
			}
		}
	}

}

void Game::Update(void) {
	COORD coord;//プレイヤーの座標
	COORD wark_coord;//outcoordでもらったやつ格納
	COORD enemyCoord;//敵キャラクターの座標
	COORD playerpos;//プレイヤーの座標(画面内の座標)
	COORD enemypos;//敵キャラクターの座標(画面内の座標)
	COORD ToCoord;//敵キャラクターの初期位置
	//ステージ情報保管用
	PieceT  blockinfo;
	PieceT tate;
	PieceT yoko;
	PieceT vertex;
	//------------------
	bool DIVE_TRIG = false;//仮想キー
	bool EnemyAttackFlag = false;//攻撃可能かどうか
	bool EnemyAttack = false;//攻撃成功したかどうか

	//更新
	if (GetKeyboardTrigger(DIK_SPACE)) {
		DIVE_TRIG = true;
	}
	//コントローラでの入力処理
	if (g_pDIDevGamePad) {
		if (GetGamePadTrigger(0) || GetGamePadTrigger(1) || GetGamePadTrigger(2) || GetGamePadTrigger(3)) {
			DIVE_TRIG = true;
		}
	}


	//プレイヤーの更新-------------------------------
	if (character != NULL)
	{
		character->Update(Dive_State);//引数で潜っているかのフラグを渡す

		wark_coord = character->OutCoord();//キャラの頂点座標受け取っておく

		tate = stageBoardSystem->stage->OutSide_Tate(wark_coord.X / 2, wark_coord.Y / 2);
		yoko = stageBoardSystem->stage->OutSide_Yoko(wark_coord.X / 2, wark_coord.Y / 2);
		if (Dive_State == 1) {
			character->CheckMove(tate.isPassagable, yoko.isPassagable);//戻すやつ
		}
		coord = character->OutCoord();
		playerpos = character->OutPos();
	}
	//-----------------------------------------------
	if (DIVE_TRIG)//潜る、浮上する
	{
		tate = stageBoardSystem->stage->OutSide_Tate(coord.X / 2, coord.Y / 2);
		yoko = stageBoardSystem->stage->OutSide_Yoko(coord.X / 2, coord.Y / 2);
		vertex = stageBoardSystem->stage->OutVertexInfo(coord.X / 2, coord.Y / 2);
		if (character->CheckDive(tate.isPassagable,yoko.isPassagable,vertex.isPassagable)) {
			//浮上できる場所か判定
			//潜るのを切り替える
			Dive_State = !Dive_State;
			PlaySound(SunaSE);
		}
	}

	//敵キャラクターの更新------------------------------------------------------------------------------------------------------------------------------------
	if (brownBear != NULL) {
		for (int UpdateNum = 0; UpdateNum < Enemy_MaxNum; UpdateNum++) {
			if (brownBear[UpdateNum].isAlive) {
				enemyCoord = brownBear[UpdateNum].OutBlockCoord();//座標取り出し
				enemypos = brownBear[UpdateNum].OutPos();

				if ((enemypos.X - playerpos.X)*(enemypos.X - playerpos.X) + (enemypos.Y - playerpos.Y)*(enemypos.Y - playerpos.Y) < (MASUWIDTH / 2 * 1.5f)*(MASUWIDTH / 2 * 1.5f)) {
					//攻撃可能範囲内ならフラグをtrueに
					EnemyAttackFlag = true;
				}
				else {
					EnemyAttackFlag = false;
				}

				blockinfo = stageBoardSystem->stage->OutBlockInfo(enemyCoord.X,enemyCoord.Y);//足元のマス情報取り出し
				if (blockinfo.isCut) {
					//穴に落ちる
					brownBear[UpdateNum].FallintoHole();
					if (PollSound(FallSE) == false) {
						PlaySound(FallSE);
					}
				}

				//穴を避ける
				brownBear[UpdateNum].AvoidHole(
					stageBoardSystem->stage->OutBlockInfo(enemyCoord.X + 1, enemyCoord.Y),
					stageBoardSystem->stage->OutBlockInfo(enemyCoord.X, enemyCoord.Y - 1),
					stageBoardSystem->stage->OutBlockInfo(enemyCoord.X - 1, enemyCoord.Y),
					stageBoardSystem->stage->OutBlockInfo(enemyCoord.X, enemyCoord.Y + 1)
				);

				//別の敵キャラを避ける
				
					for (int avoidNum = 0; avoidNum < Enemy_MaxNum; avoidNum++) {
						if (avoidNum == UpdateNum) {
							continue;//自分自身はスキップ
						}
						if (brownBear[avoidNum].isAlive == false) {
							continue;//死んだ敵はスキップ
						}
						brownBear[UpdateNum].AvoidAnotherEnemy(brownBear[avoidNum].OutBlockCoord());
					}
				
				switch (UpdateNum) {
				case 0:
					ToCoord.X = ENEMY1_DEFPOS_X;
					ToCoord.Y = ENEMY1_DEFPOS_Y;
					break;

				case 1:
					ToCoord.X = ENEMY2_DEFPOS_X;
					ToCoord.Y = ENEMY2_DEFPOS_Y;
					break;
				}
				EnemyAttack = brownBear[UpdateNum].Update(character->OutPos(), Dive_State, EnemyAttackFlag,ToCoord);
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------
	if (EnemyAttack) {
		//プレイヤーへダメージ
		character->Hit();

	}
	//ステージ情報の更新
	if (stageBoardSystem != NULL) {
		if (Dive_State)//潜っていない時のみ切り取る
		{
			stageBoardSystem->CutBoard(coord);//キャラクターの位置を切る
			stageBoardSystem->BoardUpdate(coord);
			stageBoardSystem->stage->Check_Passagable();//切り取られたところには行けないようにするやつ
			stageBoardSystem->stage->CheckCakeFall();//ケーキが切り落ちてるか
			stageBoardSystem->stage->FallingCake();
		}
	}
	ui->TIME();
}


void Game::Gettime(void) {
	ui->GTIME();
}

bool Game::OutClearFlg(void)//敵が0ならtrueを返す
{
	//for (int num = 0; num < Enemy_MaxNum; num++)
	//{
	//	//クマさん配列の生存フラグを参照していって全員が消えてたら
	//	//敵の数が0になる処理
	//	if (brownBear[num].isAlive == false)
	//	{
	//		Enemy_num--;
	//	}
	//}
	//if (Enemy_num >  0)
	//{
	//	Enemy_num = Enemy_MaxNum;//敵1体倒すだけでカウントが0になる対策
	//	return false;
	//}
	//else if (Enemy_num <= 0)
	//{
	//	return true;
	//}
	//return false;
	if (DeathEnemyNum == Enemy_MaxNum) {
		return true;
	}
	return false;
}