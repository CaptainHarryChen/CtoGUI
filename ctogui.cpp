#include"ctogui.h"
#include<cstdio>

void Display();
void Idle();
void Timer(int id);
GLubyte* ReadBMP(const char *bpath);

void (*UpdateScreen)(void) = NULL;
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
    gluOrtho2D(0, scr_width, 0, scr_height);

    for(int i = 0; i < scr_height; i++)
        for(int j = 0; j < scr_width; j++)
        {
            int pos = i * scr_width + j;
            if(imgs[(int)scr[cur][pos]] != NULL)
            {
                glRasterPos2d(j, i);
                glDrawPixels(img_width, img_height, GL_BGR_EXT, GL_UNSIGNED_BYTE, imgs[(int)scr[cur][pos]]);
            }
        }

    //glRecti(50,50,150,150);
    //glRasterPos2i(100,100);
    //glDrawPixels(img_width, img_height, GL_BGR_EXT, GL_UNSIGNED_BYTE, imgs['*']);

    //glFlush();
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

void CtoGui::Init(int *argc, char **argv, int width, int height, const char *title)
{
    glutInit(argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

    int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    int cy = GetSystemMetrics(SM_CYFULLSCREEN);
    glutInitWindowPosition((cx - width) / 2, (cy - height) / 2);

    win_width = width;
    win_height = height;
    glutInitWindowSize(width, height);

    glutCreateWindow(title);

    glutDisplayFunc(&Display);
    glutIdleFunc(&Idle);

    cur=0;
}

void CtoGui::SetImgSize(int iw, int ih)
{
    img_width = iw;
    img_height = ih;
}

void CtoGui::SetImg(char ch, const char *bpath)
{
    imgs[(int)ch]=ReadBMP(bpath);
}

void CtoGui::SetScreenSize(int iw, int ih)
{
    scr_width = iw;
    scr_height = ih;
    scr[0] = new char[iw * ih];
    scr[1] = new char[iw * ih];
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
