//=======================================================
// DX21
// DirectX雛型処琁Emain.cpp]
//
//
//=======================================================
#include "main.h"
#include "Asset.h"
#include "config.h"
#include "Game.h"
#include "XAudio2.h"
#include "TexLoad.h"
#include <Time.h>
//*******************************************************
//マクロ定義
//*******************************************************
#define CLASS_NAME  "サンプル"  //ウインドウクラスの名前
#define WINDOW_NAME "KIRISAME"  //ウインドウの名前
//更新してみた、E

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
LPDIRECT3D9        g_pD3D = NULL;   // Direct3DオブジェクチE
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;//DeviceオブジェクチE描画に忁E��E

SCENE Scene = TITLE;//状態�E移用(タイトルから開姁E

int g_cnt = 0;//勝手にタイトル戻すやつ

LPDIRECT3DTEXTURE9* Title_Texture = NULL;//タイトル
LPDIRECT3DTEXTURE9* Tutorial_Texture = NULL;//チュートリアル
LPDIRECT3DTEXTURE9* PadTutorial_Texture = NULL;
LPDIRECT3DTEXTURE9* Result_Texture = NULL;//リザルチE
LPDIRECT3DTEXTURE9* Load_Texture = NULL;//ロード中
Game *game = NULL;
_TexOp *TexOp = NULL;

extern LPDIRECTINPUTDEVICE8 g_pDIDevGamePad;
//=======================================================
//メイン関数
// (WINAPI : Win32API関数を呼び出す時の規紁E
// hInstance     ; こ�Eアプリケーションのインスタンスハンドル(識別孁E
// hPrevInstance : 常にNULL(16bit時代の名残り)
// lpCmdLine     : コマンドラインから受け取った引数へのポインタ
// nCmdShow      : アプリケーションの初期ウインドウ表示方法�E持E��E
//=======================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),         //WNDCLASSEXのメモリサイズを指宁E
		CS_CLASSDC,                 //表示するウインドウのスタイルを設宁E
		WndProc,                    //ウインドウプロシージャのアドレス(関数吁Eを指宁E
		0,                          //通常は使用しなぁE�Eで"0"を指宁E
		0,                          //通常は使用しなぁE�Eで"0"を指宁E
		hInstance,                  //WinMainのパラメータのインスタンスハンドルを設宁E
		NULL,                       //使用するアイコンを指宁EWindowsがもってぁE��アイコンを使ぁE��らNULL)
		LoadCursor(NULL, IDC_ARROW),//マウスカーソルを指宁E
		(HBRUSH)(COLOR_WINDOW + 1), //ウインドウのクライアント領域の背景色を設宁E
		NULL,                       //Windowにつけるメニューを設宁E
		CLASS_NAME,                 //ウインドウクラスの名前
		NULL,                       //小さぁE��イコンが設定された場合�E惁E��を記述
	};
	HWND hWnd;
	MSG msg;

	//ウインドウクラスの登録
	RegisterClassEx(&wcex);

	//ウインドウを作�E
	hWnd = CreateWindowEx(0,//拡張ウインドウスタイル
		CLASS_NAME,     //ウインドウクラスの名前
		WINDOW_NAME,    //ウインドウの名前
		WS_OVERLAPPEDWINDOW, //ウインドウスタイル
		CW_USEDEFAULT,    //ウインドウの左上X座樁E
		CW_USEDEFAULT,    //ウインドウの左上Y座樁E
		(SCREEN_WIDTH + GetSystemMetrics(SM_CXDLGFRAME) * 2),     //ウインドウの幁E
		(SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2 +
			GetSystemMetrics(SM_CYCAPTION)),    //ウインドウの高さ
		NULL,             //親ウインドウのハンドル
		NULL,             //メニューハンドルまた�E子ウインドウID
		hInstance,        //インスタンスハンドル
		NULL);            //ウインドウ作�EチE�Eタ

						  //初期化�E琁Eウインドウを作�Eしてから行う)
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//ウインドウの表示(初期化�E琁E�E後に行う)
	ShowWindow(hWnd, nCmdShow); // 持E��されたウインドウの表示状態を設宁Eウインドウを表示)
	UpdateWindow(hWnd);         //ウインドウの状態を直ちに反映(ウインドウのクライアント領域を更新)

	DWORD OldTime = 0;
	DWORD NowTime = timeGetTime();
	//メチE��ージルーチE
	//第1引数 :メチE��ージ惁E��
	//第2引数 :ウインドウのハンドル
	//第3引数 :最初�EメチE��ージ
	//第4引数 :最後�EメチE��ージ
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)//メチE��ージを取得しなかった場吁E0"を返す
		{//Windowsの処琁E
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()が呼ばれて、WM_QUITメチE��ージが来たらループ終亁E
				break;
			}
			else
			{
				//メチE��ージの翻訳と送�E
				TranslateMessage(&msg);       //仮想キーメチE��ージを文字メチE��ージへ変換
				DispatchMessage(&msg);        //ウインドウプロシージャへメチE��ージを送�E
			}
		}
		else
		{//DirectXの処琁E
			if (NowTime - OldTime > 1000 / FPS) {
				// 更新処琁E
				Update();

				// 描画処琁E
				Draw();

				OldTime = NowTime;
			}
			NowTime = timeGetTime();
		}
	}

	// ウインドウクラスの登録を解除
	// 第1引数 : クラス吁E
	// 第2引数 : アプリケーションインスタンスのハンドル
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	//終亁E�E琁E
	Uninit();

	return (int)msg.wParam;
}

//=============================================================
// ウインドウプロシージャ
// (CALLBACK : Win32API関数を呼び出すとき�E規紁E
// hWnd     :ウインドウのハンドル
// uMsg     :メチE��ージの識別孁E
// wParam   :メチE��ージの最初�Eパラメータ
// lParam   :メチE��ージの2番目のパラメータ
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
			nID = MessageBox(hWnd, "終亁E��ますか�E�E, "終亁E, MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:
		nID = MessageBox(hWnd, "終亁E��ますか�E�E, "終亁E, MB_YESNO);
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

	return DefWindowProc(hWnd, uMsg, wParam, lParam);   //既定�E処琁E��提侁E
}

//==================================================================
// 初期化�E琁E
// hInstance : インスタンスのハンドル
// hWnd      : ウインドウのハンドル
// bWindow   : ウインドウモードにするかどぁE��
//==================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3Dオブジェクト�E生�E
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//現在のチE��スプレイモードを取征E
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//チE��イスのプレゼンチE�Eションパラメータの設宁E
	ZeroMemory(&d3dpp, sizeof(d3dpp));               //ワークをゼロクリア
	d3dpp.BackBufferWidth = SCREEN_WIDTH;            //ゲーム画面サイズ(幁E
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;          //ゲーム画面サイズ(高さ)
	d3dpp.BackBufferFormat = d3ddm.Format;           //カラーモード�E持E��E
	d3dpp.BackBufferCount = 1;                       //バックバッファの数
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;        //映像信号に同期してフリチE�Eする
	d3dpp.Windowed = bWindow;                        //ウインドウモーチE
	d3dpp.EnableAutoDepthStencil = TRUE;             //チE�Eスバッファ(Zバッファ)とスチE��シルバッファを作�E
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;       //チE�Eスバッファとして16bitを使ぁE

	if (bWindow)
	{//ウインドウモーチE
		d3dpp.FullScreen_RefreshRateInHz = 0;    //リフレチE��ュレーチE持E��できなぁE�Eで0固宁E
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		//インターバル(VSyncを征E��ずに描画)
	}
	else
	{//フルスクリーンモーチE
		d3dpp.FullScreen_RefreshRateInHz = 0;  //リフレチE��ュレーチE持E��できなぁE�Eで0固宁E
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		//インターバル(VSyncを征E��て描画)
	}

	//チE��イスオブジェクト�E生�E
	//[チE��イス作�E制御]<描画>と<頂点処琁Eをハードウェアで行う
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,       //チE��スプレイアダプタ	
		D3DDEVTYPE_HAL,                          //チE��イスタイチE
		hWnd,                                    //フォーカスするウインドウへのハンドル
		D3DCREATE_HARDWARE_VERTEXPROCESSING,     //チE��イス作�E制御の絁E��合わぁE
		&d3dpp,                                  //チE��イスのプレゼンチE�Eションパラメータ
		&g_pD3DDevice)))                         //チE��イスインターフェースへのポインタ
	{
		//[チE��イス作�E制御]<描画>をハードウェアで行い、E頂点処琁EはCPUで行う
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			//上記�E設定が失敗したら
			//[チE��イス作�E制御]<描画>と頂点処琁E��CPUで行う
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				//初期化失敁E
				return E_FAIL;
			}
		}
	}
	InitInput(hInstance, hWnd);//入力情報の初期匁E
	InitSound();

	Title_Texture = LoadTexture(TITLE_FILE, Title_Texture);//タイトルのチE��スチャ読み込み
	Tutorial_Texture = LoadTexture(Tutorial_FILE, Tutorial_Texture);//チュートリアルのチE��スチャ読み込み
	PadTutorial_Texture = LoadTexture(PadTutorial_FILE, PadTutorial_Texture);//ゲームパッド�EチュートリアルのチE��スチャ読み込み
	Result_Texture = LoadTexture(RESULT_FILE, Result_Texture);//リザルト�EチE��スチャ読み込み
	Load_Texture = LoadTexture(LOAD_FILE, Load_Texture);//ロード画面のチE��スチャ読み込み


	TexOp = new _TexOp;

	TexOp->LoadAllTexture();
	game = new Game;
	game->Init();

	return S_OK;
}

//==================================================
//　終亁E�E琁E
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

	TexOp->ReleaseAllTexture();
	if (TexOp != NULL) {
		delete TexOp;
	}
	if (game != NULL) {
		delete game;
	}
	if (g_pD3DDevice != NULL)
	{// チE��イスの開放
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if (g_pD3D != NULL)
	{// Direct3Dオブジェクト�E開放
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}


//===================================================
// 更新処琁E
//===================================================
void Update(void)
{
	bool ENTER_TRIG = false;//仮想キー
	UpdateInput();//キー入力�E更新

	if (GetKeyboardTrigger(DIK_RETURN)) {
		//決定キーに対応する�Eタン・キーが押されたら
		ENTER_TRIG = true;
	}
	if (g_pDIDevGamePad) {
		if (GetGamePadTrigger(0) || GetGamePadTrigger(1) || GetGamePadTrigger(2) || GetGamePadTrigger(3)) {
			ENTER_TRIG = true;
		}
	}

	switch (Scene)
	{
	case TITLE:
		if (!PollSound(TitleBGM))//非�E生状態ならBGMを�E甁E
		{
			PlaySound(TitleBGM);
		}
		//タイトル画面の操作�E琁E
		if (ENTER_TRIG)
		{
			if (PollSound(TitleBGM)) {
				StopSound(TitleBGM);//タイトルBGM停止
			}
			Scene = TUTORIAL;
		}
		break;


	case TUTORIAL:

		//タイトル画面の操作�E琁E
		if (ENTER_TRIG)
		{
			Scene = GAME_STAGE1;

			game->Init();

			game->Edit();

			game->SetCoord();

			game->Gettime();

			JoypadDI_X = GetGamePadLeftStickX();
			JoypadDI_Y = GetGamePadLeftStickY();




		}
		break;

	case GAME_STAGE1:
		if (!PollSound(GamePlayBGM))//非�E生状態ならBGMを�E甁E
		{
			PlaySound(GamePlayBGM);
		}
		if (game != NULL) {
			game->Update();
		}
		//ゲームのCLEAR条件を記�E
		if (game->OutClearFlg())
		{

			Scene = GAME_STAGE2;

			game->Init();

			game->Edit();

			game->SetCoord();


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
		if (PollSound(GamePlayBGM)) {//ゲームBGM停止
			StopSound(GamePlayBGM);
		}
		//リザルト画面の操作�E琁E
		g_cnt++;
		if (ENTER_TRIG || g_cnt >= SCENE_TIME)
		{
			Scene = LOAD;
			g_cnt = 0;
		}
		break;

	case LOAD://ロード中(こ�E間にゲームクラスのdeleteとnewを行う)
		if (game != NULL)
		{
			game->EnemyUnInit();

			delete game;
			game = new Game;//こいつに時間かかめE
			game->Init();
			Scene = TITLE;
		}
		break;
	}

}

//===================================================
// 描画処琁E
//===================================================
void Draw(void)
{
	// バックバッファ&Zバッファのクリア
	// Count   : pRects冁E��ある矩形の数
	// pRects  : クリアする矩形の配�E(NULL持E��でビューポ�Eト矩形全体をクリア)
	// Flags   : クリアするサーフェスを示すフラグ(NULL持E��でビューポ�Eト矩形全体をクリア)
	//         : [D3DCLEAR_TARGET - レンダリングターゲチE��をクリアしてColorパラメータの値にする]
	//         : [D3DCLEAR_ZBUFFER - 深度(Z)バッファをクリアしてZパラメータの値にする]
	//         : [D3DCLEAR_STENCIL - スチE��シルバッファをクリアしてStencilパラメータの値にする]
	// Color   : サーフェスをクリアする色
	// Z       : チE�Eスバッファに保存する値
	// Stencil : スチE��シルバッファに保存する値(整数)
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(255, 255, 255, 255), 1.0f, 0);

	// Direct3Dによる描画の開姁E
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		//具体的な描画処琁E��ココに記述してぁE��
		switch (Scene)//状態�E移
		{
		case TITLE:
			//タイトル画面の描画処琁E
			Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), Title_Texture, 0, 0, 1, 1);
			break;

		case TUTORIAL:
			//チュートリアル画面の描画処琁E
			if (g_pDIDevGamePad) {
				Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), PadTutorial_Texture, 0, 0, 1, 1);
			}
			else {
				Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), Tutorial_Texture, 0, 0, 1, 1);
			}
			break;

		case GAME_STAGE1:
		case GAME_STAGE2:
			//ゲーム画面の描画処琁E
			if (game != NULL) {
				game->Draw();
			}
			break;

		case RESULT:
			//リザルト�E描画処琁E
			Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), Result_Texture, 0, 0, 1, 1);
			break;

		case LOAD:
			//ロード中
			Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), Load_Texture, 0, 0, 1, 1);
			break;
		}


		//Direct3Dによる描画の終亁E
		g_pD3DDevice->EndScene();
	}

	// バックバッファとフロントバチE��ァの入れ替ぁE
	// pSourceRect           : 転送�E矩形
	// pDestRect             : 転送�E矩形
	// hDestWindowOverride   : 転送�Eウインドウへのポインタ
	// pDirtyRegion          : 通常はNULLで
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}