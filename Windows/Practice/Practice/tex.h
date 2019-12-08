#pragma once

int texFromBMP(const char* _filename, unsigned char* _colorKey = nullptr);		//	.bmpを読み込んでテクスチャを生成する(カラーキーは生成してから渡す)
int texFromBMP(const char* _filename,											//	カラーキーを生成して、texFromBMPにカラーキーを渡す
	unsigned char _colorKeyR,
	unsigned char _colorKeyG,
	unsigned char _colorKeyB);		
