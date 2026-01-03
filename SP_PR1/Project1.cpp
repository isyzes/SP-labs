// Project1.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "Project1.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    LPSTR lpszCmdLine, int nCmdShow)
{
    // Получаем текущее время
    SYSTEMTIME st;
    GetLocalTime(&st);

    // Форматируем строку
    wchar_t message[256];
    wsprintfW(message, L"Привет из Win32 приложения с графическим интерфейсом.\nАвтор: <Игнатович, 40332>\nDesign time = %02d.%02d.%04d, %02d:%02d:%02d\nRun time = %02d.%02d.%04d, %02d:%02d:%02d",
        st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond,
        st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);

    // Переменные для обработки сообщений
    int iRetValue1, iRetValue2;
    const wchar_t* lpszMBTitle = L"Изучаем MessageBox";
    const wchar_t* lpszResponce;

    // Цикл для обработки первого окна сообщения
    do {
        iRetValue1 = MessageBox(NULL, L"Нажмите одну из кнопок.",
            lpszMBTitle, MB_ABORTRETRYIGNORE | MB_ICONEXCLAMATION | MB_SETFOREGROUND);
        switch (iRetValue1) {
        case IDABORT:
            lpszResponce = L"Нажата кнопка Прервать (ABORT)";
            break;
        case IDRETRY:
            lpszResponce = L"Нажата кнопка Повторить (RETRY)";
            break;
        case IDIGNORE:
            lpszResponce = L"Нажата кнопка Пропустить (IGNORE)";
            break;
        default:
            lpszResponce = L"Ответ мне не понятен.";
        }

        // Формируем сообщение для второго окна
        wchar_t buf[200] = L"";
        wcscat_s(buf, sizeof(buf) / sizeof(buf[0]), lpszResponce);
        wcscat_s(buf, sizeof(buf) / sizeof(buf[0]), L"\nПродолжить изучение возвращаемых значений?");

        // Второе окно сообщения с константами стиля
        iRetValue2 = MessageBox(NULL, buf, lpszMBTitle,
            MB_YESNO | MB_ICONERROR | MB_SETFOREGROUND);
    } while (iRetValue2 != IDNO);

    // Выводим финальное сообщение с информацией о времени
    MessageBoxW(NULL, message, L"Сообщение", MB_OK | MB_ICONINFORMATION | MB_DEFBUTTON2);

    return 0;
}


//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
//    LPSTR lpszCmdLine, int nCmdShow)
//{
//    // Получаем текущее время
//    SYSTEMTIME st;
//    GetLocalTime(&st);
//
//    // Форматируем строку
//    wchar_t message[256];
//    wsprintfW(message, L"Привет из Win32 приложения с графическим интерфейсом.\nАвтор: <Игнатович, 40332>\nDesign time = %02d.%02d.%04d, %02d:%02d:%02d\nRun time = %02d.%02d.%04d, %02d:%02d:%02d",
//        st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond,
//        st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
//
//    // Выводим сообщение с различными стилями и изображениями
//    MessageBoxW(NULL, message, L"Сообщение", MB_OK | MB_ICONINFORMATION | MB_DEFBUTTON2);
//
//    return 0;
//}


//int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
//    LPSTR lpszCmdLine, int nCmdShow)
//{
//    // Получаем текущее время
//    SYSTEMTIME st;
//    GetLocalTime(&st);
//
//    // Форматируем строку
//    wchar_t message[256];
//    wsprintfW(message, L"Привет из Win32 приложения с графическим интерфейсом.\nАвтор: <Игнатович, 40332>\nDesign time = %02d.%02d.%04d, %02d:%02d:%02d\nRun time = %02d.%02d.%04d, %02d:%02d:%02d",
//        st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond,
//        st.wDay, st.wMonth, st.wYear, st.wHour, st.wMinute, st.wSecond);
//
//    // Выводим сообщение
//    MessageBoxW(NULL, message, L"Сообщение", MB_OK);
//    return 0;
//}


//int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
//                     _In_opt_ HINSTANCE hPrevInstance,
//                     _In_ LPWSTR    lpCmdLine,
//                     _In_ int       nCmdShow)
//{
//    UNREFERENCED_PARAMETER(hPrevInstance);
//    UNREFERENCED_PARAMETER(lpCmdLine);
//
//    // TODO: Разместите код здесь.
//
//    // Инициализация глобальных строк
//    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
//    LoadStringW(hInstance, IDC_PROJECT1, szWindowClass, MAX_LOADSTRING);
//    MyRegisterClass(hInstance);
//
//    // Выполнить инициализацию приложения:
//    if (!InitInstance (hInstance, nCmdShow))
//    {
//        return FALSE;
//    }
//
//    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_PROJECT1));
//
//    MSG msg;
//
//    // Цикл основного сообщения:
//    while (GetMessage(&msg, nullptr, 0, 0))
//    {
//        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
//        {
//            TranslateMessage(&msg);
//            DispatchMessage(&msg);
//        }
//    }
//
//    return (int) msg.wParam;
//}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_PROJECT1));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_PROJECT1);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Разобрать выбор в меню:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Добавьте сюда любой код прорисовки, использующий HDC...
            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Обработчик сообщений для окна "О программе".
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
