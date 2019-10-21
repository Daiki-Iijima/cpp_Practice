# include<GL/glut.h>

#define WIDTH 320
#define HEIGHT 240

void Line2D(int x1,int y1,int x2,int y2,float size)
{
	glLineWidth(size);
	glBegin(GL_LINES);
	glVertex2i(x1,y1);
	glVertex2i(x2,y2);
	glEnd();
}

void display(void)
{
	//	初期化
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glColor4f(0.0f,0.0f,0.0f,1.0f);
	Line2D(20,40,200,180,1.0);

	//	リフレッシュ
	glFlush();
}


void Init()
{
	glClearColor(1.0,1.0,1.0,1.0);
	glOrtho(0,WIDTH,HEIGHT,0,-1,1);
}

int main(int argc,char *argv[])
{
	// glutInitWindowPositionを使うとよくわからないエラーになる
	
	glutInitWindowSize(WIDTH,HEIGHT);

	glutInit(&argc,argv);

	glutInitDisplayMode(GLUT_RGBA);

	glutCreateWindow("Draw Line");

	glutDisplayFunc(display);

	Init();

	glutMainLoop();

	return 0;
}
