#include <iostream>
#include "D:\project\cpp\SP\SP_LB2-4\IGNATOVICH_DLL\DLLProject.h"


int main() {
    int a = 5, b = 10, c = 2;
    double result;

    float average = Fun41(a, b);
    std::cout << "Average of " << a << " and " << b << " is: " << average << std::endl;

    float product = Fun42(a);
    std::cout << "Product of " << a << ", " << b << ", and " << c << " is: " << product << std::endl;

    Fun43(4.0, &result);
    std::cout << "Double of 4.0 is: " << result << std::endl;

    std::cout << "Total DLL calls: " << g_nDllCallsCount << std::endl;
    std::cout << "Function calls count: " << g_nFnCallsCount << std::endl;

    return 0;
}