#pragma once
//ステージを管理するクラス
//ステージの描画もここで行う
#include <Windows.h>
#include "config.h"
#include "2dpolygon.h"


typedef enum {
	//頂点・辺・マスの区別
	Vertex,
	Side,
	Side_Tate,
	Side_Yoko,
	Block,
}StageTypeT;

typedef struct {
	//頂点・辺・マスの情報内容の構造体
	COORD Coord;
	bool isCut;
	bool isActive;
	bool isPassagable;//通行可能かtrueなら通行可能
	LPDIRECT3DTEXTURE9* Texture;
}PieceT;

typedef struct
{
	//岩のやつ
	COORD Coord;
	LPDIRECT3DTEXTURE9* Texture;
}RockState;

typedef struct
{
	//ケーキのやつ
	COORD Coord;
	COORD MathPos;//ケーキのマス
	LPDIRECT3DTEXTURE9* Texture;
	float Size;
	bool isFalling;
	bool isAlive;

}CakeState;

class Stage {
	//ステージ情報のクラス
	//辺やマスの切れ落ちる処理など
private:
	LPDIRECT3DTEXTURE9* sideTextureYoko;
	LPDIRECT3DTEXTURE9* sideTextureTate;
	LPDIRECT3DTEXTURE9* blockTexture;
	LPDIRECT3DTEXTURE9* cutblockTexture;
	LPDIRECT3DTEXTURE9* wallTexture;
	LPDIRECT3DTEXTURE9* rockTexture;

	StageTypeT StageData[STAGESIZE_IGOY][STAGESIZE_IGOX];//ステージ全体の頂点・辺・マスをまとめて、それぞれがどれになっているか管理する
	PieceT Side_Tate[SIDEVERTICAL_NUM][SIDENUM_TATE];//縦の辺
	PieceT Side_Yoko[SIDEHORIZONTAL_NUM][SIDENUM_YOKO];//横の辺
	PieceT Blockinfo[TATEMASU_NUM][YOKOMASU_NUM];//マス
	PieceT Vertexinfo[VERTEXY_NUM][VERTEXX_NUM];//頂点
	RockState Rockinfo[ROCK_NUM];//岩の情報格納
	CakeState Cakeinfo[CAKE_NUM];//ケーキの情報格納
public:
	Stage(void);
	~Stage(void);
	
	float Tu = 0;//UV座標
	float Tv = 0;

	void Init(void);//初期化関数
	//↑の関数内に入れる-------
	void StageDataInit(void);
	void SideInit(void);
	void BlockInit(void);
	void VertexInit(void);
	void RockInit(void);
	void CakeInit(void);
	//-------------------------

	void Edit(void);//初期化後の手入力編集
	//↑の関数内に入れる-------
	void StageDataEdit(void);
	void SideEdit(void);
	void BlockEdit(void);
	void VertexEdit(void);
	//-------------------------

	void SetCoord(void);//ステージごとに座標を編集
	//↑の関数内に入れる-------
	void SetRockCoord(void);
	void SetCakeCoord(void);
	//-------------------------
	void SideDraw(void);
	void BlockDraw(void);
	void WallDraw(void);
	void RockDraw(bool);
	void CakeDraw(void);

	void StageCut(int,int);
	//ステージ情報取り出し関数
	PieceT OutSide_Tate(int, int);
	PieceT OutSide_Yoko(int, int);
	PieceT OutBlockInfo(int, int);
	PieceT OutVertexInfo(int, int);
	//------------------------
	void Check_Passagable(void);//通行可能かどうかを判定
	void CheckCakeFall(void);//ケーキが落ちたか
	void FallingCake(void);//ケーキが落ちた時の処理

};