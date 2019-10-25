#include <GLUT/glut.h>
#include <OpenGL/OpenGL.h>
#include <string>

using namespace std;

#define WIDTH 400
#define HEIGHT 500

#define POSITION_X 100
#define POSITION_Y 100

char keyname;

//  文字列描画
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

void Init()
{
    //  背景色を設定
    glClearColor(0.0, 0.0, 0.0, 1.0);

    //  スクリーン上の表示領域をビューポートに比例させる
    glOrtho(0, WIDTH, HEIGHT, 0, -1, 1);
}

void idle(void)
{
    //  再描画イベントを発生させる
    glutPostRedisplay();
}

//  再描画
void display(void)
{
    //	背景色を指定
    glClearColor(0.0, 0.0, 0.0, 1.0);

    //  背景を塗りつぶす
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    //  文字の色指定
    glColor3d(1.0, 1.0, 1.0);

    string keystr = string() + keyname;

    //  文字列描画
    DrawString(keystr, WIDTH, HEIGHT, 10, 10);

    //	リフレッシュ
    glFlush();
}

void key(unsigned char key, int x, int y)
{
    keyname = key;
}

int main(int argc, char *argv[])
{
    //  Window情報設定(位置、サイズ)
    glutInitWindowPosition(POSITION_X, POSITION_Y);
    glutInitWindowSize(WIDTH, HEIGHT);

    //  GLUT初期化
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_RGBA);
    glutCreateWindow("初期化");

    //  ウィンドウを再描画する必要が出た時に呼ばれる
    glutDisplayFunc(display);

    //  プログラムが暇な時に呼ばれる
    glutIdleFunc(idle);

    //  キーボード入力取得
    glutKeyboardFunc(key);

    //
    Init();

    //  glut関係の処理ループを開始する
    glutMainLoop();
    return 0;
}