///-----------------------------------------------
//繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ繧ｯ繝ｩ繧ｹ縺ｮ蜃ｦ逅・
//-----------------------------------------------
#include "Character.h"
#include "config.h"
#include "Asset.h"
#include "main.h"
#include "TexLoad.h"

#define HOSEITI 15.0f//繧ｭ繝｣繝ｩ繧ｯ繧ｿ繝ｼ謠冗判菴咲ｽｮ隱ｿ謨ｴ逕ｨ
extern LPDIRECT3DDEVICE9 g_pD3DDevice;
extern LPDIRECTINPUTDEVICE8 g_pDIDevGamePad;

Character::Character(void) {
	//蛻晄悄菴咲ｽｮ繧偵そ繝・ヨ
	Coord.X = PLAYER_STARTPOS_X;
	Coord.Y = PLAYER_STARTPOS_Y;

	//蛻晄悄菴咲ｽｮ縺ｮ驟榊・繧偵そ繝・ヨ
	GroundInfo.X = (PLAYER_STARTPOS_X - STAGEPOS_YOKO) / (MASUWIDTH / 2);
	GroundInfo.Y = (PLAYER_STARTPOS_Y - STAGEPOS_TATE) / (MASUHEIGHT / 2);

	////蛻晄悄菴咲ｽｮ縺ｮ鬆らせ繧ｻ繝・ヨ
	Pos_Vertex.X = 2;
	Pos_Vertex.Y = 4;
	//繝・け繧ｹ繝√Ε繧ｻ繝・ヨ
	Texture = TexOp->PlayerTex;
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

	//蛻晄悄菴咲ｽｮ縺ｮ驟榊・繧偵そ繝・ヨ
	GroundInfo.X = (inCoord.X - STAGEPOS_YOKO) / (MASUWIDTH / 2);
	GroundInfo.Y = (inCoord.Y - STAGEPOS_TATE) / (MASUHEIGHT / 2);

	////蛻晄悄菴咲ｽｮ縺ｮ鬆らせ繧ｻ繝・ヨ
	Pos_Vertex.X = 2;
	Pos_Vertex.Y = 4;

	//繧ｵ繝｡縺ｮ蜷代″繧貞・譛溽憾諷九↓
	Tu = 0;
	Tv = 0;

}

void Character::Draw(void) {
	//繧ｭ繝｣繝ｩ繧呈緒逕ｻ
	Draw2dPolygon(Coord.X - CHARA_SIZE / 2, Coord.Y - CHARA_SIZE + HOSEITI, CHARA_SIZE, CHARA_SIZE, D3DCOLOR_ARGB(255, 255, 255, 255), Texture, Tu, Tv, 0.5f, 0.5f);
}

void Character::Move(void)
{
	//莉ｮ諠ｳ繧ｭ繝ｼ
	bool UP_TRIG = false;
	bool DOWN_TRIG = false;
	bool LEFT_TRIG = false;
	bool RIGHT_TRIG = false;

	memcpy(&wark_coord, &Coord, sizeof(COORD));//繧ｭ繝｣繝ｩ縺ｮ蠎ｧ讓吶・騾驕ｿ逕ｨ
	memcpy(&wark_groundinfo, &GroundInfo, sizeof(COORD));//繧ｭ繝｣繝ｩ縺ｮ驟榊・蠎ｧ讓吶・騾驕ｿ逕ｨ

														 //蟇ｾ蠢懊☆繧九く繝ｼ縺梧款縺輔ｌ縺溘ｉ(繧ｭ繝ｼ繝懊・繝・
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

	//蟇ｾ蠢懊☆繧九・繧ｿ繝ｳ縺梧款縺輔ｌ縺溘ｉ(繧ｲ繝ｼ繝繝代ャ繝・
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

	if (UP_TRIG)//1繝槭せ荳翫↓遘ｻ蜍・
	{
		if (Coord.X % MASUWIDTH == 0)//鬆らせ縺ｫ縺・ｋ譎ゅ・縺ｿ遘ｻ蜍募庄閭ｽ縺ｫ
		{
			Coord.Y -= MASUHEIGHT / 2;
			GroundInfo.Y--;//驟榊・縺ｮY蠎ｧ讓吶ｒ繝槭う繝翫せ
			Pos_Vertex.Y -= (short)0.5;
		}

	}
	else if (DOWN_TRIG)//・代・繧ｹ荳九↓遘ｻ蜍・
	{
		if (Coord.X % MASUWIDTH == 0)//鬆らせ縺ｫ縺・ｋ譎ゅ・縺ｿ遘ｻ蜍募庄閭ｽ縺ｫ
		{
			Coord.Y += MASUHEIGHT / 2;
			GroundInfo.Y++;//驟榊・縺ｮY蠎ｧ讓吶ｒ繝励Λ繧ｹ
			Pos_Vertex.Y += (short)0.5;
		}

	}
	if (LEFT_TRIG)//1繝槭せ蟾ｦ縺ｫ遘ｻ蜍・
	{
		if (Coord.Y % MASUHEIGHT == 0)//鬆らせ縺ｫ縺・ｋ譎ゅ・縺ｿ遘ｻ蜍募庄閭ｽ縺ｫ
		{
			Coord.X -= MASUWIDTH / 2;
			GroundInfo.X--;//驟榊・縺ｮX蠎ｧ讓吶ｒ繝槭う繝翫せ
			Pos_Vertex.X -= (short)0.5;
		}

	}
	else if (RIGHT_TRIG)//1繝槭せ蜿ｳ縺ｫ遘ｻ蜍・
	{
		if (Coord.Y % MASUHEIGHT == 0)//鬆らせ縺ｫ縺・ｋ譎ゅ・縺ｿ遘ｻ蜍募庄閭ｽ縺ｫ
		{
			Coord.X += MASUWIDTH / 2;
			GroundInfo.X++;//驟榊・縺ｮX蠎ｧ讓吶ｒ繝励Λ繧ｹ
			Pos_Vertex.X += (short)0.5;
		}

	}

	///////////////////////////////////////
	// 繝槭せ縺ｮ螟悶↓蜃ｺ縺ｪ縺・ｈ縺・↓縺吶ｋ蜃ｦ逅・
	///////////////////////////////////////
	if (Coord.X <= STAGEPOS_YOKO)//蟾ｦ縺ｫ繝昴Ο繝ｪ縺励◆繧・
	{
		Coord.X = STAGEPOS_YOKO;
		GroundInfo.X = 0;
		Pos_Vertex.X = 0;
	}
	if (Coord.Y <= STAGEPOS_TATE)//荳翫↓繝昴Ο繝ｪ縺励◆繧・
	{
		Coord.Y = STAGEPOS_TATE;
		GroundInfo.Y = 0;
		Pos_Vertex.Y = 0;
	}
	if (Coord.X >= STAGEPOS_YOKO + (MASUWIDTH*YOKOMASU_NUM))//蜿ｳ縺ｫ繝昴Ο繝ｪ縺励◆繧・
	{
		Coord.X = STAGEPOS_YOKO + (MASUWIDTH*YOKOMASU_NUM);
		GroundInfo.X = STAGESIZE_IGOX - 1;
		Pos_Vertex.X = VERTEXX_NUM;
	}
	if (Coord.Y >= STAGEPOS_TATE + (MASUHEIGHT*TATEMASU_NUM))//荳九↓繝昴Ο繝ｪ縺励◆繧・
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
	//譖ｴ譁ｰ

	Move();

	if (GroundInfo.X % 2 == 0 && GroundInfo.Y % 2 == 0)//鬆らせ縺ｫ縺・ｋ蝣ｴ蜷・
	{
		Stagetype = Vertex;
	}
	else if (GroundInfo.X % 2 == 1 && GroundInfo.Y % 2 == 0) {//讓ｪ霎ｺ
		Stagetype = Side_Yoko;
	}
	else if (GroundInfo.X % 2 == 0 && GroundInfo.Y % 2 == 1) {//邵ｦ霎ｺ
		Stagetype = Side_Tate;
	}

}
StageTypeT Character::OutStageType(void) {
	//StageType繧定ｿ斐☆
	return Stagetype;
}
void Character::Hit(void) {
	//謾ｻ謦・｣溘ｉ縺｣縺滓凾縺ｮ蜃ｦ逅・
	int a = 0;///莉ｮ
}
COORD Character::OutCoord(void) {
	//繧ｹ繝・・繧ｸ蜀・・菴咲ｽｮ繧貞叙繧雁・縺・
	return GroundInfo;
}
COORD Character::OutPos(void) {
	//蠎ｧ讓吶ｒ蜿悶ｊ蜃ｺ縺・
	return Coord;
}