#include"ctogui.h"
using namespace CtoGui;

const int dir[4][2] = {{-1, 0}, {0, 1}, {1, 0}, {0, -1}};
int Map[40][60];
int x, y;

void UDLR(int key, int kx, int ky)
{
    int tx, ty;
    if(key==GLUT_KEY_UP)
    {
        tx = x + dir[0][0];
        ty = y + dir[0][1];
        if(tx >= 0 && ty >= 0 && tx < 40 && ty < 60)
        {
            x = tx;
            y = ty;
        }
    }
    if(key==GLUT_KEY_RIGHT)
    {
        tx = x + dir[1][0];
        ty = y + dir[1][1];
        if(tx >= 0 && ty >= 0 && tx < 40 && ty < 60)
        {
            x = tx;
            y = ty;
        }
    }
    if(key==GLUT_KEY_DOWN)
    {
        tx = x + dir[2][0];
        ty = y + dir[2][1];
        if(tx >= 0 && ty >= 0 && tx < 40 && ty < 60)
        {
            x = tx;
            y = ty;
        }
    }
    if(key==GLUT_KEY_LEFT)
    {
        tx = x + dir[3][0];
        ty = y + dir[3][1];
        if(tx >= 0 && ty >= 0 && tx < 40 && ty < 60)
        {
            x = tx;
            y = ty;
        }
    }
}

void WSAD(unsigned char key, int kx, int ky)
{
    int tx, ty;
    if(key=='w')
    {
        tx = x + dir[0][0];
        ty = y + dir[0][1];
        if(tx >= 0 && ty >= 0 && tx < 40 && ty < 60)
        {
            x = tx;
            y = ty;
        }
    }
    if(key=='d')
    {
        tx = x + dir[1][0];
        ty = y + dir[1][1];
        if(tx >= 0 && ty >= 0 && tx < 40 && ty < 60)
        {
            x = tx;
            y = ty;
        }
    }
    if(key=='s')
    {
        tx = x + dir[2][0];
        ty = y + dir[2][1];
        if(tx >= 0 && ty >= 0 && tx < 40 && ty < 60)
        {
            x = tx;
            y = ty;
        }
    }
    if(key=='a')
    {
        tx = x + dir[3][0];
        ty = y + dir[3][1];
        if(tx >= 0 && ty >= 0 && tx < 40 && ty < 60)
        {
            x = tx;
            y = ty;
        }
    }
}

void Move(int button, int state, int mx, int my)
{
    if(button == GLUT_LEFT_BUTTON)
    {
        if(state == GLUT_DOWN)
        {
            x = mx;
            y = my;
        }
    }
}

scene S;

void Update()
{
    memset(Map, 0, sizeof Map);
    y=(y+1)%60;
    Map[x][y] = 1;
    for(int i = 0;i <= 40;i++)
        for(int j = 0;j <= 60;j++)
            Map[i][j] = 1;
    S = scene((int*)Map, 40, 60);
    DrawScreen(S);
}

int main(int argc, char *argv[])
{
    Init(&argc, argv, 400, 600, "Test");
    SetImg(1, "img\\star.bmp");
    SetUpdateScreenFunc(Update);
    SetSpecialFunc(UDLR);
    SetKeyFunc(WSAD);
    SetMouseFunc(Move);
    //SetUpdateTimer(100);

    BeginPlay();
    return 0;
}
