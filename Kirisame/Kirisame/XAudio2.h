//=============================================================================
//
// サウンド処理 [XAudio2.h]
//
//=============================================================================
#ifndef _XAUDIO2_H_
#define _XAUDIO2_H_

#include <xaudio2.h>

// サウンドファイル
typedef enum
{
	AttackSE = 0,		// サンプルBGM
	ChargeSE,			// サンプルBGM
	FallSE,				// サンプルSE
	MissAttackSE,		// サンプルSE
	SunaSE,				// サンプルSE
	TitleBGM,
	GamePlayBGM,
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitSound(void);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void PauseSound(SOUND_LABEL label);
bool PollSound(SOUND_LABEL label);
#endif
