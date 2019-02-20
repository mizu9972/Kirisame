//=============================================================================
//
// ���͏��� [input.cpp]
//
//=============================================================================
#include "input.h"


//*****************************************************************************
// �萔��`
//*****************************************************************************
const int NUM_KEY_MAX = 256;

//*****************************************************************************
// �v���g�^�C�v�錾
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
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECTINPUT8			g_pDInput = NULL;					// IDirectInput8�C���^�[�t�F�[�X�ւ̃|�C���^
LPDIRECTINPUTDEVICE8	g_pDIDevKeyboard = NULL;			// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�L�[�{�[�h)
LPDIRECTINPUTDEVICE8    g_pDIDevGamePad = NULL;				// IDirectInputDevuce8�C���^�[�t�F�[�X�ւ̃|�C���^(�Q�[���p�b�h)
LPDIRECTINPUTDEVICE8	g_pDIDevMouse = NULL;				// IDirectInputDevice8�C���^�[�t�F�[�X�ւ̃|�C���^(�}�E�X)
BYTE					g_keyState[NUM_KEY_MAX];			// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateTrigger[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateRepeat[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
BYTE					g_keyStateRelease[NUM_KEY_MAX];		// �L�[�{�[�h�̏�Ԃ��󂯎�郏�[�N
int						g_keyStateRepeatCnt[NUM_KEY_MAX];	// �L�[�{�[�h�̃��s�[�g�J�E���^
DIMOUSESTATE2			g_mouseState;						// �}�E�X�̏�Ԃ��󂯎�郏�[�N
DIMOUSESTATE2			g_mouseStateTrigger;				// �}�E�X�̏�Ԃ��󂯎�郏�[�N
POINT					g_MousePoint;						// �}�E�X�̌��ݍ��W
DIJOYSTATE              g_joystickstate;                    // �Q�[���p�b�h�̏�Ԃ��󂯎�郏�[�N
DIJOYSTATE				g_joysticksTrigger;					// �Q�[���p�b�h�̏�Ԃ��󂯎�郏�[�N
HWND					g_hwnd;

DIDEVCAPS g_diDevCaps;

//�����\���L�[�̓��͂��v���O�����N���㏟��ɓ��͂���Ă����ԂȂ�
//���̐��l��ۑ����A�����Œ�������
//���̒����ɗ��p����p�̕ϐ�
long JoypadDI_X;
long JoypadDI_Y;
//=============================================================================
// ���͏����̏�����
//=============================================================================
HRESULT InitInput(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	if(!g_pDInput)
	{
		// DirectInput�I�u�W�F�N�g�̍쐬
		hr = DirectInput8Create(hInst, DIRECTINPUT_VERSION,
									IID_IDirectInput8, (void**)&g_pDInput, NULL);
	}

	// �L�[�{�[�h�̏�����
	InitKeyboard(hInst, hWnd);

	// �}�E�X�̏�����
	InitMouse(hInst, hWnd);

	// �Q�[���p�b�h�̏�����
	InitGamePad(hInst, hWnd);

	g_hwnd = hWnd;

	return hr;
}

//=============================================================================
// ���͏����̏I������
//=============================================================================
void UninitInput(void)
{
	// �L�[�{�[�h�̏I������
	UninitKeyboard();

	// �}�E�X�̏I������
	UninitMouse();

	//�Q�[���p�b�h�̏I������
	UnInitGamePad();

	if(g_pDInput)
	{
		g_pDInput->Release();
		g_pDInput = NULL;
	}
}

//=============================================================================
// ���͏����̍X�V����
//=============================================================================
void UpdateInput(void)
{
	// �L�[�{�[�h�̍X�V
	UpdateKeyboard();

	// �}�E�X�̍X�V
	UpdateMouse();

	//�Q�[���p�b�h�̍X�V
	UpdateGamePad();
}

//=============================================================================
// �L�[�{�[�h�̏�����
//=============================================================================
HRESULT InitKeyboard(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = g_pDInput->CreateDevice(GUID_SysKeyboard, &g_pDIDevKeyboard, NULL);
	if(FAILED(hr) || g_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd, "�L�[�{�[�h���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = g_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_pDIDevKeyboard->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�L�[�{�[�h�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDIDevKeyboard->Acquire();

	return S_OK;
}

//=============================================================================
// �L�[�{�[�h�̏I������
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
// �L�[�{�[�h�̍X�V����
//=============================================================================
HRESULT UpdateKeyboard(void)
{
	HRESULT hr;
	BYTE keyStateOld[256];

	// �O��̃f�[�^��ۑ�
	memcpy(keyStateOld, g_keyState, NUM_KEY_MAX);

	// �f�o�C�X����f�[�^���擾
	hr = g_pDIDevKeyboard->GetDeviceState(sizeof(g_keyState), g_keyState);
	if(SUCCEEDED(hr))
	{
		for(int cnt = 0; cnt < NUM_KEY_MAX; cnt++)
		{
			// �g���K�[�ƃ����[�X���擾
			g_keyStateTrigger[cnt] = ((keyStateOld[cnt] ^ g_keyState[cnt]) & g_keyState[cnt]);
			//g_keyStateRelease[cnt] = 
			
			// �L�[��������Ă���Ȃ烊�s�[�g�̔��菈��
			if(g_keyState[cnt])
			{
				if(g_keyStateRepeatCnt[cnt] < 20)
				{
					g_keyStateRepeatCnt[cnt]++;
					// �u������́v�������́u�{�^���������Ă���20�t���[���o�߁v
					// �����ꍇ�A���s�[�g�p�z��̃f�[�^��L���ɂ���
					//if(  ||  )
					// ��L�����ȊO�̓��s�[�g�p�z��̃f�[�^�𖳌��ɂ���
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
		// �L�[�{�[�h�ւ̃A�N�Z�X�����擾
		hr = g_pDIDevKeyboard->Acquire();
	}

	return hr;
}

//=============================================================================
// �L�[�{�[�h�̃v���X��Ԃ��擾
//=============================================================================
bool GetKeyboardPress(int key)
{
	return ((g_keyState[key] & 0x80) != 0);
}

//=============================================================================
// �L�[�{�[�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool GetKeyboardTrigger(int key)
{
	return ((g_keyStateTrigger[key] & 0x80) != 0);
}

//=============================================================================
// �L�[�{�[�h�̃��s�[�g��Ԃ��擾
//=============================================================================
bool GetKeyboardRepeat(int key)
{
	return ((g_keyStateRepeat[key] & 0x80) != 0);
}

//=============================================================================
// �L�[�{�[�h�̃����|�X��Ԃ��擾
//=============================================================================
bool GetKeyboardRelease(int key)
{
	return ((g_keyStateRelease[key] & 0x80) != 0);
}

//=============================================================================
// �}�E�X�̏�����
//=============================================================================
HRESULT InitMouse(HINSTANCE hInst, HWND hWnd)
{
	HRESULT hr;

	// �f�o�C�X�I�u�W�F�N�g���쐬
	hr = g_pDInput->CreateDevice(GUID_SysMouse, &g_pDIDevMouse, NULL);
	if(FAILED(hr) || g_pDIDevMouse == NULL)
	{
		MessageBox(hWnd, "�}�E�X���˂��I", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�[�^�t�H�[�}�b�g��ݒ�
	hr = g_pDIDevMouse->SetDataFormat(&c_dfDIMouse2);
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�E�X�̃f�[�^�t�H�[�}�b�g��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �������[�h��ݒ�i�t�H�A�O���E���h����r�����[�h�j
	hr = g_pDIDevMouse->SetCooperativeLevel(hWnd, (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE));
	if(FAILED(hr))
	{
		MessageBox(hWnd, "�}�E�X�̋������[�h��ݒ�ł��܂���ł����B", "�x���I", MB_ICONWARNING);
		return hr;
	}

	// �f�o�C�X�̐ݒ�
	{
		DIPROPDWORD dipdw;

		dipdw.diph.dwSize = sizeof(dipdw);
		dipdw.diph.dwHeaderSize = sizeof(dipdw.diph);
		dipdw.diph.dwObj = 0;
		dipdw.diph.dwHow = DIPH_DEVICE;
		dipdw.dwData = DIPROPAXISMODE_REL;

		hr = g_pDIDevMouse->SetProperty(DIPROP_AXISMODE, &dipdw.diph);
	}

	// �L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	g_pDIDevMouse->Acquire();

	return S_OK;
}

//=============================================================================
// �}�E�X�̏I������
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
// �}�E�X�̍X�V����
//=============================================================================
HRESULT UpdateMouse(void)
{
	HRESULT hr;
	DIMOUSESTATE2 mouseStateOld;

	// �O��̃f�[�^��ۑ�
	mouseStateOld = g_mouseState;
		
	GetCursorPos(&g_MousePoint);
	ScreenToClient(g_hwnd, &g_MousePoint);


	// �f�o�C�X����f�[�^���擾
	hr = g_pDIDevMouse->GetDeviceState(sizeof(g_mouseState), &g_mouseState);
	if(SUCCEEDED(hr))
	{
		// �g���K�[�̎擾
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
// �}�E�X�f�[�^�擾(���v���X)
//=============================================================================
bool GetMouseLeftPress(void)
{
	return ((g_mouseState.rgbButtons[0] & 0x80) != 0);
}

//=============================================================================
// �}�E�X�f�[�^�擾(���g���K�[)
//=============================================================================
bool GetMouseLeftTrigger(void)
{
	return ((g_mouseStateTrigger.rgbButtons[0] & 0x80) != 0);
}

//=============================================================================
// �}�E�X�f�[�^�擾(�E�v���X)
//=============================================================================
bool GetMouseRightPress(void)
{
	return ((g_mouseState.rgbButtons[1] & 0x8) != 00);
}

//=============================================================================
// �}�E�X�f�[�^�擾(�E�g���K�[)
//=============================================================================
bool GetMouseRightTrigger(void)
{
	return ((g_mouseStateTrigger.rgbButtons[1] & 0x80) != 0);
}

//=============================================================================
// �}�E�X�f�[�^�擾(�����v���X)
//=============================================================================
bool GetMouseCenterPress(void)
{
	return ((g_mouseState.rgbButtons[2] & 0x80) != 0);
}

//=============================================================================
// �}�E�X�f�[�^�擾(�����g���K�[)
//=============================================================================
bool GetMouseCenterTrigger(void)
{
	return ((g_mouseState.rgbButtons[2] & 0x80) != 0);
}

//=============================================================================
// �}�E�X�f�[�^�擾(�w���ړ�)
//=============================================================================
long GetMouseAxisX(void)
{
	return g_mouseState.lX;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�x���ړ�)
//=============================================================================
long GetMouseAxisY(void)
{
	return g_mouseState.lY;
}

//=============================================================================
// �}�E�X�f�[�^�擾(�y���ړ�)
//=============================================================================
long GetMouseAxisZ(void)
{
	return g_mouseState.lZ;
}
//=============================================================================
// �}�E�X���W�擾(X)
//=============================================================================
long GetMouseX(void)
{
	return g_MousePoint.x;
}

//=============================================================================
// �}�E�X���W�擾(Y)
//=============================================================================
long GetMouseY(void)
{
	return g_MousePoint.y;
}

//=============================================================================
//�Q�[���p�b�h�̏�����
//=============================================================================
HRESULT InitGamePad(HINSTANCE hInst, HWND hWnd) {
	HRESULT hr;

	hr = g_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL, EnumJoysticksCallback, NULL, DIEDFL_ATTACHEDONLY);
	if (FAILED(hr) || g_pDIDevGamePad == NULL) {
		//MessageBox(hWnd, "�f�o�C�X�쐬�Ɏ��s���܂���", "Error", MB_OK);//�Q�[���p�b�h���ڑ�����Ă��Ȃ����͂����Ōx��
		return FALSE;
	}

	hr = g_pDIDevGamePad->SetDataFormat(&c_dfDIJoystick);
	if (FAILED(hr)) {
		MessageBox(hWnd, "�f�[�^�t�H�[�}�b�g�Ɏ��s���܂���", "Error", MB_OK);
		return FALSE;
	}

	hr = g_pDIDevGamePad->SetCooperativeLevel(hWnd, DISCL_EXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)) {
		MessageBox(hWnd, "cooperative level�Ɏ��s���܂���", "Error", MB_OK);
		return FALSE;
	}

	g_diDevCaps.dwSize = sizeof(DIDEVCAPS);
	hr = g_pDIDevGamePad->GetCapabilities(&g_diDevCaps);
	if (FAILED(hr)) {
		MessageBox(hWnd, "device capabilities�Ɏ��s���܂���", "Error", MB_OK);
		return FALSE;
	}

	hr = g_pDIDevGamePad->EnumObjects(EnumAxesCallback, (VOID*)hWnd, DIDFT_AXIS);
	if (FAILED(hr)) {
		MessageBox(hWnd, "�v���p�e�B�̃Z�b�g�Ɏ��s���܂���", "Error", MB_OK);
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
// �Q�[���p�b�h�̏I������
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
//�Q�[���p�b�h�̃R�[���o�b�N�֐�
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
//�Q�[���p�b�h�̓��͎󂯎��֐�
//=============================================================================
BOOL UpdateGamePad(void) {
	HRESULT hr;
	DIJOYSTATE OldJoyState;

	OldJoyState = g_joystickstate;
	//DIMOUSESTATE2 mouseStateOld;

	//// �O��̃f�[�^��ۑ�
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

	//���͂��󂯎�����Ƃ��̏���------------------
	//�e���v���[�g------------------------------
	/*if (g_joystickstate.lX == -1000) {//��
	//����
	}
	if (g_joystickstate.lX == 1000) {//�E
	//����
	}
	if (g_joystickstate.lY == -1000) {//��
	//����
	}
	if (g_joystickstate.lY == 1000) {//��
	//����
	}
	if (g_joystickstate.rgbButtons[0] & 0x80) {//�{�^���P
	//����
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
// �Q�[���p�b�h�̉E�X�e�B�b�N�̍��W�擾(X)
//=============================================================================
long GetGamePadRightStickX(void)
{
	return g_joystickstate.lZ;
}

//=============================================================================
// �Q�[���p�b�h�̉E�X�e�B�b�N�̍��W�擾(Y)
//=============================================================================
long GetGamePadRightStickY(void)
{
	return g_joystickstate.lRz;
}
//=============================================================================
// �Q�[���p�b�h�̍��X�e�B�b�N�̍��W�擾(X)
//=============================================================================
long GetGamePadLeftStickX(void)
{
	return g_joystickstate.lX;
}

//=============================================================================
// �Q�[���p�b�h�̍��X�e�B�b�N�̍��W�擾(Y)
//=============================================================================
long GetGamePadLeftStickY(void)
{
	return g_joystickstate.lY;
}
//=============================================================================
// �Q�[���p�b�h�̃v���X��Ԃ��擾
//=============================================================================
bool GetGamePadPress(int key)
{
	return ((g_joystickstate.rgbButtons[key] & 0x80) != 0);
}
//=============================================================================
// �Q�[���p�b�h�̃g���K�[��Ԃ��擾
//=============================================================================
bool GetGamePadTrigger(int key) {
	return ((g_joysticksTrigger.rgbButtons[key] & 0x80) != 0);
}