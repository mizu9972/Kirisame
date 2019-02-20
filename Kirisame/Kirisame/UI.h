#pragma once
//UI���Ǘ�����N���X
#include <Windows.h>
#include "2dpolygon.h"
#pragma comment(lib, "winmm.lib")

class UI {
	//UI�N���X
private:
	LPDIRECT3DTEXTURE9* Scoreboard;
	LPDIRECT3DTEXTURE9* Scorenum;
	LPDIRECT3DTEXTURE9* Timeboard;
	LPDIRECT3DTEXTURE9* Timenum;
	LPDIRECT3DTEXTURE9* Playerstatus;
	LPDIRECT3DTEXTURE9* Description;
	LPDIRECT3DTEXTURE9* Background;

#define SCOREBOARD_X (40.0f)//�X�R�A�{�[�h�ʒu(X���W)
#define SCOREBOARD_Y (50.0f)//�X�R�A�{�[�h�ʒu(Y���W)
#define SCOREBOARD_W (200.0f)//�X�R�A�{�[�h��(X���W)
#define SCOREBOARD_H (150.0f)//�X�R�A�{�[�h����(Y���W)

#define TIMEBOARD_X (40.0f)//�^�C���{�[�h�ʒu(X���W)
#define TIMEBOARD_Y (300.0f)//�^�C���{�[�h�ʒu(Y���W)
#define TIMEBOARD_W (200.0f)//�^�C���{�[�h��(X���W)
#define TIMEBOARD_H (150.0f)//�^�C���{�[�h����(Y���W)

#define PUI_X (780.0f)//�v���C���[�X�e�[�^�X�ʒu(X���W)
#define PUI_Y (50.0f)//�v���C���[�X�e�[�^�X�ʒu(Y���W)
#define PUI_W (200.0f)//�v���C���[�X�e�[�^�X��(X���W)
#define PUI_H (200.0f)//�v���C���[�X�e�[�^�X����(Y���W)


#define DESCRIPTION_X (780.0f)//��������ʒu(X���W)
#define DESCRIPTION_Y (300.0f)//��������ʒu(Y���W)
#define DESCRIPTION_W (210.0f)//���������(X���W)
#define DESCRIPTION_H (180.0f)//�����������(Y���W)


#define TIMENUM_X (190.0f)//���Ԉʒu(X���W)
#define TIMENUM_Y (350.0f)//���Ԉʒu(Y���W)
#define TIMENUM_W (50.0f)//���ԕ�(X���W)
#define TIMENUM_H (100.0f)//���ԍ���(Y���W)

	int time; //����

	DWORD start;
	DWORD end;

public:
	UI(void);
	~UI(void);
	void UIDraw(bool Dive_State);
	void TIME(void);
	void GTIME(void);
};