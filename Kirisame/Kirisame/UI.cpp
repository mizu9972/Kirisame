#include "UI.h"
#include "Asset.h"
#include "main.h"
#include "config.h"
UI::UI(void) {
	//コンストラクタ
	Scoreboard = TexOp->UI_ScoreBoard;
	Scorenum = TexOp->UI_ScoreNum;
	Timeboard = TexOp->UI_TimeBoard;
	Timenum = TexOp->UI_TimeNum;
	Playerstatus = TexOp->UI_PlayerStatus;
	Description = TexOp->UI_Discription;
	Background = TexOp->UI_Background;
	Score = 0;
	Stage1Score = 0;
	Stage2Score = 0;
	wark_score = 0;
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
void UI::DrawRestMath()
{
	float Tu = 0;
	float Tv = 0;
	
	
	//取得した位を元に数字テクスチャを描画
	Tu = (Math.DigitOne % 5) * 0.2f;
	Tv = Math.DigitOne / 5 * 0.5f;
	Draw2dPolygon(SCORE_X, SCORE_Y, TIMENUM_W, TIMENUM_H, D3DCOLOR_ARGB(255, 255, 255, 255), Timenum, Tu, Tv, 0.2, 0.5);//1の位
	Tu = (Math.DigitTen % 5) * 0.2f;
	Tv = Math.DigitTen / 5 * 0.5f;
	Draw2dPolygon(SCORE_X - TIMENUM_W, SCORE_Y, TIMENUM_W, TIMENUM_H, D3DCOLOR_ARGB(255, 255, 255, 255), Timenum, Tu, Tv, 0.2, 0.5);//10の位
	Tu = (Math.DigitHundret % 5) * 0.2f;
	Tv = Math.DigitHundret / 5 * 0.5f;
	Draw2dPolygon(SCORE_X - TIMENUM_W * 2, SCORE_Y, TIMENUM_W, TIMENUM_H, D3DCOLOR_ARGB(255, 255, 255, 255), Timenum, Tu, Tv, 0.2, 0.5);//100の位
	Tu = (Math.DigitThousand % 5) * 0.2f;
	Tv = Math.DigitThousand / 5 * 0.5f;
	Draw2dPolygon(SCORE_X - TIMENUM_W * 3, SCORE_Y, TIMENUM_W, TIMENUM_H, D3DCOLOR_ARGB(255, 255, 255, 255), Timenum, Tu, Tv, 0.2, 0.5);//1000の位
}

void UI::DrawResult(void)
{
	float Tu = 0;
	float Tv = 0;

	Math.DigitOne = Score % 10;//1の位
	Math.DigitTen = (Score % 100) / 10;//10の位
	Math.DigitHundret = (Score % 1000) / 100;//100の位
	Math.DigitThousand = (Score % 10000) / 1000;//1000の位
													 //取得した位を元に数字テクスチャを描画
	Tu = (Math.DigitOne % 5) * 0.2f;
	Tv = Math.DigitOne / 5 * 0.5f;
	Draw2dPolygon(RESULT_X, RESULT_Y, TIMENUM_W, TIMENUM_H, D3DCOLOR_ARGB(255, 255, 255, 255), Timenum, Tu, Tv, 0.2, 0.5);//1の位
	Tu = (Math.DigitTen % 5) * 0.2f;
	Tv = Math.DigitTen / 5 * 0.5f;
	Draw2dPolygon(RESULT_X - TIMENUM_W, RESULT_Y, TIMENUM_W, TIMENUM_H, D3DCOLOR_ARGB(255, 255, 255, 255), Timenum, Tu, Tv, 0.2, 0.5);//10の位
	Tu = (Math.DigitHundret % 5) * 0.2f;
	Tv = Math.DigitHundret / 5 * 0.5f;
	Draw2dPolygon(RESULT_X - TIMENUM_W * 2, RESULT_Y, TIMENUM_W, TIMENUM_H, D3DCOLOR_ARGB(255, 255, 255, 255), Timenum, Tu, Tv, 0.2, 0.5);//100の位
	Tu = (Math.DigitThousand % 5) * 0.2f;
	Tv = Math.DigitThousand / 5 * 0.5f;
	Draw2dPolygon(RESULT_X - TIMENUM_W * 3, RESULT_Y, TIMENUM_W, TIMENUM_H, D3DCOLOR_ARGB(255, 255, 255, 255), Timenum, Tu, Tv, 0.2, 0.5);//1000の位

}

void UI::Calculation(int Rest)
{
	wark_score = Rest*MATHSCORE + Score;
	switch (Scene)
	{
	case GAME_STAGE1:
		Stage1Score = Rest*MATHSCORE + Score;
		break;
	case GAME_STAGE2:
		Stage2Score = Rest*MATHSCORE + Score;
		break;
	case RESULT:
		Score = Stage1Score + Stage2Score;//各ステージのスコアを合算
		break;
	}
	if (wark_score <= 0)//0以下になったら
	{
		wark_score = 0;
	}
	Math.DigitOne = wark_score % 10;//1の位
	Math.DigitTen = (wark_score % 100) / 10;//10の位
	Math.DigitHundret = (wark_score % 1000) / 100;//100の位
	Math.DigitThousand = (wark_score % 10000) / 1000;//1000の位
}