# include<GL/glut.h>

void display(void)
{
	//	塗りつぶす色を指定
	glClearColor(0.0,0.0,0.0,1.0);
	
	//	初期化
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//	リフレッシュ
	glFlush();
}

int main(int argc,char *argv[])
{
	glutInit(&argc,argv);

	glutInitDisplayMode(GLUT_RGBA);

	glutCreateWindow("A Ho New World");

	glutDisplayFunc(display);

	glutMainLoop();

	return 0;
}
