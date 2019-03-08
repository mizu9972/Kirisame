#pragma once

#include <Windows.h>
#include "config.h"
#include "main.h"
#include "Stage.h"

#define UP_MOVE_MODE (0)
#define DOWN_MOVE_MODE (1)
#define LEFT_MOVE_MODE (2)
#define RIGHT_MOVE_MODE (3)

typedef struct {
	COORD Coord;//座標
	int Count;
	bool Flag;//攻撃するかどうか
	LPDIRECT3DTEXTURE9 *Texture;//画像
}AttackInfoT;//攻撃情報

class Enemy {
protected:
	COORD Coord;//現在座標
	COORD MokuhyoCoord;//目標座標
	COORD SunabokoriCoord;//砂ぼこりコード
	float Size;
	COORD BlockCoord;//どこのマス上にいるか BlockInfo配列の添字になる
	int MoveMode = RIGHT_MOVE_MODE;//移動パターン
	int MoveCount = 0;
	float Animation = 0;//アニメーション用変数
	float AnimeCount = 0;//アニメーション速度
	int DrawCount = (int)(ENEMY_MOVESPEED * 2 * FPS);
	AttackInfoT AttackInfo;//攻撃情報
						   //移動可能か判定フラグ
	bool MoveFlagXRight = true;
	bool MoveFlagYUp = true;
	bool MoveFlagXLeft = true;
	bool MoveFlagYDown = true;

	bool isFalling = false;
	bool AttackMode = false;
	bool Attack = false;

	LPDIRECT3DTEXTURE9 *Texture;
	LPDIRECT3DTEXTURE9 *DiveTexture;

	bool AttackMove(void);
public:
	bool isAlive = true;

	Enemy(void);
	~Enemy(void);

	void Init(COORD);//初期化

	bool Update(COORD, bool, bool, COORD);//更新
	void Move(void);//移動
	void AvoidHole(PieceT, PieceT, PieceT, PieceT);
	void AvoidAnotherEnemy(COORD);
	void FallintoHole(void);//穴に落ちる処理

	void Draw(bool);//描画

	COORD OutBlockCoord(void);
	COORD OutPos(void);
};

class BrownBear :public Enemy {
private:
public:
	BrownBear(void);
	~BrownBear(void);
};