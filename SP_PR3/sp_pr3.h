#ifndef SP_PR3_H
#define SP_PR3_H

#include <windows.h>
#include <vector>
#include <string>

extern HINSTANCE g_hInst;
extern HACCEL g_hAccelTable;

const TCHAR  g_lpszClassName[] = TEXT("sp_pr3_class"); 
const TCHAR  g_lpszApplicationTitle[] = TEXT("Главное окно приложения. Программист <Игнатович Д.");
const TCHAR  g_lpszDestroyMessage[] = TEXT("Поступило сообщение WM_DESTROY, из обработчика которого и выполнен данный вывод. Сообщение поступило в связи с разрушением окна приложения.");

void CreateControls(HWND hwnd);
void HandlePaint(HWND hwnd, HDC hdc);
LRESULT CALLBACK Pr3_WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
void UpdateMenuState(HWND hwnd);
bool IsMenuItemEnabled(HMENU hMenu, UINT uID);
void CreateContextMenu(HWND hwnd, int x, int y);


#endif // SP_PR3_H