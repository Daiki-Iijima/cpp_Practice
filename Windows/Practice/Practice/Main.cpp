#include <stdio.h>
#include <time.h>
#include "glm/glm.hpp"	//	*glut.hより先に定義する必要がある
#include "glut.h"

#include "font.h"
#include "audio.h"

#include "Rect.h"

using namespace glm;

ivec2 windowSize = { 800,600 };	//	ウィンドウのサイズを定義

bool keys[256];					//	どのキーが押されているかを保持する


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

	glMatrixMode(GL_MODELVIEW);		//	モデルビュー行列モードに切り替え
	glLoadIdentity();				//	前回の射影行列が残らないように行列の初期化

	unsigned char pixels[] =		//	幅2ピクセル作成
	{
		0xff,0x00,0x00, 0x00,0xff,0x00,
		0x00,0x00,0xff, 0x00,0xff,0xff
	};

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);	//	1ピクセルからテクスチャを作成できるように設定

	glTexImage2D(						//	Vramへの転送する情報と設定
		GL_TEXTURE_2D,				//	ターゲット
		0,							//	mipmapのレベル
		GL_RGB,						//	テクスチャのフォーマット
		2, 2,						//	幅と高さ
		0,							//	ボーダー
		GL_RGB,						//	ピクセルデータのフォーマット
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

	glEnable(GL_TEXTURE_2D);		//	テクスチャを有効に

	Rect rect = { {100,100},{400,400} };
	rect.draw();

	//	======= 文字列の描画 ======
	fontBegin();
	{
		fontSetHeight(FONT_DEFAULT_HEIGHT);
		fontSetPosition(0, 0);
		//fontDraw("");
	}
	fontEnd();
	//	=====================================

	glutSwapBuffers();	//	ダブルバッファの表と裏を切り替える(スワップする)
}

void idle(void)
{
	//	=== 各種アップデート処理 ===
	audioUpdate();
	//	============================

	glutPostRedisplay();	//	再描画命令
}

//	ウィンドウサイズが変更されたときに呼ぶ
void reshape(int width, int height)
{
	printf("reshape:width%d height:%d\n", width, height);

	glViewport(			//	ビューポートを更新(更新しないと指定サイズより大きくなった時に表示できずに切れてしまう)
		0, 0,			//	座標(x,y)
		width, height	//	サイズ(w,h)
	);

	windowSize = ivec2(width, height);	//	リサイズされた値でサイズ定数を書き換える
}

void keybord(unsigned char key, int x, int y)
{
	printf("keybord: %d,(%#x)\n", key, key);

	switch (key)
	{
	case 0x1b:			//	Escapeキーで終了
		exit(0);
		break;
	}

	keys[key] = true;	//	キーが押された
}

void keybordUp(unsigned char key, int x, int y)
{
	printf("keybordUp: %d,(%#x)\n", key, key);

	keys[key] = false;	//	キーが離された
}

void passiveMotion(int _x, int _y)
{
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

	glutPassiveMotionFunc(passiveMotion);	//	マウスの移動イベントを取得

	glutMainLoop();							//	処理をglutに委託する(コールバック系はこのメソッドより前に書く)

	return 0;
}