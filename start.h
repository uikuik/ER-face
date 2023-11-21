#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <time.h>
extern IMAGE img2;
HWND cmd_hwnd;
void start(HWND hWnd)//初始化
{
  cmd_hwnd = GetConsoleWindow();
   ShowWindow(cmd_hwnd,0);
  srand(time(0));
  // 获取窗口句柄
  // 设置窗口样式,允许调整大小
  LONG style = GetWindowLong(hWnd, GWL_STYLE);
  style |= WS_THICKFRAME;
  // 设置新样式
  SetWindowLong(hWnd, GWL_STYLE, style);
  SetWindowPos(hWnd, NULL, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
  MoveWindow(hWnd,0,0,1024,576,NULL);
  setbkmode(TRANSPARENT);
}
