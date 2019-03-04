#pragma once
#include "Character.h"
#include "Enemy.h"
#include "Stage.h"
#include "CutSystem.h"
#include "UI.h"

class Game {
	//ゲーム全体管理クラス
private:
	Character* character = NULL;
	BrownBear* brownBear = NULL;
	BrownBear S1Brownbear[ENEMY_STAGE1_NUM];
	BrownBear S2Brownbear[ENEMY_STAGE2_NUM];
	StageBoardSystem* stageBoardSystem = NULL;
	bool Dive_State;//1なら潜ってない、0なら潜ってる
	bool AllocFlag;//mallocしたかまだしていないか
	UI* ui = NULL;
	int Enemy_num;//敵の数
	int Enemy_MaxNum;
public:
	Game(void);
	~Game(void);

	void Init(void);
	void EnemyUnInit(void);
	void Edit(void);
	void SetCoord(void);
	void EnemyInit(void);
	void Draw(void);
	void Update(void);
	void Gettime(void);
	bool OutClearFlg(void);//残りの敵の数を参照してCLEARかどうかを返す
};

extern int DeathEnemyNum;