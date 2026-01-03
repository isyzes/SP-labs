// SP_LB2-1.cpp : Определяет точку входа для приложения.
//
#include <windows.h>
#include <tchar.h>
#include "framework.h"
#include "SP_LB2-1.h"

#define MAX_LOADSTRING 100

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];// Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];// имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    InfoDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
INT_PTR CALLBACK    ProcessInfoDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam);
void                GetProcessInfo(HWND hwndDlg, int index);
void                OpenFileDialog(TCHAR* filePath, size_t filePathSize);


// Глобальные массивы
HANDLE ProcHandle[4] = { NULL };
DWORD ProcId[4] = { 0 };
HANDLE ThreadHandle[4] = { NULL }; // Добавлен массив для дескрипторов потоков
DWORD ThreadId[4] = { 0 };           // Массив для идентификаторов потоков
TCHAR* ProcImage[4];                // Массив для хранения изображений процесса
TCHAR CmdParam[4][260];             // Массив для хранения параметров командной строки

void FillProcessData() {
    // Статические строки
    static TCHAR processNames[4][30] = {
        _T("Текущий процесс"),
        _T("Блокнот"),
        _T("Блокнот с текстом"),
        _T("Калькулятор")
    };

    ProcImage[0] = processNames[0];
    _tcscpy_s(CmdParam[0], 260, _T("SP-LB2-1"));

    ProcImage[1] = processNames[1];
    _tcscpy_s(CmdParam[1], 260, _T("notepad.exe")); // Параметры для Блокнота

    ProcImage[2] = processNames[2];
    _tcscpy_s(CmdParam[2], 260, _T("notepad.exe D:\\project\\cpp\\SP\\readme.txt")); // Параметры для Блокнота с текстом

    ProcImage[3] = processNames[3];
    _tcscpy_s(CmdParam[3], 260, _T("calc.exe")); // Параметры для Калькулятора
}


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.
    FillProcessData();
    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_SPLB21, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_SPLB21));

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
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPLB21));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPLB21);
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
            case ID_MENU_NOTEPAD: {
                STARTUPINFO si = { sizeof(si) };
                PROCESS_INFORMATION pi;
                if (CreateProcess(NULL, CmdParam[1], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                    ProcHandle[1] = pi.hProcess;
                    ThreadHandle[1] = pi.hThread; // Сохраняем дескриптор потока
                    ProcId[1] = pi.dwProcessId;
                    CloseHandle(pi.hThread); // Закрываем дескриптор потока
                }
                //WinExec("notepad.exe", SW_SHOW);
                break;
            }
            case ID_MENU_NOTEPAD_TEXT: {
                TCHAR selectedFile[MAX_PATH] = _T("");
                OpenFileDialog(selectedFile, sizeof(selectedFile) / sizeof(selectedFile[0]));
                if (_tcslen(selectedFile) > 0) {
                    STARTUPINFO si = { sizeof(si) };
                    PROCESS_INFORMATION pi;

                    TCHAR commandLine[MAX_PATH + 50];
                    _stprintf_s(commandLine, _T("\"%s\" \"%s\""), CmdParam[1], selectedFile);
                    if (CreateProcess(NULL, commandLine, NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                        ProcHandle[2] = pi.hProcess;
                        ThreadHandle[2] = pi.hThread;
                        ProcId[2] = pi.dwProcessId;
                        CloseHandle(pi.hThread);
                    }
                }
                break;
            }

            case ID_MENU_TEST_PROC: {
                STARTUPINFO si = { sizeof(si) };
                PROCESS_INFORMATION pi;


                si.dwFlags = STARTF_USESHOWWINDOW;
                si.wShowWindow = SW_SHOW;

                // Установим размеры и положение окна
                si.dwX = 100; // Координаты X
                si.dwY = 100; // Координаты Y
                si.dwXSize = 800; // Ширина окна
                si.dwYSize = 600; // Высота окна

                // Запуск приложения TestProc
                if (CreateProcess(
                    _T("D:\\project\\cpp\\TestProc.exe"), // Путь к исполняемому файлу
                    NULL,
                    NULL,
                    NULL,
                    FALSE,
                    0,
                    NULL,
                    NULL,
                    &si,
                    &pi)
                    ) {
                    CloseHandle(pi.hThread);
                }
                else {
                    MessageBox(hWnd, _T("Не удалось запустить TestProc"), _T("Ошибка"), MB_OK);
                }
                break;
            }

            //case ID_MENU_NOTEPAD_TEXT: {
                //STARTUPINFO si = { sizeof(si) };
                //PROCESS_INFORMATION pi;
                //if (CreateProcess(NULL, CmdParam[2], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                    //ProcHandle[2] = pi.hProcess;
                    //ThreadHandle[2] = pi.hThread;
                    //ProcId[2] = pi.dwProcessId;

                //}
                //break;
            //}
            case ID_MENU_CALCULATOR: {
                STARTUPINFO si = { sizeof(si) };
                PROCESS_INFORMATION pi;
                if (CreateProcess(NULL, CmdParam[3], NULL, NULL, FALSE, 0, NULL, NULL, &si, &pi)) {
                    ProcHandle[3] = pi.hProcess;
                    ThreadHandle[3] = pi.hThread;
                    ProcId[3] = pi.dwProcessId;

                }
                break;
            }
            case ID_MENU_CLOSE_NOTEPAD: {
                if (ProcHandle[1] != NULL) {
                    TerminateProcess(ProcHandle[1], 1); // Код завершения 1
                    CloseHandle(ProcHandle[1]);
                    ProcHandle[1] = NULL; // Обнуляем дескриптор
                }
                break;
            }
            case ID_INFO_CURRENT_PROCESS:
                DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ProcessInfoDlgProc, (LPARAM)0);
                //MessageBox(hWnd, TEXT("Текущий процесс: SP-LB2-1"), TEXT("Информация"), MB_OK);
                break;
            case ID_INFO_NOTEPAD:
                DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ProcessInfoDlgProc, (LPARAM)1);
                //MessageBox(hWnd, TEXT("Блокнот (1) запущен"), TEXT("Информация"), MB_OK);
                break;
            case ID_INFO_NOTEPAD_TEXT:
                DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ProcessInfoDlgProc, (LPARAM)2);
                //MessageBox(hWnd, TEXT("Блокнот с текстом (2) запущен"), TEXT("Информация"), MB_OK);
                break;
            case ID_INFO_CALCULATOR:
                DialogBoxParam(GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG1), hWnd, ProcessInfoDlgProc, (LPARAM)3);
                //MessageBox(hWnd, TEXT("Калькулятор (3) запущен"), TEXT("Информация"), MB_OK);
                break;
            }
            break;
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

    case IDM_INFO_NOT: {
        DialogBoxParam(hInst, MAKEINTRESOURCE(IDD_DIALOG1), hWnd, InfoDlg, 1);
        break;
    }
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


INT_PTR CALLBACK InfoDlg(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG: {
        SetDlgItemText(hDlg, IDC_EDIT1, ProcImage[lParam]);
        TCHAR buff[200];
        wsprintf(buff, TEXT("HANDLE %X, ProxID %d"), ProcHandle[lParam], ProcId[lParam]);
        SetDlgItemText(hDlg, IDC_EDIT2, buff);

        DWORD dwExCode = -5;

        if (!GetExitCodeProcess(ProcHandle[lParam], &dwExCode)) {
            DWORD dwErr= GetLastError();
            wsprintf(buff, TEXT("Ошибка. Дескриптор закрыт, код %D"), dwErr);  
        }
        else if (dwExCode == STILL_ACTIVE)
        {
            lstrcpy(buff, TEXT("Процес не завершился"));
        }
        else 
        {
            wsprintf(buff, TEXT("Код завершения: %d"), dwExCode);
        }

        SetDlgItemText(hDlg, IDC_EDIT3, buff);

    }
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

// Обработчик диалогового окна
INT_PTR CALLBACK ProcessInfoDlgProc(HWND hwndDlg, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_INITDIALOG: {
        int index = (int)lParam; // Получаем индекс процесса
        GetProcessInfo(hwndDlg, index);
        return TRUE;
    }
    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK) {
            EndDialog(hwndDlg, 0);
            return TRUE;
        }
        break;
    }
    return FALSE;
}

// Получение информации о процессе
void GetProcessInfo(HWND hwndDlg, int index) {
    if (index < 0 || index > 3) {
        return;
    }

    TCHAR procName[100];
    HANDLE processHandle = ProcHandle[index];
    HANDLE threadHandle = ThreadHandle[index];
    DWORD processId = ProcId[index];
    DWORD threadId = ThreadId[index];
    DWORD exitCode;

    // Устанавливаем имя процесса
    _stprintf_s(procName, _T("%s"), ProcImage[index]);
    SetDlgItemText(hwndDlg, IDC_EDIT_PROC_NAME, procName);
    SetDlgItemText(hwndDlg, IDC_EDIT_PROC_PARAM, CmdParam[index]);

    // Получаем код завершения процесса
    if (processHandle && GetExitCodeProcess(processHandle, &exitCode)) {
        TCHAR exitCodeStr[30];
        _stprintf_s(exitCodeStr, _T("%u"), exitCode);
        SetDlgItemText(hwndDlg, IDC_EDIT_EXIT_CODE_PROC, exitCodeStr);
    }
    else {
        SetDlgItemText(hwndDlg, IDC_EDIT_EXIT_CODE_PROC, _T("Не доступно"));
    }

    // Устанавливаем дескрипторы и идентификаторы
    TCHAR handleStr[30];
    _stprintf_s(handleStr, _T("%p"), processHandle);
    SetDlgItemText(hwndDlg, IDC_EDIT_PROC_HANDLE, handleStr);

    _stprintf_s(handleStr, _T("%u"), processId);
    SetDlgItemText(hwndDlg, IDC_EDIT_PROC_ID, handleStr);

    _stprintf_s(handleStr, _T("%p"), threadHandle);
    SetDlgItemText(hwndDlg, IDC_EDIT_THREAD_HANDLE, handleStr);

    _stprintf_s(handleStr, _T("%u"), threadId);
    SetDlgItemText(hwndDlg, IDC_EDIT_THREAD_ID, handleStr);

    // Получаем код завершения потока
    DWORD threadExitCode;
    if (threadHandle && GetExitCodeThread(threadHandle, &threadExitCode)) {
        TCHAR threadExitCodeStr[30];
        _stprintf_s(threadExitCodeStr, _T("%u"), threadExitCode);
        SetDlgItemText(hwndDlg, IDC_EDIT_EXIT_CODE_THREAD, threadExitCodeStr);
    }
    else {
        SetDlgItemText(hwndDlg, IDC_EDIT_EXIT_CODE_THREAD, _T("Не доступно"));
    }
}

void OpenFileDialog(TCHAR* filePath, size_t filePathSize) {
    OPENFILENAME ofn;       // Диалог открытия файла
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = NULL;
    ofn.lpstrFile = filePath; // Путь к файлу
    ofn.nMaxFile = (DWORD)filePathSize;
    ofn.lpstrFilter = _T("Text Files\0*.txt\0All Files\0*.*\0");
    ofn.lpstrTitle = _T("Выберите текстовый файл");
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Открываем диалоговое окно
    if (GetOpenFileName(&ofn)) {

    }
}