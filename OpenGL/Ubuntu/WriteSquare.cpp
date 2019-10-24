# include<GL/glut.h>

#define WIDTH 320
#define HEIGHT 240

void Square2D(int x1,int y1,int x2,int y2,float size)
{
	//	先の太さを指定
	glLineWidth(size);

	//	LINE_LOOP : 折れ線を描画する
	glBegin(GL_LINE_LOOP);

	glVertex2i(x1,y1);
	glVertex2i(x2,y1);
	glVertex2i(x2,y2);
	glVertex2i(x1,y2);

	glEnd();
}

void Square2D(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4,float size)
{
	//	先の太さを指定
	glLineWidth(size);

	//	LINE_LOOP : 折れ線を描画する
	glBegin(GL_LINE_LOOP);

	glVertex2i(x1,y1);
	glVertex2i(x2,y2);
	glVertex2i(x3,y3);
	glVertex2i(x4,y4);

	glEnd();
}

void SquareFill2D(int x1,int y1,int x2,int y2)
{
	//	QUADS : 四角形を描画する
	glBegin(GL_QUADS);

	glVertex2i(x1,y1);
	glVertex2i(x2,y1);
	glVertex2i(x2,y2);
	glVertex2i(x1,y2);

	glEnd();
}

void SquareFill2D(int x1,int y1,int x2,int y2,int x3,int y3,int x4,int y4)
{
	//	QUADS : 四角形を描画する
	glBegin(GL_LINE_LOOP);

	glVertex2i(x1,y1);
	glVertex2i(x2,y2);
	glVertex2i(x3,y3);
	glVertex2i(x4,y4);

	glEnd();
}

void display(void)
{
	//	初期化
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

	//	各図形の描画
	//	色
	//	描画位置
	glColor4f(1.0f,0.0f,1.0f,1.0f);
	Square2D(30,20,100,60,1.0f);
 	
	glColor4f(0.0f,0.0f,1.0f,1.0f);
 	Square2D(150,20,240,40,280,80,130,100,3.0f);
 	
	glColor4f(0.0f,0.5f,0.0f,1.0f);
 	SquareFill2D(40,150,90,200);

 	glColor4f(1.0f,0.5f,0.0f,1.0f);
 	SquareFill2D(150,170,300,150,280,200,170,220);

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
