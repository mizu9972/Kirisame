#ifndef POLYGON_H //�萔����`����Ă��Ȃ����
#define POLYGON_H //�萔���`(���̃w�b�_�[�t�@�C�����ǂݍ��܂ꂽ����)
#include <windows.h>
#include <d3d9.h>

//�v���g�^�C�v�錾
void Draw2dPolygon(float x, float y, float w, float h, D3DCOLOR c);

void Draw2dPolygon(float x, float y,  float w, float h, 
					D3DCOLOR c,LPDIRECT3DTEXTURE9 *tex,
					float tu,float tv,float tw,float th);



LPDIRECT3DTEXTURE9* LoadTexture(char*, LPDIRECT3DTEXTURE9*);

bool ReleaseTexture(LPDIRECT3DTEXTURE9*);


#endif