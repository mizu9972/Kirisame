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

	ui = new UI;

	AllocFlag = false;
	//Enemy_num = ENEMY_STAGE1_NUM;//敵の数
}

Game::~Game(void) {
	//チE��トラクタ
	if (character != NULL) {
		delete character;
	}

	if (stageBoardSystem != NULL) {
		delete stageBoardSystem;
	}
	if (ui != NULL) {
		delete ui;
	}
}

void Game::Init(void) {
	//初期匁E
	switch (Scene) {
	case GAME_STAGE1:
		//brownBear = S1Brownbear;
		if (AllocFlag == false) {
			brownBear = (BrownBear*)malloc(sizeof(BrownBear) * ENEMY_STAGE1_NUM);
			AllocFlag = true;
		}
		else if (AllocFlag == true) {
			brownBear = (BrownBear*)realloc(brownBear, sizeof(BrownBear) * ENEMY_STAGE1_NUM);
		}
		break;

	case GAME_STAGE2:
		//brownBear = S2Brownbear;
		if (AllocFlag == false) {
			brownBear = (BrownBear*)malloc(sizeof(BrownBear) * ENEMY_STAGE2_NUM);
			AllocFlag = true;
		}
		else if (AllocFlag == true) {
			brownBear = (BrownBear*)realloc(brownBear, sizeof(BrownBear) * ENEMY_STAGE2_NUM);
		}
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
	COORD setCoord = { PLAYER_STARTPOS_X ,PLAYER_STARTPOS_Y };
	character->SetCoord(setCoord);

	Dive_State = true;//最初�E潜ってなぁE��態からスターチE
	DeathEnemyNum = 0;
	InitSound();


}

void Game::Edit(void) {
	//直接編雁E
	stageBoardSystem->stage->Edit();

}

void Game::SetCoord(void) {
	//オブジェクト�E座標格紁E
	stageBoardSystem->stage->SetCoord();

}

void Game::EnemyInit(void) {
	//敵の初期匁E
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

void Game::EnemyUnInit(void) {
	free(brownBear);
}

void Game::Draw(void) {
	//描画
	//UIの描画
	if (ui != NULL) {
		ui->UIDraw(Dive_State);
		ui->DrawRestMath(stageBoardSystem->stage->RestMathCheck());
	}
	if (Dive_State)//浮上してるとぁE
	{
		stageBoardSystem->stage->Tu = 0.0f;
		stageBoardSystem->stage->Tv = 0.0f;
	}
	else {//潜ってぁE��晁E
		stageBoardSystem->stage->Tu = 0.5f;
		stageBoardSystem->stage->Tv = 0.0f;
	}
	//スチE�Eジの描画
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
	COORD coord;//プレイヤーの座樁E
	COORD wark_coord;//outcoordでもらったやつ格紁E
	COORD enemyCoord;//敵キャラクターの座樁E
	COORD playerpos;//プレイヤーの座樁E画面冁E�E座樁E
	COORD enemypos;//敵キャラクターの座樁E画面冁E�E座樁E
	COORD ToCoord;//敵キャラクターの初期位置
				  //スチE�Eジ惁E��保管用
	PieceT  blockinfo;
	PieceT tate;
	PieceT yoko;
	PieceT vertex;
	//------------------
	bool DIVE_TRIG = false;//仮想キー
	bool EnemyAttackFlag = false;//攻撁E��能かどぁE��
	bool EnemyAttack = false;//攻撁E�E功したかどぁE��

							 //更新
	if (GetKeyboardTrigger(DIK_SPACE)) {
		DIVE_TRIG = true;
	}
	//コントローラでの入力�E琁E
	if (g_pDIDevGamePad) {
		if (GetGamePadTrigger(0) || GetGamePadTrigger(1) || GetGamePadTrigger(2) || GetGamePadTrigger(3)) {
			DIVE_TRIG = true;
		}
	}


	//プレイヤーの更新-------------------------------
	if (character != NULL)
	{
		character->Update(Dive_State);//引数で潜ってぁE��か�Eフラグを渡ぁE

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
	if (DIVE_TRIG)//潜る、浮上すめE
	{
		tate = stageBoardSystem->stage->OutSide_Tate(coord.X / 2, coord.Y / 2);
		yoko = stageBoardSystem->stage->OutSide_Yoko(coord.X / 2, coord.Y / 2);
		vertex = stageBoardSystem->stage->OutVertexInfo(coord.X / 2, coord.Y / 2);
		if (character->CheckDive(tate.isPassagable, yoko.isPassagable, vertex.isPassagable)) {
			//浮上できる場所か判宁E
			//潜るのを�Eり替える
			Dive_State = !Dive_State;
			PlaySound(SunaSE);
		}
	}

	//敵キャラクターの更新------------------------------------------------------------------------------------------------------------------------------------
	if (brownBear != NULL) {
		for (int UpdateNum = 0; UpdateNum < Enemy_MaxNum; UpdateNum++) {
			if (brownBear[UpdateNum].isAlive) {
				enemyCoord = brownBear[UpdateNum].OutBlockCoord();//座標取り�EぁE
				enemypos = brownBear[UpdateNum].OutPos();

				if ((enemypos.X - playerpos.X)*(enemypos.X - playerpos.X) + (enemypos.Y - playerpos.Y)*(enemypos.Y - playerpos.Y) < (MASUWIDTH / 2 * 1.5f)*(MASUWIDTH / 2 * 1.5f)) {
					//攻撁E��能篁E��冁E��らフラグをtrueに
					EnemyAttackFlag = true;
				}
				else {
					EnemyAttackFlag = false;
				}

				blockinfo = stageBoardSystem->stage->OutBlockInfo(enemyCoord.X, enemyCoord.Y);//足允E�Eマス惁E��取り出ぁE
				if (blockinfo.isCut) {
					//穴に落ちめE
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
						continue;//自刁E�E身はスキチE�E
					}
					if (brownBear[avoidNum].isAlive == false) {
						continue;//死んだ敵はスキチE�E
					}
					brownBear[UpdateNum].AvoidAnotherEnemy(brownBear[avoidNum].OutBlockCoord());
				}
				switch (Scene) {
				case GAME_STAGE1:
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
					break;

				case GAME_STAGE2:
					switch (UpdateNum) {
					case 0:
						ToCoord.X = ENEMY3_DEFPOS_X;
						ToCoord.Y = ENEMY3_DEFPOS_Y;
						break;

					case 1:
						ToCoord.X = ENEMY4_DEFPOS_X;
						ToCoord.Y = ENEMY4_DEFPOS_Y;
						break;

					case 2:
						ToCoord.X = ENEMY5_DEFPOS_X;
						ToCoord.Y = ENEMY5_DEFPOS_Y;
						break;

					case 3:
						ToCoord.X = ENEMY6_DEFPOS_X;
						ToCoord.Y = ENEMY6_DEFPOS_Y;
						break;

					case 4:
						ToCoord.X = ENEMY7_DEFPOS_X;
						ToCoord.Y = ENEMY7_DEFPOS_Y;
						break;
					}
					break;
				}
				EnemyAttack = brownBear[UpdateNum].Update(character->OutPos(), Dive_State, EnemyAttackFlag, ToCoord);
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------
	if (EnemyAttack) {

		//プレイヤーへダメージ
		character->Hit();

	}
	//スチE�Eジ惁E��の更新
	if (stageBoardSystem != NULL) {
		if (Dive_State)//潜ってぁE��ぁE��のみ刁E��取る
		{
			stageBoardSystem->CutBoard(coord);//キャラクターの位置を�EめE
			stageBoardSystem->BoardUpdate(coord);
			stageBoardSystem->stage->Check_Passagable();//刁E��取られたところには行けなぁE��ぁE��するめE��
			stageBoardSystem->stage->CheckCakeFall();//ケーキが�Eり落ちてるか
		}
		stageBoardSystem->stage->FallingCake();//ケーキが落ちてぁE��処琁E
	}
	ui->TIME();
}


void Game::Gettime(void) {
	ui->GTIME();
}

bool Game::OutClearFlg(void)//敵ぁEならtrueを返す
{
	if (DeathEnemyNum == Enemy_MaxNum) {
		return true;
	}
	return false;
}