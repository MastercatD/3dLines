
#include "framework.h"
#include "3dLines.h"
#include "Segment3D.h"


#define MAX_LOADSTRING 100
#define BUTTON_X 1
#define BUTTON_Y 2
#define BUTTON_Z 3
#define LINE_BUTTON_1 4
#define LINE_BUTTON_2 17

#define EDIT_X_11 5
#define EDIT_X_12 6
#define EDIT_Y_11 7
#define EDIT_Y_12 8
#define EDIT_Z_11 9
#define EDIT_Z_12 10

#define EDIT_X_21 11
#define EDIT_X_22 12
#define EDIT_Y_21 13
#define EDIT_Y_22 14
#define EDIT_Z_21 15
#define EDIT_Z_22 16

enum class Axis {
  x = 0,
  y = 1,
  z = 2
};


// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
bool draw1 = false, draw2 = false;
Axis axis = Axis::x;
Segment3D line1, line2;

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



void DrawLine(Segment3D line, HDC hDc, Axis axis) {
  int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
  
  switch (axis) {
    case Axis::x:
      x1 = line.GetStart()->GetZ();
      y1 = line.GetStart()->GetZ();
      x2 = line.GetEnd()->GetY();
      y2 = line.GetEnd()->GetY();
      break;
    case Axis::y:
      x1 = line.GetStart()->GetX();
      y1 = line.GetStart()->GetX();
      x2 = line.GetEnd()->GetZ();
      y2 = line.GetEnd()->GetZ();
      break;
    case Axis::z:
      x1 = line.GetStart()->GetX();
      y1 = line.GetStart()->GetX();
      x2 = line.GetEnd()->GetY();
      y2 = line.GetEnd()->GetY();
      break;
  }
  MoveToEx(hDc, 675 + x1, 265 - y1, NULL); 
  LineTo(hDc, 675 + x2, 265 - y2);
}

HWND CreateText(HWND hWnd, const wchar_t text[255], int x, int y) {
  HWND hText = CreateWindow(TEXT("EDIT"), text, 
    WS_CHILD | WS_VISIBLE, 
    x, y, 
    20, 20, 
    hWnd, 0, 
    0, 0);
  return hText;
}

HWND CreateEdit(HWND hWnd, HINSTANCE hInstance, int x, int y, int idEdit) {
  HWND hEdit = CreateWindow(L"EDIT", L"", 
    WS_BORDER | WS_CHILD | WS_VISIBLE, 
    x, y, 
    60, 20, 
    hWnd, (HMENU)idEdit, 
    hInstance, nullptr);
  return hEdit;
}


HWND CreateButton(HWND hWnd, HINSTANCE hInstance, const wchar_t text[255], int x, int y, int idButton) {
  HWND hButton = CreateWindow(L"button", text,
    WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
    x, y,
    90, 30,
    hWnd,
    (HMENU)idButton,
    hInstance, NULL);
  return hButton;
}

void CreatePanel(HWND hWnd, HINSTANCE hInstance, int x, int y, int idStart) {
  CreateEdit(hWnd, hInstance, x + 30, y, idStart++);
  CreateText(hWnd, L"X1:", x, y);
  CreateEdit(hWnd, hInstance, x + 30, y + 35, idStart++);
  CreateText(hWnd, L"Y1:", x, y + 35);
  CreateEdit(hWnd, hInstance, x + 30, y + 70, idStart++);
  CreateText(hWnd, L"Z1:", x, y + 70);
  CreateEdit(hWnd, hInstance, x + 130, y, idStart++);
  CreateText(hWnd, L"X2:", x + 100, y);
  CreateEdit(hWnd, hInstance, x + 130, y + 35, idStart++);
  CreateText(hWnd, L"Y2:", x + 100, y + 35);
  CreateEdit(hWnd, hInstance, x + 130, y + 70, idStart++);
  CreateText(hWnd, L"Z2:", x + 100, y + 70);
}

bool GetValue(HWND hWnd, int edit_id, int& valueDestination) {
  int Length = GetWindowTextLengthW(GetDlgItem(hWnd, edit_id)) + 1;
  wchar_t* TValue = new wchar_t[Length];
  GetWindowTextW(GetDlgItem(hWnd, edit_id), TValue, Length);
  valueDestination = _wtoi(TValue);
  delete (TValue);
  return true;
}

Segment3D CheckPanel(HWND hWnd, HINSTANCE hInstance, int idStart) {
  int x, y, z;
  GetValue(hWnd, idStart++, x);
  GetValue(hWnd, idStart++, y);
  GetValue(hWnd, idStart++, z);
  Vector3D point1(x, y, z);
  GetValue(hWnd, idStart++, x);
  GetValue(hWnd, idStart++, y);
  GetValue(hWnd, idStart++, z);
  Vector3D point2(x, y, z);
  return Segment3D(point1, point2);
}




int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);



    // Инициализация глобальных строк
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_MY3DLINES, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Выполнить инициализацию приложения:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_MY3DLINES));

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




ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_MY3DLINES));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_MY3DLINES);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}


BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Сохранить маркер экземпляра в глобальной переменной


   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
     CW_USEDEFAULT, 0, CW_USEDEFAULT, 0, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
     return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);
   
   CreateButton(hWnd, hInstance, L"Ось X", 20, 20, BUTTON_X);
   CreateButton(hWnd, hInstance, L"Ось Y", 20, 55, BUTTON_Y);
   CreateButton(hWnd, hInstance, L"Ось Z", 20, 90, BUTTON_Z);

   CreatePanel(hWnd, hInstance, 20, 135, 10);
   CreateButton(hWnd, hInstance, L"Отрезок 1", 20, 240, LINE_BUTTON_1);

   CreatePanel(hWnd, hInstance, 20, 285, 20);
   CreateButton(hWnd, hInstance, L"Отрезок 2", 20, 390, LINE_BUTTON_2);
   
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
      // Разобрать выбор в меню:
      switch (wmId)
      {
      case LINE_BUTTON_1:
        line1 = CheckPanel(hWnd, hInst, 10);
        draw1 = true;
        InvalidateRect(hWnd, NULL, NULL);//обновляем окно
        break;
      case LINE_BUTTON_2:
        line2 = CheckPanel(hWnd, hInst, 20);
        draw2 = true;
        InvalidateRect(hWnd, NULL, NULL);//обновляем окно
        break;
      case BUTTON_X:
        axis = Axis::x;
        InvalidateRect(hWnd, NULL, NULL);
        break;
      case BUTTON_Y:
        axis = Axis::y;
        InvalidateRect(hWnd, NULL, NULL);
        break;
      case BUTTON_Z:
        axis = Axis::z;
        InvalidateRect(hWnd, NULL, NULL);
        break;
      case IDM_ABOUT:
        DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
        break;
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
            Rectangle(hdc, 250, 20, 1100, 510);
            MoveToEx(hdc, 675, 20, NULL);
            LineTo(hdc, 675, 510);
            MoveToEx(hdc, 250, 265, NULL);
            LineTo(hdc, 1100, 265);
            if (draw1) {
              DrawLine(line1, hdc, axis);
            }
            if (draw2) {
              DrawLine(line2, hdc, axis);
            }

            //675
            // 265
            //hdc = BeginPaint(hWnd, &ps);
            //здесь можно вставить какие-нибудь функции рисования
            //DrawLine()
              //обновляем окно
            //ValidateRect(hWnd, NULL);
            //заканчиваем рисовать
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

INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
