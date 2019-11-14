#include <stdio.h>
#include "glm/glm.hpp"	//	*glut.hより先に定義する必要がある
#include "glut.h"

#include "font.h"
#include "Rect.h"

using namespace glm;

ivec2 windowSize = { 800,600 };	//	ウィンドウのサイズを定義

bool keys[256];		//	どのキーが押されているかを保持する

Rect rect1 = Rect(vec2(100, 100), vec2(100, 200));
Rect rect2 = Rect(vec2(windowSize.x / 2, windowSize.y / 2), vec2(200, 100));

//	描画が必要になったら
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);	//	色情報をリセットする


	//	アスペクト比が変わらないようにする
	glMatrixMode(GL_PROJECTION);	//	射影モードを変更する
	glLoadIdentity();				//	前回の射影行列が残らないように行列の初期化
	gluOrtho2D(						//	2次元空間を定義(Ortho:正射影)
		0, windowSize.x,				//	left,right
		windowSize.y, 0				//	bottom,top
	);

	//	ティーポットモデルに対する変換
	glMatrixMode(GL_MODELVIEW);		//	モデルビュー行列モードに切り替え
	glLoadIdentity();				//	前回の射影行列が残らないように行列の初期化


	if (rect1.intersect(rect2))
		glColor3ub(0xff, 0x00, 0x00);
	else
		glColor3ub(0x00, 0x00, 0xff);
	//	矩形の描画
	rect1.draw();

	glColor3ub(0x00, 0xff, 0x00);

	rect2.draw();

	//	======= 文字列の描画(font.cpp) ======
	fontBegin();
	fontSetColor(0, 0xff, 0);
	fontSetPosition(0, windowSize.y - fontGetSize() * 2);
	fontSetSize(FONT_DEFAULT_SIZE / 2);
	fontEnd();
	//	=====================================

	glutSwapBuffers();	//	ダブルバッファの表と裏を切り替える(スワップする)
	//glFlush();			//	シングルバッファの場合
}

void idle(void)
{
	//	描画しているものを動かす場合、ここで処理する
	float f = 5;
	if (keys['w'])	rect1.m_position.y -= f;
	if (keys['s'])	rect1.m_position.y += f;
	if (keys['a'])	rect1.m_position.x -= f;
	if (keys['d'])	rect1.m_position.x += f;

	glutPostRedisplay();	//	再描画命令
}

//	ウィンドウサイズが変更されたときに呼ぶ
void reshape(int width, int height)
{
	//printf("reshape:width%d height:%d\n", width, height);

	glViewport(			//	ビューポートを更新(更新しないと指定サイズより大きくなった時に表示できずに切れてしまう)
		0, 0,			//	座標(x,y)
		width, height	//	サイズ(w,h)
	);

	windowSize = ivec2(width, height);	//	リサイズされた値でサイズ定数を書き換える
}

void keybord(unsigned char key, int x, int y)
{
	if (key == 0x1b)	//	Escapeキーで終了
		exit(0);

	//printf("keybord: %d,(%#x)\n", key, key);

	keys[key] = true;	//	キーが押された
}

void keybordUp(unsigned char key, int x, int y)
{
	//printf("keybordUp: %d,(%#x)\n", key, key);

	keys[key] = false;	//	キーが離された
}

int main(int argc, char *argv[])
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GL_DOUBLE);			//	ダブルバッファを使用する(やらない場合シングルバッファ)

	glutInitWindowPosition(640, 0);			//	Window位置(やらなくてもいい)
	glutInitWindowSize(windowSize.x, windowSize.y);			//	Window大きさ(やらなくてもいい)

	glutCreateWindow("多々タイトル");		//	Windowのタイトル設定
	glutDisplayFunc(display);				//	描画が必要になったら呼ばれるコールバックの設定

	glutIdleFunc(idle);						//	GLUTの手が空いた時に呼ばれるコールバックの設定

	glutReshapeFunc(reshape);				//	Windowのサイズが変わったら呼ばれるコールバックの設定

	glutKeyboardFunc(keybord);				//	キーボードイベントを取得
	glutIgnoreKeyRepeat(GL_TRUE);			//	キーボードの押しっぱなし状態を無効にすることをTrueにする
	glutKeyboardUpFunc(keybordUp);			//	キーボードが離されたときイベント

	glutMainLoop();							//	処理をglutに委託する(コールバック系はこのメソッドより前に書く)


	return 0;
}