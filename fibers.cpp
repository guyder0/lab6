#include "framework.h"
#include "Resource.h"
#include <random>

// параметры передаваемые в поток
typedef struct {
    RECT rect;
    int numb;
} DATA, * PDATA;

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static HWND hwnd;
WCHAR szFiberChildClass[100] = L"fiberchild";
static int width = 600, height = 300;
HDC hdc;

static PDATA pDataArray[2];
LPVOID FiberArray[2], mainFiber;
BOOL stopfibers = false, end = false;

DWORD WINAPI FiberMessageCycle(LPVOID) {
    HINSTANCE hInstance = GetModuleHandle(nullptr);
    RegisterFiberChild(hInstance);
    hwnd = CreateWindow(szFiberChildClass, L"Здесь работают волокна", WS_CAPTION, CW_USEDEFAULT, 0, width, height, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);

    MSG msg;
    while (GetMessage(&msg, nullptr, 0, 0)) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return 0;
}

ATOM RegisterFiberChild(HINSTANCE hInstance)
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
    wcex.lpszMenuName = szFiberChildClass;
    wcex.lpszClassName = szFiberChildClass;
    wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

HWND GetFiberChild() {
    return hwnd;
}

DWORD WINAPI MyFiberFunction(LPVOID lpParam) {
    PDATA data = (PDATA)lpParam;
    HBRUSH brush;
    srand((int)data->numb);
    while (not stopfibers) {
        brush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        FillRect(hdc, &(data->rect), brush);
        DeleteObject(brush);
        Sleep(100);
    }
    return 0;
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
    case WM_CREATE:
    {
        ShowWindow(hWnd, SW_SHOW);
        //for (int i = 0; i < 2; i++) {
        //    pDataArray[i] = (PDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(DATA));
        //    if (pDataArray[i] == NULL) DestroyWindow(hWnd);
        //    pDataArray[i]->numb = i;
        //}
        //LPVOID mainFiber = ConvertThreadToFiber(nullptr);
        //pDataArray[0]->rect = { 0, 0, width / 2, height };
        //pDataArray[1]->rect = { width / 2, 0, width, height };
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
        //for (int i = 0; i < 2; i++) {
        //    DeleteFiber(FiberArray[i]);
        //    if (pDataArray[i] != NULL)
        //        HeapFree(GetProcessHeap(), 0, pDataArray[i]);
        //    ConvertFiberToThread;
        //}
        //stopfibers = true;
        PostQuitMessage(0);
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}