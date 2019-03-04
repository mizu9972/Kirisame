//-----------------------------------------------
//�G�L�����N�^�[�̏���
//-----------------------------------------------
#include "main.h"
#include "Enemy.h"
#include "Asset.h"
#include "config.h"
#include "TexLoad.h"
#include "XAudio2.h"

//�U�������Ƃ��ɉ�ʑS�̂ɃG�t�F�N�g��������
//�����̍U���ŃG�t�F�N�g���d�����Ȃ��悤�ɃO���[�o���Œ�`
bool AttackEffectFlag = false;
int AttackEffectCount = 0;

extern int DeathEnemyNum;

Enemy::Enemy(void) {
	//�R���X�g���N�^
	MoveFlagXRight = true;
	MoveFlagYUp = true;
	MoveFlagXLeft = true;
	MoveFlagYDown = true;

	isFalling = false;
	AttackMode = false;
	Attack = false;
	isAlive = true;
}

Enemy::~Enemy(void) {
	//�f�X�g���N�^

}

void Enemy::Init(COORD GetCoord) {
	//������

	//�󂯎�������W���𔽉f����
	Coord.X = GetCoord.X;
	Coord.Y = GetCoord.Y;
	
	//�ǂ̃}�X�ɂ��邩
	BlockCoord.X = ((GetCoord.X - (STAGEPOS_YOKO - (MASUWIDTH / 2))) / MASUWIDTH) - 1;
	BlockCoord.Y = ((GetCoord.Y - (STAGEPOS_TATE - (MASUHEIGHT / 2))) / MASUHEIGHT) - 1;

	Size = ENEMY_SIZE;
	isAlive = true;
	  MoveFlagXRight = true;
	  MoveFlagYUp = true;
	  MoveFlagXLeft = true;
	  MoveFlagYDown = true;

	  isFalling = false;
	  AttackMode = false;
	  Attack = false;

	  MoveMode = RIGHT_MOVE_MODE;//�ړ��p�^�[��
	  MoveCount = 0;
	  Animation = 0;//�A�j���[�V�����p�ϐ�
	  AnimeCount = 0;//�A�j���[�V�������x
	  DrawCount = (int)(ENEMY_MOVESPEED * 2 * FPS);

	  Texture = TexOp->BrownBearTex;
	  AttackInfo.Texture = TexOp->EnemyAttackTex;
	  DiveTexture = TexOp->EnemySunabokoriTex;

	  MoveFlagXRight = true;
	  MoveFlagYUp = true;
	  MoveFlagXLeft = true;
	  MoveFlagYDown = true;

	  isFalling = false;
	  AttackMode = false;
	  Attack = false;
	  isAlive = true;

}

bool Enemy::Update(COORD PlayerCoord,bool Diveflag,bool Attackflag,COORD ToCoord) {
	//�X�V����
	bool ReturnBool = false;

	//�A�j���[�V�����i�s
	if (AnimeCount > 0.5f * FPS) {//�A�j���[�V�������x
		if (Animation > 4) {//�A�j���[�V�������Đ����������烊�Z�b�g
			Animation = 0;
		}
		else {
			Animation += 1;
		}
		AnimeCount = 0;
	}
	else {
		AnimeCount += 1;
	}
	
	//�U���\�Ȃ�U�������ɓ���
	if (AttackMode == false && Attackflag == true && Diveflag == 1) {
		AttackMode = true;//�U�������t���O
		memcpy(&AttackInfo.Coord, &PlayerCoord, sizeof(COORD));//�U��������W���Z�b�g
	}
	//�����ʒu����̃v���C���[�̋����v�Z------------------------------------------------
	COORD SubCoord;

	SubCoord.X = (short)sqrt((ToCoord.X - PlayerCoord.X) * (ToCoord.X - PlayerCoord.X));
	SubCoord.Y = (short)sqrt((ToCoord.Y - PlayerCoord.Y) * (ToCoord.Y - PlayerCoord.Y));
	//----------------------------------------------------------------------------------

	//�G�L�����N�^�[�ƃv���C���[�̋����v�Z--------------------------------------------------
	COORD SubPersonalCoord;

	SubPersonalCoord.X = (short)sqrt((Coord.X - PlayerCoord.X) * (Coord.X - PlayerCoord.X));
	SubPersonalCoord.Y = (short)sqrt((Coord.Y - PlayerCoord.Y) * (Coord.Y - PlayerCoord.Y));
	//--------------------------------------------------------------------------------------

	if (Diveflag) {//�����Ă��邩
		//�v���C���[�̈ʒu�ɂ���Ĉړ�����ڕW�n�_�𒲐�����
		if ((SubCoord.X < ENEMY_SEARCH_RAD * MASUWIDTH) && (SubCoord.Y < ENEMY_SEARCH_RAD * MASUHEIGHT)) {
			//�v���C���[���e���g���[�ɋ߂Â�����

			//�v���C���[�̈ʒu���ړ��ڕW�ɂ���
			MokuhyoCoord.X = PlayerCoord.X;
			MokuhyoCoord.Y = PlayerCoord.Y;
		}
		else {
			//�Ⴄ�Ȃ珉���ʒu��ڕW��
			MokuhyoCoord.X = ToCoord.X;
			MokuhyoCoord.Y = ToCoord.Y;
		}

		if ((SubPersonalCoord.X < ENEMY_PERSONAL_SPACE * MASUWIDTH) && (SubPersonalCoord.Y < ENEMY_PERSONAL_SPACE * MASUWIDTH)) {
			//�v���C���[�����Ȃ�߂��ɂ���Ȃ�ǂ�������
			MokuhyoCoord.X = PlayerCoord.X;
			MokuhyoCoord.Y = PlayerCoord.Y;
		}
	}
	else {
		//�v���C���[�������Ă���Ȃ珉���ʒu��ڕW��
		MokuhyoCoord.X = ToCoord.X;
		MokuhyoCoord.Y = ToCoord.Y;
	}
	if (isFalling == false) {
		if (ENEMY_MOVESPEED * FPS < MoveCount) {//��莞�Ԃ��Ƃɏ���
			if (AttackMode == true && Diveflag == 1) {
				AttackInfo.Flag = true;//�U������
				if ((Coord.X - PlayerCoord.X) * (Coord.X - PlayerCoord.X) + (Coord.Y - PlayerCoord.Y) * (Coord.Y - PlayerCoord.Y) < (MASUWIDTH / 2 * 1.5) * (MASUWIDTH / 2 * 1.5)) {
					//����
					AttackEffectFlag = true;//�G�t�F�N�g�`��t���O��true��
					ReturnBool = AttackMove();//�U������
				}
				else {
					//�O��
					AttackMode = false;
					if (PollSound(AttackSE) == false) {
						PlaySound(MissAttackSE);//�U�����sSE
					}
				}
			}
			else if (AttackMode == false || Diveflag == 0){
				AttackInfo.Flag = false;//�U�����Ȃ�
				Move();//�ړ�
			}
			MoveCount = 0;//�s�������烊�Z�b�g
		}
		MoveCount += 1;
	}

	return ReturnBool;//�U���ɐ����������ǂ�����Ԃ�
}

void Enemy::Move(void) {
	bool MoveFlag = true;//�ړ��\������p

	if (MokuhyoCoord.X == Coord.X && MokuhyoCoord.Y == Coord.Y) {
		return;
	}
	//�ڕW���W�Ǝ����̍��W�̍����v�Z--------
	COORD SubCoord;

	SubCoord.X = MokuhyoCoord.X - Coord.X;
	SubCoord.Y = MokuhyoCoord.Y - Coord.Y;
	//--------------------------------------

		//X����Y���̂��ڕW�Ƃ̋���������ق���D�悵�Ĉړ�����
	if ((SubCoord.X) * (SubCoord.X) > (SubCoord.Y) * (SubCoord.Y)) {
		//X���W�̕�������Ă���ꍇ---------------------------------
		if (MoveFlag) {
			//�ړ��\�Ȃ�ڕW���W�ڎw���Ĉړ�
			if (SubCoord.X > 0) {
				//�E�ړ�
				if (MoveFlagXRight) {
					MoveMode = RIGHT_MOVE_MODE;
					Coord.X += ENEMY_SPEED * MASUWIDTH;
					BlockCoord.X += 1;
					MoveFlag = false;
				}
			}
			else if (SubCoord.X < 0) {
				//���ړ�
				if (MoveFlagXLeft) {
					MoveMode = LEFT_MOVE_MODE;
					Coord.X -= ENEMY_SPEED * MASUWIDTH;
					BlockCoord.X -= 1;
					MoveFlag = false;
				}
			}
			else {
				//�������Ȃ�
			}
		}

		//X���̕�������Ă��邪�A�ʂ�Ȃ�������---------------------
		//Y�����ʂֈړ�(Y���ړ��Ɠ�������)
		if (MoveFlag == true && (MoveFlagXRight == false || MoveFlagXLeft == false)) {
			//�ړ��\�Ȃ�ڕW���W�ڎw���Ĉړ�
			if (SubCoord.Y > 0) {
				//���ړ�
				if (MoveFlagYDown) {
					MoveMode = DOWN_MOVE_MODE;
					Coord.Y += ENEMY_SPEED * MASUHEIGHT;
					BlockCoord.Y += 1;
					MoveFlag = false;
				}
			}
			else if (SubCoord.Y <= 0) {
				//��ړ�
				if (MoveFlagYUp) {
					MoveMode = UP_MOVE_MODE;
					Coord.Y -= ENEMY_SPEED * MASUHEIGHT;
					BlockCoord.Y -= 1;
					MoveFlag = false;
				}
			}
			else {
				//�������Ȃ�
			}
		}
		//----------------------------------------------------------

	}
	else{
		//Y���̕�������Ă���ꍇ-----------------------------------
		if (MoveFlag ) {
			//�ړ��\�Ȃ�ڕW���W�ڎw���Ĉړ�
			if (SubCoord.Y > 0) {
				//���ړ�
				if (MoveFlagYDown) {
					MoveMode = DOWN_MOVE_MODE;
					Coord.Y += ENEMY_SPEED * MASUHEIGHT;
					BlockCoord.Y += 1;
					MoveFlag = false;
				}
			}
			else if (SubCoord.Y < 0) {
				//��ړ�
				if (MoveFlagYUp) {
					MoveMode = UP_MOVE_MODE;
					Coord.Y -= ENEMY_SPEED * MASUHEIGHT;
					BlockCoord.Y -= 1;
					MoveFlag = false;
				}
			}
			else{
				//�������Ȃ�
			}
		}

		//----------------------------------------------------------


		//Y���̕�������Ă��邪�A�ʂ�Ȃ�������---------------------
		//X�����ʂֈړ�(X���ړ��Ɠ�������)
		if (MoveFlag == true && (MoveFlagYUp == false || MoveFlagYDown == false)) {
			//�ړ��\�Ȃ�ڕW���W�ڎw���Ĉړ�
			if (SubCoord.X > 0) {
				//�E�ړ�
				if (MoveFlagXRight) {
					MoveMode = RIGHT_MOVE_MODE;
					Coord.X += ENEMY_SPEED * MASUWIDTH;
					BlockCoord.X += 1;
					MoveFlag = false;
				}
			}
			else if (SubCoord.X <= 0) {
				//���ړ�
				if (MoveFlagXLeft) {
					MoveMode = LEFT_MOVE_MODE;
					Coord.X -= ENEMY_SPEED * MASUWIDTH;
					BlockCoord.X -= 1;
					MoveFlag = false;
				}
			}
			else {
				//�������Ȃ�
			}
		}

		//----------------------------------------------------------

	}
	//��ʊO�ɂ����Ȃ��悤��(X��)
	while (Coord.X < STAGEPOS_YOKO) {
		Coord.X += ENEMY_SPEED * MASUWIDTH;
		BlockCoord.X += 1;
	}
	while (Coord.X > STAGEPOS_YOKO + YOKOMASU_NUM * MASUWIDTH) {
		Coord.X -= ENEMY_SPEED * MASUWIDTH;
		BlockCoord.X -= 1;
	}
	//��ʊO�ɂ����Ȃ��悤��(Y��)
	while (Coord.Y < STAGEPOS_TATE) {
		Coord.Y += ENEMY_SPEED * MASUHEIGHT;
		BlockCoord.Y += 1;
	}
	while (Coord.Y >= STAGEPOS_TATE + TATEMASU_NUM * MASUHEIGHT) {
		Coord.Y -= ENEMY_SPEED * MASUWIDTH;
		BlockCoord.Y -= 1;
	}
}

bool Enemy::AttackMove(void) {
	//�U�����쏈��
	AttackMode = false;
	PlaySound(AttackSE);
	return true;
}

void Enemy::AvoidHole(PieceT RightBlock, PieceT UpBlock, PieceT LeftBlock, PieceT DownBlock) {
	//��������Ēʂ鏈��
	//�㉺���E�Ɍ�������Ȃ�ʂ�Ȃ��悤�ɂ���

	//�E
	if (RightBlock.isActive) {
		if (RightBlock.isCut == true || RightBlock.isPassagable == false) {
			MoveFlagXRight = false;
		}
		else {
			MoveFlagXRight = true;
		}
	}
	//��
	if (UpBlock.isActive) {
		if (UpBlock.isCut == true || UpBlock.isPassagable == false) {
			MoveFlagYUp = false;
		}
		else {
			MoveFlagYUp = true;
		}
	}
	//��
	if (LeftBlock.isActive) {
		if (LeftBlock.isCut == true || LeftBlock.isPassagable == false) {
			MoveFlagXLeft = false;
		}
		else {
			MoveFlagXLeft = true;
		}
	}
	//��
	if (DownBlock.isActive) {
		if (DownBlock.isCut == true || DownBlock.isPassagable == false) {
			MoveFlagYDown = false;
		}
		else {
			MoveFlagYDown = true;
		}
	}
}

void Enemy::AvoidAnotherEnemy(COORD AnotherCoord) {
	//�ׂ̃}�X�ɂق��̓G�L����������ꍇ���̕����ւ̈ړ����ł��Ȃ�����
	//�E
	if (BlockCoord.X + 1 == AnotherCoord.X && BlockCoord.Y == AnotherCoord.Y) {
		MoveFlagXRight = false;
	}
	//��
	if (BlockCoord.X - 1 == AnotherCoord.X && BlockCoord.Y == AnotherCoord.Y) {
		MoveFlagXLeft = false;
	}
	//��
	if (BlockCoord.Y + 1 == AnotherCoord.Y && BlockCoord.X == AnotherCoord.X) {
		MoveFlagYDown = false;
	}
	//��
	if (BlockCoord.Y - 1 == AnotherCoord.Y && BlockCoord.X == AnotherCoord.X) {
		MoveFlagYUp = false;
	}
}
void Enemy::FallintoHole(void) {
	//���ɗ����鏈��
	isFalling = true;
	if (Size <= 0) {
		isAlive = false;
		DeathEnemyNum += 1;
	}
	Size -= 1;
}
void Enemy::Draw(bool DiveFlag) {
	//�`�揈��

	if (DiveFlag) {
		//�G�L�����N�^�[�`��
		Draw2dPolygon(Coord.X - Size / 2, Coord.Y - Size / 2, Size, Size, D3DCOLOR_ARGB(255, 255, 255, 255), Texture, 0.25f * Animation, 0.25f * MoveMode, 0.25f, 0.25f);
	}
	else {
		if (ENEMY_MOVESPEED * 3 * FPS < DrawCount) {

			memcpy(&SunabokoriCoord, &Coord, sizeof(COORD));
			//���ڂ���`��
			Draw2dPolygon(SunabokoriCoord.X - Size / 2, SunabokoriCoord.Y - Size / 2, Size, Size, D3DCOLOR_ARGB(255, 255, 255, 255), DiveTexture, 0, 0, 1, 1);
			DrawCount = 0;
		}

		DrawCount += 1;

	}
	if (AttackInfo.Flag) {
		if (AttackInfo.Count < 0.2f * FPS) {
			//�܍��`��
			Draw2dPolygon(AttackInfo.Coord.X - MASUWIDTH / 2.0f, AttackInfo.Coord.Y - MASUHEIGHT / 2.0f, MASUWIDTH, MASUHEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), AttackInfo.Texture, 0, 0, 1.0f, 1.0f);
			AttackInfo.Count += 1;
		}
		else {
			AttackInfo.Count = 0;
			AttackInfo.Flag = false;
		}
	}
	if (AttackEffectFlag) {
		//�U���G�t�F�N�g�`��
		if (AttackEffectCount < 0.1f * FPS) {
			Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(128, 200, 0, 0));//�Ԃ��ۂ��G�t�F�N�g
			AttackEffectCount += 1;
		}
		else {
			AttackEffectCount = 0;
			AttackEffectFlag = false;
		}

	}
}

COORD Enemy::OutBlockCoord(void) {
	return BlockCoord;
}

COORD Enemy::OutPos(void) {
	return Coord;
}

BrownBear::BrownBear(void) {
	//�R���X�g���N�^
	Texture = TexOp->BrownBearTex;
	AttackInfo.Texture = TexOp->EnemyAttackTex;
	DiveTexture = TexOp->EnemySunabokoriTex;
}

BrownBear::~BrownBear(void) {
	//�f�X�g���N�^
}