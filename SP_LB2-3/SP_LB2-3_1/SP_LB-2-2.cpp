#include <windows.h>
#include <process.h>
#include <string>
#include <ctime>
#include <vector>
#include "resource.h"  

const char* NAME = "Игатович";
const int MAX_THREADS = 5;
const int STRING_LENGTH = 50;
const int N = 2;

// Глобальные переменные
HANDLE hSecThread[MAX_THREADS] = { NULL };
DWORD dwSecThreadId[MAX_THREADS] = { 0 };
UINT g_uThCount = 0;
HWND g_hWnd;
HANDLE hMutex = NULL;
int runCounters[MAX_THREADS] = { 0 };
BOOL isSynchronized = TRUE; 

struct ThreadData {
    int threadIndex;
    int* runCounter;
};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
DWORD WINAPI ThreadFunction(LPVOID lpParam);
void DrawTextInWindow(HWND hwnd, const char* text, int xPos, int threadIndex, int runCount);
void CreateThread();
void DestroyThreadByIndex(int threadIndex);
void ShowThreadInfoDialog(int threadIndex);
INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);


DWORD WINAPI ThreadFunction(LPVOID lpParam) {
    ThreadData* data = (ThreadData*)lpParam;
    int threadIndex = data->threadIndex;
    int* runCounter = data->runCounter;

    std::string message = "Вторичный поток " + std::to_string(threadIndex + 1) + " создал: " + std::string(NAME);

    while (true) {
        if (isSynchronized && hMutex != NULL) {
            DWORD waitResult = WaitForSingleObject(hMutex, INFINITE);

            if (waitResult == WAIT_OBJECT_0) {
                for (int iteration = 0; iteration < N; iteration++) {
                    for (int shift = 0; shift < STRING_LENGTH; shift++) {
                        DrawTextInWindow(g_hWnd, message.c_str(), shift, threadIndex, *runCounter);
                        Sleep(100);
                    }

                    (*runCounter)++;
                    Sleep(300);
                }

                ReleaseMutex(hMutex);
                Sleep(1000);
            }
            else {
                MessageBoxA(NULL, "Ошибка ожидания мьютекса", "Ошибка", MB_OK);
                break;
            }
        }
        else {
            for (int iteration = 0; iteration < N; iteration++) {
                for (int shift = 0; shift < STRING_LENGTH; shift++) {
                    DrawTextInWindow(g_hWnd, message.c_str(), shift, threadIndex, *runCounter);
                    Sleep(100);
                }

                (*runCounter)++;
                Sleep(300);
            }

            Sleep(1000);
        }

        if (WaitForSingleObject(hSecThread[threadIndex], 0) == WAIT_OBJECT_0) {
            break;
        }
    }

    delete data;
    return 0;
}

void DrawTextInWindow(HWND hwnd, const char* text, int xPos, int threadIndex, int runCount) {
    HDC hdc = GetDC(hwnd);

    RECT clearRect;
    clearRect.left = 0;
    clearRect.top = 30 + (threadIndex * 60);
    clearRect.right = 800;
    clearRect.bottom = 30 + (threadIndex * 60) + 50;
    FillRect(hdc, &clearRect, (HBRUSH)(COLOR_WINDOW + 1));

    TextOutA(hdc, xPos, 30 + (threadIndex * 60), text, strlen(text));

    char counterText[100];
    sprintf_s(counterText, "Пробежек: %d", runCount);
    TextOutA(hdc, 400, 30 + (threadIndex * 60), counterText, strlen(counterText));

    ReleaseDC(hwnd, hdc);
}

void CreateThread() {
    if (g_uThCount < MAX_THREADS) {
        int threadIndex = g_uThCount;

        ThreadData* data = new ThreadData;
        data->threadIndex = threadIndex;
        data->runCounter = &runCounters[threadIndex];

        hSecThread[threadIndex] = CreateThread(
            NULL,
            0,
            ThreadFunction,
            data,
            0,
            &dwSecThreadId[threadIndex]
        );

        if (hSecThread[threadIndex] == NULL) {
            MessageBox(NULL, TEXT("Не удалось создать поток"), TEXT("Ошибка"), MB_OK);
            delete data;
        }
        else {
            g_uThCount++;

            char debugMsg[100];
            sprintf_s(debugMsg, "Создан поток %d (ID: %lu)\n", threadIndex + 1, dwSecThreadId[threadIndex]);
            OutputDebugStringA(debugMsg);
        }
    }
    else {
        MessageBox(NULL, TEXT("Достигнуто максимальное количество потоков"), TEXT("Ошибка"), MB_OK);
    }
}

void DestroyThreadByIndex(int threadIndex) {
    if (threadIndex < MAX_THREADS && hSecThread[threadIndex]) {
        TerminateThread(hSecThread[threadIndex], 0);
        CloseHandle(hSecThread[threadIndex]);

        for (int i = threadIndex; i < g_uThCount - 1; i++) {
            hSecThread[i] = hSecThread[i + 1];
            dwSecThreadId[i] = dwSecThreadId[i + 1];
            runCounters[i] = runCounters[i + 1];
        }

        hSecThread[g_uThCount - 1] = NULL;
        dwSecThreadId[g_uThCount - 1] = 0;
        runCounters[g_uThCount - 1] = 0;

        g_uThCount--;
    }
}

void ShowThreadInfoDialog(int threadIndex) {
    if (threadIndex < g_uThCount) {
        HWND hDialog = CreateDialog(
            GetModuleHandle(NULL),
            MAKEINTRESOURCE(IDD_THREAD_INFO_DIALOG),
            g_hWnd,
            DialogProc
        );
        ShowWindow(hDialog, SW_SHOW);

        wchar_t buffer[256];
        swprintf_s(buffer, sizeof(buffer), L"ID потока: %u", dwSecThreadId[threadIndex]);
        SetDlgItemText(hDialog, ID_IDENTIFIER, buffer);

        swprintf_s(buffer, sizeof(buffer), L"Дескриптор: %p", hSecThread[threadIndex]);
        SetDlgItemText(hDialog, ID_DESCRIPTOR, buffer);

       
    }
}

INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL) {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE, PWSTR pCmdLine, int nCmdShow) {
    const wchar_t CLASS_NAME[] = L"MyWindowClass";
    WNDCLASS wc = {};
    wc.lpfnWndProc = WindowProc;
    wc.hInstance = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);

    hMutex = CreateMutex(
        NULL,
        FALSE,
        L"Global\\MyRunningStringMutex"
    );

    if (hMutex == NULL) {
        MessageBox(NULL, TEXT("Не удалось создать мьютекс"), TEXT("Ошибка"), MB_OK);
        return 1;
    }

    if (GetLastError() == ERROR_ALREADY_EXISTS) {
        OutputDebugString(L"Мьютекс уже существует, присоединяемся...\n");
    }

    g_hWnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Потоки в Win32 - Бегущая строка",
        WS_OVERLAPPEDWINDOW | WS_VISIBLE,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        800,
        400,
        nullptr,
        LoadMenu(hInstance, MAKEINTRESOURCE(IDC_SPLB22)),
        hInstance,
        nullptr
    );

    if (g_hWnd == NULL) {
        MessageBox(NULL, TEXT("Не удалось создать окно"), TEXT("Ошибка"), MB_OK);
        CloseHandle(hMutex);
        return 1;
    }

    ShowWindow(g_hWnd, nCmdShow);
    UpdateWindow(g_hWnd);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    for (int i = 0; i < MAX_THREADS; i++) {
        if (hSecThread[i]) {
            DestroyThreadByIndex(i);
        }
    }

    if (hMutex) {
        CloseHandle(hMutex);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch (uMsg) {
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case ID_THREAD1_CREATE:
        case ID_THREAD2_CREATE:
            CreateThread();
            break;

        case ID_THREAD1_DESTROY:
            DestroyThreadByIndex(0);
            break;

        case ID_THREAD2_DESTROY:
            DestroyThreadByIndex(1);
            break;

        case ID_SYNC_ENABLE:
            isSynchronized = TRUE;

            if (hMutex == NULL) {
                hMutex = CreateMutex(NULL, FALSE, L"Global\\MyRunningStringMutex");
                if (hMutex == NULL) {
                    MessageBox(hwnd, TEXT("Не удалось создать мьютекс"), TEXT("Ошибка"), MB_OK);
                    break;
                }
            }

            MessageBox(hwnd, TEXT("Синхронизация включена"), TEXT("Информация"), MB_OK);
            break;

        case ID_SYNC_DISABLE:
            isSynchronized = FALSE;
            MessageBox(hwnd, TEXT("Синхронизация выключена"), TEXT("Информация"), MB_OK);
            break;

        case ID_THREAD1_INFO:
            if (0 < g_uThCount) {
                ShowThreadInfoDialog(0);
            }
            break;

        case ID_INFO_THREAD2:
            if (1 < g_uThCount) {
                ShowThreadInfoDialog(1);
            }
            break;
        }
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;

        EndPaint(hwnd, &ps);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    return 0;
}