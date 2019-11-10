#pragma once

//	わかりやすいようにfontを枕につける

void fontBegin();	//	フォント描画開始
void fontEnd();		//	フォント描画終了

void fontSetPosition(float _x, float _y);	//	フォント位置
void fontSetSize(float _size);				//	フォントサイズ
void fontSetColor(unsigned char _red, unsigned char _green, unsigned char _blue);	//フォントカラー
void fontDraw(const char _format, ...);	//	フォントを描画する