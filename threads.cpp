#include "framework.h"
#include "Resource.h"
#include <random>

// параметры передаваемые в поток
typedef struct {
    HDC hdc;
    RECT rect;
} DATA, * PDATA;

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
WCHAR szThreadChildClass[100] = L"threadchild";
int width = 600, height = 300;
HWND ghWnd;

PDATA pDataArray[2];
HANDLE hThreadArray[2];
DWORD dwThreadIdArray[2];
BOOL stopthreads = false;

ATOM RegisterThreadChild(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc = WndProc;
    wcex.cbClsExtra = 0;
    wcex.cbWndExtra = 0;
    wcex.hInstance = hInstance;
    wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB6));
    wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
    wcex.lpszMenuName = szThreadChildClass;
    wcex.lpszClassName = szThreadChildClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

POINT GetThreadChildSize() {
    POINT p = { width, height };
    return p;
}

DWORD WINAPI MyThreadFunction(LPVOID lpParam) {
    PDATA data = (PDATA)lpParam;
    HBRUSH brush;
    srand((int)data->hdc);
    while (not stopthreads) {
        brush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        FillRect(data->hdc, &(data->rect), brush);
        DeleteObject(brush);
        Sleep(100);
    }
    ReleaseDC(ghWnd, data->hdc);
    return 0;
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_CREATE:
    {
        ghWnd = hWnd;
        for (int i = 0; i < 2; i++) {
            pDataArray[i] = (PDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(DATA));
            if (pDataArray[i] == NULL) DestroyWindow(hWnd);
        }
        pDataArray[0]->hdc = GetDC(hWnd);
        pDataArray[0]->rect = { 0, 0, width / 2, height };
        pDataArray[1]->hdc = GetDC(hWnd);
        pDataArray[1]->rect = { width / 2, 0, width, height };
        for (int i = 0; i < 2; i++)
            hThreadArray[i] = CreateThread(NULL, 0, MyThreadFunction, pDataArray[i], 0, &dwThreadIdArray[i]);

    }
    break;
    case WM_PAINT:
    {
        PAINTSTRUCT ps;
        HDC hdc = BeginPaint(hWnd, &ps);
        EndPaint(hWnd, &ps);
    }
    break;
    case WM_DESTROY:
    {
        for (int i = 0; i < 2; i++) {
            CloseHandle(hThreadArray[i]);
            if (pDataArray[i] != NULL)
                HeapFree(GetProcessHeap(), 0, pDataArray[i]);
        }
        stopthreads = true;
        WaitForMultipleObjects(2, hThreadArray, TRUE, INFINITE);
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}