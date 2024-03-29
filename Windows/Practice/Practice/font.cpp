#include <stdio.h>
#include <stdarg.h>

#include "font.h"

#include "glm/glm.hpp"
#include "glut.h"

using namespace glm;

static vec2 screenSize = { 256, 256 };

static void* font = GLUT_STROKE_ROMAN;	//	フォント
static vec2 position;
static vec2 origin;
static float height = FONT_DEFAULT_HEIGHT;
static float weight = 1;

void fontBegin()
{
	glPushMatrix();						//	現在設定されている行列情報を保持
	glPushAttrib(GL_ALL_ATTRIB_BITS);	//	現在設定されている描画属性を保持

	glMatrixMode(GL_PROJECTION);		//	射影モードを変更する
	glLoadIdentity();					//	前回の射影行列が残らないように行列の初期化

	gluOrtho2D(							//	2次元空間を定義(Ortho:正射影)
		0, screenSize.x,					//	left,right
		screenSize.y, 0					//	bottom,top
	);

	glMatrixMode(GL_MODELVIEW);		//	モデルビュー行列モードに切り替え
	glLoadIdentity();				//	前回の射影行列が残らないように行列の初期化


	//	各種設定を無効にする
	glDisable(GL_DEPTH_TEST);		//	深度テスト
	glDisable(GL_LIGHTING);			//	ライティング
	glDisable(GL_TEXTURE_2D);		//	2Dテクスチャ
}

void fontEnd()
{
	glPopMatrix();						//	fontBeginで保持した行列情報を戻す
	glPopAttrib();						//	fontBeginで保持した描画属性を戻す
}

void fontFont(int _font)
{
	switch (_font)				//	番号によってフォントを切り替える
	{
	case FONT_FONT_ROMAN:
		font = font = GLUT_STROKE_ROMAN;
		break;
	case FONT_FONT_MONO_ROMAN:
		font = GLUT_STROKE_MONO_ROMAN;			//	等間隔のフォント
		break;
	}
}

void fontScreenSize(float _width, float _height)
{
	screenSize = vec2(_width, _height);
}

void fontPosition(float _x, float _y)
{
	origin = position = vec2(_x, _y);
}

void fontHeight(float _height)
{
	height = _height;
}

float fontGetWidth(int _character)
{
	return glutStrokeWidth(				//	文字の幅を取得
		font,							//	使用しているフォント
		_character						//	対象の文字
	)* height / FONT_DEFAULT_HEIGHT;	//	文字にかかっている倍率をかける
}

float fontGetLength(const unsigned char* _string)
{
	return glutStrokeLength(	//	文字列の幅を取得
		font,					//	長さを知りたいフォント
		_string					//	文字列
	) * height / FONT_DEFAULT_HEIGHT;
}

float fontGetHeight()
{
	return height;
}

float fontGetLineHeight()
{
	return height + FONT_DEFAULT_LINE_SPACE * height / FONT_DEFAULT_HEIGHT;
}

float fontGetWeightMin()
{
	GLfloat weight[2];
	glGetFloatv(
		GL_LINE_WIDTH_RANGE,
		weight
	);

	return weight[0];
}

float fontGetWeightMax()
{
	GLfloat weight[2];
	glGetFloatv(
		GL_LINE_WIDTH_RANGE,
		weight
	);

	return weight[1];
}

void fontWeight(float _weight)
{
	weight = _weight;
}

float fontGetWeight()
{
	return weight;
}

void fontDraw(const char *_format, ...)
{
	va_list argList;
	va_start(argList, _format);

	char str[256];

	vsprintf_s(
		str,
		_format,
		argList
	);

	glLineWidth(weight);									//	線の太さを設定

	va_end(argList);

	char* p = str;

	//	=== 描画する文字列を1文字ずつ描画する ===
	{
		for (; *p != '\0' && *p != '\n'; p++)						//	*pが\0(空文字)か\n(改行コード)の場合ループを終わる
		{
			glPushMatrix();											//	位置、大きさ(上下反転)を変更したいので行列を保存 
			{
				glTranslatef(position.x, position.y + height, 0);	//	位置を変更
				float s = height / FONT_DEFAULT_HEIGHT;
				glScalef(s, -s, s);									//	大きさを変更

				glutStrokeCharacter(font, *p);						//	文字を描く
				position.x += fontGetWidth(*p);						//	上で描いた文字分の座標を動かしておく(次に描画する文字が重ならないように)
			}
			glPopMatrix();											//	元に戻す

		}
	}
	//	=======================================

	if (*p == '\n')											//	改行コードで終了していた場合
	{
		position.x = origin.x;								//	xの位置は最初にセットした位置に戻す
		position.y += fontGetLineHeight();					//	文字描画位置を下にずらす

		fontDraw(++p);										//	再度描画命令を出す(再帰処理)
	}
}



