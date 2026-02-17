#include <windows.h>
#include "Lib1.h"

// Создаем разделяемую секцию данных
#pragma data_seg(".SHARED")
    // Инициализированные переменные в разделяемой секции
int x = 0;
int y = 0;
#pragma data_seg()

// Указываем, что секция .SHARED должна быть разделяемой
#pragma comment(linker, "/SECTION:.SHARED,RWS")

int add() { return x + y;}

int sub() {return x - y;}

int mult() {return x * y;}

int div_func() {
    if (y == 0) {
        return 0;
    }
    return x / y;
}

BOOL APIENTRY DllMain(HMODULE hModule,
    DWORD  ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}