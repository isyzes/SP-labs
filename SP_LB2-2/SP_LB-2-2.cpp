// SP_LB-2-2.cpp : Определяет точку входа для приложения.
//
#include <windows.h>
#include <process.h>
#include <string>
#include <memory>
#include "framework.h"
#include "SP_LB-2-2.h"

#define MAX_LOADSTRING 100
const char* NAME = "Игнатович";
const int MAX_THREADS = 2; // Максимальное количество потоков

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Глобальные переменные
HANDLE hSecThread[MAX_THREADS] = { NULL };
DWORD dwSecThreadId[MAX_THREADS] = { 0 };
int g_threadIndices[MAX_THREADS] = { 0 };
HWND g_hWnd; // Дескриптор главного окна
int g_uThCount = 0;
std::clock_t startTimes[MAX_THREADS]; // Время создания потоков

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
void                DrawTextInWindow(HWND hwnd, const char* text, int xPos, int threadIndex);
void                CreateThread(int threadIndex);
void                CreatesleepThread(int threadIndex);
void                SuspendThreadByIndex(int threadIndex);
void                ResumeThreadByIndex(int threadIndex);
void                DestroyThreadByIndex(int threadIndex);
void                IncreaseThreadPriority(int threadIndex);
void                DecreaseThreadPriority(int threadIndex);
void                ShowThreadInfoDialog(int threadIndex);
INT_PTR CALLBACK    DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SPLB22, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLB22));

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

    return (int) msg.wParam;
}


DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    std::unique_ptr<int> pThreadIndex(static_cast<int*>(lpParam));
    int threadIndex = *pThreadIndex;
    int xPos = 10;
    int lineHeight = 50;

    while (true) {
        // Получаем контекст устройства окна
        HDC hdc = GetDC(g_hWnd);

        // Получаем размеры клиентской области
        RECT rect;
        GetClientRect(g_hWnd, &rect);
        int windowWidth = rect.right - rect.left;

        // Очищаем область для этого потока
        RECT clearRect = { 0, threadIndex * lineHeight, windowWidth, (threadIndex + 1) * lineHeight + 20 };
        FillRect(hdc, &clearRect, (HBRUSH)(COLOR_WINDOW + 1));

        std::string message = "Вторичный поток " + std::to_string(threadIndex + 1) + " создал " + std::string(NAME);

        // Рисуем текст
        SetBkMode(hdc, TRANSPARENT); // Прозрачный фон текста
        TextOutA(hdc, xPos, threadIndex * lineHeight + 10, message.c_str(), message.length());

        // Освобождаем контекст устройства
        ReleaseDC(g_hWnd, hdc);

        // Обновляем позицию
        xPos += 2;
        if (xPos > windowWidth) {
            xPos = -300;
        }

        Sleep(50);
    }
    return 0;
}


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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLB22));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLB22);
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

   g_hWnd = hWnd;

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
            case ID_THREAD1_CREATE: {
                CreateThread(0);
                break;
            }
            case ID_THREAD1_WAIT: {
                CreatesleepThread(0);
                break;
            }
            case ID_THREAD1_SUSPEND: {
                SuspendThreadByIndex(0);
                break;
            }
            case ID_THREAD1_CONTINUE: {
                ResumeThreadByIndex(0);
                break;
            }
            case ID_THREAD1_DESTROY: {
                DestroyThreadByIndex(0);
                break;
            }
            case ID_THREAD1_INCREASE_PRIORITY: {
                IncreaseThreadPriority(0);
                break;
            }
            case ID_THREAD1_DECREASE_PRIORITY: {
                DecreaseThreadPriority(0);
                break;
            }




            case ID_THREAD2_CREATE: {
                CreateThread(1);
                break;
            }
            case ID_THREAD2_WAIT: {
                CreatesleepThread(0);
                break;
            }
            case ID_THREAD2_SUSPEND: {
                SuspendThreadByIndex(1);
                break;
            }
            case ID_THREAD2_CONTINUE: {
                ResumeThreadByIndex(1);
                break;
            }
            case ID_THREAD2_DESTROY: {
                DestroyThreadByIndex(1);
                break;
            }
            case ID_THREAD2_INCREASE_PRIORITY: {
                IncreaseThreadPriority(1);
                break;
            }
            case ID_THREAD2_DECREASE_PRIORITY: {
                DecreaseThreadPriority(1);
                break;
            }

            case ID_THREAD1_INFO: {
                ShowThreadInfoDialog(0);
                break;
            }
            case ID_INFO_THREAD2: {
                ShowThreadInfoDialog(1);
                break;
            }



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
    
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}



void IncreaseThreadPriority(int threadIndex) {
    if (threadIndex < 0 || threadIndex >= MAX_THREADS) {
        MessageBox(NULL, TEXT("Неверный индекс потока"), TEXT("Ошибка"), MB_OK);
        return;
    }

    if (hSecThread[threadIndex] == NULL) {
        MessageBox(NULL, TEXT("Поток не существует"), TEXT("Ошибка"), MB_OK);
        return;
    }

    // Получаем текущий приоритет потока
    int currentPriority = GetThreadPriority(hSecThread[threadIndex]);

    if (currentPriority == THREAD_PRIORITY_ERROR_RETURN) {
        MessageBox(NULL, TEXT("Не удалось получить приоритет потока"), TEXT("Ошибка"), MB_OK);
        return;
    }

    // Определяем следующий более высокий приоритет
    int newPriority = currentPriority;

    switch (currentPriority) {
    case THREAD_PRIORITY_IDLE:
        newPriority = THREAD_PRIORITY_LOWEST;
        break;
    case THREAD_PRIORITY_LOWEST:
        newPriority = THREAD_PRIORITY_BELOW_NORMAL;
        break;
    case THREAD_PRIORITY_BELOW_NORMAL:
        newPriority = THREAD_PRIORITY_NORMAL;
        break;
    case THREAD_PRIORITY_NORMAL:
        newPriority = THREAD_PRIORITY_ABOVE_NORMAL;
        break;
    case THREAD_PRIORITY_ABOVE_NORMAL:
        newPriority = THREAD_PRIORITY_HIGHEST;
        break;
    case THREAD_PRIORITY_HIGHEST:
        newPriority = THREAD_PRIORITY_TIME_CRITICAL;
        break;
    case THREAD_PRIORITY_TIME_CRITICAL:
        MessageBox(NULL, TEXT("Поток уже имеет максимальный приоритет"), TEXT("Информация"), MB_OK);
        return;
    default:
        // Если приоритет нестандартный, устанавливаем нормальный
        newPriority = THREAD_PRIORITY_NORMAL;
        break;
    }

    // Устанавливаем новый приоритет
    if (SetThreadPriority(hSecThread[threadIndex], newPriority)) {
        // Формируем сообщение об успехе
        char buffer[256];
        const char* priorityName = "";

        switch (newPriority) {
        case THREAD_PRIORITY_IDLE: priorityName = "Idle (самый низкий)"; break;
        case THREAD_PRIORITY_LOWEST: priorityName = "Lowest"; break;
        case THREAD_PRIORITY_BELOW_NORMAL: priorityName = "Below Normal"; break;
        case THREAD_PRIORITY_NORMAL: priorityName = "Normal"; break;
        case THREAD_PRIORITY_ABOVE_NORMAL: priorityName = "Above Normal"; break;
        case THREAD_PRIORITY_HIGHEST: priorityName = "Highest"; break;
        case THREAD_PRIORITY_TIME_CRITICAL: priorityName = "Time Critical (самый высокий)"; break;
        default: priorityName = "Неизвестный"; break;
        }

        sprintf_s(buffer, "Приоритет потока %d повышен до:\n%s (%d)",
            threadIndex + 1, priorityName, newPriority);
        MessageBoxA(NULL, buffer, "Приоритет изменен", MB_OK | MB_ICONINFORMATION);
    }
    else {
        MessageBox(NULL, TEXT("Не удалось изменить приоритет потока"), TEXT("Ошибка"), MB_OK);
    }
}

void DecreaseThreadPriority(int threadIndex) {
    if (threadIndex < 0 || threadIndex >= MAX_THREADS) {
        MessageBox(NULL, TEXT("Неверный индекс потока"), TEXT("Ошибка"), MB_OK);
        return;
    }

    if (hSecThread[threadIndex] == NULL) {
        MessageBox(NULL, TEXT("Поток не существует"), TEXT("Ошибка"), MB_OK);
        return;
    }

    // Получаем текущий приоритет потока
    int currentPriority = GetThreadPriority(hSecThread[threadIndex]);

    if (currentPriority == THREAD_PRIORITY_ERROR_RETURN) {
        MessageBox(NULL, TEXT("Не удалось получить приоритет потока"), TEXT("Ошибка"), MB_OK);
        return;
    }

    // Определяем следующий более низкий приоритет
    int newPriority = currentPriority;

    switch (currentPriority) {
    case THREAD_PRIORITY_TIME_CRITICAL:
        newPriority = THREAD_PRIORITY_HIGHEST;
        break;
    case THREAD_PRIORITY_HIGHEST:
        newPriority = THREAD_PRIORITY_ABOVE_NORMAL;
        break;
    case THREAD_PRIORITY_ABOVE_NORMAL:
        newPriority = THREAD_PRIORITY_NORMAL;
        break;
    case THREAD_PRIORITY_NORMAL:
        newPriority = THREAD_PRIORITY_BELOW_NORMAL;
        break;
    case THREAD_PRIORITY_BELOW_NORMAL:
        newPriority = THREAD_PRIORITY_LOWEST;
        break;
    case THREAD_PRIORITY_LOWEST:
        newPriority = THREAD_PRIORITY_IDLE;
        break;
    case THREAD_PRIORITY_IDLE:
        // Уже минимальный приоритет
        MessageBox(NULL, TEXT("Поток уже имеет минимальный приоритет"), TEXT("Информация"), MB_OK);
        return;
    default:
        // Если приоритет нестандартный, устанавливаем нормальный
        newPriority = THREAD_PRIORITY_NORMAL;
        break;
    }

    // Устанавливаем новый приоритет
    if (SetThreadPriority(hSecThread[threadIndex], newPriority)) {
        // Формируем сообщение об успехе
        char buffer[256];
        const char* priorityName = "";

        switch (newPriority) {
        case THREAD_PRIORITY_IDLE: priorityName = "Idle (самый низкий)"; break;
        case THREAD_PRIORITY_LOWEST: priorityName = "Lowest"; break;
        case THREAD_PRIORITY_BELOW_NORMAL: priorityName = "Below Normal"; break;
        case THREAD_PRIORITY_NORMAL: priorityName = "Normal"; break;
        case THREAD_PRIORITY_ABOVE_NORMAL: priorityName = "Above Normal"; break;
        case THREAD_PRIORITY_HIGHEST: priorityName = "Highest"; break;
        case THREAD_PRIORITY_TIME_CRITICAL: priorityName = "Time Critical (самый высокий)"; break;
        default: priorityName = "Неизвестный"; break;
        }

        sprintf_s(buffer, "Приоритет потока %d понижен до:\n%s (%d)",
            threadIndex + 1, priorityName, newPriority);
        MessageBoxA(NULL, buffer, "Приоритет изменен", MB_OK | MB_ICONINFORMATION);
    }
    else {
        MessageBox(NULL, TEXT("Не удалось изменить приоритет потока"), TEXT("Ошибка"), MB_OK);
    }
}



void CreatesleepThread(int threadIndex) {
    CreateThread(threadIndex);
    SuspendThread(hSecThread[threadIndex]);
}

void CreateThread(int threadIndex) {
    if (hSecThread[threadIndex] == NULL) {
        // Сохраняем индекс в глобальном массиве
        g_threadIndices[threadIndex] = threadIndex;

        hSecThread[threadIndex] = CreateThread(
            NULL,
            0,
            ThreadFunction,
            &g_threadIndices[threadIndex], // передаем указатель на глобальную переменную
            0,
            &dwSecThreadId[threadIndex]
        );

        if (hSecThread[threadIndex] == NULL) {
            MessageBox(NULL, TEXT("Не удалось создать поток"), TEXT("Ошибка"), MB_OK);
        }
        g_uThCount++;
        startTimes[threadIndex] = std::clock();
    }
    else {
        MessageBox(NULL, TEXT("Поток уже создан"), TEXT("Ошибка"), MB_OK);
    }
}

void WaitThread(int threadIndex) {
    if (hSecThread[threadIndex]) {
        
    }
}

void SuspendThreadByIndex(int threadIndex) {
    if (hSecThread[threadIndex]) {
        SuspendThread(hSecThread[threadIndex]); // Приостановка потока
    }
}

void ResumeThreadByIndex(int threadIndex) {
    if (hSecThread[threadIndex]) {
        ResumeThread(hSecThread[threadIndex]); // Возобновление потока
    }
}

void DestroyThreadByIndex(int threadIndex) {
    if (hSecThread[threadIndex]) {
        TerminateThread(hSecThread[threadIndex], 0); // Завершение потока
        CloseHandle(hSecThread[threadIndex]);
        hSecThread[threadIndex] = NULL;
    }
    g_uThCount--;
}




void DrawTextInWindow(HWND hwnd, const char* text, int xPos, int threadIndex) {
    HDC hdc = GetDC(hwnd);
    TextOutA(hdc, xPos, 30 + (threadIndex * 40), text, strlen(text)); // Вывод текста
    ReleaseDC(hwnd, hdc);
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

void ShowThreadInfoDialog(int threadIndex) {
    HWND hDialog = CreateDialog(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_THREAD_INFO_DIALOG), g_hWnd, DialogProc);
    ShowWindow(hDialog, SW_SHOW);

    wchar_t buffer[256];

    // Идентификатор потока
    swprintf_s(buffer, _countof(buffer), L"%u", dwSecThreadId[threadIndex]);
    SetDlgItemText(hDialog, ID_IDENTIFIER, buffer);

    // Дескриптор потока
    swprintf_s(buffer, _countof(buffer), L"%p", hSecThread[threadIndex]);
    SetDlgItemText(hDialog, ID_DESCRIPTOR, buffer);

    // Состояние потока
    DWORD exitCode;
    GetExitCodeThread(hSecThread[threadIndex], &exitCode);
    swprintf_s(buffer, _countof(buffer), L"Код: %lu", exitCode);
    SetDlgItemText(hDialog, ID_STATUS, buffer);

    // Относительный приоритет потока
    int priority = GetThreadPriority(hSecThread[threadIndex]);
    swprintf_s(buffer, _countof(buffer), L"%d", priority);
    SetDlgItemText(hDialog, ID_PRIORITY, buffer);

    // Время работы потока
    double duration = (std::clock() - startTimes[threadIndex]) / (double)CLOCKS_PER_SEC;
    swprintf_s(buffer, _countof(buffer), L"%.2f секунд", duration);
    SetDlgItemText(hDialog, ID_RUNTIME, buffer);
}

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}