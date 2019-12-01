#include <stdio.h>
#include <Windows.h>

#include "glut.h"

#include "tex.h"

int texFromBMP(const char* _fileName, unsigned char* _colorKey)
{
	FILE *pFile;
	fopen_s(&pFile, _fileName, "rb");					//	ファイルを開く

	if (pFile == nullptr)								//	nullチェック
	{
		printf("%s open failed!\n", _fileName);
		return 1;
	}
	else
		printf("%s file Opend!\n", _fileName);

	BITMAPFILEHEADER bf;								//	ヘッダーのデータを格納する構造体を定義
	fread(&bf, sizeof(BITMAPFILEHEADER), 1, pFile);		//	読み込み
	//printf("bfSize : %d\n", bf);

	BITMAPINFOHEADER bi;								//	ヘッダーの情報を格納する構造体を定義
	fread(&bi, sizeof(BITMAPINFOHEADER), 1, pFile);		//	読み込み
	//printf("biWidth:%d biHeight:%d\n", bi.biWidth, bi.biHeight);
	//printf("biBitCout:%d\n", bi.biBitCount);

	typedef struct
	{
		unsigned char r, g, b, a;
	}RGBA;	//	32ビットなので

	RGBA *pixels = (RGBA*)malloc(sizeof RGBA * bi.biWidth * bi.biHeight);	//	ピクセルのメモリを確保
	fread(pixels, sizeof RGBA, bi.biWidth * bi.biHeight, pFile);			//	ピクセルデータの読み込み

	//	=== 色の補正 ===
	for (int y = 0; y < bi.biHeight; y++)
		for (int x = 0; x < bi.biWidth; x++)
		{
			RGBA *pPixel = &pixels[y * bi.biWidth + x];	//	入れ替えるピクセルのアドレス
			unsigned char temp = pPixel->r;				//	R(赤)情報を保存
			pPixel->r = pPixel->b;						//	R(赤)にB(青)情報をコピー
			pPixel->b = temp;							//	B(青)にR(赤)情報をコピー

			pPixel->a = (_colorKey != nullptr) &&		//	渡されたColorKeyあり、すべての色情報が一致していたら
				(pPixel->r == _colorKey[0]) &&
				(pPixel->g == _colorKey[1]) &&
				(pPixel->b == _colorKey[2])
				? 0x00									//	一致したら
				: 0xff;									//	一致していなかったら
		}
	//	================

	//	=== 上下の反転 ===
	for (int y = 0; y < bi.biHeight / 2; y++)	//	上半分だけ
		for (int x = 0; x < bi.biWidth; x++)
		{
			RGBA *pPixel0 = &pixels[y * bi.biWidth + x];					//	一番上の行 -> 下へ
			RGBA *pPixel1 = &pixels[(bi.biHeight - 1 - y)* bi.biWidth + x];	//	一番下の行 -> 上へ

			RGBA temp = *pPixel0;											//	スワップ処理
			*pPixel0 = *pPixel1;
			*pPixel1 = temp;

		}
	//	================


	glTexImage2D(					//	Vramへの転送する情報と設定
		GL_TEXTURE_2D,				//	ターゲット
		0,							//	mipmapのレベル
		GL_RGBA,					//	テクスチャのフォーマット
		bi.biWidth, bi.biHeight,	//	幅と高さ
		0,							//	ボーダー
		GL_RGBA,						//	ピクセルデータのフォーマット
		GL_UNSIGNED_BYTE,			//	ピクセルひとつずつの型(Char型は1バイトなのでバイト)
		pixels						//	作成したピクセルデータ	
	);

	glTexParameteri(				//	テクスチャのパラメータの設定
		GL_TEXTURE_2D,				//	ターゲット
		GL_TEXTURE_MAG_FILTER,		//	テクスチャの拡大時
		GL_NEAREST					//	補間設定(NEAREST : 補間をしない)
	);

	glTexParameteri(				//	テクスチャのパラメータの設定
		GL_TEXTURE_2D,				//	ターゲット
		GL_TEXTURE_MIN_FILTER,		//	テクスチャの縮小時
		GL_NEAREST					//	補間設定(NEAREST : 補間をしない)
	);

	free(pixels);										//	確保していたピクセルのメモリを解放
	fclose(pFile);										//	ファイルを閉じる

	return 0;
}
