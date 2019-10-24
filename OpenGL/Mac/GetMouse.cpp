#include <GLUT/GLUT.h>
#include <OpenGL/OpenGL.h>
#include <stdlib.h>
#include <string>
using namespace std;

#define WIDTH 400
#define HEIGHT 320

int MouseX, MouseY;
bool M_left = false;
bool M_right = false;
bool M_middle = false;
int wheel;

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

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	string str = "MousePos X: ";

	glColor3d(1.0, 1.0, 1.0);
	DrawString(str, WIDTH, HEIGHT, 10, 10);

	//	リフレッシュ
	glFlush();
}
void idle(void)
{
	glutPostRedisplay();
}
void Init()
{
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
}
//マウス(クリック)
void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		M_left = true;
	}
	else if (button == GLUT_LEFT_BUTTON && state == GLUT_UP)
	{
		M_left = false;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN)
	{
		M_right = true;
	}
	else if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP)
	{
		M_right = false;
	}
	if (button == GLUT_MIDDLE_BUTTON && state == GLUT_DOWN)
	{
		M_middle = true;
	}
	else if (button == GLUT_MIDDLE_BUTTON && state == GLUT_UP)
	{
		M_middle = false;
	}
}

//ドラッグ
void drag(int x, int y)
{
	MouseX = x;
	MouseY = y;
}

//パッシブ
void passive(int x, int y)
{
	MouseX = x;
	MouseY = y;
}
//ホイール
void MouseWheel(int wheel_number, int direction, int x, int y)
{
	if (direction == 1)
	{
		wheel++;
	}
	else
	{
		wheel--;
	}
}
int main(int argc, char *argv[])
{
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
	glutCreateWindow("マウスイベント");
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutMotionFunc(drag);
	glutPassiveMotionFunc(passive);
	// glutMouseWheelFunc(MouseWheel); //ホイールコールバック(glutfreeだと使えるらしい)
	glutIdleFunc(idle);
	Init();
	glutMainLoop();
	return 0;
}