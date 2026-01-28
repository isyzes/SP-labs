// dllmain.cpp : Определяет точку входа для приложения DLL.
#include "framework.h"
#include <iostream>
#include <sstream>

// Определяем экспортируемые переменные
extern "C" __declspec(dllexport) int g_nDllCallsCount = 0;
extern "C" __declspec(dllexport) int g_nFnCallsCount = 0;

void OutputDebugMessage(const std::string& message) {
#ifdef _DEBUG
    OutputDebugStringA(message.c_str());
#endif
    // Также можно выводить в консоль или файл
    std::cout << message << std::endl;
}

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    const char* moduleName = "DLLProject.dll";
    const char* authorName = "IGNATOVICH";

    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH: {
        std::stringstream ss;
        ss << "Library is loading " << moduleName
            << ". Project " << authorName << std::endl;
        OutputDebugMessage(ss.str());

        // Увеличиваем счетчик загрузок (разделяемая переменная)
        InterlockedIncrement((LONG*)&g_nDllCallsCount);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

extern "C" {

    __declspec(dllexport) float Fun41(int a, int b) {
        g_nDllCallsCount++;
        g_nFnCallsCount++;
        return static_cast<float>(a + b) / 2;
    }

    __declspec(dllexport) double WINAPI Fun42(int a) {
        g_nDllCallsCount++;
        g_nFnCallsCount++;
        return static_cast<double>(a * a * a);
    }

    __declspec(dllexport) void Fun43(double in, double* out) {
        g_nDllCallsCount++;
        g_nFnCallsCount++;
        *out = in * 2.0;
    }
}