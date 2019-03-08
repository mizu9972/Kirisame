#pragma once

#include <Windows.h>
#include "2dpolygon.h"
#include "Stage.h"

class Character {
	//キャラクタークラス
private:
	COORD Coord;//座標
	COORD GroundInfo;//辺とか頂点とかの配列格納するやつ
	COORD Pos_Vertex;//現在どこの頂点にいるか
	LPDIRECT3DTEXTURE9* Texture;//テクスチャ
	LPDIRECT3DTEXTURE9* LightTexture;

	float Tu;//U
	float Tv;//V
	int KeyWait;//キーのウェイト
	bool Inputflg;//入力間隔調整用のフラグ

	COORD wark_coord = Coord;//キャラの座標の退避用
	COORD wark_groundinfo = GroundInfo;//キャラの配列座標の退避用

	StageTypeT Stagetype;
public:


	Character(void);
	~Character(void);
	void SetCoord(COORD);
	void Hit(void);//攻撃くらい処理

	StageTypeT OutStageType(void);
	void Move(void);//移動関数
	void Update(bool Dive_State);//更新処理
	void Draw(void);//描画
	void CheckMove(bool tate, bool yoko);//動けるか判定
	bool CheckDive(bool tate, bool yoko, bool vertex);
	COORD OutCoord(void);
	COORD OutPos(void);
};