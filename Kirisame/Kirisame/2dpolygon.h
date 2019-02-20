#ifndef POLYGON_H //定数が定義されていなければ
#define POLYGON_H //定数を定義(このヘッダーファイルが読み込まれた時に)
#include <windows.h>
#include <d3d9.h>

//プロトタイプ宣言
void Draw2dPolygon(float x, float y, float w, float h, D3DCOLOR c);

void Draw2dPolygon(float x, float y,  float w, float h, 
					D3DCOLOR c,LPDIRECT3DTEXTURE9 *tex,
					float tu,float tv,float tw,float th);



LPDIRECT3DTEXTURE9* LoadTexture(char*, LPDIRECT3DTEXTURE9*);

bool ReleaseTexture(LPDIRECT3DTEXTURE9*);


#endif