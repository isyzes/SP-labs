#include <iostream>
#include <Windows.h>

// Объявления
typedef float(*Fun41_t)(int, int);
typedef double(__stdcall* Fun42_t)(int);
typedef void(*Fun43_t)(double, double*);

int main() {
    // Загружаем DLL 

    HMODULE hDll = LoadLibrary(L"D:\\project\\cpp\\SP\\SP_LB2-4\\x64\\Debug\\IGNATOVICH_DLL.dll");

    if (!hDll) {
        std::cerr << "Failed to load DLL. Error: " << GetLastError() << std::endl;
        return 1;
    }

    // Получаем указатели на функции
    Fun41_t pFun41 = (Fun41_t)GetProcAddress(hDll, "Fun41");
    Fun42_t pFun42 = (Fun42_t)GetProcAddress(hDll, "Fun42");
    Fun43_t pFun43 = (Fun43_t)GetProcAddress(hDll, "Fun43");

    // Получаем указатели на глобальные переменные
    volatile int* pDllCallsCount = (volatile int*)GetProcAddress(hDll, "g_nDllCallsCount");
    volatile int* pFnCallsCount = (volatile int*)GetProcAddress(hDll, "g_nFnCallsCount");

    if (!pFun41 || !pFun42 || !pFun43 || !pDllCallsCount || !pFnCallsCount) {
        std::cerr << "Failed to get function addresses" << std::endl;
        FreeLibrary(hDll);
        return 1;
    }

    // Использование функций
    int a = 5, b = 10, c = 2;
    double result;

    float average = pFun41(a, b);
    std::cout << "Average of " << a << " and " << b << " is: " << average << std::endl;

    double product = pFun42(a);
    std::cout << "Product of " << a << ", " << a << ", and " << a << " is: " << product << std::endl;

    pFun43(4.0, &result);
    std::cout << "Double of 4.0 is: " << result << std::endl;

    std::cout << "Total DLL calls: " << *pDllCallsCount << std::endl;
    std::cout << "Function calls count: " << *pFnCallsCount << std::endl;

    FreeLibrary(hDll);
    return 0;
}