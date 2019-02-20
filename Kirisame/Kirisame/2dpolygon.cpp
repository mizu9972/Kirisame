#include "main.h"
#include "2dpolygon.h"
//�O���[�o���ϐ�
extern LPDIRECT3D9 g_pD3D;//main.cpp�Ő錾�����O���[�o���ϐ����g�p
extern LPDIRECT3DDEVICE9 g_pD3DDevice;

//���_���\����
typedef struct
{
	float x, y, z;//���_�̍��W
	float rhw;
	D3DCOLOR c;
	float U, V;
}VERTEX2D;


void Draw2dPolygon(float x, float y, float w, float h, D3DCOLOR c) {
	VERTEX2D v[4];//���_�f�[�^

	v[0].x = x;
	v[0].y = y;
	v[0].z = 0;
	v[0].rhw = 1.0f;
	v[0].c = c;

	v[1].x = x + w;
	v[1].y = y;
	v[1].z = 0;
	v[1].rhw = 1.0f;
	v[1].c = c;

	v[2].x = x;
	v[2].y = y + h;
	v[2].z = 0;
	v[2].rhw = 1.0f;
	v[2].c = c;

	v[3].x = x + w;
	v[3].y = y + h;
	v[3].z = 0;
	v[3].rhw = 1.0f;
	v[3].c = c;

	g_pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE);
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//�����x�𗘗p
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//�����������𗘗p
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(VERTEX2D));
}

void Draw2dPolygon(float x, float y, float w, float h,
	D3DCOLOR c, LPDIRECT3DTEXTURE9 *tex,
	float tu, float tv, float tw, float th)
{
	VERTEX2D v[4];//���_�f�[�^
	 /*����̒��_*/
	v[0].x = x;
	v[0].y = y;
	v[0].z = 0;
	v[0].rhw = 1.0f;
	v[0].c = c;//�F
	v[0].U = tu;
	v[0].V = tv;

	/*�E��̒��_*/
	v[1].x = x + w;
	v[1].y = y;
	v[1].z = 0;
	v[1].rhw = 1.0f;
	v[1].c = c;
	v[1].U = tu + tw;
	v[1].V = tv;

	/*�����̒��_*/
	v[2].x = x;
	v[2].y = y + h;
	v[2].z = 0;
	v[2].rhw = 1.0f;
	v[2].c = c;
	v[2].U = tu;
	v[2].V = tv + th;

	/*�E���̒��_*/
	v[3].x = x + w;
	v[3].y = y + h;
	v[3].z = 0;
	v[3].rhw = 1.0f;
	v[3].c = c;
	v[3].U = tu + tw;
	v[3].V = tv + th;

	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//�����x�𗘗p����ݒ�

	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);//�摜�̐F

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);//�w��F

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);//��L��ώZ

	g_pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE|D3DFVF_TEX1);

	g_pD3DDevice->SetTexture(0, *tex);

	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(VERTEX2D));

}




LPDIRECT3DTEXTURE9* LoadTexture(char* name, LPDIRECT3DTEXTURE9* tex)
{
	tex = (LPDIRECT3DTEXTURE9*)malloc(sizeof(LPDIRECT3DTEXTURE9));

	HRESULT hr;
	hr = D3DXCreateTextureFromFile(g_pD3DDevice, name, tex);//�e�N�X�`����ǂݍ���

	if (FAILED(hr))//�ǂݍ��ݎ��s�����Ƃ�
	{
		MessageBox(NULL, "Err", "�e�N�X�`���ǂݍ��݃G���[", MB_OK);
		return false;

	}
	return tex;
}

bool ReleaseTexture(LPDIRECT3DTEXTURE9* tex)
{
	if (tex != NULL)
	{
		(*tex)->Release();//�摜�����
		free(tex);//�������̉��
		tex = NULL;
	}
	return true;
}