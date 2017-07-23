#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include <windows.h>
#include "automatas.h"

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hisnt, LPSTR pCmdLine, int nCmdShow)
{
    // Register the window class.
    const wchar_t CLASS_NAME[]  = L"Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        "TP1",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, 500, 300,

        NULL,       // Parent window
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }


    HWND label = CreateWindow(TEXT("static"), TEXT("ST_U"),
                            WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                            200, 50, 140, 20,
                            hwnd, (HMENU)(501),
                            (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE), NULL);
    SetWindowText(label, TEXT("Lexema:"));
    HWND Answer = CreateWindow(TEXT("static"), TEXT(""),
                            WS_CHILD | WS_VISIBLE | WS_TABSTOP,
                            200, 150, 250, 50,
                            hwnd, (HMENU)(502),
                            (HINSTANCE) GetWindowLong (hwnd, GWL_HINSTANCE), NULL);
    HWND textbox = CreateWindow(TEXT("Edit"), TEXT(""), WS_CHILD | WS_VISIBLE | WS_BORDER, 200, 100, 140, 20, hwnd, (HMENU)(503), NULL, NULL);

    HWND button = CreateWindow(
                            TEXT("BUTTON"),  // Predefined class; Unicode assumed
                            TEXT("Chequear"),      // Button text
                            WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,  // Styles
                            200,         // x position
                            200,         // y position
                            100,        // Button width
                            50,        // Button height
                            hwnd,     // Parent window
                            (HMENU)(504),
                            (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE),
                            NULL);      // Pointer not needed.

    // Obtains input from the textbox and puts it into the char array
    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    char text[1024];
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
        break;
    case WM_COMMAND:
        switch( HIWORD( wParam ) )
        {
            case BN_CLICKED:
                if ( LOWORD( wParam ) == 504 ) {
                    GetWindowText(GetDlgItem(hwnd, 503), text, 1024);
                    SetWindowText(GetDlgItem(hwnd, 502), (esUn(text) ? esUn(text) : "Nada"));
                    //button = (HWND)lParam;
                    return 0;
                }
        }
        break;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);

            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));

            EndPaint(hwnd, &ps);
        }
        return 0;

    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
