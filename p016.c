#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#pragma comment(lib, "gdi32.lib")
#define MY_BUTTON_ID 1
int Shombol = 0;                   
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
        case WM_COMMAND:
            if(LOWORD(wParam) == MY_BUTTON_ID) {  
                ++Shombol;
            }
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd); 
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            char buf[50];
            sprintf(buf, "%d", Shombol);
            SetTextColor(hdc, RGB(0,0,0));
            SetBkMode(hdc, TRANSPARENT);
            TextOut(hdc, 390, 70, buf, strlen(buf));

            EndPaint(hwnd, &ps);
        }
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
            FILE *f = fopen("YourScore.txt", "w");
                if(f) {
                    fprintf(f, "%d", Shombol);
                    fclose(f);
                }
            return 0;
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
}
int main() {
    FILE *f = fopen("YourScore.txt", "r");
        if(f) {
            fscanf(f, "%d", &Shombol);
            fclose(f);
        } else {
            Shombol = 0;
        }
    HINSTANCE hInstance = GetModuleHandle(NULL);
    WNDCLASSEX wc = {0};
        wc.cbSize = sizeof(WNDCLASSEX);
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(NULL); 
        wc.lpszClassName = "mainPrograminterface";
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);   
    RegisterClassEx(&wc);
    HWND hwnd = CreateWindowEx(0, "mainPrograminterface", "Shombol counter", WS_CAPTION | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,800, 600, NULL, NULL, GetModuleHandle(NULL), NULL);
    HWND hwndButton = CreateWindow("BUTTON", "Click Me!", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 340, 260, 120, 80,hwnd, (HMENU)MY_BUTTON_ID, hInstance, NULL);
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    MSG msg = {0};
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}