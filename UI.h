#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include <vector>
#include <sstream>
#include <math.h>
#include <cstdlib>
#include <iostream>
#include <string>
#include <fstream>
using namespace std;
HWND hWnd;
IMAGE img2;
IMAGE STOP_M;
IMAGE LOCK_M;
IMAGE NO_LOCK_M;
IMAGE LOCK_draw;//渲染时
IMAGE NO_LOCK_draw;//渲染时

IMAGE Error_M;//错误
IMAGE Warn_M;// 警告
IMAGE Error_draw;//错误 渲染时
IMAGE Warn_draw;//警告 渲染时

IMAGE Frame_M;//工具箱

IMAGE Toolbox_M;//工具箱
int X=0;
int Y=0;
bool msg_xy_f=0;//是否启动快速鼠标位置获取模式
int FPS=0;
int save_time=0;//自动保存时间s 0为不启动自动保存

bool Li_win_wh()//检查窗口高度变化
{
  RECT rect;
  GetWindowRect(hWnd, &rect);


  if((X!=(rect.right - rect.left))||(Y!=(rect.bottom - rect.top)))
    {
      X = rect.right - rect.left;
      Y = rect.bottom - rect.top;
      return 1;
    }
  return 0;
}
class EasyInBox//输入框
{
  public:
    int left, top, right, bottom; // 控件的坐标
    string data="";//数据
    RECT r ;
    void Create(int x1, int y1, int x2, int y2,string in_data)
    {
      left = x1;
      top = y1;
      right = x2;
      bottom = y2;
      data=in_data;
      r= {left+4, top-1, right-1, bottom-1};
    }
    void draw()
    {
      int oldlinecolor = getlinecolor();
      setlinecolor(RGB(221,231,240));
      rectangle(left, top, right, bottom);
      // setlinecolor(RGB(243,102,56));
      rectangle(left+1, top+1, right-1, bottom-1);
      drawtext(_T(data.c_str()), &r,DT_LEFT|DT_WORDBREAK | DT_END_ELLIPSIS);
      setlinecolor(oldlinecolor);
    }
    string OnClick(int x, int y)
    {
      int oldlinecolor = getlinecolor();
      if(x >= left && x <= right && y >= top && y <= bottom)
        {
          setlinecolor(RGB(243,102,56));
          rectangle(left+1, top+1, right-1, bottom-1);
          FlushBatchDraw();
          drawtext(_T(data.c_str()), &r,DT_LEFT|DT_WORDBREAK | DT_END_ELLIPSIS);
          cout<<"\n输入框输入";
          int k;
          while (1)
            {
              if (kbhit())
                {
                  k=getch();
                  if (0==k || 0xE0==k) k=k<<8|getch();
                }
              else break;
            }
          Sleep(100);
          while(1)
            {
              char ch='?';
              if (_kbhit())
                {
                  ch=getch();
                }
              if (GetAsyncKeyState(VK_LBUTTON) && 0x8000) // 左键按下
                {
                  cout<<"\n输入框输入 退出";
                  Sleep(100);
                  break;
                }
              if(ch!='?')
                {

                  if(ch == '\b')
                    {
                      if(data.size()>0)
                        data.pop_back();
                      clearrectangle(left+1, top+1, right-1, bottom-1);
                      setlinecolor(RGB(243,102,56));
                      rectangle(left+1, top+1, right-1, bottom-1);
                      drawtext(_T(data.c_str()), &r,DT_LEFT|DT_WORDBREAK | DT_END_ELLIPSIS);
                      FlushBatchDraw();
                      continue;
                    }
                  string m;
                  m.assign(1,ch);
                  data+=m;
                  rectangle(left-1, top-1, right-1, bottom-1);
                  drawtext(_T(data.c_str()), &r,DT_LEFT|DT_WORDBREAK | DT_END_ELLIPSIS);
                }
              FlushBatchDraw();
            }
        }
      setlinecolor(oldlinecolor);
      return data;
    }
};
class EasyCheckBox//复选框
{
  public:
    int left, top, right, bottom; // 控件的坐标
    bool checked; // 是否选中
    bool checked_show=0;//显示状态
    EasyCheckBox()
    {
      checked = false;
    }
    void Create(int x1, int y1, int x2, int y2)
    {
      left = x1;
      top = y1;
      right = x2;
      bottom = y2;
    }
    void Draw()
    {
      // 绘制空心矩形
      int oldlinecolor = getlinecolor();
      int oldfillcolor = getfillcolor();
      setlinecolor(RGB(221,231,240));
      roundrect(left, top, right, bottom,10,10);
      // 根据选中状态绘制对钩或空心
      if(checked_show)
        {
          setfillcolor(RGB(243,102,56));
          solidroundrect(left+3,top+3,right-3,bottom-3,10,10);
        }
      if(checked)
        {
          setlinecolor(RGB(243,102,56));
          line(left, top, left + ((right-left)/2),  bottom);
          line(left + ((right-left)/2), bottom, right, top );
        }
      else
        {
          roundrect(left, top, right, bottom,10,10);
        }
      setlinecolor(oldlinecolor);
      setfillcolor(oldfillcolor);
    }
    bool HitTest(int x, int y)
    {
      return (x >= left && x <= right && y >= top && y <= bottom);
    }
    void OnClick(int x, int y)
    {
      if(HitTest(x, y))
        {
          checked = !checked;
          Draw();
          Sleep(100);
        }
    }
};


// 实现按钮控件
class EasyButton
{
  public:
    int _left = 0, _top = 0, _right = 0, _bottom = 0;	// 控件坐标,相对于窗口左下角 计算后的坐标
    int left = 0, top = 0, right = 0, bottom = 0;//控件坐标
    const TCHAR *text = NULL;							// 控件内容
    string name="?";//名字
    void (*userfunc)() = NULL;						// 控件消息


    void Create(int x1, int y1, int x2, int y2, const TCHAR* title,string Na, void (*func)())
    {
      //text = new wchar_t[wcslen(title) + 1];
      //wcscpy_s(text, wcslen(title) + 1, title);
      text = _tcsdup(title); // 复制一份标题
      name=Na;
      left = x1, top = y1, right = x2, bottom = y2;
      userfunc = func;
      RECT rect;
      GetWindowRect(hWnd, &rect);
      int x = rect.right - rect.left;
      int y = rect.bottom - rect.top;

      _left=x-left;
      _top=y-top;

      _right=x-right;
      _bottom=y-bottom;

      // 绘制用户界面
      Show();
    }

    void XY_()//重新计算XY坐标
    {
      _left=X-left;
      _top=Y-top;

      _right=X-right;
      _bottom=Y-bottom;
      //cout<<"win===:"<<x_<<" "<<y_<<endl;
      //cout<<"cl:"<<left<<" "<<top<<endl;
    }

    ~EasyButton()
    {
      //if (text != NULL)
      // free(text);
    }

    bool Check(int x, int y)
    {
      cout<<"win:"<<left<<" "<<top<<endl;
      cout<<"cl:"<<x<<" "<<y<<endl;
      return (_left >= x && x >= _right && _top >= y && y >= _bottom);
    }

    // 绘制界面
    void Show()
    {
      int oldlinecolor = getlinecolor();
      int oldbkcolor = getbkcolor();
      int oldfillcolor = getfillcolor();

      setlinecolor(RGB(221,231,240));			// 设置画线颜色
      settextcolor(RGB(195,204,212));//设置字体颜色
      setfillcolor(RGB(33, 43, 53));			// 设置填充颜色
      setbkcolor(RGB(33, 43, 53));// 设置背景颜色


      fillrectangle(_left, _top, _right, _bottom);

      // TCHAR* textTCHAR = new TCHAR[wcslen(text)+1];
      //wcstombs(textTCHAR, text, wcslen(text)+1);

      outtextxy(_left + (_right - _left - textwidth(text) + 1) / 2, _top + (_bottom - _top - textheight(text) + 1) / 2, text);

      setlinecolor(oldlinecolor);
      setbkcolor(oldbkcolor);
      setfillcolor(oldfillcolor);
    }

    void OnMessage()
    {
      int oldlinecolor = getlinecolor();
      int oldbkcolor = getbkcolor();
      int oldfillcolor = getfillcolor();
      setlinecolor(RGB(221,231,240));			// 设置画线颜色
      settextcolor(RGB(195,204,212));//设置字体颜色
      setfillcolor(RGB(33, 43, 53));			// 设置填充颜色
      setbkcolor(RGB(33, 43, 53));// 设置背景颜色
      fillrectangle(_left+1, _top+1, _right-1, _bottom-1);
      //TCHAR* textTCHAR = new TCHAR[wcslen(text)+1];
      //wcstombs(textTCHAR, text, wcslen(text)+1);
      outtextxy(_left + (_right - _left - textwidth(text) + 1) / 2, _top + (_bottom - _top - textheight(text) + 1) / 2, text);
      setlinecolor(oldlinecolor);
      setbkcolor(oldbkcolor);
      setfillcolor(oldfillcolor);
      FlushBatchDraw();
      Sleep(100);
      Show();
      FlushBatchDraw();
      if (userfunc != NULL)
        userfunc();
    }
};

class EasyButtonNO//不对齐左下角
{
  public:
    int left = 0, top = 0, right = 0, bottom = 0;	// 控件坐标
    const TCHAR *text = NULL;
    string name="?";//名字
    void (*userfunc)() = NULL;						// 控件消息
    float anim=1.01;//动画速度
    bool draw_anim=0;//动画是否开启
    float right_anim=0;//动画坐标

    void Create(int x1, int y1, int x2, int y2, const TCHAR* title,string Na, void (*func)())
    {
      text = _tcsdup(title); // 复制一份标题
      name=Na;
      left = x1, top = y1, right = x2, bottom = y2;
      userfunc = func;
      // 绘制用户界面
      Show();
    }

    ~EasyButtonNO()
    {
      // if (text != NULL)
      // free(text);
    }
    bool Check(int x, int y)
    {
      return (left <= x && x <= right && top <= y && y <= bottom);
    }
    // 绘制界面
    void Show()
    {
      int oldlinecolor = getlinecolor();
      int oldbkcolor = getbkcolor();
      int oldfillcolor = getfillcolor();
      setlinecolor(RGB(221,231,240));			// 设置画线颜色
      settextcolor(RGB(195,204,212));//设置字体颜色
      setfillcolor(RGB(33, 43, 53));			// 设置填充颜色
      setbkcolor(RGB(33, 43, 53));// 设置背景颜色
      fillroundrect(left, top, right, bottom,5,5);
      // TCHAR* textTCHAR = new TCHAR[wcslen(text)+1];
      //wcstombs(textTCHAR, text, wcslen(text)+1);
      if(draw_anim)//动画
        {
          setlinecolor(RGB(243,102,56));			// 设置画线颜色
          settextcolor(RGB(195,204,212));//设置字体颜色
          setfillcolor(RGB(243,102,56));			// 设置填充颜色
          setbkcolor(RGB(33, 43, 53));// 设置背景颜色
          fillroundrect(left, top,  (right-right_anim)+left, bottom,5,5);
          right_anim=right_anim*anim;
          if(right_anim> right)
            {
              cout<<"动画结束\n";
              draw_anim=0;
            }
        }
      outtextxy(left + (right - left - textwidth(text) + 1) / 2, top + (bottom - top - textheight(text) + 1) / 2, text);

      setlinecolor(oldlinecolor);
      setbkcolor(oldbkcolor);
      setfillcolor(oldfillcolor);
    }
    void OnMessage()
    {

      int oldlinecolor = getlinecolor();
      int oldbkcolor = getbkcolor();
      int oldfillcolor = getfillcolor();
      setlinecolor(RGB(243,102,56));			// 设置画线颜色
      settextcolor(RGB(195,204,212));//设置字体颜色
      setfillcolor(RGB(243,102,56));			// 设置填充颜色
      setbkcolor(RGB(33, 43, 53));// 设置背景颜色
      fillroundrect(left+1, top+1, right-1, bottom-1,5,5);
      outtextxy(left + (right - left - textwidth(text) + 1) / 2, top + (bottom - top - textheight(text) + 1) / 2, text);
      FlushBatchDraw();
      if (userfunc != NULL)
        userfunc();
      if(draw_anim==0)
        {
          draw_anim=1;
          right_anim=left+1;
          cout<<"动画启动\n";
        }

      //TCHAR* textTCHAR = new TCHAR[wcslen(text)+1];
      //wcstombs(textTCHAR, text, wcslen(text)+1);
      setlinecolor(oldlinecolor);
      setbkcolor(oldbkcolor);
      setfillcolor(oldfillcolor);
      FlushBatchDraw();
      Sleep(100);
      Show();
      FlushBatchDraw();
    }
};

class Text_box//文本框
{
  public:
    int h=0,w=0;

    void draw_Text_box()
    {
      int oldlinecolor = getlinecolor();
      int oldbkcolor = getbkcolor();
      int oldfillcolor = getfillcolor();
      setlinecolor(RGB(221,231,240));			// 设置画线颜色
      settextcolor(RGB(195,204,212));//设置字体颜色
      setfillcolor(RGB(33, 43, 53));			// 设置填充颜色
      setbkcolor(RGB(33, 43, 53));// 设置背景颜色
      fillrectangle(X-300,0,X, Y);
      setlinecolor(oldlinecolor);
      setbkcolor(oldbkcolor);
      setfillcolor(oldfillcolor);
    }
} BOX;


//-----------------------------------------------UI管理
vector<EasyButton> UI_EasyButton;
vector<EasyButtonNO> UIEasyButtonNO;

int NEW_Button(int x1, int y1, int x2, int y2, const TCHAR* title, string name,void (*func)())
{
  EasyButton *M = new EasyButton; // 在这里分配内存

  M->Create(x1,y1,x2,y2, title, name,func);

  UI_EasyButton.push_back(*M);

  return distance(UI_EasyButton.crbegin(), UI_EasyButton.crend()) - 1;//返回当前下表
}
int NEW_Button_no_dui(int x1, int y1, int x2, int y2, const TCHAR* title, string name,void (*func)())
{
  EasyButtonNO *M = new EasyButtonNO;

  M->Create(x1,y1,x2,y2, title, name,func);

  UIEasyButtonNO.push_back(*M);
  return distance(UIEasyButtonNO.crbegin(), UIEasyButtonNO.crend()) - 1;//返回当前下表
}

void DEL_Button_no_dui(string str)
{
  for(int i=0; i<UIEasyButtonNO.size(); i++)
    {
      if(UIEasyButtonNO[i].name==str)
        {
          UIEasyButtonNO.erase(UIEasyButtonNO.begin() + i);
        }
    }
}

bool FIND_Button_no_dui(string str)
{
  for(int i=0; i<UIEasyButtonNO.size(); i++)
    {
      if(UIEasyButtonNO[i].name==str)
        {
          return 1;
        }
    }
  return 0;
}

void draw_UI()
{
  for(int i=0; i<UI_EasyButton.size(); i++)
    {
      UI_EasyButton[i].XY_();
      UI_EasyButton[i].Show();
    }
  for(int i=0; i<UIEasyButtonNO.size(); i++)
    {
      UIEasyButtonNO[i].Show();
    }
}


bool IsMouseClickedInWindow(HWND hwnd)
{
  // 检查鼠标左键是否被按下
  if (GetAsyncKeyState(1) & 0x8000)
    {
      // 获取鼠标当前位置
      POINT pt;
      GetCursorPos(&pt);

      // 检查鼠标是否在给定窗口的客户区内
      if (ScreenToClient(hwnd, &pt))  // 将屏幕坐标转换为窗口客户区坐标
        {
          RECT rc;
          GetClientRect(hwnd, &rc);  // 获取窗口客户区的矩形区域

          // 检查鼠标位置是否在客户区内
          if (PtInRect(&rc, pt))
            {
              // 检查给定窗口是否是当前活动窗口
              if (GetForegroundWindow() == hwnd)
                {
                  while(!(GetAsyncKeyState(1) & 0x8000))
                    {
                    }
                  return true;
                }

            }
        }
    }
  return false;
}


void UI_DUI(ExMessage &msg)//UI自动对齐 包括点击检测
{
  if(Li_win_wh())
    {
      //clearcliprgn();
      putimage(0, 0,X,Y, &img2, 0, 0);
      for(int i=0; i<UI_EasyButton.size(); i++)
        {
          UI_EasyButton[i].XY_();
          UI_EasyButton[i].Show();
        }
      for(int i=0; i<UIEasyButtonNO.size(); i++)
        {
          UIEasyButtonNO[i].Show();
        }
      FlushBatchDraw();
    }

  if (IsMouseClickedInWindow(hWnd))
    {
      // 判断控件
      for(int i=0; i<UI_EasyButton.size(); i++)
        {
          if (UI_EasyButton[i].Check(msg.x, msg.y))		UI_EasyButton[i].OnMessage();
        }
      for(int i=0; i<UIEasyButtonNO.size(); i++)
        {
          if (UIEasyButtonNO[i].Check(msg.x, msg.y))		UIEasyButtonNO[i].OnMessage();
        }
      //if (btnOK.Check(msg.x, msg.y))		btnOK.OnMessage();
      //	if (btnOK2.Check(msg.x, msg.y))		btnOK2.OnMessage();
      //flushmessage();
    }
}

std::vector<int> parseString(const std::string& s)//string拆分出数字
{
  std::vector<int> result;
  std::stringstream ss(s);
  int i;

  while (ss >> i)
    {
      result.push_back(i);
    }

  return result;
}

string joinIntsToString(const vector<int>& diffs)//vector拆分出数字
{

  if (diffs.empty())
    {
      return "";
    }

  stringstream ss;

  ss << diffs[0];

  for (int i = 1; i < diffs.size(); ++i)
    {
      ss << " " << diffs[i];
    }

  return ss.str();
}

/*void CenterImage(IMAGE &img, int x, int y)遗弃
{

  // 计算图像中心坐标
  int imgX = img.getwidth() / 2;
  int imgY = img.getheight() / 2;

  // 计算偏移量
  int offsetX = x - imgX;
  int offsetY = y - imgY;

  // 调整图像到指定中心坐标
  IMAGE newimg(img.getwidth() + abs(offsetX) * 2, img.getheight() + abs(offsetY) * 2);
  for(int i = 0; i < img.getwidth(); i++)
    {
      for(int j = 0; j < img.getheight(); j++)
        {
          setPixel(i + offsetX, j + offsetY, img.getPixel(i, j));
        }
    }

  // 保存中心坐标
  imgX += offsetX;
  imgY += offsetY;

  // 替换原图像
  img.Release();
  img = newimg;
}*/

bool isJava8OrHigherInstalled()
{
  system("java -version > temp.txt 2>&1");

  std::ifstream file("temp.txt");
  if (!file.is_open())
    {
      std::cerr << "Failed to open temp.txt" << std::endl;
      return false;
    }

  std::string line;
  while (std::getline(file, line))
    {
      // 检查输出中是否包含 'java version "1.8' 或更高的版本
      if (line.find("java version \"1.") != std::string::npos)
        {
          int version = std::stoi(line.substr(line.find("java version \"1.") + 16, 1));
          if (version >= 8)
            {
              file.close();
              system("del temp.txt"); // 删除临时文件
              return true;
            }
        }
    }

  file.close();
  system("del temp.txt"); // 删除临时文件
  return false;
}

// 将窗口置顶并取消边框
void SetWindowTopMostNoBorder(HWND hwnd)
{
  // 将窗口置顶
  SetWindowPos(hwnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);

  // 取消边框
  LONG style = GetWindowLong(hwnd, GWL_STYLE);
  style &= ~(WS_BORDER | WS_THICKFRAME);
  SetWindowLong(hwnd, GWL_STYLE, style);
}
void SetWindowNormalWithBorder(HWND hwnd)
{
  // 添加边框
  LONG style = GetWindowLong(hwnd, GWL_STYLE);
  style |= (WS_BORDER | WS_THICKFRAME);
  SetWindowLong(hwnd, GWL_STYLE, style);

  // 取消置顶
  SetWindowPos(hwnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}
void MaximizeWindowWithTaskbar(HWND hwnd)//最大化，不覆盖
{

  // 获取屏幕工作区域大小(排除任务栏)
  RECT workArea;
  SystemParametersInfo(SPI_GETWORKAREA, 0, &workArea, 0);

  // 将窗口最大化至工作区域
  SetWindowPos(hwnd, NULL,
               workArea.left, workArea.top,
               workArea.right - workArea.left,
               workArea.bottom - workArea.top,
               SWP_NOZORDER | SWP_FRAMECHANGED);

  // 禁用最大化按钮
  DWORD style = GetWindowLong(hwnd, GWL_STYLE);
  style &= ~WS_MAXIMIZEBOX;
  SetWindowLong(hwnd, GWL_STYLE, style);
}

void ScrollText(const std::string& text, int area_x, int area_y, int area_width, int area_height, int font_size, COLORREF font_color, int speed)
{
  // 保存设置
  LOGFONT original_font;
  gettextstyle(&original_font);

  // 设置字体
  LOGFONT font;
  gettextstyle(&font);
  font.lfHeight = font_size;
  _tcscpy_s(font.lfFaceName, _T("Segoe UI"));
  font.lfQuality = ANTIALIASED_QUALITY;
  settextstyle(&font);
  setbkmode(OPAQUE); // 关闭透明混合
  // 分割字符串
  vector<string> lines;
  stringstream ss(text);
  string line;
  while (getline(ss, line, '|'))
    {
      lines.push_back(line);
    }

  // 逐行显示
  int y = area_y;
  for (string line : lines)
    {
      for (int i = 0; i <= line.length(); i++)
        {
          //clearrectangle(area_x, area_y, area_x + area_width, area_y + area_height);
          outtextxy(area_x, y, line.substr(0, i).c_str());
          Sleep(speed);
          rectangle(area_x,area_y,area_x + area_width,area_y + area_height);
          FlushBatchDraw();
          if (_kbhit())
            {
              break;
            }
        }
      y += font_size;
    }

  // 恢复设置
  setbkmode(TRANSPARENT);
  settextstyle(&original_font);
}


void THANK()//捐赠者感谢
{
  int left = (X - 700) / 2;
  int top = (Y - 400) / 2;
  top=top-50;
  int oldlinecolor = getlinecolor();
  int oldbkcolor = getbkcolor();
  int oldfillcolor = getfillcolor();
  setlinecolor(RGB(221,231,240));			// 设置画线颜色
  settextcolor(RGB(195,204,212));//设置字体颜色
  setfillcolor(RGB(33, 43, 53));			// 设置填充颜色
  setbkcolor(RGB(33, 43, 53));// 设置背景颜色
  settextstyle(32, 0, _T("Segoe UI"));//字体
  fillrectangle(left, top, left+700, top+400);
  RECT r = {left,0,X,40};
  string _o="ERface 感谢你们的捐赠与支持";
  outtextxy(left+5,top,_T(_o.c_str()));
  setlinecolor(RGB(243,102,56));
  ScrollText("感谢所有在ERface开发过程中给予帮助的朋友们!|这段时间,在各位的支持和协助下,ERface得以顺利推进,并最终完成了0.6.9版本的开发工作。|作为一个独立开发者,在这个项目中我收获了很多宝贵的经验和启发。|尤其要感谢那些在代码、界面、测试等方面给出建议的朋友们,你们的反馈对完善ERface起到了至关重要的作用。|也要感谢那些在我困难时给予鼓励的朋友们,你们给了我继续前进的动力。|ERface当前支持小米手环8 pro的表盘开发中,这是一个让我由衷自豪的成果。|这得益于每一位在这个项目中付出时间和精力的朋友。|再次感谢你们,ERface会不断改进ヾ(≧▽≦*)o|任意键继续",left, top+50, 700, 400,18,GREEN,30);
  getch();
  clearrectangle(left, top+50, left+700, top+400);
  ScrollText("致谢名单(不分先后):|@卡卡|@我吃柚子|@GiveMeFive|@Rain.Cl|@XYZ|@(~QwQ~)|@Zhou|@Panda|@与众不童|@huangtao1189|@蒙迪欧|任意键继续",left, top+50, 700, 400,25,GREEN,100);
  FlushBatchDraw();
  getch();
}

void NO_USE_UI(HWND hwnd, ExMessage &Msg,HDC &mainDC,HDC &imgDC)//焦点是否在ERface
{
  if (!(GetForegroundWindow() == hwnd))
    {
      cout<<"焦点不在\n";
      imgDC = GetImageHDC(&STOP_M);//获取DC
      for(float i=1; i<Y; i=i*1.05)
        {
          //putimage(0, ((int)i-Y),X,Y, &STOP_M,X,Y 0, 0);//渲染
          StretchBlt( mainDC,0,((int)i-Y),X,Y,imgDC,0, 0,1920,1080,SRCCOPY);// 目标 DC // 目标位置的坐标 // 目标位置的长宽 // 源 DC // 源位置的坐标 // 源位置的长宽 // 贴图的光栅模式
          FlushBatchDraw();
          if ((GetForegroundWindow() == hwnd))
            {
              Msg.x=400;
              Msg.y=400;
              Msg.message=0;
              flushmessage(-1);
              Sleep(100);
              return;
            }
        }
      int JI=0;//计数
      while(!(GetForegroundWindow() == hwnd))
        {
          StretchBlt( mainDC,0,0,X,Y,imgDC,0, 0,1920,1080,SRCCOPY);// 目标 DC // 目标位置的坐标 // 目标位置的长宽 // 源 DC // 源位置的坐标 // 源位置的长宽 // 贴图的光栅模式
          FlushBatchDraw();
          JI++;
        }
      if(JI>100)
        {
          cout<<"JI:"<<JI<<endl;
          Msg.x=400;
          Msg.y=400;
          Msg.message=0;
          flushmessage(-1);
          for(float j=1; j<Y; j=j*1.3)
            {
              // putimage(0, ,X,Y, &STOP_M, 0, 0);//渲染
              StretchBlt( mainDC,0,(0-(int)j),X,Y,imgDC,0, 0,1920,1080,SRCCOPY);// 目标 DC // 目标位置的坐标 // 目标位置的长宽 // 源 DC // 源位置的坐标 // 源位置的长宽 // 贴图的光栅模式
              FlushBatchDraw();
            }
        }
      Msg.x=400;
      Msg.y=400;
      Msg.message=0;
      flushmessage(-1);
      Sleep(100);
      cout<<"焦点在\n";
    }
}
//--------------------------------------------------------------输入框
std::string ShowInputBox()
{
  const char *className = "input";
  const char *windowTitle = "请输入表盘名字(回车确定)";

  // Register window class
  WNDCLASSEX wc = {};
  wc.cbSize = sizeof(WNDCLASSEX);
  wc.lpfnWndProc = DefWindowProc;
  wc.hInstance = GetModuleHandle(NULL);
  wc.lpszClassName = className;
  RegisterClassEx(&wc);

  // Create window
  HWND hwnd = CreateWindowEx(
                0, className, windowTitle,
                WS_OVERLAPPEDWINDOW,
                CW_USEDEFAULT, CW_USEDEFAULT, 410, 100,
                NULL, NULL, GetModuleHandle(NULL), NULL);

  // Create input box
  HWND hwndInput = CreateWindowEx(
                     0, "EDIT", "",
                     WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,
                     10, 10, 200, 25,
                     hwnd, (HMENU)1, GetModuleHandle(NULL), NULL);


  // Show window
  ShowWindow(hwnd, SW_SHOW);

  // Message loop
  std::string result;
  MSG msg;
  while (GetMessage(&msg, NULL, 0, 0))
    {
      TranslateMessage(&msg);
      DispatchMessage(&msg);
      if (GetAsyncKeyState(VK_RETURN) & 0x8000)
        {
          // Button clicked, get input box text
          int length = GetWindowTextLength(hwndInput);
          char buffer[256];
          GetWindowText(hwndInput, buffer, length + 1);
          result = buffer;
          break;
        }
    }

  // Clean up
  DestroyWindow(hwnd);
  UnregisterClass(className, GetModuleHandle(NULL));

  return result;
}

double getScale(int newX, int newY, int oldX, int oldY)
{
  double xScale = (double)newX / oldX;
  double yScale = (double)newY / oldY;

  // 返回缩放比例,这里取 x、y 轴缩放比例的最小值
  return std::min(xScale, yScale);
}

void hideCursorIfFocused(HWND windowHandle)
{
  HWND foregroundWindow = GetForegroundWindow(); // 获取当前焦点窗口的句柄

  if (foregroundWindow == NULL)
    {
      // 获取窗口句柄失败，可以在这里处理错误
      return;
    }

  if (foregroundWindow == windowHandle)
    {
      // 如果输入的窗口句柄就是焦点窗口，则隐藏光标
      ShowCursor(FALSE);
    }
}


#ifndef ZOOM_H
#define ZOOM_H

void ZoomImage(IMAGE* P,IMAGE* Q,double ZoomRate,bool HighQuality=false,double ZoomRate2=0)
{
  //����д�ڶ����Ų�����Ĭ�Ϻ͵�һ���
  
  if(ZoomRate<0||ZoomRate2<0)
  return ;
  
  if(ZoomRate2==0)
    ZoomRate2=ZoomRate;

  //�������ű����趨Ŀ��ͼ���С
  P->Resize((int)(Q->getwidth()*ZoomRate),(int)(Q->getheight()*ZoomRate2));

  //�ֱ��ԭͼ���Ŀ��ͼ���ȡָ��
  DWORD* M=GetImageBuffer(P);
  DWORD* N=GetImageBuffer(Q);

  //ѡ���������ʹ��˫���Բ�ֵ�㷨
  if(HighQuality)
    {
      for(int i=0; i<P->getheight(); i++)
        {
          for(int j=0; j<P->getwidth(); j++)
            {
              //���Ŀ��ͼ���Ӧ���ص���ԭͼ�ĸ������겢ȡ��
              int X_=(int)((j+0.5)/ZoomRate-0.5);
              int Y_=(int)((i+0.5)/ZoomRate2-0.5);

              //����ȡ��������A1(X,Y), A2(X+1,Y), A3(X,Y+1), A4(X+1,Y+1)�����������ٽ�4�������ɫƽ��ֵ��
              M[j+i*P->getwidth()]=RGB((GetRValue(N[X_+Y_*Q->getwidth()])+GetRValue(N[(X_+1)+Y_*Q->getwidth()])+GetRValue(N[X_+(Y_+1)*Q->getwidth()])+GetRValue(N[(X_+1)+(Y_+1)*Q->getwidth()]))/4,
                                       (GetGValue(N[X_+Y_*Q->getwidth()])+GetGValue(N[(X_+1)+Y_*Q->getwidth()])+GetGValue(N[X_+(Y_+1)*Q->getwidth()])+GetGValue(N[(X_+1)+(Y_+1)*Q->getwidth()]))/4,
                                       (GetBValue(N[X_+Y_*Q->getwidth()])+GetBValue(N[(X_+1)+Y_*Q->getwidth()])+GetBValue(N[X_+(Y_+1)*Q->getwidth()])+GetBValue(N[(X_+1)+(Y_+1)*Q->getwidth()]))/4);
            }
        }
    }
  else
    //ѡ��������򰴳��淽������
    {
      for(int i=0; i<P->getheight(); i++)
        for(int j=0; j<P->getwidth(); j++)
          //����Ŀ��ͼ�����ص�λ��������ԭͼ�����ص㸳ֵ
          M[j+i*P->getwidth()]=N[(int)(j/ZoomRate)+(int)(i/ZoomRate2)*Q->getwidth()];
    }
}
int SAVET=0;//当前自动保存计时
#endif

// 计算渲染帧数的函数
void CalculateFrameRate(ExMessage msg)
{

  // 使用静态变量保存上一帧时间和帧数
  static int frameCount = 0;
  static double lastTime = 0;

  // 获取当前时间
  double currentTime = GetTickCount() / 1000.0;

  // 如果是第一帧,记录起始时间
  if (lastTime == 0)
    {
      lastTime = currentTime;
    }

  // 增加帧数
  frameCount++;

  // 如果时间间隔大于1秒
  if (currentTime - lastTime >= 1.0)
    {
      SAVET++;//自动保存时间s++
      // 计算帧率
      double frameRate = frameCount / (currentTime - lastTime);
      // 显示帧率
      FPS=(int)frameRate;
      // 重置帧数和时间
      frameCount = 0;
      lastTime = currentTime;
      cout<<msg.x<<" "<<msg.y<<endl;
    }

}
void changeImageColor(IMAGE* img, int x, int y)//渲染贴图PRO
{
  int width = img->getwidth();
  int height = img->getheight();

  // 获取图像的像素缓冲区指针
  DWORD* imgPixels = GetImageBuffer(img);

  // 获取绘图窗口的缓冲区指针
  DWORD* screenPixels = GetImageBuffer(NULL);

  // 遍历图像的每个像素
  for (int i = 0; i < width * height; i++)
    {
      int imgX = i % width;  // 图像中当前像素的X坐标
      int imgY = i / width;  // 图像中当前像素的Y坐标

      int screenX = x + imgX;  // 在绘图窗口中的X坐标
      int screenY = y + imgY;  // 在绘图窗口中的Y坐标
      DWORD color = imgPixels[i];

      /*    // 判断像素是否在绘图窗口范围内
          if (screenX < 0 || screenX >= getwidth() || screenY < 0 || screenY >= getheight())
            {
              // 跳过不在绘图窗口范围内的像素
              continue;
            }

          // 获取当前像素的颜色值


          // 判断是否为黑色
          if (GetRValue(color) == 0 && GetGValue(color) == 0 && GetBValue(color) == 0)
            {
              // 跳过黑色像素
              continue;
            }
      */
      // 进行颜色转换
      DWORD newColor = (color);

      // 计算在绘图窗口缓冲区中的索引
      int screenIndex = screenY * getwidth() + screenX;

      // 更新绘图窗口缓冲区中的像素颜色
      screenPixels[screenIndex] = newColor;
    }
}
void GetMousePosInClient(HWND hWnd,ExMessage &msg)//指针相对于这个窗口的工作区的坐标
{
  POINT pt;
  GetCursorPos(&pt);

  RECT rect;
  GetClientRect(hWnd, &rect);

  ClientToScreen(hWnd, (LPPOINT)&rect);

  pt.x -= rect.left;
  pt.y -= rect.top;

  msg.x=pt.x;
  msg.y=pt.y;
}


