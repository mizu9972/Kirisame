#include "Game.h"
#include "XAudio2.h"

extern LPDIRECTINPUTDEVICE8 g_pDIDevGamePad;
extern long JoypadDI_X;
extern long JoypadDI_Y;
int DeathEnemyNum = 0;

Game::Game(void) {
	//ƒRƒ“ƒXƒgƒ‰ƒNƒ^
	character = new Character;
	stageBoardSystem = new StageBoardSystem;

	ui = new UI;

	AllocFlag = false;
	//Enemy_num = ENEMY_STAGE1_NUM;//“G‚Ì”
}

Game::~Game(void) {
	//ƒfƒXƒgƒ‰ƒNƒ^
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
	//‰Šú‰»
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

	Dive_State = true;//Å‰‚Íö‚Á‚Ä‚È‚¢ó‘Ô‚©‚çƒXƒ^[ƒg
	DeathEnemyNum = 0;
	InitSound();


}

void Game::Edit(void) {
	//’¼Ú•ÒW
	stageBoardSystem->stage->Edit();

}

void Game::SetCoord(void) {
	//ƒIƒuƒWƒFƒNƒg‚ÌÀ•WŠi”[
	stageBoardSystem->stage->SetCoord();

}

void Game::EnemyInit(void) {
	//“G‚Ì‰Šú‰»
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
	//•`‰æ
	//UI‚Ì•`‰æ
	if (ui != NULL) {
		ui->UIDraw(Dive_State);
		ui->Calculation(stageBoardSystem->stage->RestMathCheck());
		ui->DrawRestMath();
	}
	if (Dive_State)//•‚ã‚µ‚Ä‚é‚Æ‚«
	{
		stageBoardSystem->stage->Tu = 0.0f;
		stageBoardSystem->stage->Tv = 0.0f;
	}
	else {//ö‚Á‚Ä‚¢‚é
		stageBoardSystem->stage->Tu = 0.5f;
		stageBoardSystem->stage->Tv = 0.0f;
	}
	//ƒXƒe[ƒW‚Ì•`‰æ
	if (stageBoardSystem != NULL) {
		stageBoardSystem->stage->BlockDraw();
		stageBoardSystem->stage->SideDraw();
		stageBoardSystem->stage->WallDraw();
		stageBoardSystem->stage->RockDraw(Dive_State);
		stageBoardSystem->stage->CakeDraw();
	}
	//ƒvƒŒƒCƒ„[‚Ì•`‰æ
	if (character != NULL) {
		character->Draw();
	}
	//“GƒLƒƒƒ‰ƒNƒ^[‚Ì•`‰æ
	if (brownBear != NULL) {
		for (int DrawNum = 0; DrawNum < Enemy_MaxNum; DrawNum++) {
			if (brownBear[DrawNum].isAlive) {
				brownBear[DrawNum].Draw(Dive_State);
			}
		}
	}

}

void Game::Update(void) {
	COORD coord;//ƒvƒŒƒCƒ„[‚ÌÀ•W
	COORD wark_coord;//outcoord‚Å‚à‚ç‚Á‚½‚â‚ÂŠi”[
	COORD enemyCoord;//“GƒLƒƒƒ‰ƒNƒ^[‚ÌÀ•W
	COORD playerpos;//ƒvƒŒƒCƒ„[‚ÌÀ•W(‰æ–Ê“à‚ÌÀ•W)
	COORD enemypos;//“GƒLƒƒƒ‰ƒNƒ^[‚ÌÀ•W(‰æ–Ê“à‚ÌÀ•W)
	COORD ToCoord;//“GƒLƒƒƒ‰ƒNƒ^[‚Ì‰ŠúˆÊ’u
				  //ƒXƒe[ƒWî•ñ•ÛŠÇ—p
	PieceT  blockinfo;
	PieceT tate;
	PieceT yoko;
	PieceT vertex;
	//------------------
	bool DIVE_TRIG = false;//‰¼‘zƒL[
	bool EnemyAttackFlag = false;//UŒ‚‰Â”\‚©‚Ç‚¤‚©
	bool EnemyAttack = false;//UŒ‚¬Œ÷‚µ‚½‚©‚Ç‚¤‚©

							 //XV
	if (GetKeyboardTrigger(DIK_SPACE)) {
		DIVE_TRIG = true;
	}
	//ƒRƒ“ƒgƒ[ƒ‰‚Å‚Ì“ü—Íˆ—
	if (g_pDIDevGamePad) {
		if (GetGamePadTrigger(0) || GetGamePadTrigger(1) || GetGamePadTrigger(2) || GetGamePadTrigger(3)) {
			DIVE_TRIG = true;
		}
	}


	//ƒvƒŒƒCƒ„[‚ÌXV-------------------------------
	if (character != NULL)
	{
		character->Update(Dive_State);//ˆø”‚Åö‚Á‚Ä‚¢‚é‚©‚Ìƒtƒ‰ƒO‚ğ“n‚·

		wark_coord = character->OutCoord();//ƒLƒƒƒ‰‚Ì’¸“_À•Wó‚¯æ‚Á‚Ä‚¨‚­

		tate = stageBoardSystem->stage->OutSide_Tate(wark_coord.X / 2, wark_coord.Y / 2);
		yoko = stageBoardSystem->stage->OutSide_Yoko(wark_coord.X / 2, wark_coord.Y / 2);
		if (Dive_State == 1) {
			character->CheckMove(tate.isPassagable, yoko.isPassagable);//–ß‚·‚â‚Â
		}
		coord = character->OutCoord();
		playerpos = character->OutPos();
	}
	//-----------------------------------------------
	if (DIVE_TRIG)//ö‚éA•‚ã‚·‚é
	{
		tate = stageBoardSystem->stage->OutSide_Tate(coord.X / 2, coord.Y / 2);
		yoko = stageBoardSystem->stage->OutSide_Yoko(coord.X / 2, coord.Y / 2);
		vertex = stageBoardSystem->stage->OutVertexInfo(coord.X / 2, coord.Y / 2);
		if (character->CheckDive(tate.isPassagable, yoko.isPassagable, vertex.isPassagable)) {
			//•‚ã‚Å‚«‚éêŠ‚©”»’è
			//ö‚é‚Ì‚ğØ‚è‘Ö‚¦‚é
			Dive_State = !Dive_State;
			PlaySound(SunaSE);
		}
	}

	//“GƒLƒƒƒ‰ƒNƒ^[‚ÌXV------------------------------------------------------------------------------------------------------------------------------------
	if (brownBear != NULL) {
		for (int UpdateNum = 0; UpdateNum < Enemy_MaxNum; UpdateNum++) {
			if (brownBear[UpdateNum].isAlive) {
				enemyCoord = brownBear[UpdateNum].OutBlockCoord();//À•Wæ‚èo‚µ
				enemypos = brownBear[UpdateNum].OutPos();

				if ((enemypos.X - playerpos.X)*(enemypos.X - playerpos.X) + (enemypos.Y - playerpos.Y)*(enemypos.Y - playerpos.Y) < (MASUWIDTH / 2 * 1.5f)*(MASUWIDTH / 2 * 1.5f)) {
					//UŒ‚‰Â”\”ÍˆÍ“à‚È‚çƒtƒ‰ƒO‚ğtrue‚É
					EnemyAttackFlag = true;
				}
				else {
					EnemyAttackFlag = false;
				}

				blockinfo = stageBoardSystem->stage->OutBlockInfo(enemyCoord.X, enemyCoord.Y);//‘«Œ³‚Ìƒ}ƒXî•ñæ‚èo‚µ
				if (blockinfo.isCut) {
					//ŒŠ‚É—‚¿‚é
					brownBear[UpdateNum].FallintoHole();
					if (PollSound(FallSE) == false) {
						PlaySound(FallSE);
					}
				}

				//ŒŠ‚ğ”ğ‚¯‚é
				brownBear[UpdateNum].AvoidHole(
					stageBoardSystem->stage->OutBlockInfo(enemyCoord.X + 1, enemyCoord.Y),
					stageBoardSystem->stage->OutBlockInfo(enemyCoord.X, enemyCoord.Y - 1),
					stageBoardSystem->stage->OutBlockInfo(enemyCoord.X - 1, enemyCoord.Y),
					stageBoardSystem->stage->OutBlockInfo(enemyCoord.X, enemyCoord.Y + 1)
				);

				//•Ê‚Ì“GƒLƒƒƒ‰‚ğ”ğ‚¯‚é

				for (int avoidNum = 0; avoidNum < Enemy_MaxNum; avoidNum++) {
					if (avoidNum == UpdateNum) {
						continue;//©•ª©g‚ÍƒXƒLƒbƒv
					}
					if (brownBear[avoidNum].isAlive == false) {
						continue;//€‚ñ‚¾“G‚ÍƒXƒLƒbƒv
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
				if (EnemyAttack == false) {
					EnemyAttack = brownBear[UpdateNum].Update(character->OutPos(), Dive_State, EnemyAttackFlag, ToCoord);
				}
			}
		}
	}
	//--------------------------------------------------------------------------------------------------------------------------------------------------------
	if (EnemyAttack) {

		//ƒvƒŒƒCƒ„[‚Öƒ_ƒ[ƒW
		character->Hit();

	}
	//ƒXƒe[ƒWî•ñ‚ÌXV
	if (stageBoardSystem != NULL) {
		if (Dive_State)//ö‚Á‚Ä‚¢‚È‚¢‚Ì‚İØ‚èæ‚é
		{
			stageBoardSystem->CutBoard(coord);//ƒLƒƒƒ‰ƒNƒ^[‚ÌˆÊ’u‚ğØ‚é
			stageBoardSystem->BoardUpdate(coord);
			stageBoardSystem->stage->Check_Passagable();//Ø‚èæ‚ç‚ê‚½‚Æ‚±‚ë‚É‚Ís‚¯‚È‚¢‚æ‚¤‚É‚·‚é‚â‚Â
			stageBoardSystem->stage->CheckCakeFall();//ƒP[ƒL‚ªØ‚è—‚¿‚Ä‚é‚©
			stageBoardSystem->stage->CheckRockFall();//Šâ‚ªØ‚è—‚¿‚Ä‚é‚©
		}
		stageBoardSystem->stage->FallingCake();//ƒP[ƒL‚ª—‚¿‚Ä‚¢‚­ˆ—
		stageBoardSystem->stage->FallingRock();//Šâ‚ª—‚¿‚Ä‚¢‚­ˆ—
	}
	ui->TIME();

}


void Game::Gettime(void) {
	ui->GTIME();
}

bool Game::OutClearFlg(void)//“G‚ª0‚È‚çtrue‚ğ•Ô‚·
{
	if (DeathEnemyNum == Enemy_MaxNum) {
		return true;
	}
	return false;
}

void Game::ResultDraw(void)//ãƒªã‚¶ãƒ«ãƒˆå‡ºã™
{
	ui->Calculation(stageBoardSystem->stage->RestMathCheck());
	ui->DrawResult();
}