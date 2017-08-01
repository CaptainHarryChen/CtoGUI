#include"ctogui.h"
#include<cstdio>

void Display();
void Idle();
void Timer(int id);
GLubyte* ReadBMP(const char *bpath);

void (*UpdateScreen)(void) = NULL;
void (*KeyProcess)(unsigned char, int, int) = NULL;
void (*SpecialProcess)(int, int, int) = NULL;
void (*MouseProcess)(int, int, int, int);
int cur = 0;
bool timer_setted = false;
char *scr[2] = {NULL};
GLubyte *imgs[256] = {NULL};

void Display()
{
    using namespace CtoGui;

    glClear(GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0, scr_width, scr_height, 0);

    for(int i = 0; i < scr_height; i++)
        for(int j = 0; j < scr_width; j++)
        {
            int pos = i * scr_width + j;
            if(imgs[(int)scr[cur][pos]] != NULL)
            {
                glRasterPos2d(j, i + 1);
                glDrawPixels(img_width, img_height, GL_BGR_EXT, GL_UNSIGNED_BYTE, imgs[(int)scr[cur][pos]]);
            }
        }

    glutSwapBuffers();
}

void Idle()
{
    using namespace CtoGui;

    if(timer_setted == false)
    {
        if(UpdateScreen != NULL)
            (*UpdateScreen)();
        Display();
    }
}

void Timer(int id)
{
    using namespace CtoGui;

    switch(id)
    {
    case TIMER_UPDATE:
        if(UpdateScreen != NULL)
            (*UpdateScreen)();
        Display();
        glutTimerFunc(time_to_update, &Timer, TIMER_UPDATE);
        break;
    default:
        break;
    }
}

void OnKeyBoard(unsigned char key, int x, int y)
{
    int tx, ty;
    CtoGui::ChangeXY(x, y, tx, ty);
    if(KeyProcess != NULL)
        KeyProcess(key, ty, tx);
}

void OnSpecialKey(int key, int x,int y)
{
    int tx, ty;
    CtoGui::ChangeXY(x, y, tx, ty);
    if(SpecialProcess != NULL)
        SpecialProcess(key, ty, tx);
}

void OnMouse(int button, int state, int x, int y)
{
    int tx, ty;
    CtoGui::ChangeXY(x, y, tx, ty);
    if(MouseProcess != NULL)
        MouseProcess(button, state, ty, tx);
}

void CtoGui::Init(int *argc, char **argv, int height, int width, const char *title)
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    int cy = GetSystemMetrics(SM_CYFULLSCREEN);
    glutInitWindowPosition((cx - width) / 2, (cy - height) / 2);

    win_height = height;
    win_width = width;
    glutInitWindowSize(width, height);

    glutCreateWindow(title);

    glutDisplayFunc(&Display);
    glutIdleFunc(&Idle);
    glutKeyboardFunc(&OnKeyBoard);
    glutSpecialFunc(&OnSpecialKey);
    glutMouseFunc(&OnMouse);

    cur=0;
}

void CtoGui::SetImgSize(int ih, int iw)
{
    img_height = ih;
    img_width = iw;
}

void CtoGui::SetImg(char ch, const char *bpath)
{
    imgs[(int)ch]=ReadBMP(bpath);
}

void CtoGui::SetScreenSize(int ih, int iw)
{
    scr_height = ih;
    scr_width = iw;
    scr[0] = new char[ih * iw];
    scr[1] = new char[ih * iw];
}

void CtoGui::SetUpdateTimer(int tim)
{
    timer_setted = true;
    time_to_update = tim;
    glutTimerFunc(time_to_update, &Timer, TIMER_UPDATE);
}

void CtoGui::SetUpdateScreenFunc(void (*p)())
{
    UpdateScreen = p;
}

void CtoGui::SetKeyFunc(void(*p)(unsigned char, int, int))
{
    KeyProcess = p;
}

void CtoGui::SetSpecialFunc(void(*p)(int, int, int))
{
    SpecialProcess = p;
}

void CtoGui::SetMouseFunc(void(*p)(int, int, int, int))
{
    MouseProcess = p;
}

void CtoGui::BeginPlay()
{
    glutMainLoop();
}

void CtoGui::DrawScreen(const char *buf)
{
    cur ^= 1;
    memcpy(scr[cur], buf, scr_width * scr_height);
}

GLubyte* ReadBMP(const char *bpath)
{
    using namespace CtoGui;

    GLint ImageWidth,ImageHeight;
    GLint PixelLength;
    GLubyte *PixelData;
    FILE* pFile = fopen(bpath, "rb");
    if(pFile == 0)
        return NULL;

    fseek(pFile, 0x0012, SEEK_SET);
    fread(&ImageWidth, sizeof(ImageWidth), 1, pFile);
    fread(&ImageHeight, sizeof(ImageHeight), 1, pFile);
    if(ImageWidth != img_width)
        return NULL;
    if(ImageHeight != img_height)
        return NULL;

    PixelLength = ImageWidth * 3;
    while(PixelLength % 4 != 0)
        PixelLength++;
    PixelLength *= ImageHeight;

    PixelData = new GLubyte[PixelLength];
    if(PixelData == NULL)
        return NULL;

    fseek(pFile, 54, SEEK_SET);
    fread(PixelData, PixelLength, 1, pFile);

    fclose(pFile);

    return PixelData;
}

void CtoGui::ChangeXY(int x, int y, int &resx, int &resy)
{
    resx = x / img_width;
    resy = y / img_height;
}

