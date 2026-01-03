#include <windows.h>
#include <tchar.h>
#include <vector>    // для std::vector
#include <string>    // для std::string
#include <utility>   // для std::pair

// Прототип оконной процедуры
LRESULT CALLBACK Pr2_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

// Глобальные переменные
const TCHAR g_lpszClassName[] = TEXT("sp_pr2_class");
const TCHAR g_lpszApplicationTitle[] = TEXT("Главное окно приложения. Программист <Игнатович Д.>");
const TCHAR g_lpszDestroyMessage[] = TEXT("Поступило сообщение WM_DESTROY, из обработчика которого и выполнен данный вывод. Сообщение поступило в связи с разрушением окна приложения");
static int x = 50, y = 200;

HINSTANCE g_hInst;

// Главная функция
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    WNDCLASSEX wc;
    HWND hwnd;
    MSG msg;
    g_hInst = hInstance;
    // Заполнение структуры WNDCLASSEX
    wc.cbSize = sizeof(WNDCLASSEX);
    wc.style = CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc = Pr2_WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_EXCLAMATION);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = CreateSolidBrush(RGB(127, 0, 0)); // Цвет фона
    wc.lpszMenuName = NULL;
    wc.lpszClassName = g_lpszClassName;
    wc.hIconSm = LoadIcon(NULL, IDI_EXCLAMATION);

    // Регистрация класса главного окна
    if (!RegisterClassEx(&wc)) {
        MessageBox(NULL, TEXT("Ошибка регистрации класса окна!"), TEXT("Ошибка"), MB_ICONERROR);
        return 1;
    }

    // Создание главного окна

    hwnd = CreateWindowEx(NULL,
        g_lpszClassName,
		g_lpszApplicationTitle,
        WS_OVERLAPPEDWINDOW & ~WS_MAXIMIZEBOX,
        500, 300,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

    if (hwnd == NULL) {
        MessageBox(NULL, TEXT("Ошибка создания окна!"), TEXT("Ошибка"), MB_ICONERROR);
        return 1;
    }

    // Отображение главного окна
    ShowWindow(hwnd, nCmdShow);
    UpdateWindow(hwnd);

    // Главный цикл обработки сообщений
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    // Возврат значения завершения программы
    return msg.wParam;
}



// Оконная процедура для обработки сообщений главного окна приложения
LRESULT CALLBACK Pr2_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static std::vector<std::pair<std::string, POINT>> texts;
    static const char* textToAdd = "Обработка сообщения WM_LBUTTONDOWN, которое посылается в окно при щелчке левой кнопки мыши.";
    static HWND hButtonSave;
    static HWND hButtonAdd;
    static HWND hButtonExit;
    static HWND hEdit;
    static HWND hListBox;

    static TCHAR pszTextBuff[500];

#define IDC_BTN_SAVE 150
#define IDC_BTN_ADD 151
#define IDC_EDIT1 152
#define IDC_LISTBOX 153

    switch (msg) {
        case WM_DESTROY: {
                MessageBox(hwnd, g_lpszDestroyMessage, TEXT("Закрытие приложения"), MB_OK | MB_ICONINFORMATION);
                PostQuitMessage(0); // Код завершения равен 5
                break;
        }

        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            // Выводим текст при обработке WM_PAINT
            const char* paintMessage = "Обработка сообщения WM_PAINT. Это сообщение окно получает после того, как оно было закрыто другим окном и затем открыто.";
            TextOutA(hdc, 20, 100, paintMessage, strlen(paintMessage));

            // Рисуем тексты из вектора
            for (const auto& entry : texts) {
                RECT rect;
                SetRect(&rect, entry.second.x, entry.second.y, entry.second.x + 500, entry.second.y + 50);
                DrawTextA(hdc, entry.first.c_str(), -1, &rect, DT_SINGLELINE | DT_NOCLIP);
            }

            EndPaint(hwnd, &ps);

            break;
        }


        case WM_LBUTTONDOWN: {
            POINT pt;
            pt.x = LOWORD(lParam);
            pt.y = HIWORD(lParam);
            texts.emplace_back(textToAdd, pt);
            InvalidateRect(hwnd, NULL, TRUE);
            break;
        }

        case WM_CREATE: {

        
            MessageBox(hwnd,
                TEXT("Выполняется обработка WM_CREATE"), TEXT("Обработка WM_CREATE")
                , MB_OK);


            hEdit = CreateWindowEx(0L, _T("Edit"), _T("Редактор"),
                WS_CHILD | WS_BORDER | WS_VISIBLE | ES_LEFT | ES_AUTOHSCROLL,
                20, 50, 160, 40, hwnd,
                (HMENU)IDC_EDIT1,
                g_hInst, NULL);
            if (hEdit == 0) return -1;

            hListBox = CreateWindowEx(0L, _T("ListBox"), _T("Список"),
                WS_CHILD | WS_BORDER | WS_VISIBLE,
                200, 50, 160, 180, hwnd,
                (HMENU)IDC_LISTBOX, g_hInst, NULL);
            if (hListBox == 0) return -1;

            hButtonSave = CreateWindowEx(0L, _T("Button"), _T("В буфер"),
                WS_CHILD | WS_BORDER | WS_VISIBLE,
                20, 240, 80, 24, hwnd,
                (HMENU)IDC_BTN_SAVE, g_hInst, NULL);
            if (hButtonSave == 0) return -1;

            hButtonAdd = CreateWindowEx(0L, _T("Button"), _T("В список"),
                WS_CHILD | WS_BORDER | WS_VISIBLE,
                120, 240, 80, 24, hwnd,
                (HMENU)IDC_BTN_ADD, g_hInst, NULL);
            if (hButtonAdd == 0) return -1;

            hButtonExit = CreateWindowEx(0L, _T("Button"), _T("Выход"),
                WS_CHILD | WS_BORDER | WS_VISIBLE, 220, 240, 80, 24, hwnd,
                (HMENU)IDCANCEL, g_hInst, NULL);
            if (hButtonExit == 0) return -1;

            return 0;
        }

        case WM_COMMAND: {
            int wmId, wmEvent;
            wmEvent = HIWORD(wParam); // Код события
            wmId = LOWORD(wParam); // Идентификатор команды меню или горячей клавиши

            switch (wmId) {
                case IDC_BTN_SAVE: {
                    int cch = SendMessage(hEdit, WM_GETTEXT, (WPARAM)500, (LPARAM)pszTextBuff);
                    if (cch == 0)
                    {
                        MessageBox(hwnd, _T("Введите текст"), _T("Читаем Edit"), MB_OK);
                    }
                    else
                    {
                        TCHAR Buff1[500] = { 0 };
                        SYSTEMTIME st;
                        GetSystemTime(&st);
                        wsprintf(Buff1, TEXT("Время : %d ч %d мин %d сек\n"), st.wHour, st.wMinute, st.wSecond);
                        lstrcat(Buff1, __TEXT("Текст в памяти: "));
                        lstrcat(Buff1, pszTextBuff);
                        MessageBox(hwnd, Buff1, TEXT("Содержимое буфера"), MB_OK);
                        break;
                    }

                }

                case IDC_BTN_ADD: {
                    int ind = SendMessage(hListBox, LB_ADDSTRING, (WPARAM)0, (LPARAM)pszTextBuff);
                    if (ind == LB_ERR)
                        MessageBox(hwnd, TEXT("Ошибка в списке"), TEXT(""), MB_OK);
                    break;
                }

                case IDCANCEL:
                    DestroyWindow(hwnd);
                    break;

                default:
                    return DefWindowProc(hwnd, msg, wParam, lParam);
                }

            break;
        }

        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}
