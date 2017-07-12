#include"ctogui.h"
#include<cstring>
#include<cstdio>

CtoGui::CtoGui()
{
    img_width = -1;
    img_height = -1;
    scr_width = -1;
    scr_height = -1;
    scr[0] = NULL;
    scr[1] = NULL;
    memset(imgs, 0, sizeof(imgs));
}

CtoGui::~CtoGui()
{
    if(scr[0])
        delete[] scr[0];
    if(scr[1])
        delete[] scr[1];
    for(int i = 0; i < 256; i++)
        if(imgs[i])
            delete[] imgs[i];
}

void CtoGui::Init(int *argc, char **argv, int width, int height, const char *title)
{
    glutInit(argc, argv);

    int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    int cy = GetSystemMetrics(SM_CYFULLSCREEN);
    glutInitWindowPosition((cx - width) / 2, (cy - height) / 2);

    win_width = width;
    win_height = height;
    glutInitWindowSize(width, height);

    glutCreateWindow(title);

    glutDisplayFunc((void(*)())&CtoGui::Display);
    glutIdleFunc((void(*)())&CtoGui::Idle);

    cur=0;
}

void CtoGui::Display()
{
    for(int i = 0; i < scr_width; i++)
        for(int j = 0; j < scr_height; j++)
        {
            int pos = i * scr_width + j;
            if(scr[cur][pos] != scr[cur ^ 1][pos] && imgs[(int)scr[cur][pos]] != NULL)
            {
                glRasterPos2d(i * img_width / win_width, j * img_height / win_height);
                glDrawPixels(img_width, img_height, GL_BGR_EXT, GL_UNSIGNED_BYTE, imgs[(int)scr[cur][pos]]);
            }
        }

    glutSwapBuffers();
}

void CtoGui::Idle()
{
    if(!timer_setted)
    {
        if(UpdateScreen != NULL)
            (*UpdateScreen)();
        Display();
    }
}

void CtoGui::Timer(int id)
{
    switch(id)
    {
    case TIMER_UPDATE:
        if(UpdateScreen != NULL)
            (*UpdateScreen)();
        Display();
        glutTimerFunc(time_to_update, (void(*)(int))&CtoGui::Timer, TIMER_UPDATE);
        break;
    default:
        break;
    }
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
    glutTimerFunc(time_to_update, (void(*)(int))&CtoGui::Timer, TIMER_UPDATE);
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

GLubyte* CtoGui::ReadBMP(const char *bpath)
{
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
