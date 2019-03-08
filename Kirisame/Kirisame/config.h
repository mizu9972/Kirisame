#pragma once
//各種設定をまとめたファイル

#define ROCK_NUM (20)//岩の数
#define CAKE_NUM (10)//ケーキの数
#define FPS (60)//処理速度
#define TATEMASU_NUM (10)//縦マスの数
#define YOKOMASU_NUM (10) //横マスの数

#define STAGEPOS_TATE (50)//ステージの描画の開始座標(画面Y座標)
#define STAGEPOS_YOKO (250)//ステージの描画の開始座標(画面X座標)

#define MASUWIDTH (50)//マスの幅
#define MASUHEIGHT (50)//マスの高さ
#define CHARA_SIZE (50.0f)//プレイヤーのサイズ
#define ENEMY_SIZE (50.0f)//敵キャラクターのサイズ
#define CAKESIZE (50)//ケーキのサイズ

#define STAGESIZE_IGOX (21) //ステージ一辺のサイズ(頂点・ふち含む)X軸
#define STAGESIZE_IGOY (21) //ステージ一辺のサイズ(頂点・ふち含む)Y軸

#define SQUARE_NUM (STAGESIZE_IGOX * STAGESIZE_IGOY) //ステージ情報の数

#define SIDEVERTICAL_NUM (10) //Y軸方面の縦辺の数
#define SIDENUM_TATE (11) //X軸方面の縦辺の数 
#define SIDEHORIZONTAL_NUM (11) //Y軸方面の横辺の数
#define SIDENUM_YOKO (10) //X軸方面の横辺の数

#define VERTEXY_NUM (TATEMASU_NUM + 1)//Y軸の頂点数
#define VERTEXX_NUM (YOKOMASU_NUM + 1)//X軸の頂点数

#define SIDESIZE_HUTOSA (10)//辺の太さ 

#define WALL_SIZE_WIDTH (MASUWIDTH * YOKOMASU_NUM)
#define WALL_SIZE_HEIGHT (MASUHEIGHT * TATEMASU_NUM)
#define WALL_HUTOSA (10)

#define KEYWAIT (6)//キーの入力間隔
#define SCENE_TIME (FPS*5)//タイトルに戻るまでの時間のやつ

#define ENEMY_STAGE1_NUM (2)//敵の数
#define ENEMY_STAGE2_NUM (5)//敵の数

#define PLAYER_STARTPOS_X (300)//プレイヤーの初期位置(X座標)
#define PLAYER_STARTPOS_Y (100)//プレイヤーの初期位置(Y座標)

#define PLAYER_SPEED (1)//プレイヤーの移動速度

#define PLAYER_LIGHT_SIZEX (1.0f / (STAGESIZE_IGOX / 2 ))//光沢のサイズX
#define PLAYER_LIGHT_SIZEY (1.0f / (STAGESIZE_IGOY / 2 ))//光沢のサイズY

#define ENEMY_SPEED (1) //敵の移動速度
#define ENEMY_MOVESPEED (0.5) //敵の動作速度

#define ENEMY1_DEFPOS_X (675)//敵1の初期位置(X座標)
#define ENEMY1_DEFPOS_Y (475)//敵1の初期位置(Y座標)
#define ENEMY2_DEFPOS_X (325)//敵2の初期位置(X座標)
#define ENEMY2_DEFPOS_Y (375)//敵2の初期位置(Y座標)
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

#define ENEMY_PERSONAL_SPACE (3)//敵のパーソナルスペース
#define ENEMY_SEARCH_RAD (5)//敵の索敵範囲(マス)


//判定用定数宣言----
#define DONTCUT (0)
#define YESCUT (1)
#define CHECKED (1)
//------------------

#define GAMEPAD_DEADZONE 30000//ゲームパッドの入力無効化範囲(デッドゾーン)