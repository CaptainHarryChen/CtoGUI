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

namespace CtoGui
{
    void DLL_EXPORT Init(int *argc, char **argv, int width, int height, const char *title);
    void DLL_EXPORT SetImgSize(int iw, int ih);
    void DLL_EXPORT SetImg(char ch, const char *bpath);
    void DLL_EXPORT SetScreenSize(int iw, int ih);
    void DLL_EXPORT SetUpdateTimer(int tim);
    void DLL_EXPORT SetUpdateScreenFunc(void(*p)());
    void DLL_EXPORT BeginPlay();
    void DLL_EXPORT DrawScreen(const char *buf);

    int img_width = -1, img_height = -1;
    int scr_width = -1, scr_height = -1;
    int win_width = -1, win_height = -1;
    int time_to_update = -1;
};


#ifdef __cplusplus
}
#endif

#endif // __CTOGUI_H__
