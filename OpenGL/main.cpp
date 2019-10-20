#include <iostream>

//	Macの場合ここ２つのコメントアウトを外す
// #include <OpenGL/OpenGL.h>
// #include <GLUT/GLUT.h>

//	Ubuntuの場合コメントアウト
#include <GL/glut.h>

void draw(void)
{
    glClearColor(1, 0, 1, 0);
    glClear(GL_COLOR_BUFFER_BIT);
}

int main(int argc, char *argv[])
{
    //  GLUTの初期化
    //  引数はmainで引数になっているもの
    glutInit(&argc, argv);

    //  ウィンドウのサイズを指定
    glutInitWindowSize(600, 600);

    //  ディスプレイモードの指定
    //  GLUT_SINGLE : シングルバッファ
    //  GLUT_RGBA   : RGBAモード
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGBA);

    //  ウィンドウ生成(ウィンドウは表示はされない)
    //  ウィンドウ名を示す ASCll文字列
    glutCreateWindow("Wire_teapot");

    //  ディスプレイコールバックの登録
    //  ウィンドウの再描画が必要であると判断された時に呼び出される
    glutDisplayFunc(draw);

    //  イベント処理のループに入る(ウィンドウを表示させる)
    glutMainLoop();
    return 0;
}
