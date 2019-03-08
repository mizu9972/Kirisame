#pragma once

#include <Windows.h>
#include "2dpolygon.h"
#include "Stage.h"

class Character {
	//�L�����N�^�[�N���X
private:
	COORD Coord;//���W
	COORD GroundInfo;//�ӂƂ����_�Ƃ��̔z��i�[������
	COORD Pos_Vertex;//���݂ǂ��̒��_�ɂ��邩
	LPDIRECT3DTEXTURE9* Texture;//�e�N�X�`��
	LPDIRECT3DTEXTURE9* LightTexture;

	float Tu;//U
	float Tv;//V
	int KeyWait;//�L�[�̃E�F�C�g
	bool Inputflg;//���͊Ԋu�����p�̃t���O

	COORD wark_coord = Coord;//�L�����̍��W�̑ޔ�p
	COORD wark_groundinfo = GroundInfo;//�L�����̔z����W�̑ޔ�p

	StageTypeT Stagetype;
public:


	Character(void);
	~Character(void);
	void SetCoord(COORD);
	void Hit(void);//�U�����炢����

	StageTypeT OutStageType(void);
	void Move(void);//�ړ��֐�
	void Update(bool Dive_State);//�X�V����
	void Draw(void);//�`��
	void CheckMove(bool tate, bool yoko);//�����邩����
	bool CheckDive(bool tate, bool yoko, bool vertex);
	COORD OutCoord(void);
	COORD OutPos(void);
};