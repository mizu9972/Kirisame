#pragma once
//UIを管理するクラス
#include <Windows.h>
#include "2dpolygon.h"
#pragma comment(lib, "winmm.lib")


typedef struct
{
	int DigitOne;//1の位
	int DigitTen;//10の位
	int DigitHundret;//100の位
	int DigitThousand;//1000の位
}DIGIT;

class UI {
	//UIクラス
private:
	LPDIRECT3DTEXTURE9* Scoreboard;
	LPDIRECT3DTEXTURE9* Scorenum;
	LPDIRECT3DTEXTURE9* Timeboard;
	LPDIRECT3DTEXTURE9* Timenum;
	LPDIRECT3DTEXTURE9* Playerstatus;
	LPDIRECT3DTEXTURE9* Description;
	LPDIRECT3DTEXTURE9* Background;
	int wark_score;
	int time; //時間
	int RestMath;//残りマス
	DIGIT Math;//残りマスを位ごとに変える
	DWORD start;
	DWORD end;

public:
	int Stage1Score;//ステージ１のスコア
	int Stage2Score;//ステージ2のスコア
    static int Score;//Score
	UI(void);
	~UI(void);
	void UIDraw(bool Dive_State);
	void TIME(void);
	void GTIME(void);
	void DrawRestMath(void);//残りマスの描画
	void DrawResult(void);//スコア出す
	void Calculation(int Rest);//計算式入力するからよろしく。
};
