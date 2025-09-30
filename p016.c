#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <mmsystem.h>
#pragma comment(lib, "gdi32.lib")
#pragma comment(lib, "winmm.lib")
#define MY_BUTTON_ID 1
int Shombol = 0;              
LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
    switch(uMsg) {
        case WM_COMMAND:
            if(LOWORD(wParam) == MY_BUTTON_ID) {
                ++Shombol;
                if (Shombol%100==0)
                {
                PlaySound (TEXT("Sounds\\sound2.wav"), NULL, SND_FILENAME | SND_ASYNC);
                }
                else if (Shombol%10==0)
                {
                PlaySound (TEXT("Sounds\\sound3.wav"), NULL, SND_FILENAME | SND_ASYNC);
                }
                else
                {
                PlaySound (TEXT("Sounds\\sound.wav"), NULL, SND_FILENAME | SND_ASYNC);
                }
            }
            InvalidateRect(hwnd, NULL, TRUE);
            UpdateWindow(hwnd); 
        break;
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            SetTextColor(hdc, RGB(0, 0, 0));
            TextOut(hdc, 320, 50, "Your Shombol Size:", 18);
            char buf[50];
            sprintf(buf, "%d", Shombol);
            SetBkMode(hdc, TRANSPARENT);
            TextOut(hdc, 380, 70, buf, strlen(buf));

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
    HWND hwndButton = CreateWindow("BUTTON", "Shombol!", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 340, 260, 120, 80,hwnd, (HMENU)MY_BUTTON_ID, hInstance, NULL);
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    MSG msg = {0};
    while(GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}