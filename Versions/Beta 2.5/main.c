// Library's that Program needs them!
    #include <stdio.h> // standard input output 
    #include <stdlib.h> // for system
    #include <stdbool.h> // for Bool varibles
    #include <windows.h> // for window that create in windowsOS
    #include <mmsystem.h> // for playing sounds
    #include <string.h> // for work with strings
    #include <time.h> // for getting time and date

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
    time_t Rsec; // time that we use for calculating Autoclicker in time the program wasnt running
    bool AutoCT; // the state of Autoclicker thread | true = autoclicker is on , false = autoclicker is off/its not purchased yet
    bool AutoCSPrice; // Autoclicker Price variable

    // Soundpack Path structure
    struct Soundpack  
    {
        wchar_t sound1[50]; // every 100 shombols (shombol tala)
        wchar_t sound2[50]; // every 10 shombols (shombolidi)
        wchar_t sound3[50]; // every shombols (shombol)
    };
    struct Soundpack SPS = {0};
    // Save File Structure (and also the whole program works with this)
        struct BGCPS // backgroundcolor purchase status
        {
            bool RBGC; // red 
            bool GBGC; // green
            bool YBGC; // yellow
            bool BBGC; // blue
        };
        struct SF
        {
            wchar_t SFSignature [5]; // File Signature
            int Soundpack; // Soundpack number | 1 = A soundpack(by Aria), 2 = R soundpack(by Rasol)
            int Shombol; // the points
            int AutoCS; // AutoClicker Level | between 1 and 11 (actuly 1 means level 0 and 11 means level 10 the max level)
            struct BGCPS BPS;
            int BGC [3]; // Background color RGB (Red, Green, Blue)
            time_t Bsec; // the time that Program Stopped ==> used for Autoclicker when program isnt running
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
                PostMessageW(hwnd, WM_APP + 1, 0, 0); // for updating the window to show new value of Shombol
                Sleep(1000); // simulating 1 secound
            }
        }
        return 0; // death of thread
    }

// window process
  // Main Window Message Prosssesor function------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------             
        LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
        {
        switch(uMsg) // choose what to do with any messages from the window (or better to say controlling events)
        {
            case WM_APP + 1: // for updating main window
                InvalidateRect(hwnd, NULL, TRUE); // draw the hwnd window again
            break;
            case WM_COMMAND: // reciving the commad messages from window
                if(LOWORD(wParam) == Shombol_BUTTON_ID) // reciving Shombol_BUTTON_ID command ---> its the main button that adds shombol
                {
                    ++SFS.Shombol; // adding shombol
                    if (SFS.Shombol%100==0) // check the value of Shombol to play the correct sound (100= Shombol tala)
                        {
                            PlaySoundW (SPS.sound1, NULL, SND_FILENAME | SND_ASYNC); // play "Shombol tala" sound
                        }
                    else if (SFS.Shombol%10==0) 
                        {
                            PlaySoundW (SPS.sound2, NULL, SND_FILENAME | SND_ASYNC); // play "shombolidi" sound
                        }
                    else
                        {
                            PlaySoundW (SPS.sound3, NULL, SND_FILENAME | SND_ASYNC); // play "shombol" sound
                        } 
                }
                if(LOWORD(wParam) == SHOP_BUTTON_ID) //reciving SHOP_BUTTON_ID ---> its the button for shop menu(its not a menu but whatever it is)
                {
                    if (hwndShop == NULL) // checks that there arent any other shop window exist (I dont like infinte windows)
                    {
                        hwndShop = CreateWindowEx(0, L"Shopwindow", L"Shombol Shop", WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX, 200, 200, 700, 400, hwnd, NULL, GetModuleHandle(NULL), NULL);                                       // creating shop window
                        HWND shopOP1 = CreateWindowEx(0, L"BUTTON", L"Auto Clikcer", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 10, 100, 50, hwndShop, (HMENU)AutoClicker_BUTTON_ID, GetModuleHandle(NULL), NULL); // creating Autoclicker Button 
                        // SoundPack Buy Buttons
                        HWND shopOPS1 = CreateWindowEx(0, L"BUTTON", L"A Sound Pack", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 70, 100, 50, hwndShop, (HMENU)ASound_Pack_ID, GetModuleHandle(NULL), NULL);       // creating Asoundpack Button                          
                        HWND shopOPS2 = CreateWindowEx(0, L"BUTTON", L"R Sound Pack", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 130, 100, 50, hwndShop, (HMENU)RSound_Pack_ID, GetModuleHandle(NULL), NULL);      // creating Rsoundpack Button 
                        // SoundPack Test Buttons
                        HWND shopOPST1 = CreateWindowEx(0, L"BUTTON", L"Test", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 120, 95, 40, 25, hwndShop, (HMENU)ASound_Test, GetModuleHandle(NULL), NULL);                 // creating Asoundpack test Button 
                        HWND shopOPST2 = CreateWindowEx(0, L"BUTTON", L"Test", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 120, 155, 40, 25, hwndShop, (HMENU)RSound_Test, GetModuleHandle(NULL), NULL);                // creating Rsoundpack test Button 
                        // BackGround Color Buttons
                        HWND shopOPBGC1 = CreateWindowEx(0, L"BUTTON", L"White BG", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 10, 190, 100, 50, hwndShop, (HMENU)BGC_white, GetModuleHandle(NULL), NULL);             // creating White  backgroundcolor
                        HWND shopOPBGC2 = CreateWindowEx(0, L"BUTTON", L"Red BG", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 120, 190, 100, 50, hwndShop, (HMENU)BGC_red, GetModuleHandle(NULL), NULL);                // creating Red    backgroundcolor
                        HWND shopOPBGC3 = CreateWindowEx(0, L"BUTTON", L"Green BG", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 230, 190, 100, 50, hwndShop, (HMENU)BGC_green, GetModuleHandle(NULL), NULL);            // creating green  backgroundcolor
                        HWND shopOPBGC4 = CreateWindowEx(0, L"BUTTON", L"Yellow BG", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 340, 190, 100, 50, hwndShop, (HMENU)BGC_yellow, GetModuleHandle(NULL), NULL);          // creating yellow backgroundcolor
                        HWND shopOPBGC5 = CreateWindowEx(0, L"BUTTON", L"Blue BG", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 450, 190, 100, 50, hwndShop, (HMENU)BGC_blue, GetModuleHandle(NULL), NULL);              // creating blue   backgroundcolor
                        ShowWindow(hwndShop, SW_SHOW); // shows Shop window (draw the window)
                        UpdateWindow(hwndShop); // updating the shop window (draw again the window)
                    }
                }
                InvalidateRect(hwnd, NULL, TRUE); // updating the main window (draw again)
            break;
            case WM_PAINT: // the famous paint! yes this message is drawing somthing on display, looks like the famous paint program (its draw the actuall UI)
            {
                PAINTSTRUCT ps; // the structure and data base for drawing the window and its details
                HDC hdc = BeginPaint(hwnd, &ps); // fills the last line structue
                SetTextColor(hdc, RGB(0, 0, 0)); // as you know english its gonna luch a nuke (sets the color of Text in this window black! (RGB = Red 0, Green 0, Blue 0 --> what color have any color? yeah, black))
                SetBkMode(hdc, TRANSPARENT); // sets the background color of texts into transparent (shows the behind color)  
                HBRUSH brush = CreateSolidBrush(RGB(SFS.BGC[0], SFS.BGC[1], SFS.BGC[2])); // sets the backgound color
                FillRect(hdc, &ps.rcPaint, brush); // fills the background with color of brush (in the previous line)
                DeleteObject(brush); // throws the brush into a trash can (R.I.P brush, it serves us a lot)
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
                    switch (LOWORD(wParam))
                    { 
                        case AutoClicker_BUTTON_ID:
                            if (SFS.AutoCS < 11)
                            {
                                if (SFS.Shombol >= AutoCSPrice)
                                {
                                    SFS.Shombol -= AutoCSPrice;
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
                        break;
                        case ASound_Pack_ID:
                            SFS.Soundpack = 1;
                            wcscpy_s(SPS.sound1, 50, L"Sounds\\Pd\\sound1.wav");
                            wcscpy_s(SPS.sound2, 50, L"Sounds\\Pd\\sound11.wav");
                            wcscpy_s(SPS.sound3, 50, L"Sounds\\Pd\\sound12.wav");
                            InvalidateRect(hwndShop, NULL, TRUE);
                        break;
                        case RSound_Pack_ID:
                            SFS.Soundpack = 2;
                            wcscpy_s(SPS.sound1, 50, L"Sounds\\Pr\\sound1.wav");
                            wcscpy_s(SPS.sound2, 50, L"Sounds\\Pr\\sound11.wav");
                            wcscpy_s(SPS.sound3, 50, L"Sounds\\Pr\\sound12.wav");
                            InvalidateRect(hwndShop, NULL, TRUE);
                        break;
                        case ASound_Test:
                            PlaySoundW(L"Sounds\\Pd\\sound12.wav", NULL, SND_FILENAME | SND_ASYNC);
                        break;
                        case RSound_Test:
                            PlaySoundW(L"Sounds\\Pr\\sound12.wav", NULL, SND_FILENAME | SND_ASYNC);
                        break;
                        case BGC_white:
                                SFS.BGC[0] = 255;
                                SFS.BGC[1] = 255;
                                SFS.BGC[2] = 255;
                                UpdateWindow(hwnd);
                                InvalidateRect(hwndShop, NULL, TRUE);                            
                        break;
                        case BGC_red:
                            switch (SFS.BPS.RBGC)
                            {
                                case 0:
                                    if (SFS.Shombol >= 2000)
                                    {
                                        SFS.BPS.RBGC = 1;
                                        SFS.Shombol -= 2000;
                                        SFS.BGC[0] = 255;
                                        SFS.BGC[1] = 43;
                                        SFS.BGC[2] = 43;
                                        UpdateWindow(hwnd);
                                        InvalidateRect(hwndShop, NULL, TRUE);
                                    }
                                    else
                                    {
                                        MessageBox(hwndShop, L"You don't have enough Shombols!", L"Warning", MB_OK | MB_ICONHAND | MB_TOPMOST);
                                    }
                                break;
                                default:
                                    SFS.BGC[0] = 255;
                                    SFS.BGC[1] = 43;
                                    SFS.BGC[2] = 43;
                                    UpdateWindow(hwnd);
                                    InvalidateRect(hwndShop, NULL, TRUE);
                            }
                        break;
                        case BGC_green:
                            switch (SFS.BPS.GBGC)
                            {
                            case 0:
                                if (SFS.Shombol >= 2000)
                                {
                                    SFS.BPS.GBGC = 1;
                                    SFS.Shombol -= 2000;
                                    SFS.BGC[0] = 68;
                                    SFS.BGC[1] = 255;
                                    SFS.BGC[2] = 43;
                                    UpdateWindow(hwnd);
                                    InvalidateRect(hwndShop, NULL, TRUE);
                                }
                                else
                                {
                                    MessageBox(hwndShop, L"You don't have enough Shombols!", L"Warning", MB_OK | MB_ICONHAND | MB_TOPMOST);
                                }
                                break;
                            default:
                                SFS.BGC[0] = 68;
                                SFS.BGC[1] = 255;
                                SFS.BGC[2] = 43;
                                UpdateWindow(hwnd);
                                InvalidateRect(hwndShop, NULL, TRUE);
                            }
                        break;
                        case BGC_yellow:
                            switch (SFS.BPS.YBGC)
                            {
                            case 0:
                                if (SFS.Shombol >= 2000)
                                {
                                    SFS.BPS.YBGC = 1;
                                    SFS.Shombol -= 2000;
                                    SFS.BGC[0] = 255;
                                    SFS.BGC[1] = 241;
                                    SFS.BGC[2] = 43;
                                    UpdateWindow(hwnd);
                                    InvalidateRect(hwndShop, NULL, TRUE);
                                }
                                else
                                {
                                    MessageBox(hwndShop, L"You don't have enough Shombols!", L"Warning", MB_OK | MB_ICONHAND | MB_TOPMOST);
                                }
                                break;
                            default:
                                SFS.BGC[0] = 255;
                                SFS.BGC[1] = 241;
                                SFS.BGC[2] = 43;
                                UpdateWindow(hwnd);
                                InvalidateRect(hwndShop, NULL, TRUE);
                            }
                        break;
                        case BGC_blue:
                            switch (SFS.BPS.BBGC)
                            {
                            case 0:
                                if (SFS.Shombol >= 2000)
                                {
                                    SFS.BPS.BBGC = 1;
                                    SFS.Shombol -= 2000;
                                    SFS.BGC[0] = 43;
                                    SFS.BGC[1] = 192;
                                    SFS.BGC[2] = 255;
                                    UpdateWindow(hwnd);
                                    InvalidateRect(hwndShop, NULL, TRUE);
                                }
                                else
                                {
                                    MessageBox(hwndShop, L"You don't have enough Shombols!", L"Warning", MB_OK | MB_ICONHAND | MB_TOPMOST);
                                }
                                break;
                            default:
                                SFS.BGC[0] = 43;
                                SFS.BGC[1] = 192;
                                SFS.BGC[2] = 255;
                                UpdateWindow(hwnd);
                                InvalidateRect(hwndShop, NULL, TRUE);
                            }
                        default:
                            return DefWindowProc(hwnd, uMsg, wParam, lParam);
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
                    swprintf_s (AutoCSLevel, 11, L"Level: %d", SFS.AutoCS-1);
                    //AutoCSPrice = 5000 * SFS.AutoCS;
                    wchar_t AutoCSPricec [32];
                    swprintf_s (AutoCSPricec, 32, L"Price: %d", SFS.AutoCS*5000);
                    wchar_t SP1S [12];
                    wchar_t SP2S [12];
                    switch (SFS.Soundpack)
                    { 
                        case 1:
                            swprintf_s(SP1S, 12, L"Active!");
                            swprintf_s(SP2S, 12, L"Non Active!");
                        break;
                        case 2:
                            swprintf_s(SP1S, 12, L"Non Active!");
                            swprintf_s(SP2S, 12, L"Active!");
                        break;
                    }
                    TextOut(hdc, 115, 15, L"*Auto clicker have 10 levels and its mine for you even if the program closed!", lstrlenW(L"*Auto clicker have 10 levels and its mine for you even if the program closed!"));
                    TextOut(hdc, 120, 30, AutoCSLevel, lstrlenW(AutoCSLevel));
                    TextOut(hdc, 190, 30, AutoCSPricec, lstrlenW(AutoCSPricec));
                    TextOut(hdc, 120, 70, L"*A Sound pack (defult Voice by Aria)", lstrlenW(L"*A Sound pack (defult Voice by Aria)"));
                    TextOut(hdc, 120, 130, L"*R Sound pack (by Rasol)", lstrlenW(L" * R Sound pack(by Rasol)"));
                    TextOut(hdc, 10, 240, L"Purchased", lstrlenW(L"Price: free"));
                    switch (SFS.BPS.RBGC)
                    {
                    case 1:
                        TextOut(hdc, 120, 240, L"Purchased", lstrlenW(L"Purchased"));
                    break;
                    default:
                        TextOut(hdc, 120, 240, L"Price: 2000", lstrlenW(L"Price: 2000"));
                    }
                    switch (SFS.BPS.GBGC)
                    {
                    case 1:
                        TextOut(hdc, 230, 240, L"Purchased", lstrlenW(L"Purchased"));
                        break;
                    default:
                        TextOut(hdc, 230 , 240, L"Price: 2000", lstrlenW(L"Price: 2000"));
                    }
                    switch (SFS.BPS.YBGC)
                    {
                    case 1:
                        TextOut(hdc, 340, 240, L"Purchased", lstrlenW(L"Purchased"));
                        break;
                    default:
                        TextOut(hdc, 340, 240, L"Price: 2000", lstrlenW(L"Price: 2000"));
                    }
                    switch (SFS.BPS.BBGC)
                    {
                    case 1:
                        TextOut(hdc, 450, 240, L"Purchased", lstrlenW(L"Purchased"));
                        break;
                    default:
                        TextOut(hdc, 450, 240, L"Price: 2000", lstrlenW(L"Price: 2000"));
                    }
                    EndPaint(hwnd, &ps);
                }
                break;
                case WM_DESTROY:
                    hwndShop = NULL;
                    DestroyWindow(hwnd);
                    return 0;
                default:
                    return DefWindowProc(hwnd, uMsg, wParam, lParam);
                    InvalidateRect(hwndShop, NULL, TRUE);
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
            if (SFS.AutoCS > 1) {
                AutoCT = TRUE;
                AutoCSThread = CreateThread(NULL, 0, AutoClicker, NULL, 0, NULL);
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

        // Message Loop
            MSG msg = {0};
            while(GetMessage(&msg, NULL, 0, 0)) 
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        return 0;
        }