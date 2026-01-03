// SP_PR5.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "SP_PR5.h"
#include <windows.h>
#include <vector>
#include <string>
#include <utility>
#include <tchar.h>
#include <ctime>
#include <commdlg.h> // Для диалога открытия файла
#include <fstream>   // Для работы с файлами

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
std::vector<std::string> globalStrings;         // Глобальный массив строк
char Buffer[1024];                              // Для хранения текстовых данных из файла
char savedBuffer[256] = { 0 };

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    TextDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    ControlDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
    _In_opt_ HINSTANCE hPrevInstance,
    _In_ LPWSTR    lpCmdLine,
    _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SPPR5, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance(hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPPR5));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int)msg.wParam;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPPR5));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_SPPR5);
    wcex.lpszClassName = szWindowClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


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
        case  IDM_VIEW_TEXT: {
            MessageBox(hWnd, TEXT("Выбрана команда Текст."), TEXT("Информация"), MB_OK);
            DialogBox(hInst, MAKEINTRESOURCE(IDD_TEXT_DIALOG), hWnd, TextDlgProc);
            break;
        }
        case IDM_VIEW_CTL: {
            MessageBox(hWnd, TEXT("Выбрана команда Элементы управления."), TEXT("Информация"), MB_OK);
            DialogBox(hInst, MAKEINTRESOURCE(IDD_CONTROL_DIALOG), hWnd, ControlDlgProc);
            break;
        }
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
    static HICON hLogoIcon = NULL;
    static HFONT hFontTitle = NULL;
    static HFONT hFontNormal = NULL;

    switch (message)
    {
    case WM_INITDIALOG:
    {
        // Загрузка иконки для окна
        HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_APPLICATION));
        if (hIcon)
        {
            SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
            SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
        }

        // Загрузка логотипа из ресурсов
        hLogoIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(IDI_SMALL));

        // Создание шрифтов
        LOGFONT lf = { 0 };
        GetObject(GetStockObject(DEFAULT_GUI_FONT), sizeof(LOGFONT), &lf);

        // Шрифт для заголовка
        lf.lfHeight = -18;
        lf.lfWeight = FW_BOLD;
        hFontTitle = CreateFontIndirect(&lf);

        // Шрифт для обычного текста
        lf.lfHeight = -14;
        lf.lfWeight = FW_NORMAL;
        hFontNormal = CreateFontIndirect(&lf);

        return (INT_PTR)TRUE;
    }

    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hDlg, &ps);

        // Получаем размеры окна
        RECT rc;
        GetClientRect(hDlg, &rc);

        // Устанавливаем фоновый режим
        SetBkMode(hdc, TRANSPARENT);

        // === Отрисовка логотипа ===
        if (hLogoIcon)
        {
            // Позиция для логотипа (левый верхний угол)
            DrawIcon(hdc, 20, 20, hLogoIcon);
        }

        // === Название и цель программы ===
        // Заголовок
        HFONT hOldFont = (HFONT)SelectObject(hdc, hFontTitle);
        SetTextColor(hdc, RGB(0, 0, 139)); // Темно-синий

        RECT rcText = { 100, 20, rc.right - 20, 50 };
        DrawText(hdc, TEXT("Моя Программа"), -1, &rcText, DT_LEFT);

        // Обычный текст
        SelectObject(hdc, hFontNormal);
        SetTextColor(hdc, RGB(0, 0, 0)); // Черный

        // === Сведения о разработчике ===
        rcText.top = 100;
        rcText.bottom = 130;
        DrawText(hdc, TEXT("Разработчик: Игнатович Д.В."), -1, &rcText, DT_LEFT);

        rcText.top = 130;
        rcText.bottom = 160;
        DrawText(hdc, TEXT("Группа: 40332"), -1, &rcText, DT_LEFT);
        // === Текущие дата и время ===
        SYSTEMTIME st;
        GetLocalTime(&st);

        TCHAR szDate[50], szTime[50], szDateTime[100];

        // Форматирование даты
        GetDateFormat(LOCALE_USER_DEFAULT, DATE_SHORTDATE, &st, NULL, szDate, 50);

        // Форматирование времени
        GetTimeFormat(LOCALE_USER_DEFAULT, 0, &st, NULL, szTime, 50);

        // Объединение даты и времени
        wsprintf(szDateTime, TEXT("Текущие дата и время: %s %s"), szDate, szTime);

        rcText.top = 160;
        rcText.bottom = 190;
        DrawText(hdc, szDateTime, -1, &rcText, DT_LEFT);


        EndPaint(hDlg, &ps);
        return (INT_PTR)TRUE;
    }

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;

    case WM_CTLCOLORDLG:
        // Устанавливаем цвет фона диалога
        return (INT_PTR)GetStockObject(WHITE_BRUSH);

    case WM_CTLCOLORSTATIC:
        // Устанавливаем цвет фона для статических элементов
        SetBkMode((HDC)wParam, TRANSPARENT);
        return (INT_PTR)GetStockObject(WHITE_BRUSH);

    case WM_DESTROY:
        // Освобождение ресурсов
        if (hFontTitle)
            DeleteObject(hFontTitle);
        if (hFontNormal)
            DeleteObject(hFontNormal);
        if (hLogoIcon)
            DestroyIcon(hLogoIcon);
        break;

    case WM_ERASEBKGND:
        // Заполняем фон окна белым цветом
        RECT rc;
        GetClientRect(hDlg, &rc);
        HBRUSH hBrush = CreateSolidBrush(RGB(255, 255, 255));
        FillRect((HDC)wParam, &rc, hBrush);
        DeleteObject(hBrush);
        return (INT_PTR)TRUE;
    }

    return (INT_PTR)FALSE;
}

// Обработчик для диалогового окна загрузки текста
INT_PTR CALLBACK TextDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG: {
        // При загрузке диалога установка начального текста
        SetDlgItemTextA(hDlg, IDC_EDIT_TEXT, "Шаг 1. Начало работы");
        return TRUE;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == ID_LOAD_BUTTON) {
            OPENFILENAME ofn; // Диалог открытия файла
            TCHAR szFile[260];  // Имя файла
            memset(&ofn, 0, sizeof(ofn));
            ofn.lStructSize = sizeof(ofn);
            ofn.hwndOwner = hDlg;
            ofn.lpstrFile = szFile;
            ofn.lpstrFile[0] = '\0';
            ofn.nMaxFile = sizeof(szFile);
            ofn.lpstrFilter = L"Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0";
            ofn.nFilterIndex = 1;
            ofn.lpstrFileTitle = NULL;
            ofn.nMaxFileTitle = 0;
            ofn.lpstrInitialDir = NULL;
            ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

            // Показать диалог открытия файла
            if (GetOpenFileName(&ofn)) {
                std::ifstream file(ofn.lpstrFile);
                if (file.is_open()) {
                    file.read(Buffer, sizeof(Buffer));
                    Buffer[file.gcount()] = '\0'; // Завершение строки
                    SetDlgItemTextA(hDlg, IDC_EDIT_TEXT, Buffer);
                    file.close();
                }
            }
        }
        if (LOWORD(wParam) == IDOK) {
            EndDialog(hDlg, IDOK);
            return TRUE;
        }
        if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
        
        break;
    }
    return FALSE;
}


// Обработчик для диалогового окна управления
INT_PTR CALLBACK ControlDlgProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        return TRUE;
    case WM_COMMAND:
        if (LOWORD(wParam) == ID_READ_BUTTON) {
            char buffer[256];
            GetDlgItemTextA(hDlg, IDC_EDIT_CONTROL, buffer, sizeof(buffer));

        }
        else if (LOWORD(wParam) == ID_ADD_BUTTON) {
            char buffer[256];
            GetDlgItemTextA(hDlg, IDC_EDIT_CONTROL, buffer, sizeof(buffer));
            globalStrings.push_back(buffer); // Добавление строки в глобальный массив
            SendDlgItemMessageA(hDlg, IDC_LISTBOX_CONTROL, LB_ADDSTRING, 0, (LPARAM)buffer);
        }
    
        else if (LOWORD(wParam) == IDOK) {
            int count = SendDlgItemMessageA(hDlg, IDC_LISTBOX_CONTROL, LB_GETCOUNT, 0, 0);
            for (int i = 0; i < count; i++) {
                char buffer[256];
                SendDlgItemMessageA(hDlg, IDC_LISTBOX_CONTROL, LB_GETTEXT, i, (LPARAM)buffer);
                globalStrings.push_back(buffer); // Добавление в массив
            }
            EndDialog(hDlg, IDOK);
            InvalidateRect(GetParent(hDlg), NULL, TRUE);
            return TRUE;
        }
        else if (LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, IDCANCEL);
            return TRUE;
        }
        break;
    }
    return FALSE;
}