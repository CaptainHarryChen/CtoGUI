#ifndef __CTOGUI_H__
#define __CTOGUI_H__

#include <windows.h>

/*  To use this exported function of dll, include this header
 *  in your project.
 */

#ifdef BUILD_DLL
    #define DLL_EXPORT __declspec(dllexport)
#endif // BUILD_DLL
#ifdef TEST_DLL
    #define DLL_EXPORT
#endif // TEST_DLL
#ifndef BUILD_DLL
    #ifndef TEST_DLL
        #define DLL_EXPORT __declspec(dllimport)
    #endif // TEST_DLL
#endif // BUILD_DLL

#include"glut.h"
#include<GL/glext.h>

#define TIMER_UPDATE 1

#ifdef __cplusplus
extern "C"
{
#endif

namespace CtoGui
{
    struct DLL_EXPORT scene
    {
        int *buf;
        int height, width;
        int img_height,img_width;

        scene();
        scene(int *tbuf, int theight, int twidth);
        ~scene();
        void Draw();

    };

    void DLL_EXPORT Init(int *argc, char **argv, int height, int width, const char *title);
    void DLL_EXPORT SetImgSize(int ih, int iw);
    void DLL_EXPORT SetImg(char ch, const char *bpath);
    void DLL_EXPORT SetScreenSize(int ih, int iw);
    void DLL_EXPORT SetUpdateTimer(int tim);
    void DLL_EXPORT SetUpdateScreenFunc(void(*p)());
    void DLL_EXPORT SetKeyFunc(void(*p)(unsigned char, int, int));
    void DLL_EXPORT SetSpecialFunc(void(*p)(int, int, int));
    void DLL_EXPORT SetMouseFunc(void(*p)(int, int, int, int));
    void DLL_EXPORT DrawScreen(const scene &nowsce);
    void DLL_EXPORT BeginPlay();
    void DLL_EXPORT ChangeXY(int x, int y, int &resx, int &resy);

    int win_width = -1, win_height = -1;
    int time_to_update = -1;
};


#ifdef __cplusplus
}
#endif

#endif // __CTOGUI_H__
