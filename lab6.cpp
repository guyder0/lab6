#include "framework.h"
#include "lab6.h"

#define MAX_LOADSTRING 100
#define ID1_CREATETHREAD 1001
#define ID1_DELETETHREAD 1002
#define ID2_FIRSTAPP 2001
#define ID2_SECONDAPP 2002
#define ID3_FIRSTAPP 3001
#define ID3_SECONDAPP 3002

// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Разместите код здесь.

    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_LAB6, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_LAB6));

    MSG msg;

    // Цикл основного сообщения:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

    return (int) msg.wParam;
}



//
//  ФУНКЦИЯ: MyRegisterClass()
//
//  ЦЕЛЬ: Регистрирует класс окна.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_LAB6));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_LAB6);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   ФУНКЦИЯ: InitInstance(HINSTANCE, int)
//
//   ЦЕЛЬ: Сохраняет маркер экземпляра и создает главное окно
//
//   КОММЕНТАРИИ:
//
//        В этой функции маркер экземпляра сохраняется в глобальной переменной, а также
//        создается и выводится главное окно программы.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной
   DWORD style = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
   int width = 120, height = 30, dx = 10;
   const int m = 3, n = 4;
   RECT rect = { 0, 0, n * width + (n + 1) * dx, m * height + (m + 1) * dx };
   int cords_x[m][n], cords_y[m][n];
   for (int i = 0; i < m; i++)
       for (int j = 0; j < n; j++) {
           cords_x[i][j] = (j + 1) * dx + j * width;
           cords_y[i][j] = (i + 1) * dx + i * height;
       }

   AdjustWindowRect(&rect, style, TRUE);
   HWND hWnd = CreateWindowW(szWindowClass, szTitle, style,
      CW_USEDEFAULT, 0, rect.right - rect.left, rect.bottom - rect.top, nullptr, nullptr, hInstance, nullptr);
   CreateWindow(L"STATIC", L"Задание 1", WS_VISIBLE | WS_CHILD, cords_x[0][0], cords_y[0][0], width, height, hWnd, NULL, hInstance, NULL);
   CreateWindow(L"BUTTON", L"Создать поток", WS_VISIBLE | WS_CHILD, cords_x[1][0], cords_y[1][0], width, height, hWnd, (HMENU)ID1_CREATETHREAD, hInstance, NULL);
   CreateWindow(L"BUTTON", L"Удалить поток", WS_VISIBLE | WS_CHILD, cords_x[2][0], cords_y[2][0], width, height, hWnd, (HMENU)ID1_DELETETHREAD, hInstance, NULL);
   CreateWindow(L"STATIC", L"Задание 2,3,4,5", WS_VISIBLE | WS_CHILD, cords_x[0][1], cords_y[0][1], width, height, hWnd, NULL, hInstance, NULL);
   CreateWindow(L"BUTTON", L"1-е приложение", WS_VISIBLE | WS_CHILD, cords_x[1][1], cords_y[1][1], width, height, hWnd, (HMENU)ID2_FIRSTAPP, hInstance, NULL);
   CreateWindow(L"BUTTON", L"2-е приложение", WS_VISIBLE | WS_CHILD, cords_x[2][1], cords_y[2][1], width, height, hWnd, (HMENU)ID2_SECONDAPP, hInstance, NULL);
   CreateWindow(L"STATIC", L"Задание 6,7", WS_VISIBLE | WS_CHILD, cords_x[0][2], cords_y[0][2], width, height, hWnd, NULL, hInstance, NULL);
   CreateWindow(L"BUTTON", L"1-е приложение", WS_VISIBLE | WS_CHILD, cords_x[1][2], cords_y[1][2], width, height, hWnd, (HMENU)ID3_FIRSTAPP, hInstance, NULL);
   CreateWindow(L"BUTTON", L"2-е приложение", WS_VISIBLE | WS_CHILD, cords_x[2][2], cords_y[2][2], width, height, hWnd, (HMENU)ID3_SECONDAPP, hInstance, NULL);
   CreateWindow(L"STATIC", L"Задание 8,9", WS_VISIBLE | WS_CHILD, cords_x[0][3], cords_y[0][3], width, height, hWnd, NULL, hInstance, NULL);
   CreateWindow(L"BUTTON", L"Создать волокно", WS_VISIBLE | WS_CHILD, cords_x[1][3], cords_y[1][3], width, height, hWnd, (HMENU)ID3_FIRSTAPP, hInstance, NULL);

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  ФУНКЦИЯ: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  ЦЕЛЬ: Обрабатывает сообщения в главном окне.
//
//  WM_COMMAND  - обработать меню приложения
//  WM_PAINT    - Отрисовка главного окна
//  WM_DESTROY  - отправить сообщение о выходе и вернуться
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            switch (wmId)
            {
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
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
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}