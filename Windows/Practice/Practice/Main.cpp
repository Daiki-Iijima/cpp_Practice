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

void idle(void)
{
	glutPostRedisplay();	//	再描画命令
}

int main(int argc, char *argv[])
{
	glutInit(&argc,argv);

	glutInitDisplayMode(GL_DOUBLE);			//	ダブルバッファを使用する(やらない場合シングルバッファ)

	glutInitWindowPosition(640,0);			//	Window位置(やらなくてもいい)
	glutInitWindowSize(640,640);			//	Window大きさ(やらなくてもいい)

	glutCreateWindow("多々タイトル");		//	Windowのタイトル設定
	glutDisplayFunc(display);				//	描画が必要になったら呼ばれるコールバックの設定

	glutIdleFunc(idle);						//	GLUTの手が空いた時に呼ばれる

	glutMainLoop();							//	処理をglutに委託する(コールバック系はこのメソッドより前に書く)

	
	return 0;
}