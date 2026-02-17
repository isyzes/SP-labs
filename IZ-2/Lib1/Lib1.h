#pragma once

#define WIN32_LEAN_AND_MEAN
#include <windows.h>

extern "C" {
    __declspec(dllexport) extern int x;
    __declspec(dllexport) extern int y;

    __declspec(dllexport) int add();
    __declspec(dllexport) int sub();
    __declspec(dllexport) int mult();
    __declspec(dllexport) int div_func();
}