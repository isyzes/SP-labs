// SP_PR6.cpp : Определяет точку входа для приложения.
//

#include "framework.h"
#include "SP_PR6.h"

#define MAX_LOADSTRING 100
#define ID_FRAME_WINDOW 1

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

int posX = 40;                                  // X0
int posY = 140;                                 // Y0
int width = 200;                                // Cx
int height = 100;                               // Cy
int moveDistance = 15;                          // D
BOOL drawingMode = FALSE;                       // Режим рисования
int markerSize = 5;                             // Размер маркера
COLORREF markerColor = RGB(255, 255, 0); // Цвет маркера (CF = Кр + зел)

// Предварительные объявления функций
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
void                DrawCursorTrail(HWND hWnd, int x, int y);


// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);


int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR lpszCmdLine, int nCmdShow) {
    MSG Msg;
    WNDCLASS wc = { 0 };

    wc.lpfnWndProc = WndProc;
    wc.hInstance = hInst;
    wc.lpszClassName = TEXT("MainWinClass");
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

    RegisterClass(&wc);
    HWND hMainWnd = CreateWindow(wc.lpszClassName, TEXT("Frame Window Application"),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        400, 400, NULL, NULL, hInst, NULL);

    ShowWindow(hMainWnd, nCmdShow);
    UpdateWindow(hMainWnd);

    while (GetMessage(&Msg, NULL, 0, 0)) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }

    return Msg.wParam;
}


ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_SPPR6));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_SPPR6);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

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

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    static HWND hFrame = NULL; // Дескриптор окна рамы
    static int frameX = posX, frameY = posY; // Позиция окна рамы

    switch (msg) {
    case WM_CREATE:
        hFrame = CreateWindow(TEXT("STATIC"), NULL,
            WS_CHILD | WS_VISIBLE | SS_OWNERDRAW,
            frameX, frameY, width, height,
            hwnd, (HMENU)ID_FRAME_WINDOW, NULL, NULL);
        break;

    case WM_PAINT: {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hwnd, &ps);
        TCHAR buffer[100];
        wsprintf(buffer, TEXT("Координаты окна-рамки: (%d, %d)"), frameX, frameY);
        SetTextColor(hdc, RGB(0, 0, 0));
        TextOut(hdc, 10, 10, buffer, lstrlen(buffer));
        EndPaint(hwnd, &ps);
        break;
    }

    case WM_KEYDOWN:
        switch (wParam) {
        case 'S': // K1 - влево
            frameX -= moveDistance;
            break;
        case 'D': // K2 - вправо
            frameX += moveDistance;
            break;
        case 'X': // K3 - вверх
            frameY -= moveDistance;
            break;
        case 'C': // K4 - вниз
            frameY += moveDistance;
            break;
        case VK_F2: // Включаем режим рисования
            drawingMode = TRUE;
            break;
        case VK_F3: // Выключаем режим рисования
            drawingMode = FALSE;
            break;
        }
        MoveWindow(hFrame, frameX, frameY, width, height, TRUE);
        InvalidateRect(hwnd, NULL, TRUE);
        break;

    case WM_MOUSEMOVE:
        if (drawingMode && (wParam & MK_LBUTTON)) {
            DrawCursorTrail(hFrame, LOWORD(lParam), HIWORD(lParam));
        }
        break;

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hwnd, msg, wParam, lParam);
    }

    return 0;
}

// Функция для рисования следа курсора
void DrawCursorTrail(HWND hWnd, int x, int y) {
    HDC hdc = GetDC(hWnd);
    HBRUSH hBrush = CreateSolidBrush(markerColor);
    SelectObject(hdc, hBrush);

    // Чертим фигуру (треугольник)
    POINT triangle[3];
    triangle[0].x = x;                    // Верхняя вершина
    triangle[0].y = y - markerSize;

    triangle[1].x = x - markerSize;       // Левая нижняя вершина
    triangle[1].y = y + markerSize;

    triangle[2].x = x + markerSize;       // Правая нижняя вершина
    triangle[2].y = y + markerSize;
    Polygon(hdc, triangle, 3);

    DeleteObject(hBrush);
    ReleaseDC(hWnd, hdc);
}
