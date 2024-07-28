
#include "framework.h"
#include "3dLines.h"
#include "Segment3D.h"


#define MAX_LOADSTRING 100
#define BUTTON_X 1
#define BUTTON_Y 2
#define BUTTON_Z 3
#define LINE_BUTTON_1 4
#define LINE_BUTTON_2 5
#define COMPUTE_BUTTON 6



enum class Axis {
  x = 0,
  y = 1,
  z = 2
};


// Глобальные переменные:
HINSTANCE hInst;                                // текущий экземпляр
WCHAR szTitle[MAX_LOADSTRING];                  // Текст строки заголовка
WCHAR szWindowClass[MAX_LOADSTRING];            // имя класса главного окна
bool draw1 = false, draw2 = false, drawPoint = false;
Axis axis = Axis::x;
Segment3D line1, line2; // Отрезок 1 и отрезок 2
Vector3D point; //  Точка пересечения

// Отправить объявления функций, включенных в этот модуль кода:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);



// Функция поиска точки пересечения отрезков
bool Interseсt(Segment3D segment1, Segment3D segment2, Vector3D &pointDestination) {

  double eps = 10e-10;
  bool result = false;
  //Точки и векторы, лежащие на прямых для составления уравнений прямой в параметрическом виде
  double x01 = segment1.GetStart().GetX(), x02 = segment2.GetStart().GetX();
  double vectorX1 = (segment1.GetEnd().GetX() - x01),
         vectorX2 = (segment2.GetEnd().GetX() - x02);

  double y01 = segment1.GetStart().GetY(), y02 = segment2.GetStart().GetY();
  double vectorY1 = (segment1.GetEnd().GetY() - y01),
         vectorY2 = (segment2.GetEnd().GetY() - y02);

  double z01 = segment1.GetStart().GetZ(), z02 = segment2.GetStart().GetZ();
  double vectorZ1 = (segment1.GetEnd().GetZ() - z01),
         vectorZ2 = (segment2.GetEnd().GetZ() - z02);


  //Находим пересечение путём выражения параметров s и t из уравнений прямых в
  //параметрическом виде
  double s;
  double t;

  double divisor, dividend;
  // Решение системы уравнений
  divisor = ((x01 - x02) * vectorY1 + (y02 - y01) * vectorX1);
  dividend = (vectorY1 * vectorX2 - vectorX1 * vectorY2);
  if (dividend == 0) {
    // Решение системы уравнений выделением t через Y
    divisor = ((y01 - y02) * vectorZ1 + (z02 - z01) * vectorY1);
    dividend = (vectorZ1 * vectorY2 - vectorY1 * vectorZ2);
    s = divisor / dividend;
    t = (vectorY2 * s + y02 - y01) / vectorY1;
  } else {
    // Решение системы уравнений выделением t через X
    s = divisor / dividend;
    t = (vectorX2 * s + x02 - x01) / vectorX1;
  }

  double x = vectorX2 * s + x02;
  double y = vectorY2 * s + y02;
  double z = vectorZ2 * s + z02;
  if (abs(x - (vectorX1 * t + x01)) < eps &&
      abs(y - (vectorY1 * t + y01)) < eps &&
      abs(z - (vectorZ1 * t + z01)) <
          eps  //Если система решена неправильно, то прямые не пересекаются
      && segment1.InSegment(x, y, z) &&
      segment2.InSegment(
          x, y, z)) {  //Проверка, чтобы точка лежала в пределах обоих отрезков на прямой
    pointDestination = Vector3D(x, y, z);
    result = true;
  }
 
  return result;
}


void GetCoordinates(Vector3D p, HDC hDc, Axis axis, int &xDestination, int &yDestination) {
  switch (axis) {
    case Axis::x:
      xDestination = p.GetZ();
      yDestination = p.GetY();
      break;
    case Axis::y:
      xDestination = p.GetX();
      yDestination = p.GetZ();
      break;
    case Axis::z:
      xDestination = p.GetX();
      yDestination = p.GetY();
      break;
  }
}

void DrawLine(Segment3D line, HDC hDc, Axis axis) {
  int x1 = 0, y1 = 0, x2 = 0, y2 = 0;
  GetCoordinates(line.GetStart(), hDc, axis, x1, y1);
  GetCoordinates(line.GetEnd(), hDc, axis, x2, y2);
  MoveToEx(hDc, 675 + x1, 265 - y1, NULL); 
  LineTo(hDc, 675 + x2, 265 - y2);
}

void DrawPoint(Vector3D p, HDC hDc, Axis axis) {
  int x = 0, y = 0;
  GetCoordinates(p, hDc, axis, x, y);
  HBRUSH hBrush = CreateSolidBrush(RGB(10, 200, 100));
  SelectObject(hDc, hBrush);
  Ellipse(hDc, 670 + x, 260 - y, 680 + x, 270 - y);
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
  HWND hEdit = CreateWindow(L"EDIT", L"300", 
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
     CW_USEDEFAULT, 0, 1150, 600, nullptr, nullptr, hInstance, nullptr);

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

   CreateButton(hWnd, hInstance, L"Расчитать", 20, 430, COMPUTE_BUTTON);
   
   return TRUE;


}

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
        drawPoint = false;
        InvalidateRect(hWnd, NULL, NULL);//обновляем окно
        break;
      case LINE_BUTTON_2:
        line2 = CheckPanel(hWnd, hInst, 20);
        draw2 = true;
        drawPoint = false;
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
      case COMPUTE_BUTTON:
        drawPoint = Interseсt(line1, line2, point);
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
            if (drawPoint) {
              DrawPoint(point, hdc, axis);
            }
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
