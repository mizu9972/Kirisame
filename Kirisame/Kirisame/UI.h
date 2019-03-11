#pragma once
//UI���Ǘ�����N���X
#include <Windows.h>
#include "2dpolygon.h"
#pragma comment(lib, "winmm.lib")


typedef struct
{
	int DigitOne;//1�̈�
	int DigitTen;//10�̈�
	int DigitHundret;//100�̈�
	int DigitThousand;//1000�̈�
}DIGIT;

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
	int wark_score;
	int time; //����
	int RestMath;//�c��}�X
	DIGIT Math;//�c��}�X���ʂ��Ƃɕς���
	DWORD start;
	DWORD end;

public:
	int Stage1Score;//�X�e�[�W�P�̃X�R�A
	int Stage2Score;//�X�e�[�W2�̃X�R�A
    static int Score;//Score
	UI(void);
	~UI(void);
	void UIDraw(bool Dive_State);
	void TIME(void);
	void GTIME(void);
	void DrawRestMath(void);//�c��}�X�̕`��
	void DrawResult(void);//�X�R�A�o��
	void Calculation(int Rest);//�v�Z�����͂��邩���낵���B
};
