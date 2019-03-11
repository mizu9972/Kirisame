
#include "TexLoad.h"
#include "Asset.h"
LPDIRECT3DTEXTURE9* _TexOp::PlayerTex;//プレイヤー
LPDIRECT3DTEXTURE9* _TexOp::PlayerLightTex;//プレイヤーの光沢
LPDIRECT3DTEXTURE9* _TexOp::BrownBearTex;//敵キャラクター
LPDIRECT3DTEXTURE9* _TexOp::EnemyAttackTex;//敵キャラクター攻撃
LPDIRECT3DTEXTURE9* _TexOp::EnemySunabokoriTex;//敵キャラ砂ぼこり
											   //ステージ
LPDIRECT3DTEXTURE9* _TexOp::sideTextureYoko;
LPDIRECT3DTEXTURE9* _TexOp::sideTextureTate;
LPDIRECT3DTEXTURE9* _TexOp::blockTexture;
LPDIRECT3DTEXTURE9* _TexOp::cutblockTexture;
LPDIRECT3DTEXTURE9* _TexOp::wallTexture;
LPDIRECT3DTEXTURE9* _TexOp::rockTexture;
LPDIRECT3DTEXTURE9* _TexOp::cakeTexture;

//UI
LPDIRECT3DTEXTURE9* _TexOp::UI_ScoreBoard;
LPDIRECT3DTEXTURE9* _TexOp::UI_ScoreNum;
LPDIRECT3DTEXTURE9* _TexOp::UI_TimeBoard;
LPDIRECT3DTEXTURE9* _TexOp::UI_TimeNum;
LPDIRECT3DTEXTURE9* _TexOp::UI_PlayerStatus;
LPDIRECT3DTEXTURE9* _TexOp::UI_Discription;
LPDIRECT3DTEXTURE9* _TexOp::UI_Background;
LPDIRECT3DTEXTURE9* _TexOp::UI_ContDiscription;

_TexOp::_TexOp(void) {

}

_TexOp::~_TexOp(void) {

}

void _TexOp::LoadAllTexture(void) {
	//総テクスチャ読み込み
	//キャラクター
	PlayerTex = LoadTexture(HIRE_TEX_FILE, PlayerTex);
	PlayerLightTex = LoadTexture(HIRE_BRIGHT_TEX_FILE, PlayerLightTex);
	BrownBearTex = LoadTexture(ENEMY_BROWNBEAR_TEX_FILE, BrownBearTex);
	EnemyAttackTex = LoadTexture(ENEMY_SLASH_TEX_FILE, EnemyAttackTex);
	EnemySunabokoriTex = LoadTexture(ENEMY_SUNABOKORI_TEX_FILE, EnemySunabokoriTex);

	//ステージ
	sideTextureYoko = LoadTexture(SIDE_YOKO_TEX_FILE, sideTextureYoko);
	sideTextureTate = LoadTexture(SIDE_TATE_TEX_FILE, sideTextureTate);
	blockTexture = LoadTexture(TILE_TEX_FILE, blockTexture);
	cutblockTexture = LoadTexture(TILE_CUT_TEX_FILE, cutblockTexture);
	wallTexture = LoadTexture(WALL_TEX_FILE, wallTexture);
	rockTexture = LoadTexture(ROCK_TEX_FILE, rockTexture);
	cakeTexture = LoadTexture(CAKE_TEX_FILE, cakeTexture);

	//UI
	UI_ScoreBoard = LoadTexture(SCOREBOARD_FILE, UI_ScoreBoard);
	UI_ScoreNum = LoadTexture(SCORENUM_FILE, UI_ScoreNum);
	UI_TimeBoard = LoadTexture(TIMEBOARD_FILE, UI_TimeBoard);
	UI_TimeNum = LoadTexture(TIMENUM_FILE, UI_TimeNum);
	UI_PlayerStatus = LoadTexture(UIPlayerstatus_FILE, UI_PlayerStatus);
	UI_Discription = LoadTexture(DESCRIPTION_FILE, UI_Discription);
	UI_Background = LoadTexture(BACKGROUND_FILE, UI_Background);
	UI_ContDiscription = LoadTexture(CONTDISCRIPTION, UI_ContDiscription);
}

void _TexOp::ReleaseAllTexture(void) {
	//総テクスチャ解放
	//キャラクター
	ReleaseTexture(PlayerTex);
	ReleaseTexture(PlayerLightTex);
	ReleaseTexture(BrownBearTex);
	ReleaseTexture(EnemyAttackTex);
	ReleaseTexture(EnemySunabokoriTex);

	//ステージ
	ReleaseTexture(sideTextureYoko);
	ReleaseTexture(sideTextureTate);
	ReleaseTexture(blockTexture);
	ReleaseTexture(cutblockTexture);
	ReleaseTexture(wallTexture);
	ReleaseTexture(rockTexture);
	ReleaseTexture(cakeTexture);

	//UI
	ReleaseTexture(UI_ScoreBoard);
	ReleaseTexture(UI_ScoreNum);
	ReleaseTexture(UI_TimeBoard);
	ReleaseTexture(UI_TimeNum);
	ReleaseTexture(UI_PlayerStatus);
	ReleaseTexture(UI_Discription);
	ReleaseTexture(UI_Background);
	ReleaseTexture(UI_ContDiscription);

}