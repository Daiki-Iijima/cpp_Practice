#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#include <string>

#define WIDTH 320
#define HEIGHT 240

static void DrawString(std::string str, int w, int h, int x0, int y0)
{
	glDisable(GL_LIGHTING);
	// 平行投影にする
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();

	// 画面上にテキスト描画
	glRasterPos2f(x0, y0);
	int size = (int)str.size();
	for (int i = 0; i < size; ++i)
	{
		char ic = str[i];
		glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ic);
	}

	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
}

void display(void)
{
	//	塗りつぶす色を指定
	glClearColor(0.0, 1.0, 1.0, 1.0);

	glColor3d(1.0, 0.0, 1.0);
	DrawString("test", WIDTH, HEIGHT, 10, 10);

	//	リフレッシュ
	glFlush();
}

void Init()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
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

	Init();

	glutMainLoop();

	return 0;
}
