// Library's that Program needs them!
    #include <stdio.h> //standard input output 
    #include <stdlib.h> //for system
    #include <stdbool.h> //for Bool varibles
    #include <windows.h> //for window that create in windowsOS
    #include <mmsystem.h> //for playing sounds
    #include <string.h> //for work with strings
    #include <time.h> //for getting time and date

// adding these library to compiler to attach in compile process
    #pragma comment(lib, "gdi32.lib")
    #pragma comment(lib, "user32.lib")
    #pragma comment(lib, "winmm.lib")

// button macro
    // Main window Buttons
        #define Shombol_BUTTON_ID 1
        #define SHOP_BUTTON_ID 2
    // Shop window Buttons
        #define AutoClicker_BUTTON_ID 3
        // Sound Buttons
        #define ASound_Pack_ID 4
        #define RSound_Pack_ID 51
        #define ASound_Test 6
        #define RSound_Test 7
        // BackGround Color Buttons
        #define BGC_white 8
        #define BGC_red 9
        #define BGC_green 10
        #define BGC_yellow 11
        #define BGC_blue 12

// Golbal variables 
    HWND hwndShop = NULL; // for Shop window handeler
    HWND hwnd = NULL; // for Main window handeler
    HANDLE AutoCSThread = NULL; // autoclicker thread handeler
    bool AutoCT; // the state of Autoclicker thread | true = autoclicker is on , false = autoclicker is off/its not purchased yet
    time_t Rsec; // time that we use for calculating Autoclicker in time the program wasnt running
    bool SoundpackAPS; // Status of Active Soundpack A
    bool SoundpackRPS; // Status of Active Soundpack R
    // Soundpack Path structure
    struct Soundpack  
    {
        wchar_t sound1[50]; // every 100 shombols
        wchar_t sound2[50]; // every 10 shombols
        wchar_t sound3[50]; // every shombols
    };
    struct Soundpack SPS = {0};
    // Save File Structure
        struct SF
        {
            wchar_t SFSignature [5]; // File Signature
            int Soundpack; // Soundpack number | 1 = A soundpack(by Aria), 2 = R soundpack(by Rasol)
            int Shombol; // the points
            int AutoCS; // AutoClicker Level
            int BGC [3]; // Background color
            time_t Bsec; // the time that Program Stopped ==> used for Autoclicker when program wasnt running
        };
        struct SF SFS = {0};

// Auto CLicker Function (thread)
    DWORD WINAPI AutoClicker(LPVOID param)
    {
        if ((SFS.AutoCS >= 2 ) && (AutoCT == TRUE)) // if we have autoclicker and also when its Active the Autoclicker starts
        {
            while (AutoCT == TRUE)
            {
                SFS.Shombol += (SFS.AutoCS-1);
                PostMessage(hwnd, WM_APP + 1, 0, 0); // for updating the window to show new value of Shombol
                Sleep(1000); // simulating 1 secound
            }
        }
        return 0; // death of thread
    }

// window process
  // Main Window Message Prosssesor function------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------             
        LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
        switch(uMsg) 
        {
            case WM_APP + 1:                             // for updating main window
                InvalidateRect(hwnd, NULL, TRUE);
            break;
            case WM_COMMAND:
                if(LOWORD(wParam) == Shombol_BUTTON_ID) {
                    ++SFS.Shombol;
                    if (SFS.Shombol%100==0)
                        {
                            PlaySoundW (SPS.sound1, NULL, SND_FILENAME | SND_ASYNC);
                        }
                    else if (SFS.Shombol%10==0)
                        {
                            PlaySoundW (SPS.sound2, NULL, SND_FILENAME | SND_ASYNC);
                        }
                    else
                        {
                            PlaySoundW (SPS.sound3, NULL, SND_FILENAME | SND_ASYNC);
                        } 
                }
                if(LOWORD(wParam) == SHOP_BUTTON_ID) {
                    if (hwndShop == NULL)
                    {
                        hwndShop = CreateWindowEx(0, L"Shopwindow", L"Shombol Shop", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 200, 200, 700, 400, hwnd, NULL, GetModuleHandle(NULL), NULL);
                        HWND shopOP1 = CreateWindowEx(0, L"BUTTON", L"Auto Clikcer", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 10, 100, 50, hwndShop, (HMENU)AutoClicker_BUTTON_ID, GetModuleHandle(NULL), NULL);
                        // SoundPack Buy Buttons
                        HWND shopOPS1 = CreateWindowEx(0, L"BUTTON", L"A Sound Pack", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 70, 100, 50, hwndShop, (HMENU)ASound_Pack_ID, GetModuleHandle(NULL), NULL);
                        HWND shopOPS2 = CreateWindowEx(0, L"BUTTON", L"R Sound Pack", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 130, 100, 50, hwndShop, (HMENU)RSound_Pack_ID, GetModuleHandle(NULL), NULL);
                        // SoundPack Test Buttons
                        HWND shopOPST1 = CreateWindowEx(0, L"BUTTON", L"Test", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 120, 95, 40, 25, hwndShop, (HMENU)ASound_Test, GetModuleHandle(NULL), NULL);
                        HWND shopOPST2 = CreateWindowEx(0, L"BUTTON", L"Test", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 120, 155, 40, 25, hwndShop, (HMENU)RSound_Test, GetModuleHandle(NULL), NULL);
                        // BackGround Color Buttons
                        HWND shopOPBGC1 = CreateWindowEx(0, L"BUTTON", L"White BG", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 190, 100, 50, hwndShop, (HMENU)BGC_white, GetModuleHandle(NULL), NULL);
                        HWND shopOPBGC2 = CreateWindowEx(0, L"BUTTON", L"Red BG", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 120, 190, 100, 50, hwndShop, (HMENU)BGC_red, GetModuleHandle(NULL), NULL);
                        HWND shopOPBGC3 = CreateWindowEx(0, L"BUTTON", L"Green BG", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 230, 190, 100, 50, hwndShop, (HMENU)BGC_green, GetModuleHandle(NULL), NULL);
                        HWND shopOPBGC4 = CreateWindowEx(0, L"BUTTON", L"Yellow BG", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 340, 190, 100, 50, hwndShop, (HMENU)BGC_yellow, GetModuleHandle(NULL), NULL);
                        HWND shopOPBGC5 = CreateWindowEx(0, L"BUTTON", L"Blue BG", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 450, 190, 100, 50, hwndShop, (HMENU)BGC_blue, GetModuleHandle(NULL), NULL);
                        ShowWindow(hwndShop, SW_SHOW);
                        UpdateWindow(hwndShop);
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
                SetBkMode(hdc, TRANSPARENT);
                HBRUSH brush = CreateSolidBrush(RGB(SFS.BGC[0], SFS.BGC[1], SFS.BGC[2]));
                FillRect(hdc, &ps.rcPaint, brush);
                DeleteObject(brush);
                // variable print buffer
                    wchar_t buf[100];
                    swprintf_s(buf,100, L"%d", SFS.Shombol);
                // calculating the x axis for text
                    // Size valriables
                        SIZE shombolSize;
                        SIZE maintextSize;
                    // getting size
                        GetTextExtentPoint32(hdc, L"Your Shombol Size:", lstrlenW(L"Your Shombol Size:"), &maintextSize);
                        GetTextExtentPoint32(hdc, buf, lstrlenW(buf), &shombolSize);
                    // giving size
                        int Shombolcx= shombolSize.cx;
                        int maintextcx= maintextSize.cx;
                    // calculating the position 
                        int Shombolx= (800-Shombolcx)/2;
                        int maintextx= (800-maintextcx)/2;
                // Print main text and values
                    TextOut(hdc, maintextx, 100, L"Your Shombol Size:", lstrlenW(L"Your Shombol Size:"));
                    TextOut(hdc, Shombolx, 120, buf, lstrlenW(buf));
                EndPaint(hwnd, &ps);
            }
            break;
        // Destory the window and end of program
            case WM_DESTROY:
            {
                SFS.Bsec=Rsec;
                AutoCT = FALSE;
                if (AutoCSThread != NULL) {
                    WaitForSingleObject(AutoCSThread, INFINITE);
                    CloseHandle(AutoCSThread);
                }
                FILE *outputS = fopen("Save.sa", "wb");
                fwrite(&SFS, sizeof(struct SF), 1, outputS);
                fclose(outputS);
                PostQuitMessage(0);
                return 0;
            }
            default:
                return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

  // Shop Window Message Prosssesor function ------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------  
        LRESULT CALLBACK ShopWindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
            switch(uMsg) {
                case WM_COMMAND:
                    if (LOWORD(wParam) == AutoClicker_BUTTON_ID)
                    {
                        if (SFS.AutoCS <= 11)
                        {
                            if (SFS.Shombol >= (5000 * SFS.AutoCS))
                            {
                                SFS.Shombol -= (5000 * SFS.AutoCS);
                                SFS.AutoCS++;
                            }
                            else
                            {
                                MessageBox(hwndShop, L"You don't have enough Shombols!", L"Warning", MB_OK | MB_ICONHAND | MB_TOPMOST);
                            }
                        }
                        else
                        {
                            MessageBox(hwndShop, L"AutoClicker reached the max level!", L"Warning", MB_OK | MB_ICONHAND | MB_TOPMOST);
                        }
                        InvalidateRect(hwndShop, NULL, TRUE);
                        UpdateWindow(hwndShop);
                        InvalidateRect(hwnd, NULL, TRUE);
                        UpdateWindow(hwnd);
                    }
                    else if (LOWORD(wParam) == ASound_Pack_ID)
                    {
                        SFS.Soundpack = 1;
                        wcscpy_s(SPS.sound1, 50, L"Sounds\\Pd\\sound1.wav");
                        wcscpy_s(SPS.sound2, 50, L"Sounds\\Pd\\sound11.wav");
                        wcscpy_s(SPS.sound3, 50, L"Sounds\\Pd\\sound12.wav");
                    }
                    else if (LOWORD(wParam) == RSound_Pack_ID)
                    {
                        SoundpackRPS = 1;
                        SFS.Soundpack = 2;
                        wcscpy_s(SPS.sound1, 50, L"Sounds\\Pr\\sound1.wav");
                        wcscpy_s(SPS.sound2, 50, L"Sounds\\Pr\\sound11.wav");
                        wcscpy_s(SPS.sound3, 50, L"Sounds\\Pr\\sound12.wav");
                    }
                    else if (LOWORD(wParam) == ASound_Test)
                    {
                        PlaySoundW(L"Sounds\\Pd\\sound12.wav", NULL, SND_FILENAME | SND_ASYNC);
                    }
                    else if (LOWORD(wParam) == RSound_Test)
                    {
                        PlaySoundW(L"Sounds\\Pr\\sound12.wav", NULL, SND_FILENAME | SND_ASYNC);
                    }
                    else if (LOWORD(wParam) == BGC_white)
                    {
                        SFS.BGC[0] = 255;
                        SFS.BGC[1] = 255;
                        SFS.BGC[2] = 255;
                        InvalidateRect(hwnd, NULL, TRUE);
                        InvalidateRect(hwndShop, NULL, TRUE);
                    }
                    else if (LOWORD(wParam) == BGC_red)
                    {
                        SFS.BGC[0] = 255;
                        SFS.BGC[1] = 43;
                        SFS.BGC[2] = 43;
                        InvalidateRect(hwnd, NULL, TRUE);
                        InvalidateRect(hwndShop, NULL, TRUE);
                    }
                    else if (LOWORD(wParam) == BGC_green)
                    {
                        SFS.BGC[0] = 68;
                        SFS.BGC[1] = 255;
                        SFS.BGC[2] = 43;
                        InvalidateRect(hwnd, NULL, TRUE);
                        InvalidateRect(hwndShop, NULL, TRUE);
                    }
                    else if (LOWORD(wParam) == BGC_yellow)
                    {
                        SFS.BGC[0] = 255;
                        SFS.BGC[1] = 241;
                        SFS.BGC[2] = 43;
                        InvalidateRect(hwnd, NULL, TRUE);
                        InvalidateRect(hwndShop, NULL, TRUE);
                    }
                    else if (LOWORD(wParam) == BGC_blue)
                    {
                        SFS.BGC[0] = 43;
                        SFS.BGC[1] = 192;
                        SFS.BGC[2] = 255;
                        InvalidateRect(hwnd, NULL, TRUE);
                        InvalidateRect(hwndShop, NULL, TRUE);
                    }
                break;
                case WM_PAINT: {
                    PAINTSTRUCT ps;
                    HDC hdc = BeginPaint(hwnd, &ps);
                    SetBkMode(hdc, TRANSPARENT);
                    HBRUSH brush = CreateSolidBrush(RGB(SFS.BGC[0], SFS.BGC[1], SFS.BGC[2]));
                    FillRect(hdc, &ps.rcPaint, brush);
                    DeleteObject(brush);
                    wchar_t AutoCSLevel [11];
                    swprintf_s(AutoCSLevel, 11, L"Level: %d", SFS.AutoCS-1);
                    TextOutW(hdc, 115, 15, L"*Auto clicker have 10 levels and its mine for you even if the program closed!", lstrlenW(L"*Auto clicker have 10 levels and its mine for you even if the program closed!"));
                    TextOut(hdc, 120, 30, AutoCSLevel, lstrlenW(AutoCSLevel));
                    TextOut(hdc, 120, 70, L"*A Sound pack (defult Voice by Aria)", lstrlenW(L"*A Sound pack (defult Voice by Aria)"));
                    TextOut(hdc, 120, 130, L"*R Sound pack (by Rasol)", lstrlenW(L" * R Sound pack(by Rasol)"));

                    EndPaint(hwnd, &ps);
                }
                break;
                case WM_DESTROY:
                    hwndShop = NULL;
                    DestroyWindow(hwnd);
                    return 0;
                default:
                    return DefWindowProc(hwnd, uMsg, wParam, lParam);
            }
            return 0;
        }

// Main Function ---------------------------------------------------------------------------------------------------------------------------------------------------------------------
        int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow)
        {
            Rsec = time(NULL);
            FILE *inputS = fopen("Save.sa", "rb"); 
            if (inputS!=NULL)
            {
                fread(&SFS, sizeof(struct SF), 1, inputS);
                if (SFS.AutoCS >= 2)
                {
                    SFS.Shombol=((int)(Rsec-SFS.Bsec)*(SFS.AutoCS-1)+SFS.Shombol)<<1;
                }
                fclose(inputS);
            }
            else
            {
               SFS.Shombol=0;
               SFS.AutoCS=1;
               SFS.Soundpack=1;
               SFS.BGC[0] = 255;
               SFS.BGC[1] = 255;
               SFS.BGC[2] = 255;
            }
            if (SFS.Soundpack==1)
            {
                wcscpy_s(SPS.sound1, 50, L"Sounds\\Pd\\sound1.wav");
                wcscpy_s(SPS.sound2, 50, L"Sounds\\Pd\\sound11.wav");
                wcscpy_s(SPS.sound3, 50, L"Sounds\\Pd\\sound12.wav");
            }
            else if (SFS.Soundpack==2)
            {
                wcscpy_s(SPS.sound1, 50, L"Sounds\\Pr\\sound1.wav");
                wcscpy_s(SPS.sound2, 50, L"Sounds\\Pr\\sound11.wav");
                wcscpy_s(SPS.sound3, 50, L"Sounds\\Pr\\sound12.wav");
            }

            WNDCLASSEX wc = {0};
                wc.cbSize = sizeof(WNDCLASSEX);
                wc.lpfnWndProc = WindowProc;
                wc.hInstance = GetModuleHandle(NULL); 
                wc.lpszClassName = L"mainPrograminterface";
                wc.hbrBackground = CreateSolidBrush(RGB(SFS.BGC[0], SFS.BGC[1], SFS.BGC[2]));
                wc.hCursor = LoadCursor(NULL, IDC_ARROW);   
            RegisterClassEx(&wc);
            hwnd = CreateWindowEx(0, L"mainPrograminterface", L"Shombol counter", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, CW_USEDEFAULT, CW_USEDEFAULT,800, 600, NULL, NULL, GetModuleHandle(NULL), NULL);
            HWND hwndButton = CreateWindowEx(0, L"BUTTON", L"Shombol!", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 340, 260, 120, 80,hwnd, (HMENU)Shombol_BUTTON_ID, hInstance, NULL);
            HWND hwndButtonshop = CreateWindowEx(0, L"BUTTON", L"Shop", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 370, 360, 60, 40,hwnd, (HMENU)SHOP_BUTTON_ID, hInstance, NULL);
            ShowWindow(hwnd, SW_SHOW);
            UpdateWindow(hwnd);

        // Shop window Creation
            WNDCLASSEX shopwc = {0};
                shopwc.cbSize = sizeof(WNDCLASSEX);
                shopwc.lpfnWndProc = ShopWindowProc;
                shopwc.hInstance = GetModuleHandle(NULL); 
                shopwc.lpszClassName = L"Shopwindow";
                shopwc.hbrBackground = CreateSolidBrush(RGB(SFS.BGC[0], SFS.BGC[1], SFS.BGC[2]));
                shopwc.hCursor = LoadCursor(NULL, IDC_ARROW);
            RegisterClassEx(&shopwc);

            if (SFS.AutoCS > 1) {
                AutoCT = TRUE;
                AutoCSThread = CreateThread(NULL, 0, AutoClicker, NULL, 0, NULL);
            }

        // Message Loop
            MSG msg = {0};
            while(GetMessage(&msg, NULL, 0, 0)) {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        return 0;
        }