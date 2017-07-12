#ifndef __CTOGUI_H__
#define __CTOGUI_H__

#include"main.h"
#include<GL/glut.h>
#include<GL/glext.h>

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
    void Init(int *argc, char **argv, int width, int height, const char *title);
    void SetImgSize(int iw, int ih);
    void SetImg(char ch, const char *bpath);
    void SetScreenSize(int iw, int ih);
    void SetUpdateTimer(int tim);
    void SetUpdateScreenFunc(void(*p)());
    void BeginPlay();
    void DrawScreen(const char *buf);
    GLubyte* ReadBMP(const char *bpath);

private:
    void Display();
    void Idle();
    void Timer(int id);

    int img_width, img_height;
    int scr_width, scr_height;
    int win_width, win_height;
    GLubyte *imgs[256];

    void (*UpdateScreen)(void);

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
