# include<GL/glut.h>

void display(void)
{
	//	初期化
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//	バックバッファとフロントバッファを交換
	glutSwapBuffers();
}

void idle(void)
{
	//	再描画イベントを発火
	//	glutDisplayFuncで設定したコールバック関数が呼び出される
	glutPostRedisplay();
}

void keyboard(unsigned char key,int x,int y)
{
	//	ESCキーで終了
	if(key ==27)exit(0);
}

void Init()
{
	glClearColor(1.0,1.0,1.0,1.0);
}

//	ゲームモードを使用することによりフルスクリーンを簡単に実装することができる
//	ゲームモードを使用する場合「glutCreateWindow」を使用してはいけない(アプリケーションが落ちる)
int main(int argc,char *argv[])
{
	glutInit(&argc,argv);

	glutGameModeString("1920x1080:32@60");

	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);

	glutEnterGameMode();

	glutDisplayFunc(display);

	glutKeyboardFunc(keyboard);

	glutIdleFunc(idle);

	Init();
	
	glutMainLoop();

	return 0;
}
