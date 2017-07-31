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


