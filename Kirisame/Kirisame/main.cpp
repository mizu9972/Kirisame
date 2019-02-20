//=======================================================
// DX21
// DirectX雛型処理[main.cpp]
//
//
//=======================================================
#include "main.h"
#include "Asset.h"
#include "config.h"
#include "Game.h"
#include "XAudio2.h"
#include <Time.h>
//*******************************************************
//マクロ定義
//*******************************************************
#define CLASS_NAME  "サンプル"  //ウインドウクラスの名前
#define WINDOW_NAME "KIRISAME"  //ウインドウの名前


//*******************************************************
//プロトタイプ宣言
//*******************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*******************************************************
//グローバル変数
//*******************************************************
LPDIRECT3D9        g_pD3D = NULL;   // Direct3Dオブジェクト
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;//Deviceオブジェクト(描画に必要)

SCENE Scene = TITLE;//状態遷移用(タイトルから開始)

int g_cnt = 0;//勝手にタイトル戻すやつ

LPDIRECT3DTEXTURE9* Title_Texture = NULL;//タイトル
LPDIRECT3DTEXTURE9* Tutorial_Texture = NULL;//チュートリアル
LPDIRECT3DTEXTURE9* PadTutorial_Texture = NULL;
LPDIRECT3DTEXTURE9* Result_Texture = NULL;//リザルト
LPDIRECT3DTEXTURE9* Load_Texture = NULL;//ロード中
Game *game = NULL;

extern LPDIRECTINPUTDEVICE8 g_pDIDevGamePad;
//=======================================================
//メイン関数
// (WINAPI : Win32API関数を呼び出す時の規約
// hInstance     ; このアプリケーションのインスタンスハンドル(識別子)
// hPrevInstance : 常にNULL(16bit時代の名残り)
// lpCmdLine     : コマンドラインから受け取った引数へのポインタ
// nCmdShow      : アプリケーションの初期ウインドウ表示方法の指定
//=======================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),         //WNDCLASSEXのメモリサイズを指定
		CS_CLASSDC,                 //表示するウインドウのスタイルを設定
		WndProc,                    //ウインドウプロシージャのアドレス(関数名)を指定
		0,                          //通常は使用しないので"0"を指定
		0,                          //通常は使用しないので"0"を指定
		hInstance,                  //WinMainのパラメータのインスタンスハンドルを設定
		NULL,                       //使用するアイコンを指定(Windowsがもっているアイコンを使うならNULL)
		LoadCursor(NULL, IDC_ARROW),//マウスカーソルを指定
		(HBRUSH)(COLOR_WINDOW + 1), //ウインドウのクライアント領域の背景色を設定
		NULL,                       //Windowにつけるメニューを設定
		CLASS_NAME,                 //ウインドウクラスの名前
		NULL,                       //小さいアイコンが設定された場合の情報を記述
	};
	HWND hWnd;
	MSG msg;

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//ウインドウを作成
	hWnd = CreateWindowEx(0,//拡張ウインドウスタイル
		CLASS_NAME,     //ウインドウクラスの名前
		WINDOW_NAME,    //ウインドウの名前
		WS_OVERLAPPEDWINDOW, //ウインドウスタイル
		CW_USEDEFAULT,    //ウインドウの左上X座標
		CW_USEDEFAULT,    //ウインドウの左上Y座標
		(SCREEN_WIDTH+GetSystemMetrics(SM_CXDLGFRAME)*2),     //ウインドウの幅
		(SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2+
										GetSystemMetrics(SM_CYCAPTION)),    //ウインドウの高さ
		NULL,             //親ウインドウのハンドル
		NULL,             //メニューハンドルまたは子ウインドウID
		hInstance,        //インスタンスハンドル
		NULL);            //ウインドウ作成データ

	//初期化処理(ウインドウを作成してから行う)
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//ウインドウの表示(初期化処理の後に行う)
	ShowWindow(hWnd, nCmdShow); // 指定されたウインドウの表示状態を設定(ウインドウを表示)
	UpdateWindow(hWnd);         //ウインドウの状態を直ちに反映(ウインドウのクライアント領域を更新)

	DWORD OldTime = 0;
	DWORD NowTime = timeGetTime();
	//メッセージループ
	//第1引数 :メッセージ情報
	//第2引数 :ウインドウのハンドル
	//第3引数 :最初のメッセージ
	//第4引数 :最後のメッセージ
	while(1)
	{ 
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)//メッセージを取得しなかった場合"0"を返す
		{//Windowsの処理
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれて、WM_QUITメッセージが来たらループ終了
				break;
			}
			else
			{
				//メッセージの翻訳と送出
				TranslateMessage(&msg);       //仮想キーメッセージを文字メッセージへ変換
				DispatchMessage(&msg);        //ウインドウプロシージャへメッセージを送出
			}
		}
		else
		{//DirectXの処理
			if (NowTime - OldTime > 1000 / FPS ) {
				// 更新処理
				Update();

				// 描画処理
				Draw();

				OldTime = NowTime;
			}
			NowTime = timeGetTime();
		}
	}

	// ウインドウクラスの登録を解除
	// 第1引数 : クラス名
	// 第2引数 : アプリケーションインスタンスのハンドル
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	//終了処理
	Uninit();

	return (int)msg.wParam;
}

//=============================================================
// ウインドウプロシージャ
// (CALLBACK : Win32API関数を呼び出すときの規約)
// hWnd     :ウインドウのハンドル
// uMsg     :メッセージの識別子
// wParam   :メッセージの最初のパラメータ
// lParam   :メッセージの2番目のパラメータ
//=============================================================
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{

	switch (uMsg)
	{
		int nID;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_ESCAPE:
			nID = MessageBox(hWnd, "終了しますか？", "終了", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:
		nID = MessageBox(hWnd, "終了しますか？", "終了", MB_YESNO);
		if (nID == IDYES)
		{
			DestroyWindow(hWnd);
		}
		else
		{
			return 0;
		}
		break;

	default:
		break;
	}

	return DefWindowProc(hWnd, uMsg, wParam, lParam);   //既定の処理を提供
}

//==================================================================
// 初期化処理
// hInstance : インスタンスのハンドル
// hWnd      : ウインドウのハンドル
// bWindow   : ウインドウモードにするかどうか
//==================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクトの生成
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のディスプレイモードを取得
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//デバイスのプレゼンテーションパラメータの設定
	ZeroMemory(&d3dpp, sizeof(d3dpp));               //ワークをゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;            //ゲーム画面サイズ(幅)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;          //ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;           //カラーモードの指定
	d3dpp.BackBufferCount = 1;                       //バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;        //映像信号に同期してフリップする
	d3dpp.Windowed = bWindow;                        //ウインドウモード
	d3dpp.EnableAutoDepthStencil = TRUE;             //デプスバッファ(Zバッファ)とステンシルバッファを作成
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;       //デプスバッファとして16bitを使う

	if (bWindow)
	{//ウインドウモード
		d3dpp.FullScreen_RefreshRateInHz = 0;    //リフレッシュレート(指定できないので0固定)
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		//インターバル(VSyncを待たずに描画)
	}
	else
	{//フルスクリーンモード
		d3dpp.FullScreen_RefreshRateInHz = 0;  //リフレッシュレート(指定できないので0固定)
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		//インターバル(VSyncを待って描画)
	}

	//デバイスオブジェクトの生成
	//[デバイス作成制御]<描画>と<頂点処理>をハードウェアで行う
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,       //ディスプレイアダプタ	
		D3DDEVTYPE_HAL,                          //デバイスタイプ
		hWnd,                                    //フォーカスするウインドウへのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,     //デバイス作成制御の組み合わせ
		&d3dpp,                                  //デバイスのプレゼンテーションパラメータ
		&g_pD3DDevice)))                         //デバイスインターフェースへのポインタ
	{
		//[デバイス作成制御]<描画>をハードウェアで行い、<頂点処理>はCPUで行う
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			//上記の設定が失敗したら
			//[デバイス作成制御]<描画>と頂点処理をCPUで行う
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				//初期化失敗
				return E_FAIL;
			}
		}
	}
	InitInput(hInstance, hWnd);//入力情報の初期化
	InitSound();
	
	Title_Texture = LoadTexture(TITLE_FILE, Title_Texture);//タイトルのテクスチャ読み込み
	Tutorial_Texture = LoadTexture(Tutorial_FILE, Tutorial_Texture);//チュートリアルのテクスチャ読み込み
	PadTutorial_Texture = LoadTexture(PadTutorial_FILE, PadTutorial_Texture);//ゲームパッドのチュートリアルのテクスチャ読み込み
	Result_Texture = LoadTexture(RESULT_FILE, Result_Texture);//リザルトのテクスチャ読み込み
	Load_Texture = LoadTexture(LOAD_FILE, Load_Texture);//ロード画面のテクスチャ読み込み

	game = new Game;

	game->Init();

	return S_OK;
}

//==================================================
//　終了処理
//==================================================
void Uninit(void)
{
	UninitInput();
	UninitSound();
	
	ReleaseTexture(Title_Texture);
	ReleaseTexture(Result_Texture);
	ReleaseTexture(Load_Texture);
	ReleaseTexture(Tutorial_Texture);
	ReleaseTexture(PadTutorial_Texture);

	if (game != NULL) {
		delete game;
	}
	if (g_pD3DDevice != NULL)
	{// デバイスの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if (g_pD3D != NULL)
	{// Direct3Dオブジェクトの開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}


//===================================================
// 更新処理
//===================================================
void Update(void)
{
	bool ENTER_TRIG = false;//仮想キー
	UpdateInput();//キー入力の更新

	if (GetKeyboardTrigger(DIK_RETURN)) {
		//決定キーに対応するボタン・キーが押されたら
		ENTER_TRIG = true;
	}
	if (g_pDIDevGamePad) {
		if (GetGamePadTrigger(0)|| GetGamePadTrigger(1) ||GetGamePadTrigger(2)|| GetGamePadTrigger(3)) {
			ENTER_TRIG = true;
		}
	}

	switch (Scene)
	{
	case TITLE:

		//タイトル画面の操作処理
		if (ENTER_TRIG)
		{
			Scene = TUTORIAL;
		}
		break;


	case TUTORIAL:

		//タイトル画面の操作処理
		if (ENTER_TRIG)
		{
			Scene = GAME_STAGE1;
			
			game->Init();

			game->Edit();

			game->SetCoord();

			game->Gettime();

			JoypadDI_X = GetGamePadLeftStickX();
			JoypadDI_Y = GetGamePadLeftStickY();


			if (PollSound(TitleBGM)) {
				StopSound(TitleBGM);//タイトルBGM停止
			}
			PlaySound(GamePlayBGM);//ゲームBGM再生
		}
		break;

	case GAME_STAGE1:
		if (game != NULL) {
			game->Update();
		}
		//ゲームのCLEAR条件を記入
		if (game->OutClearFlg())
		{
			Scene = RESULT;;

			//game->Init();
			//
			//game->Edit();

			//game->SetCoord();


		}
		break;

	case GAME_STAGE2:
		if (game != NULL) {
			game->Update();
		}
		if (game->OutClearFlg()) {
			Scene = RESULT;
		}
		break;

	case RESULT:
		//リザルト画面の操作処理
		g_cnt++;
		if (ENTER_TRIG || g_cnt >= SCENE_TIME)
		{
			Scene = LOAD;
			g_cnt = 0;
		}
		break;

	case LOAD://ロード中(この間にゲームクラスのdeleteとnewを行う)
		if (game != NULL)
		{
			if (PollSound(GamePlayBGM)) {//ゲームBGM停止
				StopSound(GamePlayBGM);
			}
			delete game;
			game = new Game;//こいつに時間かかる
			game->Init();
			Scene = TITLE;
		}
		break;
	}

}

//===================================================
// 描画処理
//===================================================
void Draw(void)
{
	// バックバッファ&Zバッファのクリア
	// Count   : pRects内にある矩形の数
	// pRects  : クリアする矩形の配列(NULL指定でビューポート矩形全体をクリア)
	// Flags   : クリアするサーフェスを示すフラグ(NULL指定でビューポート矩形全体をクリア)
	//         : [D3DCLEAR_TARGET - レンダリングターゲットをクリアしてColorパラメータの値にする]
	//         : [D3DCLEAR_ZBUFFER - 深度(Z)バッファをクリアしてZパラメータの値にする]
	//         : [D3DCLEAR_STENCIL - ステンシルバッファをクリアしてStencilパラメータの値にする]
	// Color   : サーフェスをクリアする色
	// Z       : デプスバッファに保存する値
	// Stencil : ステンシルバッファに保存する値(整数)
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(255,255,255,255), 1.0f, 0);

	// Direct3Dによる描画の開始
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		//具体的な描画処理をココに記述していく
		switch (Scene)//状態遷移
		{
		case TITLE:
			//タイトル画面の描画処理
			Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), Title_Texture, 0, 0, 1, 1);
			break;

		case TUTORIAL:
			//チュートリアル画面の描画処理
			if (g_pDIDevGamePad) {
				Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), PadTutorial_Texture, 0, 0, 1, 1);
			}
			else {
				Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), Tutorial_Texture, 0, 0, 1, 1);
			}
			break;

		case GAME_STAGE1:
		case GAME_STAGE2:
			//ゲーム画面の描画処理
			if (game != NULL) {
				game->Draw();
			}
			break;

		case RESULT:
			//リザルトの描画処理
			Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), Result_Texture, 0, 0, 1, 1);
			break;

		case LOAD:
			//ロード中
			Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), Load_Texture, 0, 0, 1, 1);
			break;
		}


		//Direct3Dによる描画の終了
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバッファの入れ替え
	// pSourceRect           : 転送元矩形
	// pDestRect             : 転送先矩形
	// hDestWindowOverride   : 転送先ウインドウへのポインタ
	// pDirtyRegion          : 通常はNULLで
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}