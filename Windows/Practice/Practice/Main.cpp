#include <stdio.h>
#include <time.h>
#include "glm/glm.hpp"	//	*glut.hより先に定義する必要がある
#include "glut.h"

#include "font.h"
#include "tex.h"
#include "audio.h"

#include "Rect.h"

#define SCREEN_WIDTH (256)
#define SCREEN_HEIGHT (256)

using namespace glm;

bool keys[256];					//	どのキーが押されているかを保持する


//	描画が必要になったら
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);	//	色情報をリセットする

	//	アスペクト比が変わらないようにする
	glMatrixMode(GL_PROJECTION);	//	射影モードを変更する
	glLoadIdentity();				//	前回の射影行列が残らないように行列の初期化
	gluOrtho2D(						//	2次元空間を定義(Ortho:正射影)
		0, SCREEN_WIDTH,				//	left,right
		SCREEN_HEIGHT, 0				//	bottom,top
	);

	glMatrixMode(GL_MODELVIEW);		//	モデルビュー行列モードに切り替え
	glLoadIdentity();				//	前回の射影行列が残らないように行列の初期化

	int s = 8;	//	タイルのサイズ

	for (int i = 0; i < SCREEN_HEIGHT / s; i++)
	{
		for (int j = 0; j < SCREEN_WIDTH / s; j++)
		{
			glColor3ub(										//	色を設定
				0xff - 0xff * j / (SCREEN_WIDTH / s - 1),
				0xff * j / (SCREEN_WIDTH / s - 1),
				0xff * i / (SCREEN_WIDTH / s - 1)
			);
			glRectfv(										//	四角を描画
				(GLfloat *)&vec2(j*s, i*s),
				(GLfloat *)&(vec2(j*s, i*s) + vec2(s - 1, s - 1))//	s-1なのは間をあけるため
			);
		}
	}

	//	======= 文字列の描画 ======
	fontBegin();
	{
		fontHeight(7);
		fontWeight(2);
		fontFont(FONT_FONT_ROMAN);
		glColor3ub(0xff, 0xff, 0xff);
		for (int i = 0; i < SCREEN_HEIGHT / s; i++)
		{
			for (int j = 0; j < SCREEN_WIDTH / s; j++)
			{
				fontPosition(j*s+1, i*s);
				fontDraw("%x", (i*SCREEN_WIDTH + j) % 0x10);
			}
		}
	}
	fontEnd();

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
}

void keybord(unsigned char key, int x, int y)
{
	printf("keybord: %d,(%#x)\n", key, key);

	keys[key] = true;	//	キーが押された

	if ((key >= '0') && (key <= '3'))
	{
		int channel = key - '0';
		audioStop(channel);
		audioGain(channel, AUDIO_DEFAULT_GAIN * .1f);
		audioPlay(channel);
	}

	switch (key)
	{
	case 0x1b:			//	Escapeキーで終了
		exit(0);
		break;
		//case 'p':
		//	audioStop();
		//	audioWaveform(AUDIO_WAVEFORM_NOISE_LONG);
		//	audioFreq(audioIndexToFreq(14));	//	1789772.5f = ファミコンのCPUの周波数
		//	//audioDecay(.9f);
		//	//audioSweep(.9f, 1789772.5f / 4068);
		//	audioPlay();
		//break;
	}
}

void keybordUp(unsigned char key, int x, int y)
{
	printf("keybordUp: %d,(%#x)\n", key, key);

	keys[key] = false;	//	キーが離された

	if ((key >= '0') && (key <= '3'))
	{
		int channel = key - '0';
		audioStop(channel);
	}
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

	{
		int height = 720 - 32;				//	特に根拠はない
		int width = height * 4 / 3;			//	4:3の比率にする

		glutInitWindowSize(width, height);	//	Window大きさ(やらなくてもいい)
	}

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