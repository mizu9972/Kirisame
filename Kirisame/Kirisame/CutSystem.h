#pragma once
#include "Stage.h"
//-----------------------------------------------
//ステージが切れ落ちるヘッダーファイル
//Stageクラスを子に持っておりStageBoardSystemを経由させて処理する
//-----------------------------------------------

//-----------------------------------------
//クラス宣言
//-----------------------------------------
class StageBoardSystem {
private:
	int Board[STAGESIZE_IGOY][STAGESIZE_IGOX];//ステージ情報(切れているかどうか)
	int CheckedBoard[STAGESIZE_IGOY][STAGESIZE_IGOX];//調査情報(調査済みかどうか)


public:
	Stage *stage = NULL;

	StageBoardSystem(void);
	~StageBoardSystem(void);

	//初期化
	void InitBoard(void);
	void InitCheckedBoard(void);

	void CutBoard(COORD);
	void CutStage(int, int);
	bool CheckBoard(int,int);
	void BoardUpdate(COORD);

};