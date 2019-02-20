//=============================================================================
//
// 入力処理 [input.cpp]
//
//=============================================================================
#include "input.h"


//*****************************************************************************
// 定数定義
//*****************************************************************************
const int NUM_KEY_MAX = 256;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd);
void UninitKeyboard(void);
HRESULT UpdateKeyboard(void);
HRESULT InitMouse(HINSTANCE hInst, HWND hWnd);
void UninitMouse(void);
HRESULT UpdateMouse(void);
HRESULT InitGamePad(HINSTANCE hInst, HWND hWnd);
void UnInitGamePad(void);
BOOL UpdateGamePad(void);
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext);
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *PDIDOL, void *pContext);

//*****************************************************************************
// グローバル変数
//*****************************************************************************
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8インターフェースへのポインタ
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;			// IDirectInputDevice8インターフェースへのポインタ(キーボード)
LPDIRECTINPUTDEVICE8    g_pDIDevGamePad = NULL;				// IDirectInputDevuce8インターフェースへのポインタ(ゲームパッド)
LPDIRECTINPUTDEVICE8	g_pDIDevMouse = NULL;				// IDirectInputDevice8インターフェースへのポインタ(マウス)
BYTE					g_keyState[NUM_KEY_MAX];			// キーボードの状態を受け取るワーク
BYTE					g_keyStateTrigger[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
BYTE					g_keyStateRepeat[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
BYTE					g_keyStateRelease[NUM_KEY_MAX];		// キーボードの状態を受け取るワーク
int						g_keyStateRepeatCnt[NUM_KEY_MAX];	// キーボードのリピートカウンタ
DIMOUSESTATE2			g_mouseState;						// マウスの状態を受け取るワーク
DIMOUSESTATE2			g_mouseStateTrigger;				// マウスの状態を受け取るワーク
POINT					g_MousePoint;						// マウスの現在座標
DIJOYSTATE              g_joystickstate;                    // ゲームパッドの状態を受け取るワーク
DIJOYSTATE				g_joysticksTrigger;					// ゲームパッドの状態を受け取るワーク
HWND					g_hwnd;

DIDEVCAPS g_diDevCaps;

//もし十字キーの入力がプログラム起動後勝手に入力されている状態なら
//その数値を保存し、差分で調整する
//その調整に利用する用の変数
long JoypadDI_X;
long JoypadDI_Y;
//=============================================================================
// 入力処理の初期化
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if(!g_pDInput)
	{
		// DirectInputオブジェクトの作成
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}

	// キーボードの初期化
	InitKeyboard(hInst, hWnd);

	// マウスの初期化
	InitMouse(hInst, hWnd);

	// ゲームパッドの初期化
	InitGamePad(hInst, hWnd);

	g_hwnd = hWnd;

	return hr;
}

//=============================================================================
// 入力処理の終了処理
//=============================================================================
void UninitInput(void)
{
	// キーボードの終了処理
	UninitKeyboard();

	// マウスの終了処理
	UninitMouse();

	//ゲームパッドの終了処理
	UnInitGamePad();

	if(g_pDInput)
	{
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

//=============================================================================
// 入力処理の更新処理
//=============================================================================
void UpdateInput(void)
{
	// キーボードの更新
	UpdateKeyboard();

	// マウスの更新
	UpdateMouse();

	//ゲームパッドの更新
	UpdateGamePad();
}

//=============================================================================
// キーボードの初期化
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// デバイスオブジェクトを作成
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if(FAILED(hr) || g_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "キーボードがねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "キーボードの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// キーボードの終了処理
//=============================================================================
void UninitKeyboard(void)
{
	if(g_pDIDevKeyboard)
	{
		g_pDIDevKeyboard->Unacquire();

		g_pDIDevKeyboard->Release();
		g_pDIDevKeyboard = NULL;
	}
}

//=============================================================================
// キーボードの更新処理
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// 前回のデータを保存
	memcpy(keyStateOld, g_keyState, NUM_KEY_MAX);

	// デバイスからデータを取得
	hr = g_pDIDevKeyboard->GetDeviceState(sizeof(g_keyState), g_keyState);
	if(SUCCEEDED(hr))
	{
		for(int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			// トリガーとリリースを取得
			g_keyStateTrigger[cnt] = ((keyStateOld[cnt] ^ g_keyState[cnt]) & g_keyState[cnt]);
			//g_keyStateRelease[cnt] = 
			
			// キーが押されているならリピートの判定処理
			if(g_keyState[cnt])
			{
				if(g_keyStateRepeatCnt[cnt] < 20)
				{
					g_keyStateRepeatCnt[cnt]++;
					// 「初回入力」もしくは「ボタンを押してから20フレーム経過」
					// した場合、リピート用配列のデータを有効にする
					//if(  ||  )
					// 上記条件以外はリピート用配列のデータを無効にする
					//else
				}
			}
			else
			{
				g_keyStateRepeatCnt[cnt] = 0;
				g_keyStateRepeat[cnt] = 0;
			}
		}
	}
	else
	{
		// キーボードへのアクセス権を取得
		hr = g_pDIDevKeyboard->Acquire();
	}

	return hr;
}

//=============================================================================
// キーボードのプレス状態を取得
//=============================================================================
bool GetKeyboardPress(int key)
{
	return ((g_keyState[key] & 0x80) != 0);
}

//=============================================================================
// キーボードのトリガー状態を取得
//=============================================================================
bool GetKeyboardTrigger(int key)
{
	return ((g_keyStateTrigger[key] & 0x80) != 0);
}

//=============================================================================
// キーボードのリピート状態を取得
//=============================================================================
bool GetKeyboardRepeat(int key)
{
	return ((g_keyStateRepeat[key] & 0x80) != 0);
}

//=============================================================================
// キーボードのリリ－ス状態を取得
//=============================================================================
bool GetKeyboardRelease(int key)
{
	return ((g_keyStateRelease[key] & 0x80) != 0);
}

//=============================================================================
// マウスの初期化
//=============================================================================
HRESULT InitMouse(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// デバイスオブジェクトを作成
	hr = g_pDInput->CreateDevice(GUID_SysMouse, &g_pDIDevMouse, NULL);
	if(FAILED(hr) || g_pDIDevMouse == NULL)
	{
		MessageBox(hWnd, "マウスがねぇ！", "警告！", MB_ICONWARNING);
		return hr;
	}

	// データフォーマットを設定
	hr = g_pDIDevMouse->SetDataFormat(&c_dfDIMouse2);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マウスのデータフォーマットを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// 協調モードを設定（フォアグラウンド＆非排他モード）
	hr = g_pDIDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "マウスの協調モードを設定できませんでした。", "警告！", MB_ICONWARNING);
		return hr;
	}

	// デバイスの設定
	{
		DIPROPDWORD dipdw;

		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = DIPROPAXISMODE_REL;

		hr = g_pDIDevMouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
	}

	// キーボードへのアクセス権を獲得(入力制御開始)
	g_pDIDevMouse->Acquire();

	return S_OK;
}

//=============================================================================
// マウスの終了処理
//=============================================================================
void UninitMouse(void)
{
	if(g_pDIDevMouse)
	{
		g_pDIDevMouse->Unacquire();

		g_pDIDevMouse->Release();
		g_pDIDevMouse = NULL;
	}
}

//=============================================================================
// マウスの更新処理
//=============================================================================
HRESULT UpdateMouse(void)
{
	HRESULT hr;
	DIMOUSESTATE2 mouseStateOld;

	// 前回のデータを保存
	mouseStateOld = g_mouseState;
		
	GetCursorPos(&g_MousePoint);
	ScreenToClient(g_hwnd, &g_MousePoint);


	// デバイスからデータを取得
	hr = g_pDIDevMouse->GetDeviceState(sizeof(g_mouseState), &g_mouseState);
	if(SUCCEEDED(hr))
	{
		// トリガーの取得
		//g_mouseStateTrigger.lX = 
		//g_mouseStateTrigger.lY = 
		//g_mouseStateTrigger.lZ = 
		for(int cnt = 0; cnt < 8; cnt++)
		{
			g_mouseStateTrigger.rgbButtons[cnt] = ((mouseStateOld.rgbButtons[cnt] ^ g_mouseState.rgbButtons[cnt]) & g_mouseState.rgbButtons[cnt]);
		}
	}
	else
	{
		g_pDIDevMouse->Acquire();
	}

	return S_OK;
}

//=============================================================================
// マウスデータ取得(左プレス)
//=============================================================================
bool GetMouseLeftPress(void)
{
	return ((g_mouseState.rgbButtons[0] & 0x80) != 0);
}

//=============================================================================
// マウスデータ取得(左トリガー)
//=============================================================================
bool GetMouseLeftTrigger(void)
{
	return ((g_mouseStateTrigger.rgbButtons[0] & 0x80) != 0);
}

//=============================================================================
// マウスデータ取得(右プレス)
//=============================================================================
bool GetMouseRightPress(void)
{
	return ((g_mouseState.rgbButtons[1] & 0x8) != 00);
}

//=============================================================================
// マウスデータ取得(右トリガー)
//=============================================================================
bool GetMouseRightTrigger(void)
{
	return ((g_mouseStateTrigger.rgbButtons[1] & 0x80) != 0);
}

//=============================================================================
// マウスデータ取得(中央プレス)
//=============================================================================
bool GetMouseCenterPress(void)
{
	return ((g_mouseState.rgbButtons[2] & 0x80) != 0);
}

//=============================================================================
// マウスデータ取得(中央トリガー)
//=============================================================================
bool GetMouseCenterTrigger(void)
{
	return ((g_mouseState.rgbButtons[2] & 0x80) != 0);
}

//=============================================================================
// マウスデータ取得(Ｘ軸移動)
//=============================================================================
long GetMouseAxisX(void)
{
	return g_mouseState.lX;
}

//=============================================================================
// マウスデータ取得(Ｙ軸移動)
//=============================================================================
long GetMouseAxisY(void)
{
	return g_mouseState.lY;
}

//=============================================================================
// マウスデータ取得(Ｚ軸移動)
//=============================================================================
long GetMouseAxisZ(void)
{
	return g_mouseState.lZ;
}
//=============================================================================
// マウス座標取得(X)
//=============================================================================
long GetMouseX(void)
{
	return g_MousePoint.x;
}

//=============================================================================
// マウス座標取得(Y)
//=============================================================================
long GetMouseY(void)
{
	return g_MousePoint.y;
}

//=============================================================================
//ゲームパッドの初期化
//=============================================================================
HRESULT InitGamePad(HINSTANCE hInst, HWND hWnd) {
	HRESULT hr;

	hr = g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);
	if (FAILED(hr) || g_pDIDevGamePad == NULL) {
		//MessageBox(hWnd, "デバイス作成に失敗しました", "Error", MB_OK);//ゲームパッドが接続されていない時はここで警告
		return FALSE;
	}

	hr = g_pDIDevGamePad->SetDataFormat(&c_dfDIJoystick);
	if (FAILED(hr)) {
		MessageBox(hWnd, "データフォーマットに失敗しました", "Error", MB_OK);
		return FALSE;
	}

	hr = g_pDIDevGamePad->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)) {
		MessageBox(hWnd, "cooperative levelに失敗しました", "Error", MB_OK);
		return FALSE;
	}

	g_diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hr = g_pDIDevGamePad->GetCapabilities(&g_diDevCaps);
	if (FAILED(hr)) {
		MessageBox(hWnd, "device capabilitiesに失敗しました", "Error", MB_OK);
		return FALSE;
	}

	hr = g_pDIDevGamePad->EnumObjects(EnumAxesCallback, (VOID*)hWnd, DIDFT_AXIS);
	if (FAILED(hr)) {
		MessageBox(hWnd, "プロパティのセットに失敗しました", "Error", MB_OK);
		return FALSE;
	}

	hr = g_pDIDevGamePad->Poll();
	if (FAILED(hr)) {
		hr = g_pDIDevGamePad->Acquire();
		while (hr == DIERR_INPUTLOST) {
			hr = g_pDIDevGamePad->Acquire();
		}
	}

	return TRUE;
}
//=============================================================================
// ゲームパッドの終了処理
//=============================================================================
void UnInitGamePad(void) {
	if (g_pDIDevGamePad)
	{
		g_pDIDevGamePad->Unacquire();

		g_pDIDevGamePad->Release();
		g_pDIDevGamePad = NULL;
	}
}
//=============================================================================
//ゲームパッドのコールバック関数
//=============================================================================
BOOL CALLBACK EnumJoysticksCallback(const DIDEVICEINSTANCE *pdidInstance, VOID *pContext) {
	HRESULT hr;
	
	hr = g_pDInput->CreateDevice(pdidInstance->guidInstance, &g_pDIDevGamePad,NULL);

	if (FAILED(hr))return DIENUM_CONTINUE;

	return DIENUM_STOP;
}

//=============================================================================
//Axes CallBack
//=============================================================================
BOOL CALLBACK EnumAxesCallback(const DIDEVICEOBJECTINSTANCE *PDIDOL, void *pContext) {
	HRESULT hr;
	DIPROPRANGE diprg;

	diprg.diph.dwSize = sizeof(DIPROPRANGE);
	diprg.diph.dwHeaderSize = sizeof(DIPROPHEADER);
	diprg.diph.dwHow = DIPH_BYID;
	diprg.lMin = 0 - 1000;
	diprg.lMax = 0 + 1000;

	hr = g_pDIDevGamePad->SetProperty(DIPROP_RANGE, &diprg.diph);

	if (FAILED(hr)) return DIENUM_STOP;

	return DIENUM_CONTINUE;
}

//=============================================================================
//ゲームパッドの入力受け取り関数
//=============================================================================
BOOL UpdateGamePad(void) {
	HRESULT hr;
	DIJOYSTATE OldJoyState;

	OldJoyState = g_joystickstate;
	//DIMOUSESTATE2 mouseStateOld;

	//// 前回のデータを保存
	//mouseStateOld = g_mouseState;

	if (NULL == g_pDIDevGamePad)return FALSE;

	hr = g_pDIDevGamePad->Poll();
	if (FAILED(hr)) {
		hr = g_pDIDevGamePad->Acquire();
		while (hr == DIERR_INPUTLOST) {
			hr = g_pDIDevGamePad->Acquire();
		}
	}

	//if (FAILED(hr))  return FALSE;

	hr = g_pDIDevGamePad->GetDeviceState(sizeof(DIJOYSTATE), &g_joystickstate);
	if (FAILED(hr))return FALSE;

	//入力を受け取ったときの処理------------------
	//テンプレート------------------------------
	/*if (g_joystickstate.lX == -1000) {//左
	//処理
	}
	if (g_joystickstate.lX == 1000) {//右
	//処理
	}
	if (g_joystickstate.lY == -1000) {//上
	//処理
	}
	if (g_joystickstate.lY == 1000) {//下
	//処理
	}
	if (g_joystickstate.rgbButtons[0] & 0x80) {//ボタン１
	//処理
	}*/
	//------------------------------------------
	if (SUCCEEDED(hr)) {
		for (int cnt = 0; cnt < 32; cnt++) {
			g_joysticksTrigger.rgbButtons[cnt] = ((OldJoyState.rgbButtons[cnt] ^ g_joystickstate.rgbButtons[cnt]) & g_joystickstate.rgbButtons[cnt]);

		}

	}
	else {
		g_pDIDevGamePad->Acquire();
	}

	return TRUE;
}


//=============================================================================
// ゲームパッドの右スティックの座標取得(X)
//=============================================================================
long GetGamePadRightStickX(void)
{
	return g_joystickstate.lZ;
}

//=============================================================================
// ゲームパッドの右スティックの座標取得(Y)
//=============================================================================
long GetGamePadRightStickY(void)
{
	return g_joystickstate.lRz;
}
//=============================================================================
// ゲームパッドの左スティックの座標取得(X)
//=============================================================================
long GetGamePadLeftStickX(void)
{
	return g_joystickstate.lX;
}

//=============================================================================
// ゲームパッドの左スティックの座標取得(Y)
//=============================================================================
long GetGamePadLeftStickY(void)
{
	return g_joystickstate.lY;
}
//=============================================================================
// ゲームパッドのプレス状態を取得
//=============================================================================
bool GetGamePadPress(int key)
{
	return ((g_joystickstate.rgbButtons[key] & 0x80) != 0);
}
//=============================================================================
// ゲームパッドのトリガー状態を取得
//=============================================================================
bool GetGamePadTrigger(int key) {
	return ((g_joysticksTrigger.rgbButtons[key] & 0x80) != 0);
}