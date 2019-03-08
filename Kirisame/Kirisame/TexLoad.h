#pragma once

#include "2dpolygon.h"

class _TexOp {
	//独自でテクスチャ読み込みを行うクラス
	//Gameクラスから独立しており、ゲーム起動時に一度だけテクスチャを読み込んで
	//ループ処理を軽くさせる
private:
public:
	static LPDIRECT3DTEXTURE9* PlayerTex;//プレイヤー
	static LPDIRECT3DTEXTURE9* PlayerLightTex;//プレイヤーの光沢
	static LPDIRECT3DTEXTURE9* BrownBearTex;//敵キャラクター
	static LPDIRECT3DTEXTURE9* EnemyAttackTex;//敵キャラクター攻撃
	static LPDIRECT3DTEXTURE9* EnemySunabokoriTex;//敵キャラ砂ぼこり

	//ステージ
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