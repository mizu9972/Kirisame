#pragma once
//UIを管理するクラス
#include <Windows.h>
#include "2dpolygon.h"
#pragma comment(lib, "winmm.lib")

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

#define SCOREBOARD_X (40.0f)//スコアボード位置(X座標)
#define SCOREBOARD_Y (50.0f)//スコアボード位置(Y座標)
#define SCOREBOARD_W (200.0f)//スコアボード幅(X座標)
#define SCOREBOARD_H (150.0f)//スコアボード高さ(Y座標)

#define TIMEBOARD_X (40.0f)//タイムボード位置(X座標)
#define TIMEBOARD_Y (300.0f)//タイムボード位置(Y座標)
#define TIMEBOARD_W (200.0f)//タイムボード幅(X座標)
#define TIMEBOARD_H (150.0f)//タイムボード高さ(Y座標)

#define PUI_X (780.0f)//プレイヤーステータス位置(X座標)
#define PUI_Y (50.0f)//プレイヤーステータス位置(Y座標)
#define PUI_W (200.0f)//プレイヤーステータス幅(X座標)
#define PUI_H (200.0f)//プレイヤーステータス高さ(Y座標)


#define DESCRIPTION_X (780.0f)//操作説明位置(X座標)
#define DESCRIPTION_Y (300.0f)//操作説明位置(Y座標)
#define DESCRIPTION_W (210.0f)//操作説明幅(X座標)
#define DESCRIPTION_H (180.0f)//操作説明高さ(Y座標)


#define TIMENUM_X (190.0f)//時間位置(X座標)
#define TIMENUM_Y (350.0f)//時間位置(Y座標)
#define TIMENUM_W (50.0f)//時間幅(X座標)
#define TIMENUM_H (100.0f)//時間高さ(Y座標)

	int time; //時間

	DWORD start;
	DWORD end;

public:
	UI(void);
	~UI(void);
	void UIDraw(bool Dive_State);
	void TIME(void);
	void GTIME(void);
};