#pragma once
//�e��ݒ���܂Ƃ߂��t�@�C��

#define ROCK_NUM (20)//��̐�
#define CAKE_NUM (10)//�P�[�L�̐�
#define FPS (60)//�������x
#define TATEMASU_NUM (10)//�c�}�X�̐�
#define YOKOMASU_NUM (10) //���}�X�̐�

#define STAGEPOS_TATE (50)//�X�e�[�W�̕`��̊J�n���W(���Y���W)
#define STAGEPOS_YOKO (250)//�X�e�[�W�̕`��̊J�n���W(���X���W)

#define MASUWIDTH (50)//�}�X�̕�
#define MASUHEIGHT (50)//�}�X�̍���
#define CHARA_SIZE (50.0f)//�v���C���[�̃T�C�Y
#define ENEMY_SIZE (50.0f)//�G�L�����N�^�[�̃T�C�Y
#define CAKESIZE (50)//�P�[�L�̃T�C�Y

#define STAGESIZE_IGOX (21) //�X�e�[�W��ӂ̃T�C�Y(���_�E�ӂ��܂�)X��
#define STAGESIZE_IGOY (21) //�X�e�[�W��ӂ̃T�C�Y(���_�E�ӂ��܂�)Y��

#define SQUARE_NUM (STAGESIZE_IGOX * STAGESIZE_IGOY) //�X�e�[�W���̐�

#define SIDEVERTICAL_NUM (10) //Y�����ʂ̏c�ӂ̐�
#define SIDENUM_TATE (11) //X�����ʂ̏c�ӂ̐� 
#define SIDEHORIZONTAL_NUM (11) //Y�����ʂ̉��ӂ̐�
#define SIDENUM_YOKO (10) //X�����ʂ̉��ӂ̐�

#define VERTEXY_NUM (TATEMASU_NUM + 1)//Y���̒��_��
#define VERTEXX_NUM (YOKOMASU_NUM + 1)//X���̒��_��

#define SIDESIZE_HUTOSA (10)//�ӂ̑��� 

#define WALL_SIZE_WIDTH (MASUWIDTH * YOKOMASU_NUM)
#define WALL_SIZE_HEIGHT (MASUHEIGHT * TATEMASU_NUM)
#define WALL_HUTOSA (10)

#define KEYWAIT (6)//�L�[�̓��͊Ԋu
#define SCENE_TIME (FPS*5)//�^�C�g���ɖ߂�܂ł̎��Ԃ̂��

#define ENEMY_STAGE1_NUM (2)//�G�̐�
#define ENEMY_STAGE2_NUM (5)//�G�̐�

#define PLAYER_STARTPOS_X (300)//�v���C���[�̏����ʒu(X���W)
#define PLAYER_STARTPOS_Y (100)//�v���C���[�̏����ʒu(Y���W)

#define PLAYER_SPEED (1)//�v���C���[�̈ړ����x

#define PLAYER_LIGHT_SIZEX (1.0f / (STAGESIZE_IGOX / 2 ))//����̃T�C�YX
#define PLAYER_LIGHT_SIZEY (1.0f / (STAGESIZE_IGOY / 2 ))//����̃T�C�YY

#define ENEMY_SPEED (1) //�G�̈ړ����x
#define ENEMY_MOVESPEED (0.5) //�G�̓��쑬�x

#define ENEMY1_DEFPOS_X (675)//�G1�̏����ʒu(X���W)
#define ENEMY1_DEFPOS_Y (475)//�G1�̏����ʒu(Y���W)
#define ENEMY2_DEFPOS_X (325)//�G2�̏����ʒu(X���W)
#define ENEMY2_DEFPOS_Y (375)//�G2�̏����ʒu(Y���W)
#define ENEMY3_DEFPOS_X (475)
#define ENEMY3_DEFPOS_Y (275)
#define ENEMY4_DEFPOS_X (675)
#define ENEMY4_DEFPOS_Y (275)
#define ENEMY5_DEFPOS_X (325)
#define ENEMY5_DEFPOS_Y (475)
#define ENEMY6_DEFPOS_X (475)
#define ENEMY6_DEFPOS_Y (475)
#define ENEMY7_DEFPOS_X (725)
#define ENEMY7_DEFPOS_Y (525)

#define ENEMY_PERSONAL_SPACE (3)//�G�̃p�[�\�i���X�y�[�X
#define ENEMY_SEARCH_RAD (5)//�G�̍��G�͈�(�}�X)


//����p�萔�錾----
#define DONTCUT (0)
#define YESCUT (1)
#define CHECKED (1)
//------------------

#define GAMEPAD_DEADZONE 30000//�Q�[���p�b�h�̓��͖������͈�(�f�b�h�]�[��)