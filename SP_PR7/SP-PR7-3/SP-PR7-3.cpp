#include <windows.h>
#include <commdlg.h>
#include <gdiplus.h>
#include <tchar.h>
#include <iostream>

#pragma comment(lib, "gdiplus.lib")
using namespace Gdiplus;

// Глобальные переменные
HINSTANCE hInst;
HBITMAP hBmp = NULL; // Дескриптор загруженного изображения
ULONG_PTR gdiplusToken;

// Прототипы функций
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void LoadBitmapImage(const TCHAR* filePath);

// Функция инициализации GDI+
void InitializeGDIPlus(ULONG_PTR& gdiplusToken) {
    GdiplusStartupInput gdiplusStartupInput;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);
}

// Функция завершения работы GDI+
void ShutdownGDIPlus() {
    GdiplusShutdown(gdiplusToken);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int nCmdShow) {
    ULONG_PTR gdiplusToken;
    InitializeGDIPlus(gdiplusToken);

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
            RECT rc;
            GetClientRect(hWnd, &rc);
            StretchBlt(hDC, 0, 0, rc.right, rc.bottom, hCompDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);
            SelectObject(hCompDC, hOldBmp);
            DeleteDC(hCompDC);
        }
        EndPaint(hWnd, &ps);
        break;
    }

    case WM_LBUTTONDOWN: {
        // Загрузка изображения
        OPENFILENAME ofn;
        TCHAR szFile[260];

        // Настройка диалога
        ZeroMemory(&ofn, sizeof(ofn));
        ofn.lStructSize = sizeof(ofn);
        ofn.hwndOwner = hWnd;
        ofn.lpstrFile = szFile;
        ofn.lpstrFile[0] = '\0';
        ofn.nMaxFile = sizeof(szFile);
        ofn.lpstrFilter = TEXT("Bitmap Files (*.bmp)\0*.bmp\0All Files (*.*)\0*.*\0");
        ofn.nFilterIndex = 1;
        ofn.lpstrFileTitle = NULL;
        ofn.nMaxFileTitle = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

        // Открытие диалога
        if (GetOpenFileName(&ofn)) {
            LoadBitmapImage(ofn.lpstrFile);
            InvalidateRect(hWnd, NULL, TRUE); // Перерисовываем окно
        }
        break;
    }

    case WM_KEYDOWN: {
        if (wParam == 'S') {
            // Сохранить метафайл
            OPENFILENAME ofn;
            TCHAR szFile[260];

            // Настройка диалога
            ZeroMemory(&ofn, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hWnd;
            ofn.lpstrFile = szFile;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = TEXT("Metafile (*.wmf)\0*.wmf\0All Files (*.*)\0*.*\0");
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_OVERWRITEPROMPT;

            // Открытие диалога
            if (GetSaveFileName(&ofn)) {
                // Сохраняем метафайл
                HDC hdc = CreateMetaFile(ofn.lpstrFile);
                if (hdc != NULL) {
                    HDC hCompDC = CreateCompatibleDC(hdc);
                    HGDIOBJ hOldBmp = SelectObject(hCompDC, hBmp);

                    BITMAP bmp;
                    GetObject(hBmp, sizeof(BITMAP), &bmp);
                    StretchBlt(hdc, 0, 0, bmp.bmWidth, bmp.bmHeight, hCompDC, 0, 0, bmp.bmWidth, bmp.bmHeight, SRCCOPY);

                    SelectObject(hCompDC, hOldBmp);
                    DeleteDC(hCompDC);
                    CloseMetaFile(hdc);
                }
            }
        }
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

// Функция загрузки битмапа
void LoadBitmapImage(const TCHAR* filePath) {
    if (hBmp) {
        DeleteObject(hBmp); // Освобождаем предыдущий объект
    }
    hBmp = (HBITMAP)LoadImage(NULL, filePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);
}