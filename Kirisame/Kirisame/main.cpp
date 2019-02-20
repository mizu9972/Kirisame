//=======================================================
// DX21
// DirectX���^����[main.cpp]
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
//�}�N����`
//*******************************************************
#define CLASS_NAME  "�T���v��"  //�E�C���h�E�N���X�̖��O
#define WINDOW_NAME "KIRISAME"  //�E�C���h�E�̖��O


//*******************************************************
//�v���g�^�C�v�錾
//*******************************************************
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow);
void Uninit(void);
void Update(void);
void Draw(void);

//*******************************************************
//�O���[�o���ϐ�
//*******************************************************
LPDIRECT3D9        g_pD3D = NULL;   // Direct3D�I�u�W�F�N�g
LPDIRECT3DDEVICE9 g_pD3DDevice = NULL;//Device�I�u�W�F�N�g(�`��ɕK�v)

SCENE Scene = TITLE;//��ԑJ�ڗp(�^�C�g������J�n)

int g_cnt = 0;//����Ƀ^�C�g���߂����

LPDIRECT3DTEXTURE9* Title_Texture = NULL;//�^�C�g��
LPDIRECT3DTEXTURE9* Tutorial_Texture = NULL;//�`���[�g���A��
LPDIRECT3DTEXTURE9* PadTutorial_Texture = NULL;
LPDIRECT3DTEXTURE9* Result_Texture = NULL;//���U���g
LPDIRECT3DTEXTURE9* Load_Texture = NULL;//���[�h��
Game *game = NULL;

extern LPDIRECTINPUTDEVICE8 g_pDIDevGamePad;
//=======================================================
//���C���֐�
// (WINAPI : Win32API�֐����Ăяo�����̋K��
// hInstance     ; ���̃A�v���P�[�V�����̃C���X�^���X�n���h��(���ʎq)
// hPrevInstance : ���NULL(16bit����̖��c��)
// lpCmdLine     : �R�}���h���C������󂯎���������ւ̃|�C���^
// nCmdShow      : �A�v���P�[�V�����̏����E�C���h�E�\�����@�̎w��
//=======================================================
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wcex =
	{
		sizeof(WNDCLASSEX),         //WNDCLASSEX�̃������T�C�Y���w��
		CS_CLASSDC,                 //�\������E�C���h�E�̃X�^�C����ݒ�
		WndProc,                    //�E�C���h�E�v���V�[�W���̃A�h���X(�֐���)���w��
		0,                          //�ʏ�͎g�p���Ȃ��̂�"0"���w��
		0,                          //�ʏ�͎g�p���Ȃ��̂�"0"���w��
		hInstance,                  //WinMain�̃p�����[�^�̃C���X�^���X�n���h����ݒ�
		NULL,                       //�g�p����A�C�R�����w��(Windows�������Ă���A�C�R�����g���Ȃ�NULL)
		LoadCursor(NULL, IDC_ARROW),//�}�E�X�J�[�\�����w��
		(HBRUSH)(COLOR_WINDOW + 1), //�E�C���h�E�̃N���C�A���g�̈�̔w�i�F��ݒ�
		NULL,                       //Window�ɂ��郁�j���[��ݒ�
		CLASS_NAME,                 //�E�C���h�E�N���X�̖��O
		NULL,                       //�������A�C�R�����ݒ肳�ꂽ�ꍇ�̏����L�q
	};
	HWND hWnd;
	MSG msg;

	//�E�C���h�E�N���X�̓o�^
	RegisterClassEx(&wcex);

	//�E�C���h�E���쐬
	hWnd = CreateWindowEx(0,//�g���E�C���h�E�X�^�C��
		CLASS_NAME,     //�E�C���h�E�N���X�̖��O
		WINDOW_NAME,    //�E�C���h�E�̖��O
		WS_OVERLAPPEDWINDOW, //�E�C���h�E�X�^�C��
		CW_USEDEFAULT,    //�E�C���h�E�̍���X���W
		CW_USEDEFAULT,    //�E�C���h�E�̍���Y���W
		(SCREEN_WIDTH+GetSystemMetrics(SM_CXDLGFRAME)*2),     //�E�C���h�E�̕�
		(SCREEN_HEIGHT + GetSystemMetrics(SM_CXDLGFRAME) * 2+
										GetSystemMetrics(SM_CYCAPTION)),    //�E�C���h�E�̍���
		NULL,             //�e�E�C���h�E�̃n���h��
		NULL,             //���j���[�n���h���܂��͎q�E�C���h�EID
		hInstance,        //�C���X�^���X�n���h��
		NULL);            //�E�C���h�E�쐬�f�[�^

	//����������(�E�C���h�E���쐬���Ă���s��)
	if (FAILED(Init(hInstance, hWnd, TRUE)))
	{
		return -1;
	}

	//�E�C���h�E�̕\��(�����������̌�ɍs��)
	ShowWindow(hWnd, nCmdShow); // �w�肳�ꂽ�E�C���h�E�̕\����Ԃ�ݒ�(�E�C���h�E��\��)
	UpdateWindow(hWnd);         //�E�C���h�E�̏�Ԃ𒼂��ɔ��f(�E�C���h�E�̃N���C�A���g�̈���X�V)

	DWORD OldTime = 0;
	DWORD NowTime = timeGetTime();
	//���b�Z�[�W���[�v
	//��1���� :���b�Z�[�W���
	//��2���� :�E�C���h�E�̃n���h��
	//��3���� :�ŏ��̃��b�Z�[�W
	//��4���� :�Ō�̃��b�Z�[�W
	while(1)
	{ 
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE) != 0)//���b�Z�[�W���擾���Ȃ������ꍇ"0"��Ԃ�
		{//Windows�̏���
			if (msg.message == WM_QUIT)
			{// PostQuitMessage()���Ă΂�āAWM_QUIT���b�Z�[�W�������烋�[�v�I��
				break;
			}
			else
			{
				//���b�Z�[�W�̖|��Ƒ��o
				TranslateMessage(&msg);       //���z�L�[���b�Z�[�W�𕶎����b�Z�[�W�֕ϊ�
				DispatchMessage(&msg);        //�E�C���h�E�v���V�[�W���փ��b�Z�[�W�𑗏o
			}
		}
		else
		{//DirectX�̏���
			if (NowTime - OldTime > 1000 / FPS ) {
				// �X�V����
				Update();

				// �`�揈��
				Draw();

				OldTime = NowTime;
			}
			NowTime = timeGetTime();
		}
	}

	// �E�C���h�E�N���X�̓o�^������
	// ��1���� : �N���X��
	// ��2���� : �A�v���P�[�V�����C���X�^���X�̃n���h��
	UnregisterClass(CLASS_NAME, wcex.hInstance);
	//�I������
	Uninit();

	return (int)msg.wParam;
}

//=============================================================
// �E�C���h�E�v���V�[�W��
// (CALLBACK : Win32API�֐����Ăяo���Ƃ��̋K��)
// hWnd     :�E�C���h�E�̃n���h��
// uMsg     :���b�Z�[�W�̎��ʎq
// wParam   :���b�Z�[�W�̍ŏ��̃p�����[�^
// lParam   :���b�Z�[�W��2�Ԗڂ̃p�����[�^
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
			nID = MessageBox(hWnd, "�I�����܂����H", "�I��", MB_YESNO);
			if (nID == IDYES)
			{
				DestroyWindow(hWnd);
			}
			break;
		}
		break;

	case WM_CLOSE:
		nID = MessageBox(hWnd, "�I�����܂����H", "�I��", MB_YESNO);
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

	return DefWindowProc(hWnd, uMsg, wParam, lParam);   //����̏������
}

//==================================================================
// ����������
// hInstance : �C���X�^���X�̃n���h��
// hWnd      : �E�C���h�E�̃n���h��
// bWindow   : �E�C���h�E���[�h�ɂ��邩�ǂ���
//==================================================================
HRESULT Init(HINSTANCE hInstance, HWND hWnd, BOOL bWindow)
{
	D3DPRESENT_PARAMETERS d3dpp;
	D3DDISPLAYMODE d3ddm;

	// Direct3D�I�u�W�F�N�g�̐���
	g_pD3D = Direct3DCreate9(D3D_SDK_VERSION);
	if (g_pD3D == NULL)
	{
		return E_FAIL;
	}

	//���݂̃f�B�X�v���C���[�h���擾
	if (FAILED(g_pD3D->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &d3ddm)))
	{
		return E_FAIL;
	}

	//�f�o�C�X�̃v���[���e�[�V�����p�����[�^�̐ݒ�
	ZeroMemory(&d3dpp, sizeof(d3dpp));               //���[�N���[���N���A
	d3dpp.BackBufferWidth = SCREEN_WIDTH;            //�Q�[����ʃT�C�Y(��)
	d3dpp.BackBufferHeight = SCREEN_HEIGHT;          //�Q�[����ʃT�C�Y(����)
	d3dpp.BackBufferFormat = d3ddm.Format;           //�J���[���[�h�̎w��
	d3dpp.BackBufferCount = 1;                       //�o�b�N�o�b�t�@�̐�
	d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;        //�f���M���ɓ������ăt���b�v����
	d3dpp.Windowed = bWindow;                        //�E�C���h�E���[�h
	d3dpp.EnableAutoDepthStencil = TRUE;             //�f�v�X�o�b�t�@(Z�o�b�t�@)�ƃX�e���V���o�b�t�@���쐬
	d3dpp.AutoDepthStencilFormat = D3DFMT_D16;       //�f�v�X�o�b�t�@�Ƃ���16bit���g��

	if (bWindow)
	{//�E�C���h�E���[�h
		d3dpp.FullScreen_RefreshRateInHz = 0;    //���t���b�V�����[�g(�w��ł��Ȃ��̂�0�Œ�)
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_IMMEDIATE;
		//�C���^�[�o��(VSync��҂����ɕ`��)
	}
	else
	{//�t���X�N���[�����[�h
		d3dpp.FullScreen_RefreshRateInHz = 0;  //���t���b�V�����[�g(�w��ł��Ȃ��̂�0�Œ�)
		d3dpp.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
		//�C���^�[�o��(VSync��҂��ĕ`��)
	}

	//�f�o�C�X�I�u�W�F�N�g�̐���
	//[�f�o�C�X�쐬����]<�`��>��<���_����>���n�[�h�E�F�A�ōs��
	if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,       //�f�B�X�v���C�A�_�v�^	
		D3DDEVTYPE_HAL,                          //�f�o�C�X�^�C�v
		hWnd,                                    //�t�H�[�J�X����E�C���h�E�ւ̃n���h��
		D3DCREATE_HARDWARE_VERTEXPROCESSING,     //�f�o�C�X�쐬����̑g�ݍ��킹
		&d3dpp,                                  //�f�o�C�X�̃v���[���e�[�V�����p�����[�^
		&g_pD3DDevice)))                         //�f�o�C�X�C���^�[�t�F�[�X�ւ̃|�C���^
	{
		//[�f�o�C�X�쐬����]<�`��>���n�[�h�E�F�A�ōs���A<���_����>��CPU�ōs��
		if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
			D3DDEVTYPE_HAL,
			hWnd,
			D3DCREATE_SOFTWARE_VERTEXPROCESSING,
			&d3dpp,
			&g_pD3DDevice)))
		{
			//��L�̐ݒ肪���s������
			//[�f�o�C�X�쐬����]<�`��>�ƒ��_������CPU�ōs��
			if (FAILED(g_pD3D->CreateDevice(D3DADAPTER_DEFAULT,
				D3DDEVTYPE_REF,
				hWnd,
				D3DCREATE_SOFTWARE_VERTEXPROCESSING,
				&d3dpp,
				&g_pD3DDevice)))
			{
				//���������s
				return E_FAIL;
			}
		}
	}
	InitInput(hInstance, hWnd);//���͏��̏�����
	InitSound();
	
	Title_Texture = LoadTexture(TITLE_FILE, Title_Texture);//�^�C�g���̃e�N�X�`���ǂݍ���
	Tutorial_Texture = LoadTexture(Tutorial_FILE, Tutorial_Texture);//�`���[�g���A���̃e�N�X�`���ǂݍ���
	PadTutorial_Texture = LoadTexture(PadTutorial_FILE, PadTutorial_Texture);//�Q�[���p�b�h�̃`���[�g���A���̃e�N�X�`���ǂݍ���
	Result_Texture = LoadTexture(RESULT_FILE, Result_Texture);//���U���g�̃e�N�X�`���ǂݍ���
	Load_Texture = LoadTexture(LOAD_FILE, Load_Texture);//���[�h��ʂ̃e�N�X�`���ǂݍ���

	game = new Game;

	game->Init();

	return S_OK;
}

//==================================================
//�@�I������
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
	{// �f�o�C�X�̊J��
		g_pD3DDevice->Release();
		g_pD3DDevice = NULL;
	}

	if (g_pD3D != NULL)
	{// Direct3D�I�u�W�F�N�g�̊J��
		g_pD3D->Release();
		g_pD3D = NULL;
	}
}


//===================================================
// �X�V����
//===================================================
void Update(void)
{
	bool ENTER_TRIG = false;//���z�L�[
	UpdateInput();//�L�[���͂̍X�V

	if (GetKeyboardTrigger(DIK_RETURN)) {
		//����L�[�ɑΉ�����{�^���E�L�[�������ꂽ��
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

		//�^�C�g����ʂ̑��쏈��
		if (ENTER_TRIG)
		{
			Scene = TUTORIAL;
		}
		break;


	case TUTORIAL:

		//�^�C�g����ʂ̑��쏈��
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
				StopSound(TitleBGM);//�^�C�g��BGM��~
			}
			PlaySound(GamePlayBGM);//�Q�[��BGM�Đ�
		}
		break;

	case GAME_STAGE1:
		if (game != NULL) {
			game->Update();
		}
		//�Q�[����CLEAR�������L��
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
		//���U���g��ʂ̑��쏈��
		g_cnt++;
		if (ENTER_TRIG || g_cnt >= SCENE_TIME)
		{
			Scene = LOAD;
			g_cnt = 0;
		}
		break;

	case LOAD://���[�h��(���̊ԂɃQ�[���N���X��delete��new���s��)
		if (game != NULL)
		{
			if (PollSound(GamePlayBGM)) {//�Q�[��BGM��~
				StopSound(GamePlayBGM);
			}
			delete game;
			game = new Game;//�����Ɏ��Ԃ�����
			game->Init();
			Scene = TITLE;
		}
		break;
	}

}

//===================================================
// �`�揈��
//===================================================
void Draw(void)
{
	// �o�b�N�o�b�t�@&Z�o�b�t�@�̃N���A
	// Count   : pRects���ɂ����`�̐�
	// pRects  : �N���A�����`�̔z��(NULL�w��Ńr���[�|�[�g��`�S�̂��N���A)
	// Flags   : �N���A����T�[�t�F�X�������t���O(NULL�w��Ńr���[�|�[�g��`�S�̂��N���A)
	//         : [D3DCLEAR_TARGET - �����_�����O�^�[�Q�b�g���N���A����Color�p�����[�^�̒l�ɂ���]
	//         : [D3DCLEAR_ZBUFFER - �[�x(Z)�o�b�t�@���N���A����Z�p�����[�^�̒l�ɂ���]
	//         : [D3DCLEAR_STENCIL - �X�e���V���o�b�t�@���N���A����Stencil�p�����[�^�̒l�ɂ���]
	// Color   : �T�[�t�F�X���N���A����F
	// Z       : �f�v�X�o�b�t�@�ɕۑ�����l
	// Stencil : �X�e���V���o�b�t�@�ɕۑ�����l(����)
	g_pD3DDevice->Clear(0, NULL, (D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER), D3DCOLOR_RGBA(255,255,255,255), 1.0f, 0);

	// Direct3D�ɂ��`��̊J�n
	if (SUCCEEDED(g_pD3DDevice->BeginScene()))
	{
		//��̓I�ȕ`�揈�����R�R�ɋL�q���Ă���
		switch (Scene)//��ԑJ��
		{
		case TITLE:
			//�^�C�g����ʂ̕`�揈��
			Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), Title_Texture, 0, 0, 1, 1);
			break;

		case TUTORIAL:
			//�`���[�g���A����ʂ̕`�揈��
			if (g_pDIDevGamePad) {
				Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), PadTutorial_Texture, 0, 0, 1, 1);
			}
			else {
				Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), Tutorial_Texture, 0, 0, 1, 1);
			}
			break;

		case GAME_STAGE1:
		case GAME_STAGE2:
			//�Q�[����ʂ̕`�揈��
			if (game != NULL) {
				game->Draw();
			}
			break;

		case RESULT:
			//���U���g�̕`�揈��
			Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), Result_Texture, 0, 0, 1, 1);
			break;

		case LOAD:
			//���[�h��
			Draw2dPolygon(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, D3DCOLOR_ARGB(255, 255, 255, 255), Load_Texture, 0, 0, 1, 1);
			break;
		}


		//Direct3D�ɂ��`��̏I��
		g_pD3DDevice->EndScene();
	}

	// �o�b�N�o�b�t�@�ƃt�����g�o�b�t�@�̓���ւ�
	// pSourceRect           : �]������`
	// pDestRect             : �]�����`
	// hDestWindowOverride   : �]����E�C���h�E�ւ̃|�C���^
	// pDirtyRegion          : �ʏ��NULL��
	g_pD3DDevice->Present(NULL, NULL, NULL, NULL);
}