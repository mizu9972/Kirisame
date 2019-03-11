///-----------------------------------------------
//キャラクタークラスの処理
//-----------------------------------------------
#include "Character.h"
#include "config.h"
#include "Asset.h"
#include "main.h"
#include "TexLoad.h"

#define HOSEITI 15.0f//キャラクター描画位置調整用
extern LPDIRECT3DDEVICE9 g_pD3DDevice;
extern LPDIRECTINPUTDEVICE8 g_pDIDevGamePad;

typedef struct {
	float CoordX;
	float CoordY;
	float LightU;
	float LightV;
	float LightW;
	float LightH;
	float LightTexSizeU;
	float LightTexSizeV;
}_LightingT;

Character::Character(void) {
	//初期位置をセット
	Coord.X = PLAYER_STARTPOS_X;
	Coord.Y = PLAYER_STARTPOS_Y;

	//初期位置の配列をセット
	GroundInfo.X = (PLAYER_STARTPOS_X - STAGEPOS_YOKO) / (MASUWIDTH / 2);
	GroundInfo.Y = (PLAYER_STARTPOS_Y - STAGEPOS_TATE) / (MASUHEIGHT / 2);

	////初期位置の頂点セット
	Pos_Vertex.X = 2;
	Pos_Vertex.Y = 4;
	//テクスチャセット
	Texture = TexOp->PlayerTex;
	LightTexture = TexOp->PlayerLightTex;
	KeyWait = 0;
	Inputflg = false;

	UpdateInput();
	JoypadDI_X = GetGamePadLeftStickX();
	JoypadDI_Y = GetGamePadLeftStickY();

}

Character::~Character(void) {
}


void Character::SetCoord(COORD inCoord) {
	memcpy(&Coord, &inCoord, sizeof(COORD));
	//初期位置の配列をセット
	GroundInfo.X = (inCoord.X - STAGEPOS_YOKO) / (MASUWIDTH / 2);
	GroundInfo.Y = (inCoord.Y - STAGEPOS_TATE) / (MASUHEIGHT / 2);

	////初期位置の頂点セット
	Pos_Vertex.X = 2;
	Pos_Vertex.Y = 4;

	//サメの向きを初期状態に
	Tu = 0;
	Tv = 0;
}


void Character::Draw(void) {
	//キャラを描画
	Draw2dPolygon(Coord.X - CHARA_SIZE / 2, Coord.Y - CHARA_SIZE + HOSEITI, CHARA_SIZE, CHARA_SIZE, D3DCOLOR_ARGB(255, 255, 255, 255), Texture, Tu, Tv, 0.5f, 0.5f);

	//光沢描画処理
	_LightingT LightingStatus;
	float TexCoordX = GroundInfo.X / 2 * PLAYER_LIGHT_SIZEX;
	float TexCoordY = GroundInfo.Y / 2 * PLAYER_LIGHT_SIZEY;

	if (Tv < 0.5) {
		//横向きの時の設定
		LightingStatus.CoordX = Coord.X - CHARA_SIZE / 2 + CHARA_SIZE - (CHARA_SIZE * TexCoordX);
		LightingStatus.CoordY = Coord.Y - CHARA_SIZE;
		LightingStatus.LightU = 0.5f + Tu - (TexCoordX * 0.5f);
		LightingStatus.LightV = Tv;
		LightingStatus.LightW = 0.5f * PLAYER_LIGHT_SIZEX;
		LightingStatus.LightH = 0.5f;
		LightingStatus.LightTexSizeU = CHARA_SIZE * PLAYER_LIGHT_SIZEX;
		LightingStatus.LightTexSizeV = CHARA_SIZE;

		//UV値がオーバーしないように
		if (LightingStatus.LightU - Tu + LightingStatus.LightW > 0.5f) {
			LightingStatus.LightW = 0.5f - (LightingStatus.LightU - Tu);
		}
	}

	else if (Tv >= 0.5f) {
		//縦向きの時の設定
		LightingStatus.CoordX = Coord.X - CHARA_SIZE / 2;
		LightingStatus.CoordY = Coord.Y - CHARA_SIZE + CHARA_SIZE - (CHARA_SIZE * TexCoordY);
		LightingStatus.LightU = Tu;
		LightingStatus.LightV = 0.5f + Tv - (TexCoordY * 0.5f);
		LightingStatus.LightW = 0.5f;
		LightingStatus.LightH = 0.5f * PLAYER_LIGHT_SIZEY;
		LightingStatus.LightTexSizeU = CHARA_SIZE;
		LightingStatus.LightTexSizeV = CHARA_SIZE * PLAYER_LIGHT_SIZEY;

		//UV値がオーバーしないように
		if (LightingStatus.LightV - Tv + LightingStatus.LightH > 0.5f) {
			LightingStatus.LightH = 0.5f - (LightingStatus.LightV - Tv);
		}
	}
	//光沢描画
	Draw2dPolygon(LightingStatus.CoordX, LightingStatus.CoordY + HOSEITI, LightingStatus.LightTexSizeU, LightingStatus.LightTexSizeV, D3DCOLOR_ARGB(255, 255, 255, 255), LightTexture,
		LightingStatus.LightU, LightingStatus.LightV, LightingStatus.LightW, LightingStatus.LightH);

}

void Character::Move(void)
{
	//仮想キー

	bool UP_TRIG = false;
	bool DOWN_TRIG = false;
	bool LEFT_TRIG = false;
	bool RIGHT_TRIG = false;

	memcpy(&wark_coord, &Coord, sizeof(COORD));//キャラの座標の退避用
	memcpy(&wark_groundinfo, &GroundInfo, sizeof(COORD));//キャラの配列座標の退避用
														 
	//対応するキーが押されたら(キーボード)
	if (GetKeyboardPress(DIK_UP) && Inputflg == false) {
		UP_TRIG = true;
		Inputflg = true;
		Tu = 0.5f;
		Tv = 0.5f;
	}
	else if (GetKeyboardPress(DIK_DOWN) && Inputflg == false) {
		DOWN_TRIG = true;
		Inputflg = true;
		Tu = 0.0f;
		Tv = 0.5f;
	}
	if (GetKeyboardPress(DIK_LEFT) && Inputflg == false) {
		LEFT_TRIG = true;
		Inputflg = true;
		Tu = 0.0f;
		Tv = 0.0f;
	}
	else if (GetKeyboardPress(DIK_RIGHT) && Inputflg == false) {
		RIGHT_TRIG = true;
		Inputflg = true;
		Tu = 0.5f;
		Tv = 0.0f;
	}
	if (Inputflg == true)
	{
		KeyWait++;
	}
	if (KeyWait >= KEYWAIT)
	{
		KeyWait = 0;
		Inputflg = false;
	}

	//対応するボタンが押されたら(ゲームパッド)
	if (g_pDIDevGamePad) {
		if (GetGamePadLeftStickY() <= JoypadDI_Y - GAMEPAD_DEADZONE&&Inputflg == false) {
			UP_TRIG = true;
			Inputflg = true;
			Tu = 0.5f;
			Tv = 0.5f;
		}
		else if (GetGamePadLeftStickY() >= JoypadDI_Y + GAMEPAD_DEADZONE&&Inputflg == false) {
			DOWN_TRIG = true;
			Inputflg = true;
			Tu = 0.0f;
			Tv = 0.5f;
		}
		if (GetGamePadLeftStickX() <= JoypadDI_X - GAMEPAD_DEADZONE&&Inputflg == false) {
			LEFT_TRIG = true;
			Inputflg = true;
			Tu = 0.0f;
			Tv = 0.0f;
		}
		else if (GetGamePadLeftStickX() >= JoypadDI_X + GAMEPAD_DEADZONE&&Inputflg == false) {
			RIGHT_TRIG = true;
			Inputflg = true;
			Tu = 0.5f;
			Tv = 0.0f;
		}
	}

	if (UP_TRIG)//1マス上に移動
	{
		if (Coord.X % MASUWIDTH == 0)//頂点にいる時のみ移動可能に
		{
			Coord.Y -= MASUHEIGHT / 2;
			GroundInfo.Y--;//配列のY座標をマイナス
			Pos_Vertex.Y -= (short)0.5;
		}

	}
	else if (DOWN_TRIG)//１マス下に移動
	{
		if (Coord.X % MASUWIDTH == 0)//頂点にいる時のみ移動可能に
		{
			Coord.Y += MASUHEIGHT / 2;
			GroundInfo.Y++;//配列のY座標をプラス
			Pos_Vertex.Y += (short)0.5;
		}

	}
	if (LEFT_TRIG)//1マス左に移動
	{
		if (Coord.Y % MASUHEIGHT == 0)//頂点にいる時のみ移動可能に
		{
			Coord.X -= MASUWIDTH / 2;
			GroundInfo.X--;//配列のX座標をマイナス
			Pos_Vertex.X -= (short)0.5;
		}

	}
	else if (RIGHT_TRIG)//1マス右に移動
	{
		if (Coord.Y % MASUHEIGHT == 0)//頂点にいる時のみ移動可能に
		{
			Coord.X += MASUWIDTH / 2;
			GroundInfo.X++;//配列のX座標をプラス
			Pos_Vertex.X += (short)0.5;
		}

	}

	///////////////////////////////////////
	// マスの外に出ないようにする処理
	///////////////////////////////////////
	if (Coord.X <= STAGEPOS_YOKO)//左にポロリしたら
	{
		Coord.X = STAGEPOS_YOKO;
		GroundInfo.X = 0;
		Pos_Vertex.X = 0;
	}
	if (Coord.Y <= STAGEPOS_TATE)//上にポロリしたら
	{
		Coord.Y = STAGEPOS_TATE;
		GroundInfo.Y = 0;
		Pos_Vertex.Y = 0;
	}
	if (Coord.X >= STAGEPOS_YOKO + (MASUWIDTH*YOKOMASU_NUM))//右にポロリしたら
	{
		Coord.X = STAGEPOS_YOKO + (MASUWIDTH*YOKOMASU_NUM);
		GroundInfo.X = STAGESIZE_IGOX - 1;
		Pos_Vertex.X = VERTEXX_NUM;
	}
	if (Coord.Y >= STAGEPOS_TATE + (MASUHEIGHT*TATEMASU_NUM))//下にポロリしたら
	{
		Coord.Y = STAGEPOS_TATE + (MASUHEIGHT*TATEMASU_NUM);
		GroundInfo.Y = STAGESIZE_IGOY - 1;
		Pos_Vertex.Y = VERTEXY_NUM;
	}


}

void Character::CheckMove(bool tate, bool yoko)
{

	if (Stagetype != Vertex) {
		switch (Stagetype) {
		case Side_Tate:
			if (tate == 0)
			{
				memcpy(&Coord, &wark_coord, sizeof(COORD));
				memcpy(&GroundInfo, &wark_groundinfo, sizeof(COORD));
			}
			break;

		case Side_Yoko:
			if (yoko == 0)
			{
				memcpy(&Coord, &wark_coord, sizeof(COORD));
				memcpy(&GroundInfo, &wark_groundinfo, sizeof(COORD));
			}
			break;
		}
	}
}

bool Character::CheckDive(bool tate, bool yoko, bool vertex) {
	switch (Stagetype) {
	case Side_Tate:
		if (tate == 0) {
			return false;
		}
		break;

	case Side_Yoko:
		if (yoko == 0) {
			return false;
		}
		break;

	case Vertex:
		if (vertex == 0) {
			return false;
		}
		break;
	}
	return true;
}
void Character::Update(bool Dive_State)
{
	//更新

	Move();

	if (GroundInfo.X % 2 == 0 && GroundInfo.Y % 2 == 0)//頂点にいる場合
	{
		Stagetype = Vertex;
	}
	else if (GroundInfo.X % 2 == 1 && GroundInfo.Y % 2 == 0) {//横辺
		Stagetype = Side_Yoko;
	}
	else if (GroundInfo.X % 2 == 0 && GroundInfo.Y % 2 == 1) {//縦辺
		Stagetype = Side_Tate;
	}

}
StageTypeT Character::OutStageType(void) {
	//StageTypeを返す
	return Stagetype;
}
void Character::Hit(void) {
	//攻撃食らった時の処理
	int a = 0;///仮
}
COORD Character::OutCoord(void) {
	//ステージ内の位置を取り出す
	return GroundInfo;
}
COORD Character::OutPos(void) {
	//座標を取り出す
	return Coord;
}