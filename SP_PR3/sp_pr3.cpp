#include <windows.h>
#include <vector>
#include <string>
#include <tchar.h>
#include "resource.h"
#include "sp_pr3.h"

HINSTANCE g_hInst;
HACCEL g_hAccelTable;

std::vector<std::pair<std::string, POINT>> texts;
const char* textToAdd = "Обработка сообщения WM_LBUTTONDOWN, которое посылается в окно при щелчке левой кнопки мыш.";

HWND hButtonSave, hButtonAdd, hButtonExit, hEdit, hListBox;

#define IDC_BTN_SAVE 150
#define IDC_BTN_ADD 151
#define IDC_EDIT1 152
#define IDC_LISTBOX 153

void CreateControls(HWND hwnd) {

    hEdit = CreateWindowEx(0L, _T("Edit"), _T("Редактор"), WS_CHILD | WS_BORDER | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL, 20, 50, 160, 40, hwnd, (HMENU)IDC_EDIT1, g_hInst, NULL);
    hListBox = CreateWindowEx(0L, _T("ListBox"), _T("Список"), WS_CHILD | WS_BORDER | WS_VISIBLE, 200, 50, 160, 180, hwnd, (HMENU)IDC_LISTBOX, g_hInst, NULL);
    hButtonSave = CreateWindowEx(0L, _T("Button"), _T("В буфер"), WS_CHILD | WS_BORDER | WS_VISIBLE, 20, 240, 80, 24, hwnd, (HMENU)IDC_BTN_SAVE, g_hInst, NULL);
    hButtonAdd = CreateWindowEx(0L, _T("Button"), _T("В список"), WS_CHILD | WS_BORDER | WS_VISIBLE, 120, 240, 80, 24, hwnd, (HMENU)IDC_BTN_ADD, g_hInst, NULL);
    hButtonExit = CreateWindowEx(0L, _T("Button"), _T("Выход"), WS_CHILD | WS_BORDER | WS_VISIBLE, 220, 240, 80, 24, hwnd, (HMENU)IDCANCEL, g_hInst, NULL);
}

void HandlePaint(HWND hwnd, HDC hdc) {
    PAINTSTRUCT ps;
    BeginPaint(hwnd, &ps);
    const char* paintMessage = "Обработка сообщения WM_PAINT. Это сообщение окно получает после того, как оно было закрыто другим окном и затем открыто.";
    TextOutA(hdc, 20, 100, paintMessage, strlen(paintMessage));
    for (const auto& entry : texts) {
        RECT rect;
        SetRect(&rect, entry.second.x, entry.second.y, entry.second.x + 500, entry.second.y + 50);
        DrawTextA(hdc, entry.first.c_str(), -1, &rect, DT_SINGLELINE | DT_NOCLIP);
    }
    EndPaint(hwnd, &ps);
}

void UpdateMenuState(HWND hwnd) {
    HMENU hMenu = GetMenu(hwnd);
    // EnableMenuItem(hMenu, ID_EDIT_SELECT, MF_BYPOSITION | (IsMenuItemEnabled(hMenu, ID_EDIT_SELECT) ? MF_ENABLED : MF_GRAYED));
    // EnableMenuItem(hMenu, ID_EDIT_COPY, MF_BYPOSITION | (IsMenuItemEnabled(hMenu, ID_EDIT_COPY) ? MF_ENABLED : MF_GRAYED));

    EnableMenuItem(hMenu, ID_EDIT_SELECT, MF_BYCOMMAND | MF_ENABLED);
    EnableMenuItem(hMenu, ID_EDIT_COPY, MF_BYCOMMAND | MF_ENABLED);

}

bool IsMenuItemEnabled(HMENU hMenu, UINT uID) {
    MENUITEMINFO mii = { sizeof(MENUITEMINFO), 0 };
    mii.fMask = MIIM_STATE;
    GetMenuItemInfo(hMenu, uID, FALSE, &mii);
    return !(mii.fState & MFS_GRAYED);
}



LRESULT CALLBACK Pr3_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    HMENU hMenu = GetMenu(hwnd);

    switch (msg) {
    case WM_CREATE: {
        CreateControls(hwnd);
        AppendMenu(hMenu, MF_STRING, 40004, TEXT("Закрыть документ"));
        DrawMenuBar(hwnd); // Обновляем меню

        return 0;
    }



    case WM_RBUTTONDOWN: { // Контекстное меню при правом клике
        int xPos = LOWORD(lParam);
        int yPos = HIWORD(lParam);
        CreateContextMenu(hwnd, xPos, yPos);
        break;
    }



    case WM_PAINT: {
        HDC hdc = GetDC(hwnd);
        HandlePaint(hwnd, hdc);
        ReleaseDC(hwnd, hdc);
        break;
    }


    case WM_LBUTTONDOWN: {
        POINT pt = { LOWORD(lParam), HIWORD(lParam) };
        texts.emplace_back(textToAdd, pt);
        InvalidateRect(hwnd, NULL, TRUE);
        break;
    }

                       //case WM_PAINT: {
                      //     PAINTSTRUCT ps;
                     //      HDC hdc = BeginPaint(hwnd, &ps);
                      //     HandlePaint(hwnd, hdc);
                      //     EndPaint(hwnd, &ps);
                      //     break;
                     //  }

    case WM_MENUSELECT: {
        HDC hdc1;
        TCHAR buf[300];
        hdc1 = GetDC(hwnd);

        // Загружаем текст подсказки из ресурсной строки
        int menuID = LOWORD(wParam);
        int size = LoadString(g_hInst, menuID, buf, 300);

        // Если строка не загружена, используем идентификатор для подсказки
        if (size == 0) {
            switch (menuID) {
            case ID_FILE_NEW:
                LoadString(g_hInst, IDS_MENU_NEW_TIP, buf, 300);
                break;
            case ID_FILE_OPEN:
                LoadString(g_hInst, IDS_MENU_OPEN_TIP, buf, 300);
                break;
            case ID_FILE_EXIT:
                LoadString(g_hInst, IDS_MENU_EXIT_TIP, buf, 300);
                break;
            case ID_HELP_ABOUT:
                LoadString(g_hInst, IDS_MENU_ABOUT_TIP, buf, 300);
                break;
            default:
                _tcscpy_s(buf, _T("Неизвестная команда"));
                break;
            }
        }

        // Получаем размеры клиентской области
        RECT rc;
        GetClientRect(hwnd, &rc);

        // Отображаем подсказку на 30 пикселей выше нижней границы окна
        TextOut(hdc1, rc.left + 10, rc.bottom - 30, buf, lstrlen(buf));

        ReleaseDC(hwnd, hdc1);
        break;
    }

    case WM_COMMAND: {
        int wmId = LOWORD(wParam);
        switch (wmId) {

        case ID_FILE_NEW:
            MessageBox(hwnd, TEXT("Выбрана команда Создать"), TEXT("Информация"), MB_OK);
            UpdateMenuState(hwnd);
            break;
        case ID_FILE_OPEN:
            MessageBox(hwnd, TEXT("Выбрана команда Открыть"), TEXT("Информация"), MB_OK);
            break;
        case ID_FILE_EXIT:
            PostMessage(hwnd, WM_DESTROY, 0, 0);
            break;
        case ID_HELP_ABOUT:
            MessageBox(hwnd, TEXT("Информация о программе"), TEXT("Информация"), MB_OK);
            break;
        case IDC_BTN_SAVE:
            MessageBox(hwnd, _T("Текст сохранён в буфер."), _T("Информация"), MB_OK);
            break;
        case IDC_BTN_ADD: {
            char buffer[256];
            GetDlgItemTextA(hwnd, IDC_EDIT1, buffer, sizeof(buffer));
            SendDlgItemMessageA(hwnd, IDC_LISTBOX, LB_ADDSTRING, 0, (LPARAM)buffer);

        }

        case 40004: // Закрытие документа
            // Делаем недоступными все команды меню "Правка"
            EnableMenuItem(hMenu, ID_EDIT_SELECT, MF_GRAYED);
            EnableMenuItem(hMenu, ID_EDIT_COPY, MF_GRAYED);
            UpdateMenuState(hwnd);
            break;

        default:
            TCHAR msg[100];
            _stprintf_s(msg, _T("Команда с идентификатором %d не реализована"), wmId);
            MessageBox(hwnd, msg, TEXT("Ошибка"), MB_OK);
            break;
        }
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void CreateContextMenu(HWND hwnd, int x, int y) {
    HMENU hContextMenu = CreatePopupMenu();

    // Добавляем все пункты меню
    AppendMenu(hContextMenu, MF_STRING, ID_EDIT_SELECT, TEXT("Выделить"));
    AppendMenu(hContextMenu, MF_STRING, ID_EDIT_COPY, TEXT("Копировать"));
    AppendMenu(hContextMenu, MF_STRING, ID_EDIT_CUT, TEXT("Вырезать"));
    AppendMenu(hContextMenu, MF_STRING, ID_EDIT_PASTE, TEXT("Вставить"));

    // Включаем/выключаем пункты меню
    EnableMenuItem(hContextMenu, ID_EDIT_SELECT,
        (true) ? MF_ENABLED : MF_GRAYED);
    EnableMenuItem(hContextMenu, ID_EDIT_COPY,
        (true) ? MF_ENABLED : MF_GRAYED);
    EnableMenuItem(hContextMenu, ID_EDIT_CUT, MF_ENABLED);  // Всегда включено
    EnableMenuItem(hContextMenu, ID_EDIT_PASTE, MF_ENABLED); // Всегда включено

    TrackPopupMenu(hContextMenu, TPM_RIGHTBUTTON, x, y, 0, hwnd, NULL);
    DestroyMenu(hContextMenu);
}

void CreateContextMenuOLD(HWND hwnd, int x, int y) {
    HMENU hContextMenu = CreatePopupMenu();
    AppendMenu(hContextMenu, MF_STRING | (IsMenuItemEnabled(GetMenu(hwnd), ID_EDIT_SELECT) ? MF_ENABLED : MF_GRAYED), ID_EDIT_SELECT, TEXT("Выделить"));
    AppendMenu(hContextMenu, MF_STRING | (IsMenuItemEnabled(GetMenu(hwnd), ID_EDIT_COPY) ? MF_ENABLED : MF_GRAYED), ID_EDIT_COPY, TEXT("Копировать"));
    TrackPopupMenu(hContextMenu, TPM_RIGHTBUTTON, x, y, 0, hwnd, NULL);
    DestroyMenu(hContextMenu);
}


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    g_hInst = hInstance;

    g_hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDR_ACCELERATOR));

    WNDCLASSEX wc = {
        sizeof(WNDCLASSEX),
        CS_HREDRAW | CS_VREDRAW,
        Pr3_WndProc,
        0,
        0,
        hInstance,
        LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1)),
        LoadCursor(hInstance, MAKEINTRESOURCE(IDC_CURSOR1)),
        CreateSolidBrush(RGB(127, 0, 0)),
        MAKEINTRESOURCE(IDR_MAINMENU),
        g_lpszClassName,
        LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1))
    };

    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, TEXT("Ошибка регистрации класса окна!"), TEXT("Ошибка"), MB_ICONERROR);
        return 1;
    }

    HMENU hMenu = LoadMenu(hInstance, MAKEINTRESOURCE(IDR_MAINMENU));

    HWND hwnd = CreateWindow(
        g_lpszClassName,
        g_lpszApplicationTitle,
        WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        500,
        400,
        NULL,
        hMenu,
        hInstance,
        NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, TEXT("Ошибка создания окна!"), TEXT("Ошибка"), MB_ICONERROR);
        return 1;
    }

    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (!TranslateAccelerator(hwnd, g_hAccelTable, &msg)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }
    return msg.wParam;
}