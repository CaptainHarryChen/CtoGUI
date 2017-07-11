#ifndef __CTOGUI_H__
#define __CTOGUI_H__

#include"main.h"
#include<GL/glut.h>

#define TIMER_UPDATE 1

#ifdef __cplusplus
extern "C"
{
#endif

class DLL_EXPORT CtoGui
{
public:
    CtoGui();
    ~CtoGui();
    void Init(int *argc, char **argv, int width, int hight, const char *title);
    void SetImgSize(int iw, int ih);
    void SetScreenSize(int iw, int ih);
    void SetUpdateTimer(int tim);
    virtual void UpdateScreen();
    void BeginPlay();
    void DrawScreen(char *buf);

private:
    void Display();
    void Idle();
    void Timer(int id);

    int img_width, img_height;
    int scr_width, scr_height;

public:
    int cur;
    int time_to_update;
    bool timer_setted;
    char *scr[2];
};


#ifdef __cplusplus
}
#endif

#endif // __CTOGUI_H__
