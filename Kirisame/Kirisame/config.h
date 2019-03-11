#pragma once
//�e��ݒ���܂Ƃ߂��t�@�C��

#define ROCK_NUM (20)//��̐�
#define CAKE_NUM (10)//�P�[�L�̐�
#define FPS (60)//�������x
#define TATEMASU_NUM (10)//�c�}�X�̐�
#define YOKOMASU_NUM (10) //���}�X�̐�

#define KEYWAIT (6)//�L�[�̓��͊Ԋu
#define SCENE_TIME (FPS*10)//�^�C�g���ɖ߂�܂ł̎��Ԃ̂��

#define ENEMY_STAGE1_NUM (2)//�G�̐�
#define ENEMY_STAGE2_NUM (5)//�G�̐�

#define PLAYER_STARTPOS_X (300)//�v���C���[�̏����ʒu(X���W)
#define PLAYER_STARTPOS_Y (100)//�v���C���[�̏����ʒu(Y���W)

#define PLAYER_SPEED (1)//�v���C���[�̈ړ����x
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

#define STAGEPOS_TATE (50)//�X�e�[�W�̕`��̊J�n���W(���Y���W)
#define STAGEPOS_YOKO (250)//�X�e�[�W�̕`��̊J�n���W(���X���W)

#define MASUWIDTH (50)//�}�X�̕�
#define MASUHEIGHT (50)//�}�X�̍���
#define CHARA_SIZE (50.0f)//�v���C���[�̃T�C�Y
#define ENEMY_SIZE (50.0f)//�G�L�����N�^�[�̃T�C�Y
#define CAKESIZE (50)//�P�[�L�̃T�C�Y

//����p�萔�錾----
#define DONTCUT (0)
#define YESCUT (1)
#define CHECKED (1)
//------------------
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
#define GAMEPAD_DEADZONE 30000//�Q�[���p�b�h�̓��͖������͈�(�f�b�h�]�[��)

//�_���֌W
#define CAKESCORE (100)
#define ENEMYSCORE (25)
#define MATHSCORE (20)
#define DAMAGESCORE (25)

#define SCOREBOARD_X (40.0f)//�X�R�A�{�[�h�ʒu(X���W)
#define SCOREBOARD_Y (50.0f)//�X�R�A�{�[�h�ʒu(Y���W)
#define SCOREBOARD_W (200.0f)//�X�R�A�{�[�h��(X���W)
#define SCOREBOARD_H (150.0f)//�X�R�A�{�[�h����(Y���W)

#define TIMEBOARD_X (40.0f)//�^�C���{�[�h�ʒu(X���W)
#define TIMEBOARD_Y (300.0f)//�^�C���{�[�h�ʒu(Y���W)
#define TIMEBOARD_W (200.0f)//�^�C���{�[�h��(X���W)
#define TIMEBOARD_H (150.0f)//�^�C���{�[�h����(Y���W)

#define PUI_X (780.0f)//�v���C���[�X�e�[�^�X�ʒu(X���W)
#define PUI_Y (50.0f)//�v���C���[�X�e�[�^�X�ʒu(Y���W)
#define PUI_W (200.0f)//�v���C���[�X�e�[�^�X��(X���W)
#define PUI_H (200.0f)//�v���C���[�X�e�[�^�X����(Y���W)


#define DESCRIPTION_X (780.0f)//��������ʒu(X���W)
#define DESCRIPTION_Y (300.0f)//��������ʒu(Y���W)
#define DESCRIPTION_W (210.0f)//���������(X���W)
#define DESCRIPTION_H (180.0f)//�����������(Y���W)


#define TIMENUM_X (190.0f)//���Ԉʒu(X���W)
#define TIMENUM_Y (350.0f)//���Ԉʒu(Y���W)
#define TIMENUM_W (50.0f)//���ԕ�(X���W)
#define TIMENUM_H (100.0f)//���ԍ���(Y���W)
#define SCORE_X (190.0f)//�X�R�A�\��
#define SCORE_Y (100.0f)

#define RESULT_X (325)//���U���g
#define RESULT_Y (250)