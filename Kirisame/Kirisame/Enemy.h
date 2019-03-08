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
	COORD Coord;//���W
	int Count;
	bool Flag;//�U�����邩�ǂ���
	LPDIRECT3DTEXTURE9 *Texture;//�摜
}AttackInfoT;//�U�����

class Enemy {
protected:
	COORD Coord;//���ݍ��W
	COORD MokuhyoCoord;//�ڕW���W
	COORD SunabokoriCoord;//���ڂ���R�[�h
	float Size;
	COORD BlockCoord;//�ǂ��̃}�X��ɂ��邩 BlockInfo�z��̓Y���ɂȂ�
	int MoveMode = RIGHT_MOVE_MODE;//�ړ��p�^�[��
	int MoveCount = 0;
	float Animation = 0;//�A�j���[�V�����p�ϐ�
	float AnimeCount = 0;//�A�j���[�V�������x
	int DrawCount = (int)(ENEMY_MOVESPEED * 2 * FPS);
	AttackInfoT AttackInfo;//�U�����
						   //�ړ��\������t���O
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

	void Init(COORD);//������

	bool Update(COORD, bool, bool, COORD);//�X�V
	void Move(void);//�ړ�
	void AvoidHole(PieceT, PieceT, PieceT, PieceT);
	void AvoidAnotherEnemy(COORD);
	void FallintoHole(void);//���ɗ����鏈��

	void Draw(bool);//�`��

	COORD OutBlockCoord(void);
	COORD OutPos(void);
};

class BrownBear :public Enemy {
private:
public:
	BrownBear(void);
	~BrownBear(void);
};