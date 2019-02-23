#include "UI.h"
#include "Asset.h"
#include "main.h"

UI::UI(void) {
	//コンストラクタ
	Scoreboard = TexOp->UI_ScoreBoard;
	Scorenum = TexOp->UI_ScoreNum;
	Timeboard = TexOp->UI_TimeBoard;
	Timenum = TexOp->UI_TimeNum;
	Playerstatus = TexOp->UI_PlayerStatus;
	Description = TexOp->UI_Discription;
	Background = TexOp->UI_Background;

}
UI::~UI(void) {
	//デストラクタ
}


void UI::UIDraw(bool Dive_State) {
	//描画
	if (Dive_State) {

		Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), Background, 0.5, 0, 0.5, 1);//背景明るい
	}
	else {
		Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), Background, 0, 0, 0.5, 1);//背景暗い
	}
	Draw2dPolygon(SCOREBOARD_X, SCOREBOARD_Y, SCOREBOARD_W, SCOREBOARD_H, D3DCOLOR_ARGB(255, 255, 255, 255), Scoreboard, 0, 0, 1, 1);//スコアボード
	Draw2dPolygon(TIMEBOARD_X, TIMEBOARD_Y, TIMEBOARD_W, TIMEBOARD_H, D3DCOLOR_ARGB(255, 255, 255, 255), Timeboard, 0, 0, 1, 1);//タイムボード
		//潜ってるか潜ってないかで画像変更																															//潜ってるか潜ってないかで画像変更																															//潜ってるか潜ってないかで画像変更
	if (Dive_State) {
		Draw2dPolygon(PUI_X, PUI_Y, PUI_W, PUI_H, D3DCOLOR_ARGB(255, 255, 255, 255), Playerstatus, 0, 0, 0.5, 1);//プレイヤーステータス潜ってない
	}
	else {
		Draw2dPolygon(PUI_X, PUI_Y, PUI_W, PUI_H, D3DCOLOR_ARGB(255, 255, 255, 255), Playerstatus, 0.5, 0, 0.5, 1);//プレイヤーステータス潜ってる
	}
	Draw2dPolygon(DESCRIPTION_X, DESCRIPTION_Y, DESCRIPTION_W, DESCRIPTION_H, D3DCOLOR_ARGB(255, 255, 255, 255), Description, 0, 0, 1, 1);//操作説明
	Draw2dPolygon(TIMENUM_X, TIMENUM_Y, TIMENUM_W, TIMENUM_H, D3DCOLOR_ARGB(255, 255, 255, 255), Timenum, time % 5 * 0.2f, time / 5 * 0.5f, 0.2f, 0.5f);//タイムの1の位
	Draw2dPolygon(TIMENUM_X - TIMENUM_W, TIMENUM_Y, TIMENUM_W, TIMENUM_H, D3DCOLOR_ARGB(255, 255, 255, 255), Timenum, time / 10 % 5 * 0.2f, time / 10 / 5 * 0.5f, 0.2f, 0.5f);//タイムの10の位
	Draw2dPolygon(TIMENUM_X - TIMENUM_W - TIMENUM_W, TIMENUM_Y, TIMENUM_W, TIMENUM_H, D3DCOLOR_ARGB(255, 255, 255, 255), Timenum, time / 100 % 5 * 0.2f, time / 100 / 5 * 0.5f, 0.2f, 0.5f);//タイムの100の位
	if (time / 1000 >= 1)
	{
		Draw2dPolygon(TIMENUM_X - TIMENUM_W - TIMENUM_W - TIMENUM_W, TIMENUM_Y, TIMENUM_W, TIMENUM_H, D3DCOLOR_ARGB(255, 255, 255, 255), Timenum, time / 1000 % 5 * 0.2f, time / 1000 / 5 * 0.5f, 0.2f, 0.5f);//タイムの100の位
	}

}

void UI::TIME(void) {
	end = timeGetTime();    // 経過時間
	time = (end - start) / 1000;//経過時間をミリ秒から秒に変換して代入
}

void UI::GTIME(void) {
	start = timeGetTime();       // スタート時間の取得
}
