#pragma once

#include "2dpolygon.h"

class _TexOp {

private:
public:
	static LPDIRECT3DTEXTURE9* PlayerTex;
	static LPDIRECT3DTEXTURE9* PlayerLightTex;
	static LPDIRECT3DTEXTURE9* BrownBearTex;
	static LPDIRECT3DTEXTURE9* EnemyAttackTex;
	static LPDIRECT3DTEXTURE9* EnemySunabokoriTex;


	static LPDIRECT3DTEXTURE9* sideTextureYoko;
	static LPDIRECT3DTEXTURE9* sideTextureTate;
	static LPDIRECT3DTEXTURE9* blockTexture;
	static LPDIRECT3DTEXTURE9* cutblockTexture;
	static LPDIRECT3DTEXTURE9* wallTexture;
	static LPDIRECT3DTEXTURE9* rockTexture;
	static LPDIRECT3DTEXTURE9* cakeTexture;

	//UI
	static LPDIRECT3DTEXTURE9* UI_ScoreBoard;
	static LPDIRECT3DTEXTURE9* UI_ScoreNum;
	static LPDIRECT3DTEXTURE9* UI_TimeBoard;
	static LPDIRECT3DTEXTURE9* UI_TimeNum;
	static LPDIRECT3DTEXTURE9* UI_PlayerStatus;
	static LPDIRECT3DTEXTURE9* UI_Discription;
	static LPDIRECT3DTEXTURE9* UI_Background;


	_TexOp(void);
	~_TexOp(void);

	void LoadAllTexture(void);
	void ReleaseAllTexture(void);
};