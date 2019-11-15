#include <stdio.h>
#include <stdarg.h>

#include "font.h"

#include "glm/glm.hpp"
#include "glut.h"

using namespace glm;

static vec2 position;
static float size = FONT_DEFAULT_SIZE;

static unsigned char color[3];

static float weight = 1;

void fontBegin()
{
	glPushMatrix();						//	現在設定されている行列情報を保持
	glPushAttrib(GL_ALL_ATTRIB_BITS);	//	現在設定されている描画属性を保持

	glMatrixMode(GL_PROJECTION);		//	射影モードを変更する
	glLoadIdentity();					//	前回の射影行列が残らないように行列の初期化

	GLint viewport[4];					//	ビューポートの情報を保存する
	glGetIntegerv(						//	ビューポートの情報を取得する
		GL_VIEWPORT,
		viewport
	);

	gluOrtho2D(							//	2次元空間を定義(Ortho:正射影)
		0, viewport[2],					//	left,right
		viewport[3], 0					//	bottom,top
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

void fontSetPosition(float _x, float _y)
{
	position = vec2(_x, _y);
}

void fontSetSize(float _size)
{
	size = _size;
}

float fontGetSize()
{
	return size;
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

void fontSetFontWeight(float _weight)
{
	weight = _weight;
}


void fontSetColor(unsigned char _red, unsigned char _green, unsigned char _blue)
{
	color[0] = _red;
	color[1] = _green;
	color[2] = _blue;
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

	glLineWidth(weight);		//	線の太さを設定

	va_end(argList);

	glColor3ub(color[0], color[1], color[2]);	//	フォントの色を変更

	glPushMatrix();						//	位置、大きさ(上下反転)を変更したいので行列を保存 
	{
		glTranslatef(position.x, position.y + size, 0);		//	位置を変更
		float s = size / FONT_DEFAULT_SIZE;
		glScalef(s, -s, s);									//	大きさを変更

		for (char* p = str; *p != '\0'; p++)		//	*pが\0(空文字)では無ければ動き続ける
			glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);		//	文字を描く

	}
	glPopMatrix();						//	元に戻す

	printf("%s\n", str);
}



