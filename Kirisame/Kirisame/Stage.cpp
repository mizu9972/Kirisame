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
	//デストラクタ
}
void Stage::Init(void) {
	//初期化

	//ステージデータ初期化
	StageDataInit();
	//辺の初期化
	SideInit();
	//マスの初期化
	BlockInit();
	//頂点の初期化
	VertexInit();
	//ROCKの初期化
	RockInit();
	//ケーキの初期化
	CakeInit();
}

void Stage::StageDataInit(void) {
	//ステージデータの初期化
	//頂点　　辺　　頂点　　辺　　頂点
	//辺　　マス　　辺　　マス　　辺
	//頂点　　辺　　頂点　　辺　　頂点
	//で並べていく
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

	//縦向きの辺の初期化
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

	//横向きの辺の初期化
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
	//マスの初期化
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
	//頂点の初期化

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
void Stage::RockInit(void)//岩の初期化
{
	for (int num = 0; num < ROCK_NUM; num++)//テクスチャ読み込み用の繰り返し処理
	{
		Rockinfo[num].Texture = TexOp->rockTexture;//テクスチャ読み込み
		Rockinfo[num].Coord.X = NULL;//一旦NULLを入れてる
		Rockinfo[num].Coord.Y = NULL;
	}
}

void Stage::CakeInit(void)//ケーキの初期化
{
	for (int num = 0; num < CAKE_NUM; num++)//テクスチャ読み込み用の繰り返し処理
	{
		Cakeinfo[num].Texture = TexOp->cakeTexture;//テクスチャ読み込み
		Cakeinfo[num].Coord.X = NULL;//一旦NULLを入れてる
		Cakeinfo[num].Coord.Y = NULL;
		Cakeinfo[num].isAlive = true;
		Cakeinfo[num].isFalling = false;
		Cakeinfo[num].Size = CAKESIZE;
	}

}

void Stage::Edit(void) {
	//直接編集

	//ステージデータ編集
	StageDataEdit();
	//辺の編集
	SideEdit();
	//マスの編集
	BlockEdit();
	//頂点の編集
	VertexEdit();
}

void Stage::StageDataEdit(void) {

}

void Stage::SideEdit(void) {

	switch (Scene)
	{
	case GAME_STAGE1:

		for (int setNum_row = 0; setNum_row < 6; setNum_row++) {
			//(6,0)～(6,5)まで
			Side_Yoko[setNum_row][6].isPassagable = false;//通行不可に

		}
		for (int setNum_column = 0; setNum_column < 7; setNum_column++) {
			//(0,5)～(6,5)まで
			Side_Tate[5][setNum_column].isPassagable = false;//通行不可に

		}
		for (int setNum_row = 6; setNum_row < 8; setNum_row++) {
			//(6,3)～(7,3)まで
			Side_Yoko[setNum_row][3].isPassagable = false;//通行不可に
		}

		break;
	case GAME_STAGE2:
		// ===縦の辺から通行不可を指定===
		//(7.2)
		Side_Tate[2][7].isPassagable = false;
		for (int setNum_row = 0; setNum_row < 4; setNum_row++) {
			//(0.3)～(3.3)まで
			Side_Tate[3][setNum_row].isPassagable = false;
		}
		//(7.3)
		Side_Tate[3][7].isPassagable = false;
		for (int setNum_column = 7; setNum_column < 11; setNum_column++) {
			//(7.6)～(10.6)まで
			Side_Tate[6][setNum_column].isPassagable = false;
		}
		// ===次に横の辺の通行不可を指定===
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

	//岩の座標格納
	SetRockCoord();
	//ケーキの座標格納
	SetCakeCoord();

}
void Stage::SetRockCoord(void) {

	switch (Scene) {

	case GAME_STAGE1:
		//座標位置を入力(6.0)～(6.5)に６つ、(0.5)～(5.5)に６つ、(3.6)～(3.7)に２つ 　計１４個
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
		//座標位置を入力(6.2)～(7.2)に２つ、(0.3)～(3.3)に３つ、(6.3)～(7.3)に２つ
		//              (1.6)、(4.6)に２つ、(6.6)～(9.6)に４つ、(6.7)～(6.9)に３つ　計１７つ
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
		////座標入力　(4.2)、(8.2)の２か所
		//Cakeinfo[0].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * 4);
		//Cakeinfo[0].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * 2);
		//Cakeinfo[1].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * 8);
		//Cakeinfo[1].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * 2);

		//座標入力　(4.2)、(8.2)の2か所
		Cakeinfo[0].MathPos.X = 4;//マスのどこにいるか
		Cakeinfo[0].MathPos.Y = 2;
		Cakeinfo[0].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * Cakeinfo[0].MathPos.X) + CAKESIZE / 2;
		Cakeinfo[0].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * Cakeinfo[0].MathPos.Y) + CAKESIZE / 2;

		Cakeinfo[1].MathPos.X = 8;
		Cakeinfo[1].MathPos.Y = 2;
		Cakeinfo[1].Coord.X = STAGEPOS_YOKO + (MASUWIDTH * Cakeinfo[1].MathPos.X) + CAKESIZE / 2;
		Cakeinfo[1].Coord.Y = STAGEPOS_TATE + (MASUHEIGHT * Cakeinfo[1].MathPos.Y) + CAKESIZE / 2;

		break;
	case GAME_STAGE2:
		//座標入力　(8.1)、(1.6)、(6.4)の３か所
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
	//切る場所から何を切るか(頂点・辺・マス)、どこを切るかを指定して切る
	switch (StageData[Y][X]) {
	case Vertex:
		Vertexinfo[Y / 2][X / 2].isCut = true;
		break;

	case Side:
		//横向きの辺か縦向きの辺か場所によって判定
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
				//切れているかどうかで描画を変える
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
				//切れているかどうかで描画を変える

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
				//切れているかどうかで描画を変える
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
	
	//横壁
	Draw2dPolygon(STAGEPOS_YOKO - WALL_HUTOSA, STAGEPOS_TATE - WALL_HUTOSA, WALL_SIZE_WIDTH + 20, WALL_HUTOSA, D3DCOLOR_ARGB(255, 255, 255, 255), wallTexture, 0, 0, 1, 1);
	Draw2dPolygon(STAGEPOS_YOKO - WALL_HUTOSA, STAGEPOS_TATE + WALL_SIZE_HEIGHT, WALL_SIZE_WIDTH + 20, WALL_HUTOSA, D3DCOLOR_ARGB(255, 255, 255, 255), wallTexture, 0, 0, 1, 1);

	//縦壁
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
//ステージ情報取り出し関数---------------------
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

void Stage::Check_Passagable(void)//通行可能か判定
{
	//////////////////////////////////////////////////////
	//横辺の通行状態確認
	for (int setNum_retu = 1; setNum_retu < SIDEHORIZONTAL_NUM - 1; setNum_retu++) {//中

		for (int setNum_gyo = 0; setNum_gyo < SIDENUM_YOKO; setNum_gyo++) {
			if (Blockinfo[setNum_retu - 1][setNum_gyo].isCut
				&&Blockinfo[setNum_retu][setNum_gyo].isCut) {//上下のブロックが切り取られていたら
				Side_Yoko[setNum_retu][setNum_gyo].isPassagable = false;//通行不可に
			}
		}
	}
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	//縦辺の通行状態確認
	for (int setNum_retu = 0; setNum_retu < SIDEVERTICAL_NUM; setNum_retu++) {//中
		for (int setNum_gyo = 1; setNum_gyo < SIDENUM_TATE - 1; setNum_gyo++) {
			if (Blockinfo[setNum_retu][setNum_gyo].isCut
				&&Blockinfo[setNum_retu][setNum_gyo - 1].isCut) {//左右のブロックが切り取られていたら
				Side_Tate[setNum_retu][setNum_gyo].isPassagable = false;//通行不可に
			}
		}
	}
	//////////////////////////////////////////////////////

	//////////////////////////////////////////////////////
	//頂点の通行状態確認
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
	PieceT CheckCake;//チャックをする用
	for (int num = 0; num < CAKE_NUM; num++)
	{
		if (Cakeinfo[num].MathPos.X == NULL&&Cakeinfo[num].MathPos.Y == NULL)//空なら処理を抜ける
		{
			break;
		}
		CheckCake = OutBlockInfo(Cakeinfo[num].MathPos.X, Cakeinfo[num].MathPos.Y);
		if (CheckCake.isCut)//ケーキの下のマスが切り取られていたら
		{
			Cakeinfo[num].isFalling = true;//落ちた
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