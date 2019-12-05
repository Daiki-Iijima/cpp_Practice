#pragma once

//	- わかりやすいようにfontを枕につける -

#define FONT_DEFAULT_HEIGHT (119.05f)					//	フォント高さ(OpenGLのリファレンスによる)
#define FONT_DEFAULT_LINE_SPACE (33.33)					//	フォントの行間

enum {												//	フォント
	FONT_FONT_ROMAN,
	FONT_FONT_MONO_ROMAN,
	FONT_FONT_MAX,
};

void fontBegin();									//	フォント描画開始
void fontEnd();										//	フォント描画終了

void fontFont(int _font);							//	フォントを設定

void fontScreenSize(float _width, float _height);	//	画面サイズを設定

void fontPosition(float _x, float _y);				//	フォント位置
void fontHeight(float _size);						//	フォントの高さを設定

float fontGetWidth(int _character);					//	フォントの幅を取得
float fontGetLength(const unsigned char* _string);	//	文字列の幅を取得
float fontGetHeight();								//	フォントの高さを取得

float fontGetLineHeight();							//	行の高さを取得

float fontGetWeightMin();							//	設定できるフォントの一番細い太さ
float fontGetWeightMax();							//	設定できるフォントの一番太い太さ

void fontWeight(float _weight);						//	フォントの太さを変更
float fontGetWeight();								//	フォントの太さを取得

void fontDraw(const char *_format, ...);			//	フォントを描画する