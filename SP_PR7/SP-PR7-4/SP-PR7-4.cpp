#include <windows.h>
#include <gdiplus.h>
#include <tchar.h>
#include "resource.h"  // Включаем файл ресурсов

#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

// Глобальные переменные
HINSTANCE hInst;
HBITMAP hBmp = NULL; // Дескриптор загруженного изображения
ULONG_PTR gdiplusToken;

// Функции
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void LoadBitmapFromResource(int resourceID);

// Функция инициализации GDI+
void InitializeGDIPlus() {
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
}

// Функция завершения работы GDI+
void ShutdownGDIPlus() {
    GdiplusShutdown(gdiplusToken);
}

// Функция WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    hInst = hInstance;
    InitializeGDIPlus();

    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("MyWindowClass");
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    RegisterClass(&wc);

    HWND hWnd = CreateWindowEx(
        0, TEXT("MyWindowClass"), TEXT("Draw Image"),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        800, 600, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    ShutdownGDIPlus();
    return (int)msg.wParam;
}

// Оконная процедура
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hDC = BeginPaint(hWnd, &ps);
        if (hBmp) {
            HDC hCompDC = CreateCompatibleDC(hDC);
            HGDIOBJ hOldBmp = SelectObject(hCompDC, hBmp);
            BITMAP bmp;
            GetObject(hBmp, sizeof(BITMAP), &bmp);
            StretchBlt(hDC, 0, 0, bmp.bmWidth, bmp.bmHeight, hCompDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
            SelectObject(hCompDC, hOldBmp);
            DeleteDC(hCompDC);
        }
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_LBUTTONDOWN: {
        // Загрузка изображения из ресурсов при нажатии кнопки
        LoadBitmapFromResource(IDB_BITMAP1);
        InvalidateRect(hWnd, NULL, TRUE); // Перерисовываем окно
        break;
    }

    case WM_DESTROY: {
        if (hBmp) DeleteObject(hBmp);
        PostQuitMessage(0);
        break;
    }

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}

// Функция загрузки битмапа из ресурсов
void LoadBitmapFromResource(int resourceID) {
    if (hBmp) {
        DeleteObject(hBmp); // Освобождаем предыдущий объект
    }
    hBmp = LoadBitmap(hInst, MAKEINTRESOURCE(resourceID));
}