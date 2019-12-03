#include <stdio.h>
#include <time.h>
#include "glm/glm.hpp"	//	*glut.hより先に定義する必要がある
#include "glut.h"

#include "font.h"
#include "tex.h"
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

	//	=== テクスチャの描画 ===
	//glEnable(GL_TEXTURE_2D);		//	テクスチャを有効に
	//glEnable(GL_BLEND);			//	透過処理を有効に

	//glBlendFunc(					//	アルファブレンド設定
	//	GL_SRC_ALPHA,				//
	//	GL_ONE_MINUS_SRC_ALPHA		//	
	//);

	//	==================


	//	======= 文字列の描画 ======
	fontBegin();
	{
		fontHeight(FONT_DEFAULT_HEIGHT / 2);
		fontWeight(fontGetWeightMax() / 2);
		fontPosition(fontGetWeight() * 2, fontGetWeight() * 2);

		//fontDraw("abc\nkkk"); fontDraw("\n"); fontDraw("def\n");

	}
	fontEnd();
	//	=====================================

	//	=== 疑似乱数生成アルゴリズム ===
	const int xorBit = 6;									//	何ビット目とXOR演算するか
	static int shiftReg = 1 << 14;							//	シフトレジスターを定義
	int result = (shiftReg ^ (shiftReg >> xorBit)) & 1;		//	0bit目と1bit目をXOR演算
	shiftReg >>= 1;											//	レジスタの値を右に1シフトさせる
	shiftReg |= result << 14;								//	or演算して計算結果を14ビット目に代入

	for (int i = 14; i >= 0; i--)
	{
		switch (i)											//	ビット番号で色分け
		{
		case 0:glColor3ub(0x00, 0xff, 0x00); break;
		case xorBit:glColor3ub(0xff, 0x00, 0x00); break;
		case 14:glColor3ub(0xff, 0xff, 0x00); break;
		default:glColor3ub(0xff, 0xff, 0xff); break;
		}

		fontDraw("%d", (shiftReg >> i) & 1);

		if (i % 4 == 0)										//	4文字区切りでカンマを代入
		{
			glColor3ub(0xff, 0xff, 0xff);
			fontDraw(",");
		}
	}
	glColor3ub(0xff, 0xff, 0x00);
	fontDraw("\n%d", result);

	glColor3ub(0xff, 0xff, 0xff);
	fontDraw("\n%#x(%d)", shiftReg, shiftReg);
	//	===============================

	glutSwapBuffers();	//	ダブルバッファの表と裏を切り替える(スワップする)
	//getchar();		//	キーボード入力待ち状態
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
	case 'p':
		audioStop();
		audioWaveform(AUDIO_WAVEFORM_NOISE_LONG);
		audioFreq(audioIndexToFreq(14));	//	1789772.5f = ファミコンのCPUの周波数
		//audioDecay(.9f);
		//audioSweep(.9f, 1789772.5f / 4068);
		audioPlay();
	break;
	}

	/*if ((key >= '0') && (key <= '4'))
	{
		audioStop();
		audioWaveform(key - '0');
		audioFreq(440);
		audioSweep(.99, 440 / 2);
		audioPlay();
	}*/

	if ((key >= '0') && (key <= '9'))
	{
		audioStop();
		int k = key - '0';
		audioWaveform(AUDIO_WAVEFORM_PULSE_12_5);
		audioFreq(440 * powf(2, (1 + k / 12.f)));
		//audioDecay(.9f);
		audioSweep(.99, 440 / 2);
		audioPlay();
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
	audioInit();

	glutInit(&argc, argv);

	glutInitDisplayMode(GL_DOUBLE);			//	ダブルバッファを使用する(やらない場合シングルバッファ)

	glutInitWindowPosition(640, 0);			//	Window位置(やらなくてもいい)
	glutInitWindowSize(windowSize.x, windowSize.y);			//	Window大きさ(やらなくてもいい)

	glutCreateWindow("多々タイトル");		//	Windowのタイトル設定

	//unsigned char colorKey[] = { 0x00,0xff,0x00 };			//	透明にしたい色
	//int result = texFromBMP("UnityChan.bmp", colorKey);		//	.bmpファイルを開く

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