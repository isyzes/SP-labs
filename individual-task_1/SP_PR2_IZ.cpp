#include <windows.h>
#include <string>
#include <sstream>

// Прототипы функций
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void UpdateClientRect(HWND hwnd);
void DrawTextCentered(HDC hdc, LPRECT rect, LPCWSTR text);

// Глобальные переменные для хранения координат
RECT g_clientRect;
HWND g_hwnd;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow) {
    static TCHAR szAppName[] = L"ClientRectApp";
    HWND hwnd;
    MSG msg;
    WNDCLASSEX wndclass;

    wndclass.cbSize = sizeof(wndclass);
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.lpfnWndProc = WndProc;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hInstance = hInstance;
    wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wndclass.lpszMenuName = NULL;
    wndclass.lpszClassName = szAppName;
    wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    RegisterClassEx(&wndclass);

    hwnd = CreateWindow(
        szAppName,
        L"Client Area Coordinates",
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        400,
        300,
        NULL,
        NULL,
        hInstance,
        NULL
    );

    g_hwnd = hwnd;

    ShowWindow(hwnd, iCmdShow);
    UpdateWindow(hwnd);

    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_CREATE: {
        // Создаем кнопку "Выход"
        CreateWindow(
            L"BUTTON",
            L"Выход",
            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            10, 10, 80, 30,
            hwnd,
            (HMENU)1,
            (HINSTANCE)GetWindowLongPtr(hwnd, GWLP_HINSTANCE),
            NULL
        );
        UpdateClientRect(hwnd);
        return 0;
    }

    case WM_SIZE:
        UpdateClientRect(hwnd);
        InvalidateRect(hwnd, NULL, TRUE);
        return 0;

    case WM_COMMAND:
        if (LOWORD(wParam) == 1) {
            PostQuitMessage(0);
        }
        return 0;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);

        // Формируем строку с координатами
        std::wstringstream ss;
        ss << L"Левый верхний: (" << g_clientRect.left << L", " << g_clientRect.top << L")\n"
            << L"Правый нижний: (" << g_clientRect.right << L", " << g_clientRect.bottom << L")";

        // Рисуем текст с выравниванием
        RECT textRect = g_clientRect;
        DrawTextCentered(hdc, &textRect, ss.str().c_str());

        EndPaint(hwnd, &ps);
        return 0;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    }

    return DefWindowProc(hwnd, message, wParam, lParam);
}

void UpdateClientRect(HWND hwnd) {
    GetClientRect(hwnd, &g_clientRect);
}

void DrawTextCentered(HDC hdc, LPRECT rect, LPCWSTR text) {
    // Устанавливаем выравнивание влево
    SetTextAlign(hdc, TA_LEFT);

    // Рассчитываем вертикальную позицию
    SIZE textSize;
    GetTextExtentPoint32(hdc, text, lstrlen(text), &textSize);

    int yPos = (rect->bottom - rect->top - textSize.cy) / 2;

    // Рисуем текст
    TextOut(hdc, rect->left, yPos, text, lstrlen(text));
}