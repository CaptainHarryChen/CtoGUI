#include"ctogui.h"
#include<cstring>

CtoGui::CtoGui()
{
    img_width = -1;
    img_height = -1;
    scr_width = -1;
    scr_height = -1;
    scr[0] = NULL;
    scr[1] = NULL;
}

CtoGui::~CtoGui()
{
    if(scr[0])
        free(scr[0]);
    if(scr[1])
        free(scr[1]);
}

void CtoGui::Init(int *argc, char **argv, int width, int hight, const char *title)
{
    glutInit(argc, argv);

    int cx = GetSystemMetrics(SM_CXFULLSCREEN);
    int cy = GetSystemMetrics(SM_CYFULLSCREEN);
    glutInitWindowPosition((cx - width) / 2, (cy - hight) / 2);

    glutInitWindowSize(width, hight);

    glutCreateWindow(title);

    glutDisplayFunc((void(*)())&CtoGui::Display);
    glutIdleFunc((void(*)())&CtoGui::Idle);

    cur=0;
}

void CtoGui::Display()
{
    glFlush();
    glutSwapBuffers();
}

void CtoGui::Idle()
{
    if(!timer_setted)
    {
        UpdateScreen();
        Display();
    }
}

void CtoGui::Timer(int id)
{
    switch(id)
    {
        case TIMER_UPDATE:
            UpdateScreen();
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

void CtoGui::SetScreenSize(int iw, int ih)
{
    scr_width = iw;
    scr_height = ih;
    scr[0] = (char*)malloc(iw * ih);
    scr[1] = (char*)malloc(iw * ih);
}

void CtoGui::SetUpdateTimer(int tim)
{
    timer_setted = true;
    time_to_update = tim;
    glutTimerFunc(time_to_update, (void(*)(int))&CtoGui::Timer, TIMER_UPDATE);
}

void CtoGui::UpdateScreen()
{

}

void CtoGui::BeginPlay()
{
    glutMainLoop();
}

void CtoGui::DrawScreen(char *buf)
{
    memcpy(scr[cur ^ 1], buf, scr_width * scr_height);
}
