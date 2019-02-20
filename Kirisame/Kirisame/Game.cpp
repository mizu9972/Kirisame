#include "Game.h"
#include "XAudio2.h"

extern LPDIRECTINPUTDEVICE8 g_pDIDevGamePad;
extern long JoypadDI_X;
extern long JoypadDI_Y;
int DeathEnemyNum = 0;
Game::Game(void) {
	//�R���X�g���N�^
	character = new Character;
	stageBoardSystem = new StageBoardSystem;
	//S2Brownbear = new BrownBear[ENEMY_STAGE2_NUM];//err
	//S1Brownbear = new BrownBear[ENEMY_STAGE1_NUM];


	ui = new UI;
	//Enemy_num = ENEMY_STAGE1_NUM;//�G�̐�
}

Game::~Game(void) {
	//�f�X�g���N�^
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
	//������
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
	Dive_State = true;//�ŏ��͐����ĂȂ���Ԃ���X�^�[�g
	DeathEnemyNum = 0;
	InitSound();

	PlaySound(TitleBGM);
}

void Game::Edit(void) {
	//���ڕҏW
	stageBoardSystem->stage->Edit();

}

void Game::SetCoord(void) {
	//�I�u�W�F�N�g�̍��W�i�[
	stageBoardSystem->stage->SetCoord();

}

void Game::EnemyInit(void) {
	//�G�̏�����
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
	//�`��
	//UI�̕`��
	if (ui != NULL) {
		ui->UIDraw(Dive_State);
	}
	if (Dive_State)//���サ�Ă�Ƃ�
	{
		stageBoardSystem->stage->Tu = 0.0f;
		stageBoardSystem->stage->Tv = 0.0f;
	}
	else {//�����Ă��鎞
		stageBoardSystem->stage->Tu = 0.5f;
		stageBoardSystem->stage->Tv = 0.0f;
	}
	//�X�e�[�W�̕`��
	if (stageBoardSystem != NULL) {
		stageBoardSystem->stage->BlockDraw();
		stageBoardSystem->stage->SideDraw();
		stageBoardSystem->stage->WallDraw();
		stageBoardSystem->stage->RockDraw(Dive_State);
		stageBoardSystem->stage->CakeDraw();
	}
	//�v���C���[�̕`��
	if (character != NULL) {
		character->Draw();
	}
	//�G�L�����N�^�[�̕`��
	if (brownBear != NULL) {
		for (int DrawNum = 0; DrawNum < Enemy_MaxNum; DrawNum++) {
			if (brownBear[DrawNum].isAlive) {
				brownBear[DrawNum].Draw(Dive_State);
			}
		}
	}

}

void Game::Update(void) {
	COORD coord;//�v���C���[�̍��W
	COORD wark_coord;//outcoord�ł��������i�[
	COORD enemyCoord;//�G�L�����N�^�[�̍��W
	COORD playerpos;//�v���C���[�̍��W(��ʓ��̍��W)
	COORD enemypos;//�G�L�����N�^�[�̍��W(��ʓ��̍��W)
	COORD ToCoord;//�G�L�����N�^�[�̏����ʒu
	//�X�e�[�W���ۊǗp
	PieceT  blockinfo;
	PieceT tate;
	PieceT yoko;
	PieceT vertex;
	//------------------
	bool DIVE_TRIG = false;//���z�L�[
	bool EnemyAttackFlag = false;//�U���\���ǂ���
	bool EnemyAttack = false;//�U�������������ǂ���

	//�X�V
	if (GetKeyboardTrigger(DIK_SPACE)) {
		DIVE_TRIG = true;
	}
	//�R���g���[���ł̓��͏���
	if (g_pDIDevGamePad) {
		if (GetGamePadTrigger(0) || GetGamePadTrigger(1) || GetGamePadTrigger(2) || GetGamePadTrigger(3)) {
			DIVE_TRIG = true;
		}
	}


	//�v���C���[�̍X�V-------------------------------
	if (character != NULL)
	{
		character->Update(Dive_State);//�����Ő����Ă��邩�̃t���O��n��

		wark_coord = character->OutCoord();//�L�����̒��_���W�󂯎���Ă���

		tate = stageBoardSystem->stage->OutSide_Tate(wark_coord.X / 2, wark_coord.Y / 2);
		yoko = stageBoardSystem->stage->OutSide_Yoko(wark_coord.X / 2, wark_coord.Y / 2);
		if (Dive_State == 1) {
			character->CheckMove(tate.isPassagable, yoko.isPassagable);//�߂����
		}
		coord = character->OutCoord();
		playerpos = character->OutPos();
	}
	//-----------------------------------------------
	if (DIVE_TRIG)//����A���シ��
	{
		tate = stageBoardSystem->stage->OutSide_Tate(coord.X / 2, coord.Y / 2);
		yoko = stageBoardSystem->stage->OutSide_Yoko(coord.X / 2, coord.Y / 2);
		vertex = stageBoardSystem->stage->OutVertexInfo(coord.X / 2, coord.Y / 2);
		if (character->CheckDive(tate.isPassagable,yoko.isPassagable,vertex.isPassagable)) {
			//����ł���ꏊ������
			//����̂�؂�ւ���
			Dive_State = !Dive_State;
			PlaySound(SunaSE);
		}
	}

	//�G�L�����N�^�[�̍X�V------------------------------------------------------------------------------------------------------------------------------------
	if (brownBear != NULL) {
		for (int UpdateNum = 0; UpdateNum < Enemy_MaxNum; UpdateNum++) {
			if (brownBear[UpdateNum].isAlive) {
				enemyCoord = brownBear[UpdateNum].OutBlockCoord();//���W���o��
				enemypos = brownBear[UpdateNum].OutPos();

				if ((enemypos.X - playerpos.X)*(enemypos.X - playerpos.X) + (enemypos.Y - playerpos.Y)*(enemypos.Y - playerpos.Y) < (MASUWIDTH / 2 * 1.5f)*(MASUWIDTH / 2 * 1.5f)) {
					//�U���\�͈͓��Ȃ�t���O��true��
					EnemyAttackFlag = true;
				}
				else {
					EnemyAttackFlag = false;
				}

				blockinfo = stageBoardSystem->stage->OutBlockInfo(enemyCoord.X,enemyCoord.Y);//�����̃}�X�����o��
				if (blockinfo.isCut) {
					//���ɗ�����
					brownBear[UpdateNum].FallintoHole();
					if (PollSound(FallSE) == false) {
						PlaySound(FallSE);
					}
				}

				//���������
				brownBear[UpdateNum].AvoidHole(
					stageBoardSystem->stage->OutBlockInfo(enemyCoord.X + 1, enemyCoord.Y),
					stageBoardSystem->stage->OutBlockInfo(enemyCoord.X, enemyCoord.Y - 1),
					stageBoardSystem->stage->OutBlockInfo(enemyCoord.X - 1, enemyCoord.Y),
					stageBoardSystem->stage->OutBlockInfo(enemyCoord.X, enemyCoord.Y + 1)
				);

				//�ʂ̓G�L�����������
				
					for (int avoidNum = 0; avoidNum < Enemy_MaxNum; avoidNum++) {
						if (avoidNum == UpdateNum) {
							continue;//�������g�̓X�L�b�v
						}
						if (brownBear[avoidNum].isAlive == false) {
							continue;//���񂾓G�̓X�L�b�v
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
		//�v���C���[�փ_���[�W
		character->Hit();

	}
	//�X�e�[�W���̍X�V
	if (stageBoardSystem != NULL) {
		if (Dive_State)//�����Ă��Ȃ����̂ݐ؂���
		{
			stageBoardSystem->CutBoard(coord);//�L�����N�^�[�̈ʒu��؂�
			stageBoardSystem->BoardUpdate(coord);
			stageBoardSystem->stage->Check_Passagable();//�؂���ꂽ�Ƃ���ɂ͍s���Ȃ��悤�ɂ�����
			stageBoardSystem->stage->CheckCakeFall();//�P�[�L���؂藎���Ă邩
			stageBoardSystem->stage->FallingCake();
		}
	}
	ui->TIME();
}


void Game::Gettime(void) {
	ui->GTIME();
}

bool Game::OutClearFlg(void)//�G��0�Ȃ�true��Ԃ�
{
	//for (int num = 0; num < Enemy_MaxNum; num++)
	//{
	//	//�N�}����z��̐����t���O���Q�Ƃ��Ă����đS���������Ă���
	//	//�G�̐���0�ɂȂ鏈��
	//	if (brownBear[num].isAlive == false)
	//	{
	//		Enemy_num--;
	//	}
	//}
	//if (Enemy_num >  0)
	//{
	//	Enemy_num = Enemy_MaxNum;//�G1�̓|�������ŃJ�E���g��0�ɂȂ�΍�
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