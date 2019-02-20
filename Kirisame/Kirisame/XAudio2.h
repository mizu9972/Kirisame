//=============================================================================
//
// �T�E���h���� [XAudio2.h]
//
//=============================================================================
#ifndef _XAUDIO2_H_
#define _XAUDIO2_H_

#include <xaudio2.h>

// �T�E���h�t�@�C��
typedef enum
{
	AttackSE = 0,		// �T���v��BGM
	ChargeSE,			// �T���v��BGM
	FallSE,				// �T���v��SE
	MissAttackSE,		// �T���v��SE
	SunaSE,				// �T���v��SE
	TitleBGM,
	GamePlayBGM,
	SOUND_LABEL_MAX,
} SOUND_LABEL;

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
HRESULT InitSound(void);
void UninitSound(void);
void PlaySound(SOUND_LABEL label);
void StopSound(SOUND_LABEL label);
void PauseSound(SOUND_LABEL label);
bool PollSound(SOUND_LABEL label);
#endif
