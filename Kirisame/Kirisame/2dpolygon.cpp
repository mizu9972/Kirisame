#include "main.h"
#include "2dpolygon.h"
//グローバル変数
extern LPDIRECT3D9 g_pD3D;//main.cppで宣言したグローバル変数を使用
extern LPDIRECT3DDEVICE9 g_pD3DDevice;

//頂点情報構造体
typedef struct
{
	float x, y, z;//頂点の座標
	float rhw;
	D3DCOLOR c;
	float U, V;
}VERTEX2D;


void Draw2dPolygon(float x, float y, float w, float h, D3DCOLOR c) {
	VERTEX2D v[4];//頂点データ

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
	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, true);//透明度を利用
	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);//半透明処理を利用
	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	g_pD3DDevice->SetTexture(0, NULL);
	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(VERTEX2D));
}

void Draw2dPolygon(float x, float y, float w, float h,
	D3DCOLOR c, LPDIRECT3DTEXTURE9 *tex,
	float tu, float tv, float tw, float th)
{
	VERTEX2D v[4];//頂点データ
	 /*左上の頂点*/
	v[0].x = x;
	v[0].y = y;
	v[0].z = 0;
	v[0].rhw = 1.0f;
	v[0].c = c;//色
	v[0].U = tu;
	v[0].V = tv;

	/*右上の頂点*/
	v[1].x = x + w;
	v[1].y = y;
	v[1].z = 0;
	v[1].rhw = 1.0f;
	v[1].c = c;
	v[1].U = tu + tw;
	v[1].V = tv;

	/*左下の頂点*/
	v[2].x = x;
	v[2].y = y + h;
	v[2].z = 0;
	v[2].rhw = 1.0f;
	v[2].c = c;
	v[2].U = tu;
	v[2].V = tv + th;

	/*右下の頂点*/
	v[3].x = x + w;
	v[3].y = y + h;
	v[3].z = 0;
	v[3].rhw = 1.0f;
	v[3].c = c;
	v[3].U = tu + tw;
	v[3].V = tv + th;

	g_pD3DDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);//透明度を利用する設定

	g_pD3DDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	g_pD3DDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);//画像の色

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);//指定色

	g_pD3DDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);//上記を積算

	g_pD3DDevice->SetFVF(D3DFVF_XYZRHW | D3DFVF_DIFFUSE|D3DFVF_TEX1);

	g_pD3DDevice->SetTexture(0, *tex);

	g_pD3DDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, v, sizeof(VERTEX2D));

}




LPDIRECT3DTEXTURE9* LoadTexture(char* name, LPDIRECT3DTEXTURE9* tex)
{
	tex = (LPDIRECT3DTEXTURE9*)malloc(sizeof(LPDIRECT3DTEXTURE9));

	HRESULT hr;
	hr = D3DXCreateTextureFromFile(g_pD3DDevice, name, tex);//テクスチャを読み込み

	if (FAILED(hr))//読み込み失敗したとき
	{
		MessageBox(NULL, "Err", "テクスチャ読み込みエラー", MB_OK);
		return false;

	}
	return tex;
}

bool ReleaseTexture(LPDIRECT3DTEXTURE9* tex)
{
	if (tex != NULL)
	{
		(*tex)->Release();//画像を解放
		free(tex);//メモリの解放
		tex = NULL;
	}
	return true;
}