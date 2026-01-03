#include <windows.h>
#include <tchar.h>

// Глобальные переменные
HINSTANCE hInst;

// Предварительное объявление функции оконной процедуры
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// Точка входа в программу
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;

    // Регистрация класса окна
    WNDCLASS wc = { 0 };
    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = TEXT("MyWindowClass");
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    // Создание окна
    HWND hWnd = CreateWindowEx(
        0, TEXT("MyWindowClass"), TEXT("SP_PR7 Application"),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        900, 400, NULL, NULL, hInstance, NULL);

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Основной цикл обработки сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Оконная процедура
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);

        // Настройка шрифта
        HFONT hFont = CreateFont(
            -MulDiv(36, GetDeviceCaps(hdc, LOGPIXELSY), 72),    // Высота шрифта в пикселях
            0,                                                  // Ширина шрифта (0 для авто)
            0,                                                  // Наклон шрифта
            0,                                                  // Начертание
            FW_BOLD,                                            // Жирный
            TRUE,                                               // Курсив
            FALSE,                                              // Подчеркнутый
            0,                                                  // Захват символов
            DEFAULT_CHARSET,                                    // Набор символов
            OUT_DEFAULT_PRECIS,                                 // Прецизионный вывод
            CLIP_DEFAULT_PRECIS,                                // Стандартный вывод
            DEFAULT_QUALITY,                                    // Качество
            DEFAULT_QUALITY,                                    // Упорядочение
            TEXT("Times New Roman"));                           // Гарнитура

        SelectObject(hdc, hFont);
        SetTextColor(hdc, RGB(110, 110, 110));                  // Серый

        // Вывод текста
        TextOut(hdc, 50, 50, TEXT("Иганотович Дмитрий Вячеславович"), lstrlen(TEXT("Иганотович Дмитрий Вячеславович ")));

        // Освобождение ресурсов
        DeleteObject(hFont);
        EndPaint(hWnd, &ps);
        break;
    }
    case WM_DESTROY: {
        PostQuitMessage(0);
        break;
    }
    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
    return 0;
}