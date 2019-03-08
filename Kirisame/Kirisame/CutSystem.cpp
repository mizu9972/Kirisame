//-----------------------------------------------
//�X�e�[�W���؂ꗎ����V�X�e��
//-----------------------------------------------
#include <Windows.h>
#include "CutSystem.h"
#include "XAudio2.h"


StageBoardSystem::StageBoardSystem(void) {
	//�R���X�g���N�^
	stage = new Stage;
}

StageBoardSystem::~StageBoardSystem(void) {
	//�f�X�g���N�^
	if (stage != NULL) {
		delete stage;
	}
}

void StageBoardSystem::InitBoard(void) {
	//�X�e�[�W��񏉊���
	for (int setY = 0; setY < STAGESIZE_IGOY; setY++) {
		for (int setX = 0; setX < STAGESIZE_IGOX; setX++) {
			Board[setY][setX] = DONTCUT;//�S�Đ؂�Ă��Ȃ���Ԃ�
		}
	}
}

void StageBoardSystem::InitCheckedBoard(void) {
	//�����p�ϐ�������
	//���^�[������������
	for (int setY = 0; setY < STAGESIZE_IGOY; setY++) {
		for (int setX = 0; setX < STAGESIZE_IGOX; setX++) {
			CheckedBoard[setY][setX] = false;//�S�Ė�������Ԃ�
		}
	}
}

void StageBoardSystem::CutBoard(COORD coord) {
	//�w��̏ꏊ��ؒf
	//�ӂⒸ�_��؂�ꍇ�̏���
	Board[coord.Y][coord.X] = YESCUT;
	stage->StageCut(coord.X, coord.Y);
}

bool StageBoardSystem::CheckBoard(int X, int Y) {
	//�}�X���؂�Ă��邩���肷��
	//�ׂ̃}�X���؂�Ă��Ȃ��Ȃ�ċA�����ŗׂ̃}�X�𔻒肵�Ă���
	//�؂�Ă����ꍇtrue��Ԃ�
	if (X >= STAGESIZE_IGOX || Y >= STAGESIZE_IGOY || X < 0 || Y < 0) {
		return false;//��ʊO�Ȃ�false��Ԃ�
	}
	if (CheckedBoard[Y][X] == CHECKED) {
		return true;//�����ς݂Ȃ�true��Ԃ�
	}

	CheckedBoard[Y][X] = CHECKED;//�����ς݂ɂ���

	if (Board[Y][X] == DONTCUT) {//�؂�Ă��Ȃ��Ȃ���͂𒲍�
								 //�ċA����
		if (CheckBoard(X + 1, Y) == false) {//�E
			return false;
		}
		if (CheckBoard(X, Y + 1) == false) {//��
			return false;
		}
		if (CheckBoard(X - 1, Y) == false) {//��
			return false;
		}
		if (CheckBoard(X, Y - 1) == false) {//��
			return false;
		}
	}

	return true;//�͂܂�Ă���
}

void StageBoardSystem::CutStage(int X, int Y) {
	//�͂܂�Ă��镔�����܂Ƃ߂Đ؂藎�Ƃ�����
	if (Board[Y][X] == DONTCUT) {
		//�؂�Ă��Ȃ��Ȃ�

		//�؂�
		if (PollSound(SunaSE) == false) {
			PlaySound(SunaSE);//���̉�
		}
		Board[Y][X] = YESCUT;
		stage->StageCut(X, Y);

		//�Ȃ����Ă���}�X�S�Đ؂��Ă���
		if (X > 1) {
			CutStage(X - 1, Y);//��
		}
		if (Y > 1) {
			CutStage(X, Y - 1);//��
		}
		if (X < STAGESIZE_IGOX - 2) {
			CutStage(X + 1, Y);//�E
		}
		if (Y < STAGESIZE_IGOY - 2) {
			CutStage(X, Y + 1);//��
		}
	}
}

void StageBoardSystem::BoardUpdate(COORD coord) {
	//�X�V����
	//�v���C���[�̎��͂̃}�X���؂�Ă��邩���肵�Ă���
	InitCheckedBoard();//����p�z�񏉊���
	if (coord.X > 1) {
		if (CheckBoard(coord.X - 1, coord.Y) == true) {//�͂܂�Ă���Ȃ�
			CutStage(coord.X - 1, coord.Y);//�͂܂�Ă���͈͂܂Ƃ߂Đ؂�
		}
	}
	//�ق��̎���̃}�X�����l�ɔ��肵�Ă���
	InitCheckedBoard();
	if (coord.Y > 1) {
		if (CheckBoard(coord.X, coord.Y - 1) == true) {
			CutStage(coord.X, coord.Y - 1);
		}
	}
	InitCheckedBoard();
	if (coord.X < STAGESIZE_IGOX - 2) {
		if (CheckBoard(coord.X + 1, coord.Y) == true) {
			CutStage(coord.X + 1, coord.Y);
		}
	}
	InitCheckedBoard();
	if (coord.Y < STAGESIZE_IGOY - 2) {
		if (CheckBoard(coord.X, coord.Y + 1) == true) {
			CutStage(coord.X, coord.Y + 1);
		}
	}
}