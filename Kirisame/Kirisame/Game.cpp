#include "Game.h"
#include "XAudio2.h"

extern LPDIRECTINPUTDEVICE8 g_pDIDevGamePad;
extern long JoypadDI_X;
extern long JoypadDI_Y;
int DeathEnemyNum = 0;
Game::Game(void) {
	//繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
	character = new Character;
	stageBoardSystem = new StageBoardSystem;

	ui = new UI;

	AllocFlag = false;
	//Enemy_num = ENEMY_STAGE1_NUM;//謨ｵ縺ｮ謨ｰ
}

Game::~Game(void) {
	//繝・せ繝医Λ繧ｯ繧ｿ
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
	//蛻晄悄蛹・
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

	Dive_State = true;//譛蛻昴・貎懊▲縺ｦ縺ｪ縺・憾諷九°繧峨せ繧ｿ繝ｼ繝・
	DeathEnemyNum = 0;
	InitSound();


}

void Game::Edit(void) {
	//逶ｴ謗･邱ｨ髮・
	stageBoardSystem->stage->Edit();

}

void Game::SetCoord(void) {
	//繧ｪ繝悶ず繧ｧ繧ｯ繝医・蠎ｧ讓呎ｼ邏・
	stageBoardSystem->stage->SetCoord();

}

void Game::EnemyInit(void) {
	//謨ｵ縺ｮ蛻晄悄蛹・
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
	//謠冗判
	//UI縺ｮ謠冗判
	if (ui != NULL) {
		ui->UIDraw(Dive_State);
		ui->DrawRestMath(stageBoardSystem->stage->RestMathCheck());
	}
	if (Dive_State)//豬ｮ荳翫＠縺ｦ繧九→縺・
	{
		stageBoardSystem->stage->Tu = 0.0f;
		stageBoardSystem->stage->Tv = 0.0f;
	}
	else {//貎懊▲縺ｦ縺・ｋ譎・
		stageBoardSystem->stage->Tu = 0.5f;
		stageBoardSystem->stage->Tv = 0.0f;
	}
	//繧ｹ繝・・繧ｸ縺ｮ謠冗判
	if (stageBoardSystem != NULL) {
		stageBoardSystem->stage->BlockDraw();
		stageBoardSystem->stage->SideDraw();
		stageBoardSystem->stage->WallDraw();
		stageBoardSystem->stage->RockDraw(Dive_State);
		stageBoardSystem->stage->CakeDraw();
	}
	//繝励Ξ繧､繝､繝ｼ縺ｮ謠冗判
	if (character != NULL) {
		character->Draw();
	}
	//謨ｵ繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｮ謠冗判
	if (brownBear != NULL) {
		for (int DrawNum = 0; DrawNum < Enemy_MaxNum; DrawNum++) {
			if (brownBear[DrawNum].isAlive) {
				brownBear[DrawNum].Draw(Dive_State);
			}
		}
	}

}

void Game::Update(void) {
	COORD coord;//繝励Ξ繧､繝､繝ｼ縺ｮ蠎ｧ讓・
	COORD wark_coord;//outcoord縺ｧ繧ゅｉ縺｣縺溘ｄ縺､譬ｼ邏・
	COORD enemyCoord;//謨ｵ繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｮ蠎ｧ讓・
	COORD playerpos;//繝励Ξ繧､繝､繝ｼ縺ｮ蠎ｧ讓・逕ｻ髱｢蜀・・蠎ｧ讓・
	COORD enemypos;//謨ｵ繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｮ蠎ｧ讓・逕ｻ髱｢蜀・・蠎ｧ讓・
	COORD ToCoord;//謨ｵ繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｮ蛻晄悄菴咲ｽｮ
				  //繧ｹ繝・・繧ｸ諠・ｱ菫晉ｮ｡逕ｨ
	PieceT  blockinfo;
	PieceT tate;
	PieceT yoko;
	PieceT vertex;
	//------------------
	bool DIVE_TRIG = false;//莉ｮ諠ｳ繧ｭ繝ｼ
	bool EnemyAttackFlag = false;//謾ｻ謦・庄閭ｽ縺九←縺・°
	bool EnemyAttack = false;//謾ｻ謦・・蜉溘＠縺溘°縺ｩ縺・°

							 //譖ｴ譁ｰ
	if (GetKeyboardTrigger(DIK_SPACE)) {
		DIVE_TRIG = true;
	}
	//繧ｳ繝ｳ繝医Ο繝ｼ繝ｩ縺ｧ縺ｮ蜈･蜉帛・逅・
	if (g_pDIDevGamePad) {
		if (GetGamePadTrigger(0) || GetGamePadTrigger(1) || GetGamePadTrigger(2) || GetGamePadTrigger(3)) {
			DIVE_TRIG = true;
		}
	}


	//繝励Ξ繧､繝､繝ｼ縺ｮ譖ｴ譁ｰ-------------------------------
	if (character != NULL)
	{
		character->Update(Dive_State);//蠑墓焚縺ｧ貎懊▲縺ｦ縺・ｋ縺九・繝輔Λ繧ｰ繧呈ｸ｡縺・

		wark_coord = character->OutCoord();//繧ｭ繝｣繝ｩ縺ｮ鬆らせ蠎ｧ讓吝女縺大叙縺｣縺ｦ縺翫￥

		tate = stageBoardSystem->stage->OutSide_Tate(wark_coord.X / 2, wark_coord.Y / 2);
		yoko = stageBoardSystem->stage->OutSide_Yoko(wark_coord.X / 2, wark_coord.Y / 2);
		if (Dive_State == 1) {
			character->CheckMove(tate.isPassagable, yoko.isPassagable);//謌ｻ縺吶ｄ縺､
		}
		coord = character->OutCoord();
		playerpos = character->OutPos();
	}
	//-----------------------------------------------
	if (DIVE_TRIG)//貎懊ｋ縲∵ｵｮ荳翫☆繧・
	{
		tate = stageBoardSystem->stage->OutSide_Tate(coord.X / 2, coord.Y / 2);
		yoko = stageBoardSystem->stage->OutSide_Yoko(coord.X / 2, coord.Y / 2);
		vertex = stageBoardSystem->stage->OutVertexInfo(coord.X / 2, coord.Y / 2);
		if (character->CheckDive(tate.isPassagable, yoko.isPassagable, vertex.isPassagable)) {
			//豬ｮ荳翫〒縺阪ｋ蝣ｴ謇縺句愛螳・
			//貎懊ｋ縺ｮ繧貞・繧頑崛縺医ｋ
			Dive_State = !Dive_State;
			PlaySound(SunaSE);
		}
	}

	//謨ｵ繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｮ譖ｴ譁ｰ------------------------------------------------------------------------------------------------------------------------------------
	if (brownBear != NULL) {
		for (int UpdateNum = 0; UpdateNum < Enemy_MaxNum; UpdateNum++) {
			if (brownBear[UpdateNum].isAlive) {
				enemyCoord = brownBear[UpdateNum].OutBlockCoord();//蠎ｧ讓吝叙繧雁・縺・
				enemypos = brownBear[UpdateNum].OutPos();

				if ((enemypos.X - playerpos.X)*(enemypos.X - playerpos.X) + (enemypos.Y - playerpos.Y)*(enemypos.Y - playerpos.Y) < (MASUWIDTH / 2 * 1.5f)*(MASUWIDTH / 2 * 1.5f)) {
					//謾ｻ謦・庄閭ｽ遽・峇蜀・↑繧峨ヵ繝ｩ繧ｰ繧稚rue縺ｫ
					EnemyAttackFlag = true;
				}
				else {
					EnemyAttackFlag = false;
				}

				blockinfo = stageBoardSystem->stage->OutBlockInfo(enemyCoord.X, enemyCoord.Y);//雜ｳ蜈・・繝槭せ諠・ｱ蜿悶ｊ蜃ｺ縺・
				if (blockinfo.isCut) {
					//遨ｴ縺ｫ關ｽ縺｡繧・
					brownBear[UpdateNum].FallintoHole();
					if (PollSound(FallSE) == false) {
						PlaySound(FallSE);
					}
				}

				//遨ｴ繧帝∩縺代ｋ
				brownBear[UpdateNum].AvoidHole(
					stageBoardSystem->stage->OutBlockInfo(enemyCoord.X + 1, enemyCoord.Y),
					stageBoardSystem->stage->OutBlockInfo(enemyCoord.X, enemyCoord.Y - 1),
					stageBoardSystem->stage->OutBlockInfo(enemyCoord.X - 1, enemyCoord.Y),
					stageBoardSystem->stage->OutBlockInfo(enemyCoord.X, enemyCoord.Y + 1)
				);

				//蛻･縺ｮ謨ｵ繧ｭ繝｣繝ｩ繧帝∩縺代ｋ

				for (int avoidNum = 0; avoidNum < Enemy_MaxNum; avoidNum++) {
					if (avoidNum == UpdateNum) {
						continue;//閾ｪ蛻・・霄ｫ縺ｯ繧ｹ繧ｭ繝・・
					}
					if (brownBear[avoidNum].isAlive == false) {
						continue;//豁ｻ繧薙□謨ｵ縺ｯ繧ｹ繧ｭ繝・・
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

		//繝励Ξ繧､繝､繝ｼ縺ｸ繝繝｡繝ｼ繧ｸ
		character->Hit();

	}
	//繧ｹ繝・・繧ｸ諠・ｱ縺ｮ譖ｴ譁ｰ
	if (stageBoardSystem != NULL) {
		if (Dive_State)//貎懊▲縺ｦ縺・↑縺・凾縺ｮ縺ｿ蛻・ｊ蜿悶ｋ
		{
			stageBoardSystem->CutBoard(coord);//繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｮ菴咲ｽｮ繧貞・繧・
			stageBoardSystem->BoardUpdate(coord);
			stageBoardSystem->stage->Check_Passagable();//蛻・ｊ蜿悶ｉ繧後◆縺ｨ縺薙ｍ縺ｫ縺ｯ陦後￠縺ｪ縺・ｈ縺・↓縺吶ｋ繧・▽
			stageBoardSystem->stage->CheckCakeFall();//繧ｱ繝ｼ繧ｭ縺悟・繧願誠縺｡縺ｦ繧九°
		}
		stageBoardSystem->stage->FallingCake();//繧ｱ繝ｼ繧ｭ縺瑚誠縺｡縺ｦ縺・￥蜃ｦ逅・
	}
	ui->TIME();
}


void Game::Gettime(void) {
	ui->GTIME();
}

bool Game::OutClearFlg(void)//謨ｵ縺・縺ｪ繧液rue繧定ｿ斐☆
{
	if (DeathEnemyNum == Enemy_MaxNum) {
		return true;
	}
	return false;
}