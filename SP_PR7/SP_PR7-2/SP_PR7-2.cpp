#include <windows.h>
#include <gdiplus.h>
#include <cmath>

#pragma comment(lib, "gdiplus.lib")

using namespace Gdiplus;

// Глобальные переменные
HINSTANCE hInst;

// Предварительное объявление функции оконной процедуры
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void             DrawFigure(HWND hWnd);
void             DrawCircle(Graphics& graphics, Pen& pen, float start, float circleRadius);
void             DrawSquare(Graphics& graphics, Pen& pen, float start, int A);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    hInst = hInstance;
    GdiplusStartupInput gdiplusStartupInput;
    ULONG_PTR gdiplusToken;
    GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, nullptr);

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
        0, TEXT("MyWindowClass"), TEXT("Draw Figure"),
        WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
        600, 400, NULL, NULL, hInstance, NULL); // Увеличиваем ширину окна

    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Главный цикл обработки сообщений
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    GdiplusShutdown(gdiplusToken);
    return (int)msg.wParam;
}

// Оконная процедура
LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    switch (msg) {
    case WM_PAINT: {
        DrawFigure(hWnd);
        break;
    }

    case WM_DESTROY:
        PostQuitMessage(0);
        break;

    default:
        return DefWindowProc(hWnd, msg, wParam, lParam);
    }
}


void DrawFigure(HWND hWnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hWnd, &ps);
    Graphics graphics(hdc);

    int A = 100;
    float circleRadius = A / 2.0;
    float start = 100.0f;

    Pen pen(Color(255, 0, 255, 0), 1);

    DrawCircle(graphics, pen, start, circleRadius);
    DrawSquare(graphics, pen, start, A);
    
    EndPaint(hWnd, &ps);
}

void DrawSquare(Graphics& graphics, Pen& pen, float start, int A) {
    // КВАДРАТ поверх круга (верхний слой)
    SolidBrush semiTransparentBrush(Color(255, 0, 0, 255));              

    PointF points[4] = {
        PointF(start, start + A), // Нижний левый угол
        PointF(start + A, start + A), // Нижний правый угол
        PointF(start + A, start), // Верхний правый угол
        PointF(start, start) // Верхний левый угол
    };

    graphics.FillPolygon(&semiTransparentBrush, points, 4);
    graphics.DrawPolygon(&pen, points, 4);

    int startInt = start;
    // Добавление горизонтальной штриховки для квадрата
    for (int y = start; y <= startInt + A; y += 17) { // Шаг в 10 пикселей для штриховки
        graphics.DrawLine(&pen, startInt, y, y, startInt);
        graphics.DrawLine(&pen, startInt + A, y, y, startInt + A);

        graphics.DrawLine(&pen, y, startInt, startInt + A, startInt + A*2 - y);
        graphics.DrawLine(&pen, startInt + A * 2 - y, startInt + A, startInt, y);
    }
}


void DrawCircle(Graphics& graphics, Pen& pen, float start, float circleRadius) {
    // КРУГ (нижний слой)
    SolidBrush circleBrush(Color(255, 0, 0, 255)); // Синий
    PointF circleCenter(start + circleRadius, start);
    graphics.FillEllipse(&circleBrush,
        circleCenter.X - circleRadius,
        circleCenter.Y - circleRadius,
        circleRadius * 2.0f,
        circleRadius * 2.0f);
    //Контур круга
    graphics.DrawEllipse(&pen,
        circleCenter.X - circleRadius,
        circleCenter.Y - circleRadius,
        circleRadius * 2.0f,
        circleRadius * 2.0f);


    // Добавление горизонтальной штриховки для круга
    for (int y = static_cast<int>(circleCenter.Y) - static_cast<int>(circleRadius);
        y < static_cast<int>(circleCenter.Y) + static_cast<int>(circleRadius);
        y += 9) {
        // Вычисляем максимальную ширину линии внутри круга
        float width = sqrt(circleRadius * circleRadius - (y - circleCenter.Y) * (y - circleCenter.Y));
        graphics.DrawLine(&pen,
            static_cast<float>(circleCenter.X) - width,
            static_cast<float>(y),
            static_cast<float>(circleCenter.X) + width,
            static_cast<float>(y));
    }
}