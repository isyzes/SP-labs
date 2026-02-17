#include <iostream>
#include <windows.h>

typedef void (*SetValuesFunc)(int, int);
typedef int (*GetFunc)();

int main()
{
    HINSTANCE hLib = LoadLibraryA("Lib1.dll");

    if (!hLib) {
        std::cerr << "Error: Failed to load Lib1.dll!" << std::endl;
        std::cout << "Press Enter to exit..." << std::endl;
        return 1;
    }

    // Получаем указатели на переменные из DLL
    int* pX = (int*)GetProcAddress(hLib, "x");
    int* pY = (int*)GetProcAddress(hLib, "y");

    // Ввод новых значений
    std::cout << "Enter the new value of x: ";
    std::cin >> *pX;

    std::cout << "Enter the new value of y: ";
    std::cin >> *pY;

    std::cout << "Values updated successfully!" << std::endl;
    std::cout << "Values: x = " << *pX << ", y = " << *pY << std::endl;


    std::cout << "\nPress Enter to exit...";
    std::cin.get();
    std::cin.get();

    return 0;
}