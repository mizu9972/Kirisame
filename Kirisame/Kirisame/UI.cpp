#include "UI.h"
#include "Asset.h"
#include "main.h"

UI::UI(void) {
	//�R���X�g���N�^
	Scoreboard = TexOp->UI_ScoreBoard;
	Scorenum = TexOp->UI_ScoreNum;
	Timeboard = TexOp->UI_TimeBoard;
	Timenum = TexOp->UI_TimeNum;
	Playerstatus = TexOp->UI_PlayerStatus;
	Description = TexOp->UI_Discription;
	Background = TexOp->UI_Background;

}
UI::~UI(void) {
	//�f�X�g���N�^
}


void UI::UIDraw(bool Dive_State) {
	//�`��
	if (Dive_State) {

		Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), Background, 0.5, 0, 0.5, 1);//�w�i���邢
	}
	else {
		Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), Background, 0, 0, 0.5, 1);//�w�i�Â�
	}
	Draw2dPolygon(SCOREBOARD_X, SCOREBOARD_Y, SCOREBOARD_W, SCOREBOARD_H, D3DCOLOR_ARGB(255, 255, 255, 255), Scoreboard, 0, 0, 1, 1);//�X�R�A�{�[�h
	Draw2dPolygon(TIMEBOARD_X, TIMEBOARD_Y, TIMEBOARD_W, TIMEBOARD_H, D3DCOLOR_ARGB(255, 255, 255, 255), Timeboard, 0, 0, 1, 1);//�^�C���{�[�h
		//�����Ă邩�����ĂȂ����ŉ摜�ύX																															//�����Ă邩�����ĂȂ����ŉ摜�ύX																															//�����Ă邩�����ĂȂ����ŉ摜�ύX
	if (Dive_State) {
		Draw2dPolygon(PUI_X, PUI_Y, PUI_W, PUI_H, D3DCOLOR_ARGB(255, 255, 255, 255), Playerstatus, 0, 0, 0.5, 1);//�v���C���[�X�e�[�^�X�����ĂȂ�
	}
	else {
		Draw2dPolygon(PUI_X, PUI_Y, PUI_W, PUI_H, D3DCOLOR_ARGB(255, 255, 255, 255), Playerstatus, 0.5, 0, 0.5, 1);//�v���C���[�X�e�[�^�X�����Ă�
	}
	Draw2dPolygon(DESCRIPTION_X, DESCRIPTION_Y, DESCRIPTION_W, DESCRIPTION_H, D3DCOLOR_ARGB(255, 255, 255, 255), Description, 0, 0, 1, 1);//�������
	Draw2dPolygon(TIMENUM_X, TIMENUM_Y, TIMENUM_W, TIMENUM_H, D3DCOLOR_ARGB(255, 255, 255, 255), Timenum, time % 5 * 0.2f, time / 5 * 0.5f, 0.2f, 0.5f);//�^�C����1�̈�
	Draw2dPolygon(TIMENUM_X - TIMENUM_W, TIMENUM_Y, TIMENUM_W, TIMENUM_H, D3DCOLOR_ARGB(255, 255, 255, 255), Timenum, time / 10 % 5 * 0.2f, time / 10 / 5 * 0.5f, 0.2f, 0.5f);//�^�C����10�̈�
	Draw2dPolygon(TIMENUM_X - TIMENUM_W - TIMENUM_W, TIMENUM_Y, TIMENUM_W, TIMENUM_H, D3DCOLOR_ARGB(255, 255, 255, 255), Timenum, time / 100 % 5 * 0.2f, time / 100 / 5 * 0.5f, 0.2f, 0.5f);//�^�C����100�̈�
	if (time / 1000 >= 1)
	{
		Draw2dPolygon(TIMENUM_X - TIMENUM_W - TIMENUM_W - TIMENUM_W, TIMENUM_Y, TIMENUM_W, TIMENUM_H, D3DCOLOR_ARGB(255, 255, 255, 255), Timenum, time / 1000 % 5 * 0.2f, time / 1000 / 5 * 0.5f, 0.2f, 0.5f);//�^�C����100�̈�
	}

}

void UI::TIME(void) {
	end = timeGetTime();    // �o�ߎ���
	time = (end - start) / 1000;//�o�ߎ��Ԃ��~���b����b�ɕϊ����đ��
}

void UI::GTIME(void) {
	start = timeGetTime();       // �X�^�[�g���Ԃ̎擾
}
