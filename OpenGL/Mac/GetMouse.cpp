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
	//	背景色を指定
	glClearColor(0.0, 0.0, 0.0, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//	各種値を「string」型に変換
	string mouseX, mouseY, mouseLeft, mouseRight, mouseMiddle;
	mouseX = to_string(MouseX);
	mouseY = to_string(MouseY);
	mouseLeft = M_left ? "True" : "false";
	mouseRight = M_right ? "True" : "false";
	mouseMiddle = M_middle ? "True" : "false";

	//	文字の色指定
	glColor3d(1.0, 1.0, 1.0);

	//	文字列としてウィンドウに描画
	DrawString(mouseX, WIDTH, HEIGHT, 10, 10);
	DrawString(mouseY, WIDTH, HEIGHT, 10, 25);
	DrawString(mouseLeft, WIDTH, HEIGHT, 10, 40);
	DrawString(mouseRight, WIDTH, HEIGHT, 10, 55);
	DrawString(mouseMiddle, WIDTH, HEIGHT, 10, 70);

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
	//	位置、サイズ指定
	glutInitWindowPosition(100, 100);
	glutInitWindowSize(WIDTH, HEIGHT);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutCreateWindow("Get Mouse");
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