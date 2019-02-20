//=============================================================================
//
// 入力処理 [input.h]
//
//=============================================================================
#ifndef _INPUT_H_
#define _INPUT_H_

#define DIRECTINPUT_VERSION 0x0800

#include <dinput.h>

#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")


//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
HRESULT InitInput(HINSTANCE hInstance, HWND hWnd);
void UninitInput(void);
void UpdateInput(void);

bool GetKeyboardPress(int key);
bool GetKeyboardTrigger(int key);
bool GetKeyboardRepeat(int key);
bool GetKeyboardRelease(int key);

bool GetMouseLeftPress(void);
bool GetMouseLeftTrigger(void);
bool GetMouseRightPress(void);
bool GetMouseRightTrigger(void);
bool GetMouseCenterPress(void);
bool GetMouseCenterTrigger(void);
long GetMouseAxisX(void);
long GetMouseAxisY(void);
long GetMouseAxisZ(void);
long GetMouseX(void);
long GetMouseY(void);

long GetGamePadRightStickX(void);
long GetGamePadRightStickY(void);
long GetGamePadLeftStickX(void);
long GetGamePadLeftStickY(void);
bool GetGamePadPress(int key);
bool GetGamePadTrigger(int key);
//*****************************************************************************
//グローバル変数定義
//*****************************************************************************
extern long JoypadDI_X;
extern long JoypadDI_Y;

#endif