#include <Windows.h>
#include <iostream> // For output window
#include <tchar.h> // For _stprintf

using namespace std;

// Global variables
extern float volume;

// Function prototypes
void IncreaseVolume(HWND hWnd);
void DecreaseVolume(HWND hWnd);
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

// Entry point for Windows GUI applications
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Register the window class
    WNDCLASSEX wcex;
    wcex.cbSize = sizeof(WNDCLASSEX);
    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = NULL;
    wcex.lpszClassName = L"VolumeControlClass";
    wcex.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if (!RegisterClassEx(&wcex)) {
        MessageBox(NULL, L"Window Registration Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Create the window
    HWND hWnd = CreateWindowEx(WS_EX_CLIENTEDGE, L"VolumeControlClass", L"Volume Control", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 300, 200, NULL, NULL, hInstance, NULL);

    if (hWnd == NULL) {
        MessageBox(NULL, L"Window Creation Failed!", L"Error!", MB_ICONEXCLAMATION | MB_OK);
        return 0;
    }

    // Show the window
    ShowWindow(hWnd, nCmdShow);
    UpdateWindow(hWnd);

    // Message loop
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return (int)msg.wParam;
}

// Function to increase the volume
void IncreaseVolume(HWND hWnd) {
    if (volume < 1.0f) {
        volume += 0.1f; // Increase volume by 10%
        waveOutSetVolume(NULL, (DWORD)(volume * 0xFFFF)); // Set the new volume level
        InvalidateRect(hWnd, NULL, TRUE); // Invalidate the window to trigger a redraw
    }
}

// Function to decrease the volume
void DecreaseVolume(HWND hWnd) {
    if (volume > 0.0f) {
        volume -= 0.1f; // Decrease volume by 10%
        waveOutSetVolume(NULL, (DWORD)(volume * 0xFFFF)); // Set the new volume level
        InvalidateRect(hWnd, NULL, TRUE); // Invalidate the window to trigger a redraw
    }
}

// Window procedure
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message) {
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        // Draw text or any other UI elements here
        RECT rect;
        GetClientRect(hWnd, &rect);
        wchar_t volumeText[50];
        _stprintf(volumeText, _T("Volume: %.1f"), volume);
        DrawText(hdc, volumeText, -1, &rect, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    case WM_CREATE:
    {
        // Create buttons for increasing and decreasing volume
        CreateWindowW(L"BUTTON", L"Increase Volume", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            50, 50, 150, 30, hWnd, (HMENU)1, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);

        CreateWindowW(L"BUTTON", L"Decrease Volume", WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON,
            50, 100, 150, 30, hWnd, (HMENU)2, (HINSTANCE)GetWindowLongPtr(hWnd, GWLP_HINSTANCE), NULL);
    }
    break;
    case WM_COMMAND:
        switch (LOWORD(wParam)) {
        case 1: // Increase Volume button clicked
            IncreaseVolume(hWnd);
            break;
        case 2: // Decrease Volume button clicked
            DecreaseVolume(hWnd);
            break;
        }
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}