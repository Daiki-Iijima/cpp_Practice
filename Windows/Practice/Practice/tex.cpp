#include <stdio.h>
#include <Windows.h>

#include "glut.h"

#include "tex.h"

int texFromBMP(const char* _fileName)
{
	FILE *pFile;
	fopen_s(&pFile, _fileName, "rb");					//	ファイルを開く

	if (pFile == nullptr)
	{
		printf("%s open failed!\n", _fileName);
		return 1;
	}
	else
	{
		printf("%s file Opend!\n", _fileName);
	}

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