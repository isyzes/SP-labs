#pragma once

#include <windows.h>

#ifdef DLLPROJECT_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

extern "C" {
    // Экспортируемые функции
    DLL_API float Fun41(int a, int b);
    DLL_API double WINAPI Fun42(int x);
    DLL_API void Fun43(double in, double* out);

    // Экспортируемые глобальные переменные
    extern DLL_API int g_nDllCallsCount;
    extern DLL_API int g_nFnCallsCount;
}