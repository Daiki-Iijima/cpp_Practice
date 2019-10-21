# include<GL/glut.h>

#define WIDTH 320
#define HEIGHT 240

void Point(int x,int y,float size)
{
	glPointSize(size);
	glBegin(GL_POINTS);
	glVertex2i(x,y);
	glEnd();
}

void display(void)
{
	//	初期化
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	glColor4f(0.0f,0.0f,1.0f,1.0f);
	Point(50,50,2.0);

	glColor4f(1.0f,0.0f,0.0f,1.0f);
	Point(250,150,10.0);
	
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
	glutInitWindowSize(WIDTH,HEIGHT);

	glutInit(&argc,argv);

	glutInitDisplayMode(GLUT_RGBA);

	glutCreateWindow("Draw Dot");

	glutDisplayFunc(display);

	Init();

	glutMainLoop();

	return 0;
}
