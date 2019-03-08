//-----------------------------------------------
//ステージが切れ落ちるシステム
//-----------------------------------------------
#include <Windows.h>
#include "CutSystem.h"
#include "XAudio2.h"


StageBoardSystem::StageBoardSystem(void) {
	//コンストラクタ
	stage = new Stage;
}

StageBoardSystem::~StageBoardSystem(void) {
	//デストラクタ
	if (stage != NULL) {
		delete stage;
	}
}

void StageBoardSystem::InitBoard(void) {
	//ステージ情報初期化
	for (int setY = 0; setY < STAGESIZE_IGOY; setY++) {
		for (int setX = 0; setX < STAGESIZE_IGOX; setX++) {
			Board[setY][setX] = DONTCUT;//全て切れていない状態に
		}
	}
}

void StageBoardSystem::InitCheckedBoard(void) {
	//調査用変数初期化
	//毎ターン初期化する
	for (int setY = 0; setY < STAGESIZE_IGOY; setY++) {
		for (int setX = 0; setX < STAGESIZE_IGOX; setX++) {
			CheckedBoard[setY][setX] = false;//全て未調査状態に
		}
	}
}

void StageBoardSystem::CutBoard(COORD coord) {
	//指定の場所を切断
	//辺や頂点を切る場合の処理
	Board[coord.Y][coord.X] = YESCUT;
	stage->StageCut(coord.X, coord.Y);
}

bool StageBoardSystem::CheckBoard(int X, int Y) {
	//マスが切れているか判定する
	//隣のマスが切れていないなら再帰処理で隣のマスを判定していく
	//切れていた場合trueを返す
	if (X >= STAGESIZE_IGOX || Y >= STAGESIZE_IGOY || X < 0 || Y < 0) {
		return false;//画面外ならfalseを返す
	}
	if (CheckedBoard[Y][X] == CHECKED) {
		return true;//調査済みならtrueを返す
	}

	CheckedBoard[Y][X] = CHECKED;//調査済みにする

	if (Board[Y][X] == DONTCUT) {//切れていないなら周囲を調査
								 //再帰処理
		if (CheckBoard(X + 1, Y) == false) {//右
			return false;
		}
		if (CheckBoard(X, Y + 1) == false) {//下
			return false;
		}
		if (CheckBoard(X - 1, Y) == false) {//左
			return false;
		}
		if (CheckBoard(X, Y - 1) == false) {//上
			return false;
		}
	}

	return true;//囲まれている
}

void StageBoardSystem::CutStage(int X, int Y) {
	//囲まれている部分をまとめて切り落とす処理
	if (Board[Y][X] == DONTCUT) {
		//切れていないなら

		//切る
		if (PollSound(SunaSE) == false) {
			PlaySound(SunaSE);//砂の音
		}
		Board[Y][X] = YESCUT;
		stage->StageCut(X, Y);

		//つながっているマス全て切っていく
		if (X > 1) {
			CutStage(X - 1, Y);//左
		}
		if (Y > 1) {
			CutStage(X, Y - 1);//上
		}
		if (X < STAGESIZE_IGOX - 2) {
			CutStage(X + 1, Y);//右
		}
		if (Y < STAGESIZE_IGOY - 2) {
			CutStage(X, Y + 1);//下
		}
	}
}

void StageBoardSystem::BoardUpdate(COORD coord) {
	//更新処理
	//プレイヤーの周囲のマスが切れているか判定していく
	InitCheckedBoard();//判定用配列初期化
	if (coord.X > 1) {
		if (CheckBoard(coord.X - 1, coord.Y) == true) {//囲まれているなら
			CutStage(coord.X - 1, coord.Y);//囲まれている範囲まとめて切る
		}
	}
	//ほかの周りのマスも同様に判定していく
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