#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>

void display(void)
{
	//	塗りつぶす色を指定(かえてみた)
	glClearColor(1.0, 1.0, 0.0, 1.0);

	//	初期化
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	リフレッシュ
	glFlush();
}

int main(int argc, char *argv[])
{
	//	ウィンドウの生成位置を指定
	glutInitWindowPosition(100, 100);

	//	ウィンドウの生成サイズを指定
	glutInitWindowSize(640, 480);

	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_RGBA);

	glutCreateWindow("A Ho New World");

	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}
