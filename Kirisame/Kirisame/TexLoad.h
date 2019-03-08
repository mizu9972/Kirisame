#pragma once

#include "2dpolygon.h"

class _TexOp {
	//�Ǝ��Ńe�N�X�`���ǂݍ��݂��s���N���X
	//Game�N���X����Ɨ����Ă���A�Q�[���N�����Ɉ�x�����e�N�X�`����ǂݍ����
	//���[�v�������y��������
private:
public:
	static LPDIRECT3DTEXTURE9* PlayerTex;//�v���C���[
	static LPDIRECT3DTEXTURE9* PlayerLightTex;//�v���C���[�̌���
	static LPDIRECT3DTEXTURE9* BrownBearTex;//�G�L�����N�^�[
	static LPDIRECT3DTEXTURE9* EnemyAttackTex;//�G�L�����N�^�[�U��
	static LPDIRECT3DTEXTURE9* EnemySunabokoriTex;//�G�L�������ڂ���

	//�X�e�[�W
	static LPDIRECT3DTEXTURE9* sideTextureYoko;
	static LPDIRECT3DTEXTURE9* sideTextureTate;
	static LPDIRECT3DTEXTURE9* blockTexture;
	static LPDIRECT3DTEXTURE9* cutblockTexture;
	static LPDIRECT3DTEXTURE9* wallTexture;
	static LPDIRECT3DTEXTURE9* rockTexture;
	static LPDIRECT3DTEXTURE9* cakeTexture;

	//UI
	static LPDIRECT3DTEXTURE9* UI_ScoreBoard;
	static LPDIRECT3DTEXTURE9* UI_ScoreNum;
	static LPDIRECT3DTEXTURE9* UI_TimeBoard;
	static LPDIRECT3DTEXTURE9* UI_TimeNum;
	static LPDIRECT3DTEXTURE9* UI_PlayerStatus;
	static LPDIRECT3DTEXTURE9* UI_Discription;
	static LPDIRECT3DTEXTURE9* UI_Background;


	_TexOp(void);
	~_TexOp(void);

	void LoadAllTexture(void);
	void ReleaseAllTexture(void);
};