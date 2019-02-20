#pragma once
//�X�e�[�W���Ǘ�����N���X
//�X�e�[�W�̕`��������ōs��
#include <Windows.h>
#include "config.h"
#include "2dpolygon.h"


typedef enum {
	//���_�E�ӁE�}�X�̋��
	Vertex,
	Side,
	Side_Tate,
	Side_Yoko,
	Block,
}StageTypeT;

typedef struct {
	//���_�E�ӁE�}�X�̏����e�̍\����
	COORD Coord;
	bool isCut;
	bool isActive;
	bool isPassagable;//�ʍs�\��true�Ȃ�ʍs�\
	LPDIRECT3DTEXTURE9* Texture;
}PieceT;

typedef struct
{
	//��̂��
	COORD Coord;
	LPDIRECT3DTEXTURE9* Texture;
}RockState;

typedef struct
{
	//�P�[�L�̂��
	COORD Coord;
	COORD MathPos;//�P�[�L�̃}�X
	LPDIRECT3DTEXTURE9* Texture;
	float Size;
	bool isFalling;
	bool isAlive;

}CakeState;

class Stage {
	//�X�e�[�W���̃N���X
	//�ӂ�}�X�̐؂ꗎ���鏈���Ȃ�
private:
	LPDIRECT3DTEXTURE9* sideTextureYoko;
	LPDIRECT3DTEXTURE9* sideTextureTate;
	LPDIRECT3DTEXTURE9* blockTexture;
	LPDIRECT3DTEXTURE9* cutblockTexture;
	LPDIRECT3DTEXTURE9* wallTexture;
	LPDIRECT3DTEXTURE9* rockTexture;

	StageTypeT StageData[STAGESIZE_IGOY][STAGESIZE_IGOX];//�X�e�[�W�S�̂̒��_�E�ӁE�}�X���܂Ƃ߂āA���ꂼ�ꂪ�ǂ�ɂȂ��Ă��邩�Ǘ�����
	PieceT Side_Tate[SIDEVERTICAL_NUM][SIDENUM_TATE];//�c�̕�
	PieceT Side_Yoko[SIDEHORIZONTAL_NUM][SIDENUM_YOKO];//���̕�
	PieceT Blockinfo[TATEMASU_NUM][YOKOMASU_NUM];//�}�X
	PieceT Vertexinfo[VERTEXY_NUM][VERTEXX_NUM];//���_
	RockState Rockinfo[ROCK_NUM];//��̏��i�[
	CakeState Cakeinfo[CAKE_NUM];//�P�[�L�̏��i�[
public:
	Stage(void);
	~Stage(void);
	
	float Tu = 0;//UV���W
	float Tv = 0;

	void Init(void);//�������֐�
	//���̊֐����ɓ����-------
	void StageDataInit(void);
	void SideInit(void);
	void BlockInit(void);
	void VertexInit(void);
	void RockInit(void);
	void CakeInit(void);
	//-------------------------

	void Edit(void);//��������̎���͕ҏW
	//���̊֐����ɓ����-------
	void StageDataEdit(void);
	void SideEdit(void);
	void BlockEdit(void);
	void VertexEdit(void);
	//-------------------------

	void SetCoord(void);//�X�e�[�W���Ƃɍ��W��ҏW
	//���̊֐����ɓ����-------
	void SetRockCoord(void);
	void SetCakeCoord(void);
	//-------------------------
	void SideDraw(void);
	void BlockDraw(void);
	void WallDraw(void);
	void RockDraw(bool);
	void CakeDraw(void);

	void StageCut(int,int);
	//�X�e�[�W�����o���֐�
	PieceT OutSide_Tate(int, int);
	PieceT OutSide_Yoko(int, int);
	PieceT OutBlockInfo(int, int);
	PieceT OutVertexInfo(int, int);
	//------------------------
	void Check_Passagable(void);//�ʍs�\���ǂ����𔻒�
	void CheckCakeFall(void);//�P�[�L����������
	void FallingCake(void);//�P�[�L�����������̏���

};