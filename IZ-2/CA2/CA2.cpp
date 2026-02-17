#include <iostream>
#include <windows.h>

typedef int (*MathFunc)();
typedef int (*GetFunc)();

int main()
{
    HINSTANCE hLib = LoadLibraryA("Lib1.dll");

    if (!hLib) {
        std::cerr << "Error: Failed to load Lib1.dll!" << std::endl;
        std::cout << "Press Enter to exit..." << std::endl;
        return 1;
    }

    // Получаем указатели на функции
    MathFunc add = (MathFunc)GetProcAddress(hLib, "add");
    MathFunc sub = (MathFunc)GetProcAddress(hLib, "sub");
    MathFunc mult = (MathFunc)GetProcAddress(hLib, "mult");
    MathFunc div = (MathFunc)GetProcAddress(hLib, "div_func");
    // Получаем указатели на переменные
    int* pX = (int*)GetProcAddress(hLib, "x");
    int* pY = (int*)GetProcAddress(hLib, "y");

    std::cout << "add(): " << *pX << " + " << *pY << " = " << add() << std::endl;
    std::cout << "sub(): " << *pX << " - " << *pY << " = " << sub() << std::endl;
    std::cout << "mult(): " << *pX << " * " << *pY << " = " << mult() << std::endl;
    std::cout << "div(): " << *pX << " / " << *pY << " = " << div() << std::endl;

    std::cout << "Press Enter to exit..." << std::endl;
    std::cin.get();
}
