#include "Stage.h"
#include "Asset.h"
#include "main.h"

extern SCENE Scene;

Stage::Stage(void) {
	//�R���X�g���N�^
	sideTextureYoko = TexOp->sideTextureYoko;
	sideTextureTate = TexOp->sideTextureTate;
	blockTexture = TexOp->blockTexture;
	cutblockTexture = TexOp->cutblockTexture;
	wallTexture = TexOp->wallTexture;
}
Stage::~Stage(void) {
	//�f�X�g���N�^
}
void Stage::Init(void) {
	//������

	//�X�e�[�W�f�[�^������
	StageDataInit();
	//�ӂ̏�����
	SideInit();
	//�}�X�̏�����
	BlockInit();
	//���_�̏�����
	VertexInit();
	//ROCK�̏�����
	RockInit();
	//�P�[�L�̏�����
	CakeInit();
}

void Stage::StageDataInit(void) {
	//�X�e�[�W�f�[�^�̏�����
	//���_�@�@�Ӂ@�@���_�@�@�Ӂ@�@���_
	//�Ӂ@�@�}�X�@�@�Ӂ@�@�}�X�@�@��
	//���_�@�@�Ӂ@�@���_�@�@�Ӂ@�@���_
	//�ŕ��ׂĂ���
	int Flag = 0;
	for (int setY = 0; setY < STAGESIZE_IGOY; setY++) {
		switch (setY % 2) {
		case 0:
			for (int setX = 0; setX < STAGESIZE_IGOX; setX++) {
				if (Flag == 0) {
					StageData[setY][setX] = Vertex;
					Flag = 1;
				}
				else if (Flag == 1) {
					StageData[setY][setX] = Side;
					Flag = 0;
				}
			}
			Flag = 0;
			break;

		case 1:
			for (int setX = 0; setX < STAGESIZE_IGOX; setX++) {
				if (Flag == 0) {
					StageData[setY][setX] = Side;
					Flag = 1;
				}
				else if (Flag == 1) {
					StageData[setY][setX] = Block;
					Flag = 0;
				}
			}
			Flag = 0;
			break;
		}
	}
}

void Stage::SideInit(void) {

	//�c�����̕ӂ̏�����
	for (int setNum_retu = 0; setNum_retu < SIDEVERTICAL_NUM; setNum_retu++) {

		for (int setNum_gyo = 0; setNum_gyo < SIDENUM_TATE; setNum_gyo++) {

			Side_Tate[setNum_retu][setNum_gyo].Coord.X = STAGEPOS_YOKO + MASUWIDTH * setNum_gyo;
			Side_Tate[setNum_retu][setNum_gyo].Coord.Y = STAGEPOS_TATE + MASUHEIGHT * setNum_retu;

			Side_Tate[setNum_retu][setNum_gyo].isCut = false;
			Side_Tate[setNum_retu][setNum_gyo].isActive = true;
			Side_Tate[setNum_retu][setNum_gyo].Texture = sideTextureTate;
			Side_Tate[setNum_retu][setNum_gyo].isPassagable = true;//�ʍs�\��

		}
	}

	//�������̕ӂ̏�����
	for (int setNum_retu = 0; setNum_retu < SIDEHORIZONTAL_NUM; setNum_retu++) {

		for (int setNum_gyo = 0; setNum_gyo < SIDENUM_YOKO; setNum_gyo++) {


			Side_Yoko[setNum_retu][setNum_gyo].Coord.X = STAGEPOS_YOKO + MASUWIDTH * setNum_gyo;
			Side_Yoko[setNum_retu][setNum_gyo].Coord.Y = STAGEPOS_TATE + MASUHEIGHT * setNum_retu;

			Side_Yoko[setNum_retu][setNum_gyo].isCut = false;
			Side_Yoko[setNum_retu][setNum_gyo].isActive = true;
			Side_Yoko[setNum_retu][setNum_gyo].Texture = sideTextureYoko;
			Side_Yoko[setNum_retu][setNum_gyo].isPassagable = true;//�ʍs�\��

		}
	}
}

void Stage::BlockInit(void) {
	//�}�X�̏�����
	for (int setY = 0; setY < TATEMASU_NUM; setY++) {

		for (int setX = 0; setX < YOKOMASU_NUM; setX++) {

			Blockinfo[setY][setX].Coord.X = STAGEPOS_YOKO + MASUWIDTH * setX;
			Blockinfo[setY][setX].Coord.Y = STAGEPOS_TATE + MASUHEIGHT * setY;

			Blockinfo[setY][setX].isCut = false;
			Blockinfo[setY][setX].isActive = true;

			Blockinfo[setY][setX].Texture = blockTexture;
			Blockinfo[setY][setX].isPassagable = true;//�ʍs�\��

		}
	}

}

void Stage::VertexInit(void) {
	//���_�̏�����

	for (int setY = 0; setY < VERTEXY_NUM; setY++) {

		for (int setX = 0; setX < VERTEXX_NUM; setX++) {

			Vertexinfo[setY][setX].Coord.X = STAGEPOS_YOKO + MASUWIDTH * setX;
			Vertexinfo[setY][setX].Coord.Y = STAGEPOS_TATE + MASUHEIGHT * setY;

			Vertexinfo[setY][setX].isCut = false;
			Vertexinfo[setY][setX].isActive = true;

			Vertexinfo[setY][setX].Texture = NULL;
			Vertexinfo[setY][setX].isPassagable = true;//�ʍs�\��

		}
	}
}
void Stage::RockInit(void)//��̏�����
{
	for (int num = 0; num < ROCK_NUM; num++)//�e�N�X�`���ǂݍ��ݗp�̌J��Ԃ�����
	{
		Rockinfo[num].Texture = TexOp->rockTexture;//�e�N�X�`���ǂݍ���
		Rockinfo[num].Coord.X = NULL;//��UNULL�����Ă�
		Rockinfo[num].Coord.Y = NULL;
	}
}

void Stage::CakeInit(void)//�P�[�L�̏�����
{
	for (int num = 0; num < CAKE_NUM; num++)//�e�N�X�`���ǂݍ��ݗp�̌J��Ԃ�����
	{
		Cakeinfo[num].Texture = TexOp->cakeTexture;//�e�N�X�`���ǂݍ���
		Cakeinfo[num].Coord.X = NULL;//��UNULL�����Ă�
		Cakeinfo[num].Coord.Y = NULL;
		Cakeinfo[num].isAlive = true;
		Cakeinfo[num].isFalling = false;
		Cakeinfo[num].Size = CAKESIZE;
	}

}

void Stage::Edit(void) {
	//���ڕҏW

	//�X�e�[�W�f�[�^�ҏW
	StageDataEdit();
	//�ӂ̕ҏW
	SideEdit();
	//�}�X�̕ҏW
	BlockEdit();
	//���_�̕ҏW
	VertexEdit();
}

void Stage::StageDataEdit(void) {

}

void Stage::SideEdit(void) {

	switch (Scene)
	{
	case GAME_STAGE1:

		for (int setNum_row = 0; setNum_row < 6; setNum_row++) {
			//(6,0)�`(6,5)�܂�
			Side_Yoko[setNum_row][6].isPassagable = false;//�ʍs�s��

		}
		for (int setNum_column = 0; setNum_column < 7; setNum_column++) {
			//(0,5)�`(6,5)�܂�
			Side_Tate[5][setNum_column].isPassagable = false;//�ʍs�s��

		}
		for (int setNum_row = 6; setNum_row < 8; setNum_row++) {
			//(6,3)�`(7,3)�܂�
			Side_Yoko[setNum_row][3].isPassagable = false;//�ʍs�s��
		}

		break;
	case GAME_STAGE2:
		// ===�c�̕ӂ���ʍs�s���w��===
		//(7.2)
		Side_Tate[2][7].isPassagable = false;
		for (int setNum_row = 0; setNum_row < 4; setNum_row++) {
			//(0.3)�`(3.3)�܂�
			Side_Tate[3][setNum_row].isPassagable = false;
		}
		//(7.3)
		Side_Tate[3][7].isPassagable = false;
		for (int setNum_column = 7; setNum_column < 11; setNum_column++) {
			//(7.6)�`(10.6)�܂�
			Side_Tate[6][setNum_column].isPassagable = false;
		}
		// ===���ɉ��̕ӂ̒ʍs�s���w��===
		Side_Yoko[3][6].isPassagable = false;
		Side_Yoko[3][7].isPassagable = false;		
		
		Side_Yoko[7][6].isPassagable = false;
		Side_Yoko[8][6].isPassagable = false;
		Side_Yoko[9][6].isPassagable = false;
		Side_Yoko[10][6].isPassagable = false;


		break;
	default:
		break;
	}
}

void Stage::BlockEdit(void) {

	switch (Scene)
	{

	case GAME_STAGE1:

		for (int setX = 0; setX < 6; setX++) {

			Blockinfo[5][setX].isPassagable = false;//�ʍs�s��

		}
		for (int setY = 0; setY < 6; setY++) {

			Blockinfo[setY][6].isPassagable = false;//�ʍs�s��

		}
		for (int setY = 6; setY < 8; setY++) {

			Blockinfo[setY][3].isPassagable = false;//�ʍs�s��

		}

		break;
	case GAME_STAGE2:

		for (int setX = 6; setX < 8; setX++) {

			Blockinfo[2][setX].isPassagable = false;//�ʍs�s��

		}
		for (int setX = 0; setX < 4; setX++) {

			Blockinfo[3][setX].isPassagable = false;//�ʍs�s��

		}
		for (int setX = 6; setX < 8; setX++) {

			Blockinfo[3][setX].isPassagable = false;//�ʍs�s��

		}
		Blockinfo[6][1].isPassagable = false;//�ʍs�s��
		Blockinfo[6][4].isPassagable = false;//�ʍs�s��

		for (int setX = 6; setX < 10; setX++) {

			Blockinfo[6][setX].isPassagable = false;//�ʍs�s��

		}
		for (int setY = 7; setY < 10; setY++) {

			Blockinfo[setY][6].isPassagable = false;//�ʍs�s��

		}


		break;
	default:
		break;
	}
}

void Stage::VertexEdit(void) {

}

void Stage::SetCoord(void) {

	//��̍��W�i�[
	SetRockCoord();
	//�P�[�L�̍��W�i�[
	SetCakeCoord();

}
void Stage::SetRockCoord(void) {

	switch (Scene) {

	case GAME_STAGE1:
		//���W�ʒu�����(6.0)�`(6.5)�ɂU�A(0.5)�`(5.5)�ɂU�A(3.6)�`(3.7)�ɂQ�� �@�v�P�S��
		for (int rocknum = 0, rockY = 0; rocknum < 6; rocknum++, rockY++) {

			Rockinfo[rocknum].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * 6);
			Rockinfo[rocknum].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * rockY);

		}
		for (int rocknum = 6, rockX = 0; rocknum < 12; rocknum++, rockX++) {

			Rockinfo[rocknum].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * rockX);
			Rockinfo[rocknum].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * 5);

		}
		for (int rocknum = 12, rockY = 6; rocknum < 14; rocknum++, rockY++) {

			Rockinfo[rocknum].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * 3);
			Rockinfo[rocknum].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * rockY);

		}

		break;
	case GAME_STAGE2:
		//���W�ʒu�����(6.2)�`(7.2)�ɂQ�A(0.3)�`(3.3)�ɂR�A(6.3)�`(7.3)�ɂQ��
		//              (1.6)�A(4.6)�ɂQ�A(6.6)�`(9.6)�ɂS�A(6.7)�`(6.9)�ɂR�@�v�P�V��
		for (int rocknum = 0, rockX = 6; rocknum < 2; rocknum++, rockX++) {

			Rockinfo[rocknum].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * rockX);
			Rockinfo[rocknum].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * 2);

		}
		for (int rocknum = 2, rockX = 0; rocknum < 6; rocknum++, rockX++) {

			Rockinfo[rocknum].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * rockX);
			Rockinfo[rocknum].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * 3);

		}
		for (int rocknum = 6, rockX = 6; rocknum < 8; rocknum++, rockX++) {

			Rockinfo[rocknum].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * rockX);
			Rockinfo[rocknum].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * 3);

		}
		Rockinfo[8].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * 1);
		Rockinfo[8].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * 6);
		Rockinfo[9].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * 4);
		Rockinfo[9].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * 6);

		for (int rocknum = 10, rockX = 6; rocknum < 14; rocknum++, rockX++) {

			Rockinfo[rocknum].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * rockX);
			Rockinfo[rocknum].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * 6);

		}
		for (int rocknum = 14, rockY = 7; rocknum < 17; rocknum++, rockY++) {

			Rockinfo[rocknum].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * 6);
			Rockinfo[rocknum].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * rockY);

		}


		break;
	default:
		break;

	}
}

void Stage::SetCakeCoord(void) {

	switch (Scene) {

	case GAME_STAGE1:
		////���W���́@(4.2)�A(8.2)�̂Q����
		//Cakeinfo[0].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * 4);
		//Cakeinfo[0].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * 2);
		//Cakeinfo[1].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * 8);
		//Cakeinfo[1].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * 2);

		//���W���́@(4.2)�A(8.2)��2����
		Cakeinfo[0].MathPos.X = 4;//�}�X�̂ǂ��ɂ��邩
		Cakeinfo[0].MathPos.Y = 2;
		Cakeinfo[0].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * Cakeinfo[0].MathPos.X) + CAKESIZE / 2;
		Cakeinfo[0].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * Cakeinfo[0].MathPos.Y) + CAKESIZE / 2;

		Cakeinfo[1].MathPos.X = 8;
		Cakeinfo[1].MathPos.Y = 2;
		Cakeinfo[1].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * Cakeinfo[1].MathPos.X) + CAKESIZE / 2;
		Cakeinfo[1].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * Cakeinfo[1].MathPos.Y) + CAKESIZE / 2;

		break;
	case GAME_STAGE2:
		//���W���́@(8.1)�A(1.6)�A(6.4)�̂R����
		Cakeinfo[0].MathPos.X = 8;
		Cakeinfo[0].MathPos.Y = 1;
		Cakeinfo[1].MathPos.X = 2;
		Cakeinfo[1].MathPos.Y = 6;
		Cakeinfo[2].MathPos.X = 3;
		Cakeinfo[2].MathPos.Y = 6;


		Cakeinfo[0].Coord.X = STAGEPOS_YOKO + (MASUWIDTH  * Cakeinfo[0].MathPos.X) + CAKESIZE / 2;
		Cakeinfo[0].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * Cakeinfo[0].MathPos.Y) + CAKESIZE / 2;
		Cakeinfo[1].Coord.X = STAGEPOS_YOKO + (MASUWIDTH* Cakeinfo[1].MathPos.X) + CAKESIZE / 2;
		Cakeinfo[1].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * Cakeinfo[1].MathPos.Y) + CAKESIZE / 2;
		Cakeinfo[2].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * Cakeinfo[2].MathPos.X) + CAKESIZE / 2;
		Cakeinfo[2].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * Cakeinfo[2].MathPos.Y) + CAKESIZE / 2;

		break;
	default:
		break;
	}

}

void Stage::StageCut(int X, int Y) {
	//�؂�ꏊ���牽��؂邩(���_�E�ӁE�}�X)�A�ǂ���؂邩���w�肵�Đ؂�
	switch (StageData[Y][X]) {
	case Vertex:
		Vertexinfo[Y / 2][X / 2].isCut = true;
		break;

	case Side:
		//�������̕ӂ��c�����̕ӂ��ꏊ�ɂ���Ĕ���
		if (Y % 2 == 0) {
			Side_Yoko[Y / 2][X / 2].isCut = true;
		}
		else if (Y % 2 == 1) {
			Side_Tate[Y / 2][X / 2].isCut = true;
		}
		break;

	case Block:
		Blockinfo[Y / 2][X / 2].isCut = true;
		break;
	}
}

void Stage::SideDraw(void) {
	//���ӕ`��
	for (int drawY = 0; drawY < SIDEHORIZONTAL_NUM; drawY++) {
		for (int drawX = 0; drawX < SIDENUM_YOKO; drawX++) {
			if (Side_Yoko[drawY][drawX].isCut) {
				//�؂�Ă��邩�ǂ����ŕ`���ς���
				Draw2dPolygon(Side_Yoko[drawY][drawX].Coord.X, Side_Yoko[drawY][drawX].Coord.Y, MASUWIDTH, SIDESIZE_HUTOSA, D3DCOLOR_ARGB(255, 255, 255, 255), sideTextureYoko, 0, 0, 1, 0.2f);
			}
			else {
				Draw2dPolygon(Side_Yoko[drawY][drawX].Coord.X, Side_Yoko[drawY][drawX].Coord.Y, MASUWIDTH, SIDESIZE_HUTOSA, D3DCOLOR_ARGB(255, 255, 255, 255), sideTextureYoko, 0, 0.5f, 1, 0.2f);

			}
		}
	}

	//�c�ӕ`��
	for (int drawY = 0; drawY < SIDEVERTICAL_NUM; drawY++) {
		for (int drawX = 0; drawX < SIDENUM_TATE; drawX++) {
			if (Side_Tate[drawY][drawX].isCut) {
				//�؂�Ă��邩�ǂ����ŕ`���ς���

				Draw2dPolygon(Side_Tate[drawY][drawX].Coord.X, Side_Tate[drawY][drawX].Coord.Y, SIDESIZE_HUTOSA, MASUHEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), sideTextureTate, 0, 0, 0.2f, 1);
			}
			else {
				Draw2dPolygon(Side_Tate[drawY][drawX].Coord.X, Side_Tate[drawY][drawX].Coord.Y, SIDESIZE_HUTOSA, MASUHEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), sideTextureTate, 0.5f, 0, 0.2f, 1);

			}
		}
	}
}

void Stage::BlockDraw(void) {
	//�}�X�`��
	for (int drawY = 0; drawY < TATEMASU_NUM; drawY++) {
		for (int drawX = 0; drawX < YOKOMASU_NUM; drawX++) {
			if (Blockinfo[drawY][drawX].isCut) {
				//�؂�Ă��邩�ǂ����ŕ`���ς���
				Draw2dPolygon(Blockinfo[drawY][drawX].Coord.X, Blockinfo[drawY][drawX].Coord.Y, MASUWIDTH, MASUHEIGHT + 1, D3DCOLOR_ARGB(255, 255, 255, 255), cutblockTexture, 0, 0, 1, 1);
			}
			else {
				Draw2dPolygon(Blockinfo[drawY][drawX].Coord.X, Blockinfo[drawY][drawX].Coord.Y, MASUWIDTH, MASUHEIGHT + 1, D3DCOLOR_ARGB(255, 255, 255, 255), blockTexture, Tu, Tv, 0.5f, 1);

			}
		}
	}
}

void Stage::WallDraw(void) {
	//�Ǖ`��
	
	//����
	Draw2dPolygon(STAGEPOS_YOKO - WALL_HUTOSA, STAGEPOS_TATE - WALL_HUTOSA, WALL_SIZE_WIDTH + 20, WALL_HUTOSA, D3DCOLOR_ARGB(255, 255, 255, 255), wallTexture, 0, 0, 1, 1);
	Draw2dPolygon(STAGEPOS_YOKO - WALL_HUTOSA, STAGEPOS_TATE + WALL_SIZE_HEIGHT, WALL_SIZE_WIDTH + 20, WALL_HUTOSA, D3DCOLOR_ARGB(255, 255, 255, 255), wallTexture, 0, 0, 1, 1);

	//�c��
	Draw2dPolygon(STAGEPOS_YOKO - WALL_HUTOSA, STAGEPOS_TATE, WALL_HUTOSA, WALL_SIZE_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), wallTexture, 0, 0, 1, 1);
	Draw2dPolygon(STAGEPOS_YOKO + WALL_SIZE_WIDTH, STAGEPOS_TATE, WALL_HUTOSA, WALL_SIZE_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), wallTexture, 0, 0, 1, 1);
}
void Stage::RockDraw(bool DiveState) {
	float UV;
	if (DiveState) {
		UV = 0;
	}
	else {
		UV = 0.5f;
	}
	for (int num = 0; num < ROCK_NUM; num++)
	{
		if (Rockinfo[num].Coord.X == NULL)
		{
			break;
		}

		Draw2dPolygon(Rockinfo[num].Coord.X, Rockinfo[num].Coord.Y, MASUWIDTH, MASUHEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255),
			Rockinfo[num].Texture, UV, 0.0f, 0.5f, 1.0f);
	}
}

void Stage::CakeDraw(void) {
	for (int num = 0; num < CAKE_NUM; num++)
	{
		if (Cakeinfo[num].Coord.X == NULL)
		{
			break;
		}

		Draw2dPolygon(Cakeinfo[num].Coord.X - Cakeinfo[num].Size / 2, Cakeinfo[num].Coord.Y - Cakeinfo[num].Size / 2, Cakeinfo[num].Size, Cakeinfo[num].Size, D3DCOLOR_ARGB(255, 255, 255, 255),
			Cakeinfo[num].Texture, 0.0f, 0.0f, 1.0f, 1.0f);
	}
}
//�X�e�[�W�����o���֐�---------------------
PieceT Stage::OutSide_Tate(int X, int Y) {
	PieceT rtn;
	if (X >= SIDENUM_TATE || X < 0 || Y >= SIDEVERTICAL_NUM || Y < 0) {
		rtn.isActive = false;
		return rtn;
	}
	return Side_Tate[Y][X];
}

PieceT Stage::OutSide_Yoko(int X, int Y) {
	PieceT rtn;
	if (X >= SIDENUM_YOKO || X < 0 || Y >= SIDEHORIZONTAL_NUM || Y < 0) {
		rtn.isActive = false;
		return rtn;
	}
	return Side_Yoko[Y][X];
}

PieceT Stage::OutBlockInfo(int X, int Y) {
	PieceT rtn;
	if (X >= TATEMASU_NUM || X < 0 || Y >= YOKOMASU_NUM || Y < 0) {
		rtn.isActive = false;
		return rtn;
	}
	return Blockinfo[Y][X];
}

PieceT Stage::OutVertexInfo(int X, int Y) {
	PieceT rtn;
	if (X >= VERTEXX_NUM || X < 0 || Y >= VERTEXY_NUM || Y < 0) {
		rtn.isActive = false;
		return rtn;
	}
	return Vertexinfo[Y][X];
}
//---------------------------------------------

void Stage::Check_Passagable(void)//�ʍs�\������
{
	//////////////////////////////////////////////////////
	//���ӂ̒ʍs��Ԋm�F
	for (int setNum_retu = 1; setNum_retu < SIDEHORIZONTAL_NUM - 1; setNum_retu++) {//��

		for (int setNum_gyo = 0; setNum_gyo < SIDENUM_YOKO; setNum_gyo++) {
			if (Blockinfo[setNum_retu - 1][setNum_gyo].isCut
				&&Blockinfo[setNum_retu][setNum_gyo].isCut) {//�㉺�̃u���b�N���؂����Ă�����
				Side_Yoko[setNum_retu][setNum_gyo].isPassagable = false;//�ʍs�s��
			}
		}
	}
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	//�c�ӂ̒ʍs��Ԋm�F
	for (int setNum_retu = 0; setNum_retu < SIDEVERTICAL_NUM; setNum_retu++) {//��
		for (int setNum_gyo = 1; setNum_gyo < SIDENUM_TATE - 1; setNum_gyo++) {
			if (Blockinfo[setNum_retu][setNum_gyo].isCut
				&&Blockinfo[setNum_retu][setNum_gyo - 1].isCut) {//���E�̃u���b�N���؂����Ă�����
				Side_Tate[setNum_retu][setNum_gyo].isPassagable = false;//�ʍs�s��
			}
		}
	}
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	//���_�̒ʍs��Ԋm�F
	for (int setNum_retu = 1; setNum_retu < VERTEXY_NUM - 1; setNum_retu++) {
		for (int setNum_gyo = 1; setNum_gyo < VERTEXX_NUM - 1; setNum_gyo++) {
			if (Blockinfo[setNum_retu][setNum_gyo].isCut
				&& Blockinfo[setNum_retu][setNum_gyo - 1].isCut
				&& Blockinfo[setNum_retu - 1][setNum_gyo].isCut
				&& Blockinfo[setNum_retu - 1][setNum_gyo - 1].isCut
				) {
				Vertexinfo[setNum_retu][setNum_gyo].isPassagable = false;
			}
		}
	}
	//////////////////////////////////////////////////////
}
void Stage::CheckCakeFall(void)
{
	PieceT CheckCake;//�`���b�N������p
	for (int num = 0; num < CAKE_NUM; num++)
	{
		if (Cakeinfo[num].MathPos.X == NULL&&Cakeinfo[num].MathPos.Y == NULL)//��Ȃ珈���𔲂���
		{
			break;
		}
		CheckCake = OutBlockInfo(Cakeinfo[num].MathPos.X, Cakeinfo[num].MathPos.Y);
		if (CheckCake.isCut)//�P�[�L�̉��̃}�X���؂����Ă�����
		{
			Cakeinfo[num].isFalling = true;//������
		}

	}
}
void Stage::FallingCake(void)
{
	for (int num = 0; num < CAKE_NUM; num++)
	{
		if (Cakeinfo[num].isFalling)
		{
			if (Cakeinfo[num].Size <= 0)
			{
				Cakeinfo[num].isAlive = false;//�P�[�L�̎�
				Cakeinfo[num].Size = 0;
			}
			Cakeinfo[num].Size -= 1;
		}
	}
}