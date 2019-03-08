#include "Stage.h"
#include "Asset.h"
#include "main.h"

extern SCENE Scene;

Stage::Stage(void) {
	//コンストラクタ
	sideTextureYoko = TexOp->sideTextureYoko;
	sideTextureTate = TexOp->sideTextureTate;
	blockTexture = TexOp->blockTexture;
	cutblockTexture = TexOp->cutblockTexture;
	wallTexture = TexOp->wallTexture;
}
Stage::~Stage(void) {
	//チE��トラクタ
}
void Stage::Init(void) {
	//初期匁E

	//スチE�EジチE�Eタ初期匁E
	StageDataInit();
	//辺の初期匁E
	SideInit();
	//マスの初期匁E
	BlockInit();
	//頂点の初期匁E
	VertexInit();
	//ROCKの初期匁E
	RockInit();
	//ケーキの初期匁E
	CakeInit();
}

void Stage::StageDataInit(void) {
	//スチE�EジチE�Eタの初期匁E
	//頂点　　辺　　頂点　　辺　　頂点
	//辺　　マス　　辺　　マス　　辺
	//頂点　　辺　　頂点　　辺　　頂点
	//で並べてぁE��
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

	//縦向きの辺の初期匁E
	for (int setNum_retu = 0; setNum_retu < SIDEVERTICAL_NUM; setNum_retu++) {

		for (int setNum_gyo = 0; setNum_gyo < SIDENUM_TATE; setNum_gyo++) {

			Side_Tate[setNum_retu][setNum_gyo].Coord.X = STAGEPOS_YOKO + MASUWIDTH * setNum_gyo;
			Side_Tate[setNum_retu][setNum_gyo].Coord.Y = STAGEPOS_TATE + MASUHEIGHT * setNum_retu;

			Side_Tate[setNum_retu][setNum_gyo].isCut = false;
			Side_Tate[setNum_retu][setNum_gyo].isActive = true;
			Side_Tate[setNum_retu][setNum_gyo].Texture = sideTextureTate;
			Side_Tate[setNum_retu][setNum_gyo].isPassagable = true;//通行可能に

		}
	}

	//横向きの辺の初期匁E
	for (int setNum_retu = 0; setNum_retu < SIDEHORIZONTAL_NUM; setNum_retu++) {

		for (int setNum_gyo = 0; setNum_gyo < SIDENUM_YOKO; setNum_gyo++) {


			Side_Yoko[setNum_retu][setNum_gyo].Coord.X = STAGEPOS_YOKO + MASUWIDTH * setNum_gyo;
			Side_Yoko[setNum_retu][setNum_gyo].Coord.Y = STAGEPOS_TATE + MASUHEIGHT * setNum_retu;

			Side_Yoko[setNum_retu][setNum_gyo].isCut = false;
			Side_Yoko[setNum_retu][setNum_gyo].isActive = true;
			Side_Yoko[setNum_retu][setNum_gyo].Texture = sideTextureYoko;
			Side_Yoko[setNum_retu][setNum_gyo].isPassagable = true;//通行可能に

		}
	}
}

void Stage::BlockInit(void) {
	//マスの初期匁E
	for (int setY = 0; setY < TATEMASU_NUM; setY++) {

		for (int setX = 0; setX < YOKOMASU_NUM; setX++) {

			Blockinfo[setY][setX].Coord.X = STAGEPOS_YOKO + MASUWIDTH * setX;
			Blockinfo[setY][setX].Coord.Y = STAGEPOS_TATE + MASUHEIGHT * setY;

			Blockinfo[setY][setX].isCut = false;
			Blockinfo[setY][setX].isActive = true;

			Blockinfo[setY][setX].Texture = blockTexture;
			Blockinfo[setY][setX].isPassagable = true;//通行可能に

		}
	}

}

void Stage::VertexInit(void) {
	//頂点の初期匁E

	for (int setY = 0; setY < VERTEXY_NUM; setY++) {

		for (int setX = 0; setX < VERTEXX_NUM; setX++) {

			Vertexinfo[setY][setX].Coord.X = STAGEPOS_YOKO + MASUWIDTH * setX;
			Vertexinfo[setY][setX].Coord.Y = STAGEPOS_TATE + MASUHEIGHT * setY;

			Vertexinfo[setY][setX].isCut = false;
			Vertexinfo[setY][setX].isActive = true;

			Vertexinfo[setY][setX].Texture = NULL;
			Vertexinfo[setY][setX].isPassagable = true;//通行可能に

		}
	}
}
void Stage::RockInit(void)//岩の初期匁E
{
	for (int num = 0; num < ROCK_NUM; num++)//チE��スチャ読み込み用の繰り返し処琁E
	{
		Rockinfo[num].Texture = TexOp->rockTexture;//チE��スチャ読み込み
		Rockinfo[num].Coord.X = NULL;//一旦NULLを�EれてめE
		Rockinfo[num].Coord.Y = NULL;
	}
}

void Stage::CakeInit(void)//ケーキの初期匁E
{
	for (int num = 0; num < CAKE_NUM; num++)//チE��スチャ読み込み用の繰り返し処琁E
	{
		Cakeinfo[num].Texture = TexOp->cakeTexture;//チE��スチャ読み込み
		Cakeinfo[num].Coord.X = NULL;//一旦NULLを�EれてめE
		Cakeinfo[num].Coord.Y = NULL;
		Cakeinfo[num].isAlive = true;
		Cakeinfo[num].isFalling = false;
		Cakeinfo[num].Size = CAKESIZE;
	}

}

void Stage::Edit(void) {
	//直接編雁E

	//スチE�EジチE�Eタ編雁E
	StageDataEdit();
	//辺の編雁E
	SideEdit();
	//マスの編雁E
	BlockEdit();
	//頂点の編雁E
	VertexEdit();
}

void Stage::StageDataEdit(void) {

}

void Stage::SideEdit(void) {

	switch (Scene)
	{
	case GAME_STAGE1:

		for (int setNum_row = 0; setNum_row < 6; setNum_row++) {
			//(6,0)�E�E6,5)まで
			Side_Yoko[setNum_row][6].isPassagable = false;//通行不可に

		}
		for (int setNum_column = 0; setNum_column < 7; setNum_column++) {
			//(0,5)�E�E6,5)まで
			Side_Tate[5][setNum_column].isPassagable = false;//通行不可に

		}
		for (int setNum_row = 6; setNum_row < 8; setNum_row++) {
			//(6,3)�E�E7,3)まで
			Side_Yoko[setNum_row][3].isPassagable = false;//通行不可に
		}

		break;
	case GAME_STAGE2:
		// ===縦の辺から通行不可を指宁E==
		//(7.2)
		Side_Tate[2][7].isPassagable = false;
		for (int setNum_row = 0; setNum_row < 4; setNum_row++) {
			//(0.3)�E�E3.3)まで
			Side_Tate[3][setNum_row].isPassagable = false;
		}
		//(7.3)
		Side_Tate[3][7].isPassagable = false;
		for (int setNum_column = 7; setNum_column < 11; setNum_column++) {
			//(7.6)�E�E10.6)まで
			Side_Tate[6][setNum_column].isPassagable = false;
		}
		// ===次に横の辺の通行不可を指宁E==
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

			Blockinfo[5][setX].isPassagable = false;//通行不可に

		}
		for (int setY = 0; setY < 6; setY++) {

			Blockinfo[setY][6].isPassagable = false;//通行不可に

		}
		for (int setY = 6; setY < 8; setY++) {

			Blockinfo[setY][3].isPassagable = false;//通行不可に

		}

		break;
	case GAME_STAGE2:

		for (int setX = 6; setX < 8; setX++) {

			Blockinfo[2][setX].isPassagable = false;//通行不可に

		}
		for (int setX = 0; setX < 4; setX++) {

			Blockinfo[3][setX].isPassagable = false;//通行不可に

		}
		for (int setX = 6; setX < 8; setX++) {

			Blockinfo[3][setX].isPassagable = false;//通行不可に

		}
		Blockinfo[6][1].isPassagable = false;//通行不可に
		Blockinfo[6][4].isPassagable = false;//通行不可に

		for (int setX = 6; setX < 10; setX++) {

			Blockinfo[6][setX].isPassagable = false;//通行不可に

		}
		for (int setY = 7; setY < 10; setY++) {

			Blockinfo[setY][6].isPassagable = false;//通行不可に

		}


		break;
	default:
		break;
	}
}

void Stage::VertexEdit(void) {

}

void Stage::SetCoord(void) {

	//岩の座標格紁E
	SetRockCoord();
	//ケーキの座標格紁E
	SetCakeCoord();

}
void Stage::SetRockCoord(void) {

	switch (Scene) {

	case GAME_STAGE1:
		//座標位置を�E劁E6.0)�E�E6.5)に�E�つ、E0.5)�E�E5.5)に�E�つ、E3.6)�E�E3.7)に�E�つ 　計１４倁E
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
		//座標位置を�E劁E6.2)�E�E7.2)に�E�つ、E0.3)�E�E3.3)に�E�つ、E6.3)�E�E7.3)に�E�つ
		//              (1.6)、E4.6)に�E�つ、E6.6)�E�E9.6)に�E�つ、E6.7)�E�E6.9)に�E�つ　計１７つ
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
		////座標�E力　(4.2)、E8.2)の�E�か所
		//Cakeinfo[0].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * 4);
		//Cakeinfo[0].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * 2);
		//Cakeinfo[1].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * 8);
		//Cakeinfo[1].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * 2);

		//座標�E力　(4.2)、E8.2)の2か所
		Cakeinfo[0].MathPos.X = 4;//マスのどこにぁE��ぁE
		Cakeinfo[0].MathPos.Y = 2;
		Cakeinfo[0].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * Cakeinfo[0].MathPos.X) + CAKESIZE / 2;
		Cakeinfo[0].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * Cakeinfo[0].MathPos.Y) + CAKESIZE / 2;

		Cakeinfo[1].MathPos.X = 8;
		Cakeinfo[1].MathPos.Y = 2;
		Cakeinfo[1].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * Cakeinfo[1].MathPos.X) + CAKESIZE / 2;
		Cakeinfo[1].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * Cakeinfo[1].MathPos.Y) + CAKESIZE / 2;

		break;
	case GAME_STAGE2:
		//座標�E力　(8.1)、E1.6)、E6.4)の�E�か所
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
	//刁E��場所から何を刁E��ぁE頂点・辺・マス)、どこを刁E��かを持E��して刁E��
	switch (StageData[Y][X]) {
	case Vertex:
		Vertexinfo[Y / 2][X / 2].isCut = true;
		break;

	case Side:
		//横向きの辺か縦向きの辺か場所によって判宁E
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
	//横辺描画
	for (int drawY = 0; drawY < SIDEHORIZONTAL_NUM; drawY++) {
		for (int drawX = 0; drawX < SIDENUM_YOKO; drawX++) {
			if (Side_Yoko[drawY][drawX].isCut) {
				//刁E��てぁE��かどぁE��で描画を変えめE
				Draw2dPolygon(Side_Yoko[drawY][drawX].Coord.X, Side_Yoko[drawY][drawX].Coord.Y, MASUWIDTH, SIDESIZE_HUTOSA, D3DCOLOR_ARGB(255, 255, 255, 255), sideTextureYoko, 0, 0, 1, 0.2f);
			}
			else {
				Draw2dPolygon(Side_Yoko[drawY][drawX].Coord.X, Side_Yoko[drawY][drawX].Coord.Y, MASUWIDTH, SIDESIZE_HUTOSA, D3DCOLOR_ARGB(255, 255, 255, 255), sideTextureYoko, 0, 0.5f, 1, 0.2f);

			}
		}
	}

	//縦辺描画
	for (int drawY = 0; drawY < SIDEVERTICAL_NUM; drawY++) {
		for (int drawX = 0; drawX < SIDENUM_TATE; drawX++) {
			if (Side_Tate[drawY][drawX].isCut) {
				//刁E��てぁE��かどぁE��で描画を変えめE

				Draw2dPolygon(Side_Tate[drawY][drawX].Coord.X, Side_Tate[drawY][drawX].Coord.Y, SIDESIZE_HUTOSA, MASUHEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), sideTextureTate, 0, 0, 0.2f, 1);
			}
			else {
				Draw2dPolygon(Side_Tate[drawY][drawX].Coord.X, Side_Tate[drawY][drawX].Coord.Y, SIDESIZE_HUTOSA, MASUHEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), sideTextureTate, 0.5f, 0, 0.2f, 1);

			}
		}
	}
}

void Stage::BlockDraw(void) {
	//マス描画
	for (int drawY = 0; drawY < TATEMASU_NUM; drawY++) {
		for (int drawX = 0; drawX < YOKOMASU_NUM; drawX++) {
			if (Blockinfo[drawY][drawX].isCut) {
				//刁E��てぁE��かどぁE��で描画を変えめE
				Draw2dPolygon(Blockinfo[drawY][drawX].Coord.X, Blockinfo[drawY][drawX].Coord.Y, MASUWIDTH, MASUHEIGHT + 1, D3DCOLOR_ARGB(255, 255, 255, 255), cutblockTexture, 0, 0, 1, 1);
			}
			else {
				Draw2dPolygon(Blockinfo[drawY][drawX].Coord.X, Blockinfo[drawY][drawX].Coord.Y, MASUWIDTH, MASUHEIGHT + 1, D3DCOLOR_ARGB(255, 255, 255, 255), blockTexture, Tu, Tv, 0.5f, 1);

			}
		}
	}
}

void Stage::WallDraw(void) {
	//壁描画

	//横壁E
	Draw2dPolygon(STAGEPOS_YOKO - WALL_HUTOSA, STAGEPOS_TATE - WALL_HUTOSA, WALL_SIZE_WIDTH + 20, WALL_HUTOSA, D3DCOLOR_ARGB(255, 255, 255, 255), wallTexture, 0, 0, 1, 1);
	Draw2dPolygon(STAGEPOS_YOKO - WALL_HUTOSA, STAGEPOS_TATE + WALL_SIZE_HEIGHT, WALL_SIZE_WIDTH + 20, WALL_HUTOSA, D3DCOLOR_ARGB(255, 255, 255, 255), wallTexture, 0, 0, 1, 1);

	//縦壁E
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
		if (Cakeinfo[num].isAlive)
		{
			Draw2dPolygon(Cakeinfo[num].Coord.X - Cakeinfo[num].Size / 2, Cakeinfo[num].Coord.Y - Cakeinfo[num].Size / 2, Cakeinfo[num].Size, Cakeinfo[num].Size, D3DCOLOR_ARGB(255, 255, 255, 255),
				Cakeinfo[num].Texture, 0.0f, 0.0f, 1.0f, 1.0f);
		}
	}
}
//スチE�Eジ惁E��取り出し関数---------------------
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

void Stage::Check_Passagable(void)//通行可能か判宁E
{
	//////////////////////////////////////////////////////
	//横辺の通行状態確誁E
	for (int setNum_retu = 1; setNum_retu < SIDEHORIZONTAL_NUM - 1; setNum_retu++) {//中

		for (int setNum_gyo = 0; setNum_gyo < SIDENUM_YOKO; setNum_gyo++) {
			if (Blockinfo[setNum_retu - 1][setNum_gyo].isCut
				&&Blockinfo[setNum_retu][setNum_gyo].isCut) {//上下�EブロチE��が�Eり取られてぁE��めE
				Side_Yoko[setNum_retu][setNum_gyo].isPassagable = false;//通行不可に
			}
		}
	}
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	//縦辺の通行状態確誁E
	for (int setNum_retu = 0; setNum_retu < SIDEVERTICAL_NUM; setNum_retu++) {//中
		for (int setNum_gyo = 1; setNum_gyo < SIDENUM_TATE - 1; setNum_gyo++) {
			if (Blockinfo[setNum_retu][setNum_gyo].isCut
				&&Blockinfo[setNum_retu][setNum_gyo - 1].isCut) {//左右のブロチE��が�Eり取られてぁE��めE
				Side_Tate[setNum_retu][setNum_gyo].isPassagable = false;//通行不可に
			}
		}
	}
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	//頂点の通行状態確誁E
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
	PieceT CheckCake;//チャチE��をする用
	for (int num = 0; num < CAKE_NUM; num++)
	{
		if (Cakeinfo[num].MathPos.X == NULL&&Cakeinfo[num].MathPos.Y == NULL)//空なら�E琁E��抜けめE
		{
			break;
		}
		CheckCake = OutBlockInfo(Cakeinfo[num].MathPos.X, Cakeinfo[num].MathPos.Y);
		if (CheckCake.isCut)//ケーキの下�Eマスが�Eり取られてぁE��めE
		{
			Cakeinfo[num].isFalling = true;//落ちぁE
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
				Cakeinfo[num].isAlive = false;//ケーキの死
				Cakeinfo[num].Size = 0;
			}
			Cakeinfo[num].Size -= 1;
		}
	}
}
int Stage::RestMathCheck(void)//残りのマスの数を返す
{
	int RestMath = 0;//残りマスの数
	PieceT Check;
	for (int num_retu = 0; num_retu < YOKOMASU_NUM; num_retu++)
	{
		for (int num_gyo = 0; num_gyo < TATEMASU_NUM; num_gyo++)
		{
			Check = OutBlockInfo(num_gyo, num_retu);
			if (Check.isCut == false)
			{
				RestMath++;
			}
		}
	}
	return RestMath;//残りマスの数を返す
}