#include "glut.h"

//	描画が必要になったら
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT);	//	色情報をリセットする

	glRotatef(
		1,		//	回転する角度
		0,0,1	//	回転軸
	);

	glutWireTeapot(1);	//	ティーポットを描画する

	glutSwapBuffers();	//	ダブルバッファの表と裏を切り替える(スワップする)
	//glFlush();			//	シングルバッファの場合
}

void timer(int value)
{
	glutPostRedisplay();	//	再描画命令

	//	再帰的に呼び出す
	glutTimerFunc(			//	指定時間後に起動するタイマー
		1000/60,			//	何ms後にタイマーを起動するか(1000ms = 1s)
		timer,				//	起動するメソッド	
		0					//	渡す値
	);
}

int main(int argc, char *argv[])
{
	glutInit(&argc,argv);

	glutInitDisplayMode(GL_DOUBLE);			//	ダブルバッファを使用する(やらない場合シングルバッファ)

	glutInitWindowPosition(640,0);			//	Window位置(やらなくてもいい)
	glutInitWindowSize(640,640);			//	Window大きさ(やらなくてもいい)

	glutCreateWindow("多々タイトル");		//	Windowのタイトル設定
	glutDisplayFunc(display);				//	描画が必要になったら呼ばれるコールバックの設定

	glutTimerFunc(							//	指定時間後に起動するタイマー
		0,		//	何ms後にタイマーを起動するか(1000ms = 1s)
		timer,	//	起動するメソッド	
		0		//	渡す値
	);
	
	glutMainLoop();							//	処理をglutに委託する(コールバック系はこのメソッドより前に書く)

	
	return 0;
}