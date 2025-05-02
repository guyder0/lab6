#include "framework.h"
#include "Resource.h"
#include <random>

// параметры передаваемые в поток
typedef struct {
    RECT rect;
    int numb;
} DATA, * PDATA;

static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void WINAPI MyFiberFunction(LPVOID);
static HWND hwnd;
WCHAR szFiberChildClass[100] = L"fiberchild";
static int width = 600, height = 300;
HDC hdc;

static PDATA pDataArray[2];
LPVOID FiberArray[2], mainFiber;

DWORD WINAPI FiberMessageCycle(LPVOID) {
    for (int i = 0; i < 2; i++) {
        pDataArray[i] = (PDATA)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(DATA));
        if (pDataArray[i] == NULL) return -1;
        pDataArray[i]->numb = i;
    }
    pDataArray[0]->rect = { 0, 0, width / 2, height };
    pDataArray[1]->rect = { width / 2, 0, width, height };

    mainFiber = ConvertThreadToFiber(nullptr);
    for (int i = 0; i < 2; i++) FiberArray[i] = CreateFiber(0, MyFiberFunction, pDataArray[i]);

    HINSTANCE hInstance = GetModuleHandle(nullptr);
    RegisterFiberChild(hInstance);
    hwnd = CreateWindow(szFiberChildClass, L"Здесь работают волокна", WS_CAPTION, CW_USEDEFAULT, 0, width, height, NULL, NULL, hInstance, NULL);
    ShowWindow(hwnd, SW_SHOW);
    UpdateWindow(hwnd);
    hdc = GetDC(hwnd);

    bool running = true;
    DWORD time = GetTickCount();

    MSG msg;
    while (running) {
        while (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
            if (msg.message == WM_QUIT) {
                running = false;
                break;
            }
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
        if (running && GetTickCount() - time >= 100) {
            time = GetTickCount();
            SwitchToFiber(FiberArray[0]);
        }
    }
    ConvertFiberToThread();
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

void WINAPI MyFiberFunction(LPVOID lpParam) {
    PDATA data = (PDATA)lpParam;
    HBRUSH brush;
    srand((int)data->numb);
    while (true) {
        brush = CreateSolidBrush(RGB(rand() % 256, rand() % 256, rand() % 256));
        FillRect(hdc, &(data->rect), brush);
        DeleteObject(brush);
        if (data->numb == 0) SwitchToFiber(FiberArray[1]);
        if (data->numb == 1) SwitchToFiber(mainFiber);
    }
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
    switch (message)
    {
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
            DeleteFiber(FiberArray[i]);
            if (pDataArray[i] != NULL)
                HeapFree(GetProcessHeap(), 0, pDataArray[i]);
        }
        ReleaseDC(hwnd, hdc);
        PostQuitMessage(0);
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}