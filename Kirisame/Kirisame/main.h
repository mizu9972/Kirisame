//======================================================
//
// DX21
// DirectX���^����[main.h]
//
//======================================================
#ifndef _MAIN_H_
#define _MAIN_H_

//******************************************************
// �C���N���[�h�t�@�C��
//******************************************************
#include <windows.h>
#include "d3dx9.h"       //�`�揈���ɕK�v
#include "2dpolygon.h"
#include "input.h"
//******************************************************
// ���C�u�����̃����N
//******************************************************
#if 1 //������"0"�ɂ����ꍇ�A"�\���v���p�e�B" -> "�����J�[" -> "����" -> ""�ǉ��̈ˑ��t�@�C���ɑΏۃ��C�u������ݒ肷��

#pragma comment (lib,"d3d9.lib")  // �`�揈���ɕK�v
#pragma comment (lib,"d3dx9.lib") // [d3d9.lib]�̊g�����C�u����
#pragma comment (lib,"dxguid.lib")// DirectX�R���|�[�l���g�g�p�ɕK�v
#pragma comment (lib,"winmm.lib")
#endif

//******************************************************
// �}�N����`
//******************************************************
#define SCREEN_WIDTH  (1000)   //�E�C���h�E�̕�
#define SCREEN_HEIGHT (600)   //�E�C���h�E�̍���

enum SCENE {//���
	TITLE,
	TUTORIAL,
	GAME_STAGE1,
	GAME_STAGE2,
	RESULT,
	LOAD
};
extern SCENE Scene;

#endif
