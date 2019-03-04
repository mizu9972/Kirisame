#pragma once
#include "Character.h"
#include "Enemy.h"
#include "Stage.h"
#include "CutSystem.h"
#include "UI.h"

class Game {
	//�Q�[���S�̊Ǘ��N���X
private:
	Character* character = NULL;
	BrownBear* brownBear = NULL;
	BrownBear S1Brownbear[ENEMY_STAGE1_NUM];
	BrownBear S2Brownbear[ENEMY_STAGE2_NUM];
	StageBoardSystem* stageBoardSystem = NULL;
	bool Dive_State;//1�Ȃ�����ĂȂ��A0�Ȃ�����Ă�
	bool AllocFlag;//malloc�������܂����Ă��Ȃ���
	UI* ui = NULL;
	int Enemy_num;//�G�̐�
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
	bool OutClearFlg(void);//�c��̓G�̐����Q�Ƃ���CLEAR���ǂ�����Ԃ�
};

extern int DeathEnemyNum;