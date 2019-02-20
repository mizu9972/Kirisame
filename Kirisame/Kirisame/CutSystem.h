#pragma once
#include "Stage.h"
//-----------------------------------------------
//�X�e�[�W���؂ꗎ����w�b�_�[�t�@�C��
//Stage�N���X���q�Ɏ����Ă���StageBoardSystem���o�R�����ď�������
//-----------------------------------------------

//-----------------------------------------
//�N���X�錾
//-----------------------------------------
class StageBoardSystem {
private:
	int Board[STAGESIZE_IGOY][STAGESIZE_IGOX];//�X�e�[�W���(�؂�Ă��邩�ǂ���)
	int CheckedBoard[STAGESIZE_IGOY][STAGESIZE_IGOX];//�������(�����ς݂��ǂ���)


public:
	Stage *stage = NULL;

	StageBoardSystem(void);
	~StageBoardSystem(void);

	//������
	void InitBoard(void);
	void InitCheckedBoard(void);

	void CutBoard(COORD);
	void CutStage(int, int);
	bool CheckBoard(int,int);
	void BoardUpdate(COORD);

};