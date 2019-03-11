///-----------------------------------------------
//�L�����N�^�[�N���X�̏���
//-----------------------------------------------
#include "Character.h"
#include "config.h"
#include "Asset.h"
#include "main.h"
#include "TexLoad.h"

#define HOSEITI 15.0f//�L�����N�^�[�`��ʒu�����p
extern LPDIRECT3DDEVICE9 g_pD3DDevice;
extern LPDIRECTINPUTDEVICE8 g_pDIDevGamePad;

typedef struct {
	float CoordX;
	float CoordY;
	float LightU;
	float LightV;
	float LightW;
	float LightH;
	float LightTexSizeU;
	float LightTexSizeV;
}_LightingT;

Character::Character(void) {
	//�����ʒu���Z�b�g
	Coord.X = PLAYER_STARTPOS_X;
	Coord.Y = PLAYER_STARTPOS_Y;

	//�����ʒu�̔z����Z�b�g
	GroundInfo.X = (PLAYER_STARTPOS_X - STAGEPOS_YOKO) / (MASUWIDTH / 2);
	GroundInfo.Y = (PLAYER_STARTPOS_Y - STAGEPOS_TATE) / (MASUHEIGHT / 2);

	////�����ʒu�̒��_�Z�b�g
	Pos_Vertex.X = 2;
	Pos_Vertex.Y = 4;
	//�e�N�X�`���Z�b�g
	Texture = TexOp->PlayerTex;
	LightTexture = TexOp->PlayerLightTex;
	KeyWait = 0;
	Inputflg = false;

	UpdateInput();
	JoypadDI_X = GetGamePadLeftStickX();
	JoypadDI_Y = GetGamePadLeftStickY();

}

Character::~Character(void) {
}


void Character::SetCoord(COORD inCoord) {
	memcpy(&Coord, &inCoord, sizeof(COORD));
	//�����ʒu�̔z����Z�b�g
	GroundInfo.X = (inCoord.X - STAGEPOS_YOKO) / (MASUWIDTH / 2);
	GroundInfo.Y = (inCoord.Y - STAGEPOS_TATE) / (MASUHEIGHT / 2);

	////�����ʒu�̒��_�Z�b�g
	Pos_Vertex.X = 2;
	Pos_Vertex.Y = 4;

	//�T���̌�����������Ԃ�
	Tu = 0;
	Tv = 0;
}


void Character::Draw(void) {
	//�L������`��
	Draw2dPolygon(Coord.X - CHARA_SIZE / 2, Coord.Y - CHARA_SIZE + HOSEITI, CHARA_SIZE, CHARA_SIZE, D3DCOLOR_ARGB(255, 255, 255, 255), Texture, Tu, Tv, 0.5f, 0.5f);

	//����`�揈��
	_LightingT LightingStatus;
	float TexCoordX = GroundInfo.X / 2 * PLAYER_LIGHT_SIZEX;
	float TexCoordY = GroundInfo.Y / 2 * PLAYER_LIGHT_SIZEY;

	if (Tv < 0.5) {
		//�������̎��̐ݒ�
		LightingStatus.CoordX = Coord.X - CHARA_SIZE / 2 + CHARA_SIZE - (CHARA_SIZE * TexCoordX);
		LightingStatus.CoordY = Coord.Y - CHARA_SIZE;
		LightingStatus.LightU = 0.5f + Tu - (TexCoordX * 0.5f);
		LightingStatus.LightV = Tv;
		LightingStatus.LightW = 0.5f * PLAYER_LIGHT_SIZEX;
		LightingStatus.LightH = 0.5f;
		LightingStatus.LightTexSizeU = CHARA_SIZE * PLAYER_LIGHT_SIZEX;
		LightingStatus.LightTexSizeV = CHARA_SIZE;

		//UV�l���I�[�o�[���Ȃ��悤��
		if (LightingStatus.LightU - Tu + LightingStatus.LightW > 0.5f) {
			LightingStatus.LightW = 0.5f - (LightingStatus.LightU - Tu);
		}
	}

	else if (Tv >= 0.5f) {
		//�c�����̎��̐ݒ�
		LightingStatus.CoordX = Coord.X - CHARA_SIZE / 2;
		LightingStatus.CoordY = Coord.Y - CHARA_SIZE + CHARA_SIZE - (CHARA_SIZE * TexCoordY);
		LightingStatus.LightU = Tu;
		LightingStatus.LightV = 0.5f + Tv - (TexCoordY * 0.5f);
		LightingStatus.LightW = 0.5f;
		LightingStatus.LightH = 0.5f * PLAYER_LIGHT_SIZEY;
		LightingStatus.LightTexSizeU = CHARA_SIZE;
		LightingStatus.LightTexSizeV = CHARA_SIZE * PLAYER_LIGHT_SIZEY;

		//UV�l���I�[�o�[���Ȃ��悤��
		if (LightingStatus.LightV - Tv + LightingStatus.LightH > 0.5f) {
			LightingStatus.LightH = 0.5f - (LightingStatus.LightV - Tv);
		}
	}
	//����`��
	Draw2dPolygon(LightingStatus.CoordX, LightingStatus.CoordY + HOSEITI, LightingStatus.LightTexSizeU, LightingStatus.LightTexSizeV, D3DCOLOR_ARGB(255, 255, 255, 255), LightTexture,
		LightingStatus.LightU, LightingStatus.LightV, LightingStatus.LightW, LightingStatus.LightH);

}

void Character::Move(void)
{
	//���z�L�[

	bool UP_TRIG = false;
	bool DOWN_TRIG = false;
	bool LEFT_TRIG = false;
	bool RIGHT_TRIG = false;

	memcpy(&wark_coord, &Coord, sizeof(COORD));//�L�����̍��W�̑ޔ�p
	memcpy(&wark_groundinfo, &GroundInfo, sizeof(COORD));//�L�����̔z����W�̑ޔ�p
														 
	//�Ή�����L�[�������ꂽ��(�L�[�{�[�h)
	if (GetKeyboardPress(DIK_UP) && Inputflg == false) {
		UP_TRIG = true;
		Inputflg = true;
		Tu = 0.5f;
		Tv = 0.5f;
	}
	else if (GetKeyboardPress(DIK_DOWN) && Inputflg == false) {
		DOWN_TRIG = true;
		Inputflg = true;
		Tu = 0.0f;
		Tv = 0.5f;
	}
	if (GetKeyboardPress(DIK_LEFT) && Inputflg == false) {
		LEFT_TRIG = true;
		Inputflg = true;
		Tu = 0.0f;
		Tv = 0.0f;
	}
	else if (GetKeyboardPress(DIK_RIGHT) && Inputflg == false) {
		RIGHT_TRIG = true;
		Inputflg = true;
		Tu = 0.5f;
		Tv = 0.0f;
	}
	if (Inputflg == true)
	{
		KeyWait++;
	}
	if (KeyWait >= KEYWAIT)
	{
		KeyWait = 0;
		Inputflg = false;
	}

	//�Ή�����{�^���������ꂽ��(�Q�[���p�b�h)
	if (g_pDIDevGamePad) {
		if (GetGamePadLeftStickY() <= JoypadDI_Y - GAMEPAD_DEADZONE&&Inputflg == false) {
			UP_TRIG = true;
			Inputflg = true;
			Tu = 0.5f;
			Tv = 0.5f;
		}
		else if (GetGamePadLeftStickY() >= JoypadDI_Y + GAMEPAD_DEADZONE&&Inputflg == false) {
			DOWN_TRIG = true;
			Inputflg = true;
			Tu = 0.0f;
			Tv = 0.5f;
		}
		if (GetGamePadLeftStickX() <= JoypadDI_X - GAMEPAD_DEADZONE&&Inputflg == false) {
			LEFT_TRIG = true;
			Inputflg = true;
			Tu = 0.0f;
			Tv = 0.0f;
		}
		else if (GetGamePadLeftStickX() >= JoypadDI_X + GAMEPAD_DEADZONE&&Inputflg == false) {
			RIGHT_TRIG = true;
			Inputflg = true;
			Tu = 0.5f;
			Tv = 0.0f;
		}
	}

	if (UP_TRIG)//1�}�X��Ɉړ�
	{
		if (Coord.X % MASUWIDTH == 0)//���_�ɂ��鎞�݈̂ړ��\��
		{
			Coord.Y -= MASUHEIGHT / 2;
			GroundInfo.Y--;//�z���Y���W���}�C�i�X
			Pos_Vertex.Y -= (short)0.5;
		}

	}
	else if (DOWN_TRIG)//�P�}�X���Ɉړ�
	{
		if (Coord.X % MASUWIDTH == 0)//���_�ɂ��鎞�݈̂ړ��\��
		{
			Coord.Y += MASUHEIGHT / 2;
			GroundInfo.Y++;//�z���Y���W���v���X
			Pos_Vertex.Y += (short)0.5;
		}

	}
	if (LEFT_TRIG)//1�}�X���Ɉړ�
	{
		if (Coord.Y % MASUHEIGHT == 0)//���_�ɂ��鎞�݈̂ړ��\��
		{
			Coord.X -= MASUWIDTH / 2;
			GroundInfo.X--;//�z���X���W���}�C�i�X
			Pos_Vertex.X -= (short)0.5;
		}

	}
	else if (RIGHT_TRIG)//1�}�X�E�Ɉړ�
	{
		if (Coord.Y % MASUHEIGHT == 0)//���_�ɂ��鎞�݈̂ړ��\��
		{
			Coord.X += MASUWIDTH / 2;
			GroundInfo.X++;//�z���X���W���v���X
			Pos_Vertex.X += (short)0.5;
		}

	}

	///////////////////////////////////////
	// �}�X�̊O�ɏo�Ȃ��悤�ɂ��鏈��
	///////////////////////////////////////
	if (Coord.X <= STAGEPOS_YOKO)//���Ƀ|����������
	{
		Coord.X = STAGEPOS_YOKO;
		GroundInfo.X = 0;
		Pos_Vertex.X = 0;
	}
	if (Coord.Y <= STAGEPOS_TATE)//��Ƀ|����������
	{
		Coord.Y = STAGEPOS_TATE;
		GroundInfo.Y = 0;
		Pos_Vertex.Y = 0;
	}
	if (Coord.X >= STAGEPOS_YOKO + (MASUWIDTH*YOKOMASU_NUM))//�E�Ƀ|����������
	{
		Coord.X = STAGEPOS_YOKO + (MASUWIDTH*YOKOMASU_NUM);
		GroundInfo.X = STAGESIZE_IGOX - 1;
		Pos_Vertex.X = VERTEXX_NUM;
	}
	if (Coord.Y >= STAGEPOS_TATE + (MASUHEIGHT*TATEMASU_NUM))//���Ƀ|����������
	{
		Coord.Y = STAGEPOS_TATE + (MASUHEIGHT*TATEMASU_NUM);
		GroundInfo.Y = STAGESIZE_IGOY - 1;
		Pos_Vertex.Y = VERTEXY_NUM;
	}


}

void Character::CheckMove(bool tate, bool yoko)
{

	if (Stagetype != Vertex) {
		switch (Stagetype) {
		case Side_Tate:
			if (tate == 0)
			{
				memcpy(&Coord, &wark_coord, sizeof(COORD));
				memcpy(&GroundInfo, &wark_groundinfo, sizeof(COORD));
			}
			break;

		case Side_Yoko:
			if (yoko == 0)
			{
				memcpy(&Coord, &wark_coord, sizeof(COORD));
				memcpy(&GroundInfo, &wark_groundinfo, sizeof(COORD));
			}
			break;
		}
	}
}

bool Character::CheckDive(bool tate, bool yoko, bool vertex) {
	switch (Stagetype) {
	case Side_Tate:
		if (tate == 0) {
			return false;
		}
		break;

	case Side_Yoko:
		if (yoko == 0) {
			return false;
		}
		break;

	case Vertex:
		if (vertex == 0) {
			return false;
		}
		break;
	}
	return true;
}
void Character::Update(bool Dive_State)
{
	//�X�V

	Move();

	if (GroundInfo.X % 2 == 0 && GroundInfo.Y % 2 == 0)//���_�ɂ���ꍇ
	{
		Stagetype = Vertex;
	}
	else if (GroundInfo.X % 2 == 1 && GroundInfo.Y % 2 == 0) {//����
		Stagetype = Side_Yoko;
	}
	else if (GroundInfo.X % 2 == 0 && GroundInfo.Y % 2 == 1) {//�c��
		Stagetype = Side_Tate;
	}

}
StageTypeT Character::OutStageType(void) {
	//StageType��Ԃ�
	return Stagetype;
}
void Character::Hit(void) {
	//�U���H��������̏���
	int a = 0;///��
}
COORD Character::OutCoord(void) {
	//�X�e�[�W���̈ʒu�����o��
	return GroundInfo;
}
COORD Character::OutPos(void) {
	//���W�����o��
	return Coord;
}