//-----------------------------------------------
//謨ｵ繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｮ蜃ｦ逅・
//-----------------------------------------------
#include "main.h"
#include "Enemy.h"
#include "Asset.h"
#include "config.h"
#include "TexLoad.h"
#include "XAudio2.h"

//謾ｻ謦・＠縺溘→縺阪↓逕ｻ髱｢蜈ｨ菴薙↓繧ｨ繝輔ぉ繧ｯ繝医ｒ縺九￠繧・
//隍・焚縺ｮ謾ｻ謦・〒繧ｨ繝輔ぉ繧ｯ繝医′驥崎､・＠縺ｪ縺・ｈ縺・↓繧ｰ繝ｭ繝ｼ繝舌Ν縺ｧ螳夂ｾｩ
bool AttackEffectFlag = false;
int AttackEffectCount = 0;

extern int DeathEnemyNum;

Enemy::Enemy(void) {
	//繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
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
	//繝・せ繝医Λ繧ｯ繧ｿ

}

void Enemy::Init(COORD GetCoord) {
	//蛻晄悄蛹・

	//蜿励￠蜿悶▲縺溷ｺｧ讓呎ュ蝣ｱ繧貞渚譏縺吶ｋ
	Coord.X = GetCoord.X;
	Coord.Y = GetCoord.Y;

	//縺ｩ縺ｮ繝槭せ縺ｫ縺・ｋ縺・
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

	  MoveMode = RIGHT_MOVE_MODE;//遘ｻ蜍輔ヱ繧ｿ繝ｼ繝ｳ
	  MoveCount = 0;
	  Animation = 0;//繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ逕ｨ螟画焚
	  AnimeCount = 0;//繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ騾溷ｺｦ
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

bool Enemy::Update(COORD PlayerCoord, bool Diveflag, bool Attackflag, COORD ToCoord) {
	//譖ｴ譁ｰ蜃ｦ逅・
	bool ReturnBool = false;

	//繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ騾ｲ陦・
	if (AnimeCount > 0.5f * FPS) {//繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ騾溷ｺｦ
		if (Animation > 4) {//繧｢繝九Γ繝ｼ繧ｷ繝ｧ繝ｳ縺悟・逕溘＠縺阪▲縺溘ｉ繝ｪ繧ｻ繝・ヨ
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

	//謾ｻ謦・庄閭ｽ縺ｪ繧画判謦・ｺ門ｙ縺ｫ蜈･繧・
	if (AttackMode == false && Attackflag == true && Diveflag == 1) {
		AttackMode = true;//謾ｻ謦・ｺ門ｙ繝輔Λ繧ｰ
		memcpy(&AttackInfo.Coord, &PlayerCoord, sizeof(COORD));//謾ｻ謦・☆繧句ｺｧ讓吶ｒ繧ｻ繝・ヨ
	}
	//蛻晄悄菴咲ｽｮ縺九ｉ縺ｮ繝励Ξ繧､繝､繝ｼ縺ｮ霍晞屬險育ｮ・-----------------------------------------------
	COORD SubCoord;

	SubCoord.X = (short)sqrt((ToCoord.X - PlayerCoord.X) * (ToCoord.X - PlayerCoord.X));
	SubCoord.Y = (short)sqrt((ToCoord.Y - PlayerCoord.Y) * (ToCoord.Y - PlayerCoord.Y));
	//----------------------------------------------------------------------------------

	//謨ｵ繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ縺ｨ繝励Ξ繧､繝､繝ｼ縺ｮ霍晞屬險育ｮ・-------------------------------------------------
	COORD SubPersonalCoord;

	SubPersonalCoord.X = (short)sqrt((Coord.X - PlayerCoord.X) * (Coord.X - PlayerCoord.X));
	SubPersonalCoord.Y = (short)sqrt((Coord.Y - PlayerCoord.Y) * (Coord.Y - PlayerCoord.Y));
	//--------------------------------------------------------------------------------------

	if (Diveflag) {//貎懊▲縺ｦ縺・ｋ縺・
				   //繝励Ξ繧､繝､繝ｼ縺ｮ菴咲ｽｮ縺ｫ繧医▲縺ｦ遘ｻ蜍輔☆繧狗岼讓吝慍轤ｹ繧定ｪｿ謨ｴ縺吶ｋ
		if ((SubCoord.X < ENEMY_SEARCH_RAD * MASUWIDTH) && (SubCoord.Y < ENEMY_SEARCH_RAD * MASUHEIGHT)) {
			//繝励Ξ繧､繝､繝ｼ縺後ユ繝ｪ繝医Μ繝ｼ縺ｫ霑代▼縺・◆繧・

			//繝励Ξ繧､繝､繝ｼ縺ｮ菴咲ｽｮ繧堤ｧｻ蜍慕岼讓吶↓縺吶ｋ
			MokuhyoCoord.X = PlayerCoord.X;
			MokuhyoCoord.Y = PlayerCoord.Y;
		}
		else {
			//驕輔≧縺ｪ繧牙・譛滉ｽ咲ｽｮ繧堤岼讓吶↓
			MokuhyoCoord.X = ToCoord.X;
			MokuhyoCoord.Y = ToCoord.Y;
		}

		if ((SubPersonalCoord.X < ENEMY_PERSONAL_SPACE * MASUWIDTH) && (SubPersonalCoord.Y < ENEMY_PERSONAL_SPACE * MASUWIDTH)) {
			//繝励Ξ繧､繝､繝ｼ縺後°縺ｪ繧願ｿ代￥縺ｫ縺・ｋ縺ｪ繧芽ｿｽ縺・ｶ壹￠繧・
			MokuhyoCoord.X = PlayerCoord.X;
			MokuhyoCoord.Y = PlayerCoord.Y;
		}
	}
	else {
		//繝励Ξ繧､繝､繝ｼ縺梧ｽ懊▲縺ｦ縺・ｋ縺ｪ繧牙・譛滉ｽ咲ｽｮ繧堤岼讓吶↓
		MokuhyoCoord.X = ToCoord.X;
		MokuhyoCoord.Y = ToCoord.Y;
	}
	if (isFalling == false) {
		if (ENEMY_MOVESPEED * FPS < MoveCount) {//荳螳壽凾髢薙＃縺ｨ縺ｫ蜃ｦ逅・
			if (AttackMode == true && Diveflag == 1) {
				AttackInfo.Flag = true;//謾ｻ謦・☆繧・
				if ((Coord.X - PlayerCoord.X) * (Coord.X - PlayerCoord.X) + (Coord.Y - PlayerCoord.Y) * (Coord.Y - PlayerCoord.Y) < (MASUWIDTH / 2 * 1.5) * (MASUWIDTH / 2 * 1.5)) {
					//蜻ｽ荳ｭ
					AttackEffectFlag = true;//繧ｨ繝輔ぉ繧ｯ繝域緒逕ｻ繝輔Λ繧ｰ繧稚rue縺ｫ
					ReturnBool = AttackMove();//謾ｻ謦・・逅・
				}
				else {
					//螟悶ｌ
					AttackMode = false;
					if (PollSound(AttackSE) == false) {
						PlaySound(MissAttackSE);//謾ｻ謦・､ｱ謨祐E
					}
				}
			}
			else if (AttackMode == false || Diveflag == 0) {
				AttackInfo.Flag = false;//謾ｻ謦・＠縺ｪ縺・
				Move();//遘ｻ蜍・
			}
			MoveCount = 0;//陦悟虚縺励◆繧峨Μ繧ｻ繝・ヨ
		}
		MoveCount += 1;
	}

	return ReturnBool;//謾ｻ謦・↓謌仙粥縺励◆縺九←縺・°繧定ｿ斐☆
}

void Enemy::Move(void) {
	bool MoveFlag = true;//遘ｻ蜍募庄閭ｽ縺句愛螳夂畑

	if (MokuhyoCoord.X == Coord.X && MokuhyoCoord.Y == Coord.Y) {
		return;
	}
	//逶ｮ讓吝ｺｧ讓吶→閾ｪ蛻・・蠎ｧ讓吶・蟾ｮ繧定ｨ育ｮ・-------
	COORD SubCoord;

	SubCoord.X = MokuhyoCoord.X - Coord.X;
	SubCoord.Y = MokuhyoCoord.Y - Coord.Y;
	//--------------------------------------

	//X霆ｸ縺ｨY霆ｸ縺ｮ繧医ｊ逶ｮ讓吶→縺ｮ霍晞屬縺後≠繧九⊇縺・ｒ蜆ｪ蜈医＠縺ｦ遘ｻ蜍輔☆繧・
	if ((SubCoord.X) * (SubCoord.X) > (SubCoord.Y) * (SubCoord.Y)) {
		//X蠎ｧ讓吶・譁ｹ縺碁屬繧後※縺・ｋ蝣ｴ蜷・--------------------------------
		if (MoveFlag) {
			//遘ｻ蜍募庄閭ｽ縺ｪ繧臥岼讓吝ｺｧ讓咏岼謖・＠縺ｦ遘ｻ蜍・
			if (SubCoord.X > 0) {
				//蜿ｳ遘ｻ蜍・
				if (MoveFlagXRight) {
					MoveMode = RIGHT_MOVE_MODE;
					Coord.X += ENEMY_SPEED * MASUWIDTH;
					BlockCoord.X += 1;
					MoveFlag = false;
				}
			}
			else if (SubCoord.X < 0) {
				//蟾ｦ遘ｻ蜍・
				if (MoveFlagXLeft) {
					MoveMode = LEFT_MOVE_MODE;
					Coord.X -= ENEMY_SPEED * MASUWIDTH;
					BlockCoord.X -= 1;
					MoveFlag = false;
				}
			}
			else {
				//菴輔ｂ縺励↑縺・
			}
		}

		//X霆ｸ縺ｮ譁ｹ縺碁屬繧後※縺・ｋ縺後・壹ｌ縺ｪ縺九▲縺滓凾---------------------
		//Y霆ｸ譁ｹ髱｢縺ｸ遘ｻ蜍・Y霆ｸ遘ｻ蜍輔→蜷後§蜃ｦ逅・
		if (MoveFlag == true && (MoveFlagXRight == false || MoveFlagXLeft == false)) {
			//遘ｻ蜍募庄閭ｽ縺ｪ繧臥岼讓吝ｺｧ讓咏岼謖・＠縺ｦ遘ｻ蜍・
			if (SubCoord.Y > 0) {
				//荳狗ｧｻ蜍・
				if (MoveFlagYDown) {
					MoveMode = DOWN_MOVE_MODE;
					Coord.Y += ENEMY_SPEED * MASUHEIGHT;
					BlockCoord.Y += 1;
					MoveFlag = false;
				}
			}
			else if (SubCoord.Y <= 0) {
				//荳顔ｧｻ蜍・
				if (MoveFlagYUp) {
					MoveMode = UP_MOVE_MODE;
					Coord.Y -= ENEMY_SPEED * MASUHEIGHT;
					BlockCoord.Y -= 1;
					MoveFlag = false;
				}
			}
			else {
				//菴輔ｂ縺励↑縺・
			}
		}
		//----------------------------------------------------------

	}
	else {
		//Y霆ｸ縺ｮ譁ｹ縺碁屬繧後※縺・ｋ蝣ｴ蜷・----------------------------------
		if (MoveFlag) {
			//遘ｻ蜍募庄閭ｽ縺ｪ繧臥岼讓吝ｺｧ讓咏岼謖・＠縺ｦ遘ｻ蜍・
			if (SubCoord.Y > 0) {
				//荳狗ｧｻ蜍・
				if (MoveFlagYDown) {
					MoveMode = DOWN_MOVE_MODE;
					Coord.Y += ENEMY_SPEED * MASUHEIGHT;
					BlockCoord.Y += 1;
					MoveFlag = false;
				}
			}
			else if (SubCoord.Y < 0) {
				//荳顔ｧｻ蜍・
				if (MoveFlagYUp) {
					MoveMode = UP_MOVE_MODE;
					Coord.Y -= ENEMY_SPEED * MASUHEIGHT;
					BlockCoord.Y -= 1;
					MoveFlag = false;
				}
			}
			else {
				//菴輔ｂ縺励↑縺・
			}
		}

		//----------------------------------------------------------


		//Y霆ｸ縺ｮ譁ｹ縺碁屬繧後※縺・ｋ縺後・壹ｌ縺ｪ縺九▲縺滓凾---------------------
		//X霆ｸ譁ｹ髱｢縺ｸ遘ｻ蜍・X霆ｸ遘ｻ蜍輔→蜷後§蜃ｦ逅・
		if (MoveFlag == true && (MoveFlagYUp == false || MoveFlagYDown == false)) {
			//遘ｻ蜍募庄閭ｽ縺ｪ繧臥岼讓吝ｺｧ讓咏岼謖・＠縺ｦ遘ｻ蜍・
			if (SubCoord.X > 0) {
				//蜿ｳ遘ｻ蜍・
				if (MoveFlagXRight) {
					MoveMode = RIGHT_MOVE_MODE;
					Coord.X += ENEMY_SPEED * MASUWIDTH;
					BlockCoord.X += 1;
					MoveFlag = false;
				}
			}
			else if (SubCoord.X <= 0) {
				//蟾ｦ遘ｻ蜍・
				if (MoveFlagXLeft) {
					MoveMode = LEFT_MOVE_MODE;
					Coord.X -= ENEMY_SPEED * MASUWIDTH;
					BlockCoord.X -= 1;
					MoveFlag = false;
				}
			}
			else {
				//菴輔ｂ縺励↑縺・
			}
		}

		//----------------------------------------------------------

	}
	//逕ｻ髱｢螟悶↓縺・°縺ｪ縺・ｈ縺・↓(X霆ｸ)
	while (Coord.X < STAGEPOS_YOKO) {
		Coord.X += ENEMY_SPEED * MASUWIDTH;
		BlockCoord.X += 1;
	}
	while (Coord.X > STAGEPOS_YOKO + YOKOMASU_NUM * MASUWIDTH) {
		Coord.X -= ENEMY_SPEED * MASUWIDTH;
		BlockCoord.X -= 1;
	}
	//逕ｻ髱｢螟悶↓縺・°縺ｪ縺・ｈ縺・↓(Y霆ｸ)
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
	//謾ｻ謦・虚菴懷・逅・
	AttackMode = false;
	PlaySound(AttackSE);
	return true;
}

void Enemy::AvoidHole(PieceT RightBlock, PieceT UpBlock, PieceT LeftBlock, PieceT DownBlock) {
	//遨ｴ繧帝∩縺代※騾壹ｋ蜃ｦ逅・
	//荳贋ｸ句ｷｦ蜿ｳ縺ｫ遨ｴ縺後≠繧九↑繧蛾壹ｌ縺ｪ縺・ｈ縺・↓縺吶ｋ

	//蜿ｳ
	if (RightBlock.isActive) {
		if (RightBlock.isCut == true || RightBlock.isPassagable == false) {
			MoveFlagXRight = false;
		}
		else {
			MoveFlagXRight = true;
		}
	}
	//荳・
	if (UpBlock.isActive) {
		if (UpBlock.isCut == true || UpBlock.isPassagable == false) {
			MoveFlagYUp = false;
		}
		else {
			MoveFlagYUp = true;
		}
	}
	//蟾ｦ
	if (LeftBlock.isActive) {
		if (LeftBlock.isCut == true || LeftBlock.isPassagable == false) {
			MoveFlagXLeft = false;
		}
		else {
			MoveFlagXLeft = true;
		}
	}
	//荳・
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
	//髫｣縺ｮ繝槭せ縺ｫ縺ｻ縺九・謨ｵ繧ｭ繝｣繝ｩ縺後＞繧句ｴ蜷医◎縺ｮ譁ｹ蜷代∈縺ｮ遘ｻ蜍輔ｒ縺ｧ縺阪↑縺上☆繧・
	//蜿ｳ
	if (BlockCoord.X + 1 == AnotherCoord.X && BlockCoord.Y == AnotherCoord.Y) {
		MoveFlagXRight = false;
	}
	//蟾ｦ
	if (BlockCoord.X - 1 == AnotherCoord.X && BlockCoord.Y == AnotherCoord.Y) {
		MoveFlagXLeft = false;
	}
	//荳・
	if (BlockCoord.Y + 1 == AnotherCoord.Y && BlockCoord.X == AnotherCoord.X) {
		MoveFlagYDown = false;
	}
	//荳・
	if (BlockCoord.Y - 1 == AnotherCoord.Y && BlockCoord.X == AnotherCoord.X) {
		MoveFlagYUp = false;
	}
}
void Enemy::FallintoHole(void) {
	//遨ｴ縺ｫ關ｽ縺｡繧句・逅・
	isFalling = true;
	if (Size <= 0) {
		isAlive = false;
		DeathEnemyNum += 1;
	}
	Size -= 1;
}
void Enemy::Draw(bool DiveFlag) {
	//謠冗判蜃ｦ逅・

	if (DiveFlag) {
		//謨ｵ繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ謠冗判
		Draw2dPolygon(Coord.X - Size / 2, Coord.Y - Size / 2, Size, Size, D3DCOLOR_ARGB(255, 255, 255, 255), Texture, 0.25f * Animation, 0.25f * MoveMode, 0.25f, 0.25f);
	}
	else {
		if (ENEMY_MOVESPEED * 3 * FPS < DrawCount) {

			memcpy(&SunabokoriCoord, &Coord, sizeof(COORD));
			//遐ゅ⊂縺薙ｊ謠冗判
			Draw2dPolygon(SunabokoriCoord.X - Size / 2, SunabokoriCoord.Y - Size / 2, Size, Size, D3DCOLOR_ARGB(255, 255, 255, 255), DiveTexture, 0, 0, 1, 1);
			DrawCount = 0;
		}

		DrawCount += 1;

	}
	if (AttackInfo.Flag) {
		if (AttackInfo.Count < 0.2f * FPS) {
			//辷ｪ逞墓緒逕ｻ
			Draw2dPolygon(AttackInfo.Coord.X - MASUWIDTH / 2.0f, AttackInfo.Coord.Y - MASUHEIGHT / 2.0f, MASUWIDTH, MASUHEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), AttackInfo.Texture, 0, 0, 1.0f, 1.0f);
			AttackInfo.Count += 1;
		}
		else {
			AttackInfo.Count = 0;
			AttackInfo.Flag = false;
		}
	}
	if (AttackEffectFlag) {
		//謾ｻ謦・お繝輔ぉ繧ｯ繝域緒逕ｻ
		if (AttackEffectCount < 0.1f * FPS) {
			Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(128, 200, 0, 0));//襍､縺｣縺ｽ縺・お繝輔ぉ繧ｯ繝・
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
	//繧ｳ繝ｳ繧ｹ繝医Λ繧ｯ繧ｿ
	Texture = TexOp->BrownBearTex;
	AttackInfo.Texture = TexOp->EnemyAttackTex;
	DiveTexture = TexOp->EnemySunabokoriTex;
}

BrownBear::~BrownBear(void) {
	//繝・せ繝医Λ繧ｯ繧ｿ
}