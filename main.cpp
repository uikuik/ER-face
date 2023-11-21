#include <graphics.h>
#include <conio.h>
#include <windows.h>
#include "start.h"
#include "UI.h"
#include <tchar.h>
#include <iostream>
#include "face.h"
//#include "info.h"
using namespace std;

vector<face_data> fd;//显示

vector<face_data> fd_;//正常表盘      | 保存
vector<face_data> fd_AOD;//熄屏幕显示 |


vector<face_data> fd_cz;//临时保存
int time_save=0;//保存计时
string Bei_img2="data//PNG//背景.png";//背景地址
int Bei_n=0;//背景下标
bool save_png_draw=0;//保存预览图标志
int fd_N=0;//要高亮的控件
int C_N=0;//组件管理器的页面 1:快速选择跳转
bool welcome_UI_END=0;//欢迎界面结束

bool cmd_show=0;//是否显示控制台


float big_draw=1.0;

void AOD_do()
{
  try
    {
      if(fd_ch==0)
        {
          fd_N=0;
          fd_.clear();
          for(int i=0; i<fd.size(); i++)
            fd_.push_back(fd.at(i));
          fd.clear();
          for(int i=0; i<fd_AOD.size(); i++)
            fd.push_back(fd_AOD.at(i));
          fd_N=0;
        }

      if(fd_ch==1)
        {
          fd_N=0;
          fd_AOD.clear();
          for(int i=0; i<fd.size(); i++)
            fd_AOD.push_back(fd[i]);
          fd.clear();
          for(int i=0; i<fd_.size(); i++)
            fd.push_back(fd_[i]);
          fd_N=0;
        }
      fd_ch=!fd_ch;
      reloadImageData(fd, face_result);
    }
  catch (const std::exception& e)
    {
      // 打印错误信息
      std::cerr << e.what() << '\n';
    }
  catch (...)
    {
      // 打印未知错误
      std::cerr << "Unknown exception!\n";
    }

}
void make_band_face()//打包表盘
{
  int E=0,E2;
  E=examine_face(fd);
  AOD_do();
  E2=examine_face(fd);
  AOD_do();
  if(E==1)
    return;
  if(E==0&&E2==0)
    {
      cout<<"\n>===无任何错误===<\n";
      for(int i=0; i<fd.size(); i++)
        fd[i].error_warn=0;
      AOD_do();
      for(int i=0; i<fd.size(); i++)
        fd[i].error_warn=0;
      AOD_do();
    }
  string s2="wfDef.json";
  string re=face_result+"WatchfacePackToolMi8Pro64.exe";
  while (re.find(s2) < re.length())
    {
      re.erase(re.find(s2), s2.size());
    }
  cout<<"打包工具路径:"<<re<<endl;
  string I="start ";
  I+=re;
  int _o=system(I.c_str());
  cout<<"cmd返回"<<_o<<endl;
  if(_o!=0)
    {
      cout<<"打包失败 ";
      MessageBox(NULL,"打包失败 请重试","确定",MB_OK|MB_ICONEXCLAMATION | MB_SETFOREGROUND);
      I="explorer ";
      I+=face_result;
      while (I.find(s2) < I.length())
        {
          I.erase(I.find(s2), s2.size());
        }
      system(I.c_str());
    }
  else
    {
      cout<<"打包成功";
      I="explorer ";
      I+=face_result;
      while (I.find(s2) < I.length())
        {
          I.erase(I.find(s2), s2.size());
        }
      system(I.c_str());
    }
}

void On_btnOk_Click()   //打开
{
  welcome_UI_END=1;
  if(biger_windows==1&&biger_ALL==0)
    {
      ShowWindow(hWnd, SW_RESTORE);
      SetWindowNormalWithBorder(hWnd);
    }
  fd_N=0;
  fd.clear();
  fd_.clear();
  fd_AOD.clear();
  fd_cz.clear();
  auxiliary_n.clear();
  face_result=chooseFiles();

  string s2="|";
  while (face_result.find(s2) < face_result.length())
    {
      face_result.erase(face_result.find(s2), s2.size());
    }

  parseFile(face_result,fd_);
  image_wh(face_result,fd_,0);

  parseFile_AOD(face_result,fd_AOD);
  image_wh(face_result,fd_AOD,1);

  fd=fd_;

  cout<<"========================\n";

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
  time_save=0;
  reloadImageData(fd, face_result);
  fd_cz=fd;
  if(fd.size()>0)
    {
      fd_N=1;
    }
}

void On_NEW_face()   //新建文件
{
  welcome_UI_END=1;
  ShowWindow(hWnd,SW_SHOWMINIMIZED);
  if(biger_windows==1&&biger_ALL==0)
    {
      ShowWindow(hWnd, SW_RESTORE);
      SetWindowNormalWithBorder(hWnd);
    }
  face_result=SelectFolder();
  string s2="|";
  while (face_result.find(s2) < face_result.length())
    {
      face_result.erase(face_result.find(s2), s2.size());
    }
  face_name=ShowInputBox();
  cout<<"face_name:"<<face_name<<endl;
  face_result+="\\";
  face_result+=face_name;
  string cmd = "md ";
  cmd+=face_result;
  system(cmd.c_str());
  cout<<"新建表盘路径:"<<face_result<<endl;
  CopyFile("data//空白底包//新建表盘工程//face_data",face_result);
  cmd = "md ";
  cmd+=face_result;
  cmd+="\\images";
  cout<<"新建路径:"<<cmd;
  system(cmd.c_str());
  cmd="md ";
  cmd+=face_result;
  cmd+="\\images_aod";
  cout<<"新建路径:"<<cmd;
  system(cmd.c_str());
  CopyFile("data//空白底包//新建表盘工程//face_data//images//preview.png",face_result+"\\images");
  //On_btnOk_Click();   //打开
  face_result+="\\wfDef.json";
  welcome_UI_END=1;
  if(biger_windows==1&&biger_ALL==0)
    {
      ShowWindow(hWnd, SW_RESTORE);
      SetWindowNormalWithBorder(hWnd);
    }
  fd_N=0;
  fd.clear();
  fd_.clear();
  fd_AOD.clear();
  fd_cz.clear();
  parseFile(face_result,fd_);
  image_wh(face_result,fd_,0);
  parseFile_AOD(face_result,fd_AOD);
  image_wh(face_result,fd_AOD,1);
  fd=fd_;
  cout<<"========================\n";
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
  time_save=0;
  reloadImageData(fd, face_result);
  fd_cz=fd;
  if(fd.size()>0)
    {
      fd_N=1;
    }
  ShowWindow(hWnd,SW_SHOWNORMAL|SW_NORMAL);
}

void On_save()   //保存
{
  bool ch=0;
  if(fd_ch==1)
    {
      AOD_do();
      cout<<"切换 正常表盘\n";
      ch=1;
    }
  //fd_=fd;
  serializeToJson(fd,fd_AOD,face_result);
  MessageBox(NULL,"已保存表盘    但请注意注意！使用第三方表盘有一定风险！","保存",MB_OK|MB_ICONINFORMATION);
  if(ch)
    AOD_do(),cout<<"切换 AOD表盘\n";
}
void On_save_time()   //自动保存
{
  if(fd_ch==1)
    {
      AOD_do();
      cout<<"切换 正常表盘\n";
    }
  //fd_=fd;
  string result=face_result;
  string s2="wfDef.json";
  while (result.find(s2) < result.length())
    {
      result.erase(result.find(s2), s2.size());
    }
  result+="wfDef-自动保存.json";
  cout<<"路径:"<<result;
  serializeToJson(fd,fd_AOD,result);
  cout<<"--已自动保存表盘 "<<endl;
}
void On_biger_windows()   //全屏
{
  if(biger_windows==0)
    {
      if(biger_ALL==0)
        {
          ShowWindow(hWnd,SW_MAXIMIZE);//最大化 hwnd 所代表的窗口
          SetWindowTopMostNoBorder(hWnd);
        }
      else
        {
          //SetWindowTopMostNoBorder(hWnd);
          MaximizeWindowWithTaskbar(hWnd);//最大化，不覆盖
        }
    }
  else
    {
      ShowWindow(hWnd, SW_RESTORE);
      SetWindowNormalWithBorder(hWnd);
    }

  biger_windows=!biger_windows;
}
void Save_png_draw()
{
  save_png_draw=1;
}

int FW,FH;//表盘全局大小
int fw,fh;//表盘渲染时的大小 w=x h=y
int fx,fy;//表盘渲染时的位置
HDC mainDC;//窗口DC
HDC imgDC;//渲染DC
int mx=0,my=0;//光标相对于可编辑区的位置
short wheel=0;//鼠标滚动值


void draw_fd_N(ExMessage msg,IMAGE *face_image,int i)   //高亮渲染
{
  if(save_png_draw)
    {
      cout<<"保存预览图\n";
      save_png_draw=0;
      string s2="wfDef.json";
      string re=face_result+"preview.png";
      while (re.find(s2) < re.length())
        {
          re.erase(re.find(s2), s2.size());
        }
      cout<<"预览图路径:"<<re<<endl;
      for(int i=0; i<fd.size(); i++)
        {
          if(dataSRC_out("天气-温度","scr")==fd[i].dataSrc||dataSRC_out("当日最高气温","scr")==fd[i].dataSrc)
            cout<<"天气-温度|当日最低气温>"<<i<<" "<<endl,fd[i].draw_data="25";
          if(dataSRC_out("当日最低气温","scr")==fd[i].dataSrc)
            cout<<"当日最低气温>"<<i<<" "<<endl,fd[i].draw_data="20";
          if(dataSRC_out("睡眠","scr")==fd[i].dataSrc)
            cout<<"睡眠>"<<i<<" "<<endl,fd[i].draw_data="7|2";
          if(dataSRC_out("压力","scr")==fd[i].dataSrc)
            cout<<"压力>"<<i<<" "<<endl,fd[i].draw_data="60";
          if(dataSRC_out("电量","scr")==fd[i].dataSrc)
            cout<<"电量>"<<i<<" "<<endl,fd[i].draw_data="80";
          if(dataSRC_out("卡路里","scr")==fd[i].dataSrc)
            cout<<"卡路里>"<<i<<" "<<endl,fd[i].draw_data="162";
          if(dataSRC_out("日期-年","scr")==fd[i].dataSrc)
            cout<<"日期-年>"<<i<<" "<<endl,fd[i].draw_data="2022";
          if(dataSRC_out("日期-月","scr")==fd[i].dataSrc)
            cout<<"日期-月>"<<i<<" "<<endl,fd[i].draw_data="8";
          if(dataSRC_out("日期-日","scr")==fd[i].dataSrc)
            cout<<"日期-日>"<<i<<" "<<endl,fd[i].draw_data="16";
          if(dataSRC_out("心率","scr")==fd[i].dataSrc)
            cout<<"心率>"<<i<<" "<<endl,fd[i].draw_data="78";
          if(dataSRC_out("星期","scr")==fd[i].dataSrc)
            cout<<"星期>"<<i<<" "<<endl,fd[i].draw_data="3";
          //
        }
      IMAGE img=draw_face(face_result,fd);
      saveimage(_T(re.c_str()),&img);
      for(int i=0; i<fd.size(); i++)
        {
          fd[i].draw_data="";
        }
    }
  int oldlinecolor_ = getlinecolor();
  setlinecolor(RGB(243,102,56));
  line(0,26,X,26);
  setlinecolor(oldlinecolor_);
  if(i==0)
    {

      return ;
    }
  i=i-1;
  // cout<<"控件坐标:"<<"X"<<fd[i].x<<"Y"<<fd[i].y<<endl;
  SetWorkingImage(face_image);//渲染光标
  setlinecolor(RGB(243,102,56));

  if(fd[i].align==1)//左对齐
    rectangle(fd[i].x,fd[i].y,(fd[i].w+fd[i].x)-1,(fd[i].h+fd[i].y)-1);

  if(fd[i].align==0)//右对齐
    rectangle(fd[i].x-fd[i].w,fd[i].y,(fd[i].x)-1,(fd[i].h+fd[i].y)-1);

  if(fd[i].align==2)//中对齐
    rectangle(fd[i].x-(fd[i].w/2),fd[i].y,(fd[i].x+(fd[i].w/2))-1,(fd[i].h+fd[i].y)-1);

  setlinecolor(WHITE);
  SetWorkingImage(NULL);
  if ((GetAsyncKeyState(164) && 0x8000)&&(GetAsyncKeyState( 46) && 0x8000))   // 删除所有辅助显示
    {
      auxiliary_data.clear();
      return ;
    }
  if ((GetAsyncKeyState(VK_CONTROL) && 0x8000)&&(GetAsyncKeyState(90) && 0x8000))   // 撤回
    {
      cout<<"撤回!\n";
      if(fd_ch==0)
        {
          fd=fd_cz;
          reloadImageData(fd, face_result);
          return ;
        }
      if(fd_ch==1)
        {
          fd=fd_AOD;
          reloadImageData(fd, face_result);
          return ;
        }
    }
  if ((GetAsyncKeyState(VK_CONTROL) && 0x8000)&&(GetAsyncKeyState(67) && 0x8000))   // 复制
    {
      cout<<"复制!\n";
      fd.push_back(fd[i]);
      reloadImageData(fd, face_result);
      Sleep(100);
    }
  if ((GetAsyncKeyState(112) && 0x8000)||reload)   // 重新加载
    {
      reload=0;
      cout<<"重新加载!\n";
      reloadImageData(fd, face_result);
      Sleep(100);
    }
  if ((GetAsyncKeyState(VK_CONTROL) && 0x8000)&&fd[i].type=="element")   // 快速选择跳转
    {
      C_N=1;
    }
  if (GetAsyncKeyState(VK_DELETE) && 0x8000&&fd[i].lock==0)   // 删除该组件
    {
      if(fd.size()>=1)
        {
          fd.erase(fd.begin() + i);
          fd_N=0;
          Sleep(100);
        }
    }
  if (GetAsyncKeyState(113) )   // 显示控制台
    {
      if(cmd_show==0)
        ShowWindow(cmd_hwnd,1);
      if(cmd_show==1)
        ShowWindow(cmd_hwnd,0);
      cmd_show=!cmd_show;
      Sleep(500);
    }
  if (GetAsyncKeyState(VK_PRIOR) && 0x8000&&fd[i].dataSrc_choose==0)   // 上图层
    {
      if(moveElement(fd,i,-1)==0)
        {
          fd_N=fd_N+1;
          Sleep(100);
          reloadImageData(fd, face_result);
        }
    }
  if (GetAsyncKeyState(VK_NEXT) && 0x8000&&fd[i].dataSrc_choose==0)   // 下图层
    {
      if(moveElement(fd,i,1)==0)
        {
          fd_N=fd_N-1;
          Sleep(100);
          reloadImageData(fd, face_result);
        }
    }
  if (GetAsyncKeyState(VK_NEXT) && 0x8000&&fd[i].dataSrc_choose==1)   // 上数据源
    {
      string m=dataSRC_out_2(fd[i].dataSrc,1);
      fd[i].dataSrc=m;
      Sleep(100);
    }
  if (GetAsyncKeyState(VK_PRIOR) && 0x8000&&fd[i].dataSrc_choose==1)   // 下数据源
    {
      string m=dataSRC_out_2(fd[i].dataSrc,-1);
      fd[i].dataSrc=m;
      Sleep(100);
    }
  if ((GetAsyncKeyState(1) && 0x8000)&&(GetAsyncKeyState(164) && 0x8000))   // 创建辅助显示
    {
      Sleep(100);
      IMAGE face_image_old=*face_image;
      IMAGE face_image_draw=*face_image;
      SetWorkingImage(&face_image_draw);
      int dx = msg.x - fx;
      int dy = msg.y - fy;
      // 将偏移量映射到原图片大小
      float _sx = (float)dx / fw * 336;
      float _sy = (float)dy / fh * 480;
      msg.x = (int)_sx;
      msg.y = (int)_sy;
      int sx=msg.x;
      int sy=msg.y;
      while(1)
        {
          setlinecolor(WHITE);
          face_image_draw=face_image_old;
          SetWorkingImage(&face_image_draw);
          GetMousePosInClient(hWnd,msg);
          dx = msg.x - fx;
          dy = msg.y - fy;
          // 将偏移量映射到原图片大小
          _sx = (float)dx / fw * 336;
          _sy = (float)dy / fh * 480;
          msg.x = (int)_sx;
          msg.y = (int)_sy;
          cout<<msg.x<<"|"<<msg.y<<" "<<sx<<"|"<<sy<<endl;
          if(abs(sx-msg.x)>abs(sy-msg.y))
            {
              line(sx,sy,msg.x,sy);
              line(sx,sy-5,sx,sy+5);
              line(msg.x,sy-5,msg.x,sy+5);
              string _o=to_string(abs(sx-msg.x));
              if((msg.x-sx)>0)
                outtextxy(sx+(msg.x-sx)/2,sy-15,_T(_o.c_str()));
              if((sx-msg.x)>0)
                outtextxy(sx-(sx-msg.x)/2,sy-15,_T(_o.c_str()));
            }
          if(abs(sx-msg.x)<abs(sy-msg.y))
            {
              line(sx,sy,sx,msg.y);
              line(sx-5,sy,sx+5,sy);
              line(sx-5,msg.y,sx+5,msg.y);
              string _o=to_string(abs(sy-msg.y));
              if((msg.y-sy)>0)
                outtextxy(sx,sy-15+(msg.y-sy)/2,_T(_o.c_str()));
              if((sy-msg.y)>0)
                outtextxy(sx,sy-15-(sy-msg.y)/2,_T(_o.c_str()));
            }
          line(msg.x-6,msg.y-6,msg.x+6,msg.y+6);
          line(msg.x+6,msg.y-6,msg.x-6,msg.y+6);
          SetWorkingImage(NULL);
          putimage(0, 0,X,Y, &img2, 0, 0);//渲染背景
          imgDC = GetImageHDC(&face_image_draw);//获取DC
          auxiliary_make(fx,fy,fw,fh,FW,FH,"MI band 8pro",fd,fd_N,&face_image_draw);//辅助线
          StretchBlt( mainDC,fx,fy,fw, fh,imgDC,0, 0,face_image->getwidth(), face_image->getheight(),SRCCOPY);// 目标 DC // 目标位置的坐标 // 目标位置的长宽 // 源 DC // 源位置的坐标 // 源位置的长宽 // 贴图的光栅模式
          FlushBatchDraw();
          if(GetAsyncKeyState(1) && 0x8000)
            {
              auxiliary_data_class M;
              M.x1=sx,M.y1=sy,M.x2=msg.x,M.y2=msg.y;
              auxiliary_data.push_back(M);
              Sleep(100);
              flushmessage();
              return ;
            }
        }
      SetWorkingImage(NULL);
    }
  if ((GetAsyncKeyState(4) && 0x8000)&&(GetAsyncKeyState(164) && 0x8000))   // 对某个控件启动辅助显示
    {
      if(fd_N!=0)
        {
          int N=fd_N-1;
          // 查询auxiliary_n是否存在N
          auto it = std::find(auxiliary_n.begin(), auxiliary_n.end(), N);
          if (it == auxiliary_n.end())
            {
              // 不存在,添加N
              auxiliary_n.push_back(N);
            }
          else
            {
              // 存在,删除
              auxiliary_n.erase(it);
            }
        }
      Sleep(200);
    }
  if (GetAsyncKeyState(38) && 0x8000)   // 上方向键按
    {
      fd[i].y=fd[i].y-1;
      Sleep(50);
    }
  else if (GetAsyncKeyState(40) && 0x8000)     // 下方向键按
    {
      fd[i].y=fd[i].y+1;
      Sleep(50);
    }
  else if (GetAsyncKeyState(37) && 0x8000 )     // 左方向键被按下
    {
      fd[i].x=fd[i].x-1;
      Sleep(50);
    }
  else if (GetAsyncKeyState(39) && 0x8000 )     // 右方向键被按下
    {
      fd[i].x=fd[i].x+1;
      Sleep(50);
    }
}



void info(ExMessage &msg,IMAGE face_image)
{
  // 计算鼠标位置相对于图片在屏幕上的左上角的偏移
  int dx = msg.x - fx;
  int dy = msg.y - fy;
  // 将偏移量映射到原图片大小
  float sx = (float)dx / fw * 336;
  float sy = (float)dy / fh * 480;
  mx = (int)sx;
  my = (int)sy;


  if (msg.message ==WM_LBUTTONDOWN)   //拖动控件+选中控件
    {
      int time;
      while(GetAsyncKeyState(1) && 0x8000&&fd[fd_N-1].lock==0)
        {
          time++;
          Sleep(10);
          if(time>5)
            {
              int mxo=mx,myo=my;
              if(fd_N==0)
                {
                  return ;
                }
              while(GetAsyncKeyState(VK_LBUTTON) < 0)
                {
                  msg = getmessage(EM_MOUSE);
                  // 计算鼠标位置相对于图片在屏幕上的左上角的偏移
                  int dx = msg.x - fx;
                  int dy = msg.y - fy;
                  // 将偏移量映射到原图片大小
                  float sx = (float)dx / fw * 336;
                  float sy = (float)dy / fh * 480;
                  mx = (int)sx;
                  my = (int)sy;
                  if(mxo!=mx||myo!=my)
                    {

                      fd[fd_N-1].x=fd[fd_N-1].x+(mx-mxo);
                      fd[fd_N-1].y=fd[fd_N-1].y+(my-myo);
                      mxo=mx,myo=my;
                      IMAGE face_image2=draw_face(face_result,fd);//读取虚拟编辑区
                      draw_fd_N(msg,&face_image2,fd_N);
                      imgDC = GetImageHDC(&face_image2);//获取DC
                      StretchBlt( mainDC,fx,fy,fw, fh,imgDC,0, 0,face_image.getwidth(), face_image.getheight(),SRCCOPY);// 目标 DC // 目标位置的坐标 // 目标位置的长宽 // 源 DC // 源位置的坐标 // 源位置的长宽 // 贴图的光栅模式
                      FlushBatchDraw();
                    }
                }
            }
        }
      //cout<<"x"<<msg.x<<" y"<<msg.y<<" fx"<<fx<<" fy"<<fy<<" fw"<<fw<<" fh"<<fh<<" "<<(msg.y >= fy)<<endl;
      if(msg.x >= fx && msg.x <= fx+fw && msg.y >= fy && msg.y <= fy+fh)
        {
          //cout<<"点击在表盘上!!\n";
          for(int i=fd.size()-1; i>-1; i--)
            {
              if (i < 0) i = 0; // 防止变成负数

              fd[i].dataSrc_choose=0;
              if(fd[i].x <= mx && mx <= (fd[i].w+fd[i].x) && fd[i].y <= my && my <= (fd[i].h+fd[i].y)&&fd[i].align==1)   //左对齐
                {
                  if (i+1 >= fd.size()) i = fd.size() - 1; // 防止过大索引
                  fd_N=i+1;
                  break;
                }
              if(fd[i].x-fd[i].w <= mx && mx <= (fd[i].x) && fd[i].y <= my && my <= (fd[i].h+fd[i].y)&&fd[i].align==0)   //右对齐
                {
                  if (i+1 >= fd.size()) i = fd.size() - 1; // 防止过大索引
                  fd_N=i+1;
                  break;
                }
              if(fd[i].x-(fd[i].w/2) <= mx && mx <= ((fd[i].w/2)+fd[i].x) && fd[i].y <= my && my <= (fd[i].h+fd[i].y)&&fd[i].align==2)   //中对齐
                {
                  if (i+1 >= fd.size()) i = fd.size() - 1; // 防止过大索引
                  fd_N=i+1;
                  break;
                }
            }
        }

    }
  if (GetAsyncKeyState(VK_RBUTTON) < 0)   //移动编辑区
    {
      int relX = msg.x - fx;
      int relY = msg.y - fy;
      while(GetAsyncKeyState(VK_RBUTTON) < 0)
        {
          GetMousePosInClient( hWnd,msg);
          int offsetX = msg.x - relX;
          int offsetY = msg.y - relY;
          fx = offsetX;
          fy = offsetY;
          putimage(0, 0,X,Y, &img2, 0, 0);//渲染背景
          StretchBlt( mainDC,fx,fy,fw, fh,imgDC,0, 0,face_image.getwidth(), face_image.getheight(),SRCCOPY);// 目标 DC // 目标位置的坐标 // 目标位置的长宽 // 源 DC // 源位置的坐标 // 源位置的长宽 // 贴图的光栅模式
          FlushBatchDraw();
        }
      flushmessage();
    }
  if(1)   //缩放
    {
      if(msg.wheel > 0)
        {
          msg.wheel=0;
          big_draw= big_draw+0.05;
          fw=FW* big_draw, fh=FH* big_draw;
        }
      else if(msg.wheel < 0)
        {
          msg.wheel=0;
          big_draw= big_draw-0.05;
          fw=FW* big_draw,fh=FH* big_draw;
        }
      return ;
    }
}


void NEW_face_data()   //新建组件--按钮操作
{
  if(face_result=="")
    {
      MessageBox(NULL,"温馨提示:工具选择json文件(底包)才能正常工作哦！","确定",MB_OK|MB_ICONEXCLAMATION | MB_SETFOREGROUND);
    }
  int left = (X - 500) / 2;
  int top = (Y - 300) / 2;
  top=top-50;
  int oldlinecolor = getlinecolor();
  int oldbkcolor = getbkcolor();
  int oldfillcolor = getfillcolor();
  setlinecolor(RGB(221,231,240));			// 设置画线颜色
  settextcolor(RGB(195,204,212));//设置字体颜色
  setfillcolor(RGB(33, 43, 53));			// 设置填充颜色
  setbkcolor(RGB(33, 43, 53));// 设置背景颜色
  settextstyle(font_b+7, 0, _T("Segoe UI"));//字体
  fillrectangle(left, top, left+500, top+300);
  string _o="新建组件";
  outtextxy(left+5,top,_T(_o.c_str()));
  setlinecolor(RGB(243,102,56));
  line(left, top+35, left+500, top+35);
  _o="控件类型:";
  outtextxy(left+5,top+40,_T(_o.c_str()));
  EasyCheckBox _element;
  EasyCheckBox _widge_dignum;
  EasyCheckBox _widge_imagelist;
  EasyCheckBox _widge_pointer;
  EasyCheckBox _element_anim;
  _element.Create(left+5,top+75,left+25,top+75+25); //+35
  _widge_dignum.Create(left+5,top+110,left+25,top+110+25); //+35
  _widge_imagelist.Create(left+5,top+145,left+25,top+145+25); //+35
  _widge_pointer.Create(left+5,top+180,left+25,top+180+25); //+35
  _element_anim.Create(left+5,top+215,left+25,top+215+25); //+35
  settextstyle(font_b, 0, _T("Segoe UI"));
  _o="图片控件";
  outtextxy(left+5+30,top+75,_T(_o.c_str()));
  _o="数字控件";
  outtextxy(left+5+30,top+110,_T(_o.c_str()));
  _o="图片列表控件";
  outtextxy(left+5+30,top+145,_T(_o.c_str()));
  _o="指针控件";
  outtextxy(left+5+30,top+180,_T(_o.c_str()));
  _o="动态图片控件";
  outtextxy(left+5+30,top+215,_T(_o.c_str()));

  ExMessage msg;
  while(1)
    {
      msg = getmessage(EM_MOUSE|EM_KEY);			// 获取消息输入

      if (msg.message == WM_LBUTTONDOWN)
        {
          _element.OnClick(msg.x,msg.y);
          _widge_dignum.OnClick(msg.x,msg.y);
          _widge_imagelist.OnClick(msg.x,msg.y);
          _widge_pointer.OnClick(msg.x,msg.y);
          _element_anim.OnClick(msg.x,msg.y);
        }
      _element.Draw();
      _widge_dignum.Draw();
      _widge_imagelist.Draw();
      _widge_pointer.Draw();
      _element_anim.Draw();
      if(_element.checked)
        {
          face_data m;
          m.type="element";
          m.x=10;
          m.y=10;
          m.h=50;
          m.w=50;
          m.image.push_back("?");
          fd.push_back(m);
          fd_N=fd.size();
          reloadImageData(fd, face_result);
          break;
        }
      if(_widge_dignum.checked)
        {
          face_data m;
          m.type="widge_dignum";
          m.x=10;
          m.y=10;
          m.h=50;
          m.w=50;
          m.showCount=1;
          m.dataSrc="3041";
          m.imageList.push_back("?");
          fd.push_back(m);
          fd_N=fd.size();
          reloadImageData(fd, face_result);
          break;
        }
      if(_widge_imagelist.checked)
        {
          face_data m;
          m.type="widge_imagelist";
          m.x=10;
          m.y=10;
          m.h=50;
          m.w=50;
          m.dataSrc="3041";
          m.imageList.push_back("?");
          //m.imageIndexList.push_back(1);
          fd.push_back(m);
          fd_N=fd.size();
          reloadImageData(fd, face_result);
          break;
        }
      if(_element_anim.checked)
        {
          face_data m;
          m.type="element_anim";
          m.x=10;
          m.y=10;
          m.h=50;
          m.w=50;
          m.imageList.push_back("?");
          //m.imageIndexList.push_back(1);
          fd.push_back(m);
          fd_N=fd.size();
          reloadImageData(fd, face_result);
          break;
        }
      if(_widge_pointer.checked)
        {
          face_data m;
          m.type="widge_pointer";
          m.x=10;
          m.y=10;
          m.h=50;
          m.w=50;
          m.dataSrc="3041";
          m.image.push_back("?");
          //m.imageIndexList.push_back(1);
          fd.push_back(m);
          fd_N=fd.size();
          reloadImageData(fd, face_result);
          break;
        }
      FlushBatchDraw();
    }

  setlinecolor(oldlinecolor);
  setbkcolor(oldbkcolor);
  setfillcolor(oldfillcolor);
  settextstyle(font_b, 0, _T("Segoe UI"));
  FlushBatchDraw();
  Sleep(100);
  //getch();
}
void welcome_UI()//欢迎界面
{
  IMAGE welcome_M;
  IMAGE OPEN_M;
  IMAGE NEW_M;
  loadimage(&welcome_M, _T("data//PNG//LOGO 4.png"),1024,545,1);
  loadimage(&NEW_M, _T("data//PNG//新建.png"),128,128,1);
  loadimage(&OPEN_M, _T("data//PNG//打开.png"),128,128,1);
  NEW_Button_no_dui(840,130,968,258, _T(""), "W",On_btnOk_Click);
  NEW_Button_no_dui(840,290,968,418,_T(""), "W",On_NEW_face);
  while(welcome_UI_END==0)
    {
      peekmessage(&msg,-1,true);
      UI_DUI(msg);//UI自动对齐 包括点击检测
      putimage(0, 0, &welcome_M);

      putimage(840,130,&OPEN_M);
      putimage(840,290,&NEW_M);
      FlushBatchDraw();
    }
  DEL_Button_no_dui("W");
  UIEasyButtonNO.clear();
}

int main()
{
//--------------------------------------------------------------------

  /* HWND hwnd = initgraph(1024,576);//EW_SHOWCONSOLE
   int screenWidth = GetSystemMetrics(SM_CXSCREEN);
   int screenHeight = GetSystemMetrics(SM_CYSCREEN);
   RECT rc;
   GetWindowRect(hwnd, &rc);
   int windowWidth = rc.right - rc.left;
   int windowHeight = rc.bottom - rc.top;
   int left = (screenWidth - windowWidth) / 2;
   int top = (screenHeight - windowHeight) / 2;

   ::SetWindowLong(hwnd, GWL_STYLE, GetWindowLong(hwnd, GWL_STYLE) & (~(WS_CAPTION | WS_SYSMENU | WS_SIZEBOX)));
   ::SetWindowLong(hwnd, GWL_EXSTYLE, GetWindowLong(hwnd, GWL_EXSTYLE) & (~(WS_EX_WINDOWEDGE | WS_EX_DLGMODALFRAME)) | WS_EX_LAYERED | WS_EX_TOOLWINDOW);

   SetWindowPos(hwnd, HWND_TOPMOST, (GetSystemMetrics(SM_CXSCREEN) - getwidth()) / 2, (GetSystemMetrics(SM_CYSCREEN) - getheight()) / 2,getwidth(), getheight(), SWP_SHOWWINDOW
                | SWP_FRAMECHANGED | SWP_DRAWFRAME);
   SetLayeredWindowAttributes(hwnd, 0x000000, 0, LWA_COLORKEY);
   setcolor(0x0000ff);
   //MoveWindow(hWnd,0,0,1024,576,NULL);
   loadimage(NULL, _T("data//PNG//LOGO.png"));
   MoveWindow(hwnd, left, top, windowWidth, windowHeight, true);
   Sleep(1300);
   //getch();*/
  closegraph();
//-----------------------------------------------------------------------LOGO

//----配置文件
  cout<<"\n读取配置文件:\n";
  int draw_fast=0;
  map<string, int*> configVars =
  {
    {"draw_fast", &draw_fast},
    {"examine_face_T", &examine_face_T},
    {"save_time", &save_time}
  };

  LoadConfig("data//default.config", &configVars);
  cout<<"draw_fast:"<<draw_fast<<endl;
  cout<<"--结束\n";
//--------

  fd=vector<face_data>();
  fd_=vector<face_data>();
  fd_AOD=vector<face_data>();

  hWnd =initgraph(1920,1080,EW_SHOWCONSOLE);
  MoveWindow(hWnd,0,0,1024,576,1);
  settextstyle(font_b, 0, _T("Segoe UI"));
  BeginBatchDraw();

  mainDC = GetImageHDC(NULL);					// 获取主窗口的 DC
  if(0<draw_fast)
    SetStretchBltMode(mainDC, STRETCH_HALFTONE),cout<<"抗锯齿开启\n";	// 设置拉伸贴图模式为抗锯齿
  if(0<examine_face_T)
    cout<<"检查examine_face模块 开启\n";	// 设置拉伸贴图模式为抗锯齿
  if(0<save_time)
    cout<<"自动保存模式启动,请注意!!! 间隔时间:"<<save_time<<"秒"<<endl;
  // 如果不设置，默认贴图模式为临近颜色，效果差，性能高

  Li_win_wh();
  FW=336,FH=480;
  fw=336,fh=480;//表盘渲染时的大小 w=x h=y
  fx=(X/2)-168,fy=0;//表盘渲染时的位置

  loadimage(&img2, _T("data//PNG//背景.png"));
  loadimage(&STOP_M, _T("data//PNG//LOGO3.png"));
  loadimage(&LOCK_M, _T("data//PNG//锁定.png"));
  loadimage(&NO_LOCK_M, _T("data//PNG//解锁.png"));
  loadimage(&Error_M, _T("data//PNG//错误.png"));
  loadimage(&Warn_M, _T("data//PNG//警告.png"));
  loadimage(&Toolbox_M, _T("data//PNG//工具箱.png"));
  loadimage(&Frame_M, _T("data//PNG//边框.png"));
  welcome_UI();//欢迎界面
  putimage(0, 0,X,Y, &img2, 0, 0);
  NEW_Button_no_dui(0,0,71, 25, _T("打开文件"), "?",On_btnOk_Click);
  UIEasyButtonNO[0].anim=1.07;
  NEW_Button_no_dui(73,0,144, 25, _T("保存文件"), "?",On_save);
  NEW_Button_no_dui(146,0,217, 25, _T("全屏窗口"), "?",On_biger_windows);
  NEW_Button_no_dui(219,0,288, 25, _T("新建控件"), "?",NEW_face_data);
  NEW_Button_no_dui(290,0,380, 25, _T("保存预览图"), "?",Save_png_draw);
  NEW_Button_no_dui(382,0,490, 25, _T("正常<->AOD"), "?",AOD_do);
  NEW_Button_no_dui(492,0,561, 25, _T("编译"), "?",make_band_face);
  NEW_Button_no_dui(562,0,631, 25, _T("设置"), "?",SET_UI);
  NEW_Button_no_dui(632,0,691, 25, _T("..."), "?",THANK);
// NEW_Button_no_dui(200,Y-70,200+30,Y-40, _T("工具"), "?",fast_UP_face);
  start(hWnd);//初始化

  while (true)
    {
      //msg = getmessage(EM_MOUSE|EM_KEY);			// 获取消息输入
      peekmessage(&msg,-1,true);
      //if(msg_xy_f)
      GetMousePosInClient(hWnd,msg);

      NO_USE_UI(hWnd,msg,mainDC,imgDC);//焦点
      C_N=0;//把组件管理器的页面重置

      putimage(0, 0,X,Y, &img2, 0, 0);//渲染背景

      IMAGE face_image=draw_face(face_result,fd);//读取虚拟编辑区

      imgDC = GetImageHDC(&face_image);//获取DC
      draw_fd_N(msg,&face_image,fd_N);//渲染高亮
      SetWorkingImage(&face_image);//渲染光标
      line(mx-6,my-6,mx+6,my+6);
      line(mx+6,my-6,mx-6,my+6);
      SetWorkingImage(NULL);
      auxiliary_make(fx,fy,fw,fh,FW,FH,"MI band 8pro",fd,fd_N,&face_image);//辅助线
      transparentimage3(&face_image,0,0,&Frame_M);
      StretchBlt( mainDC,fx,fy,fw, fh,imgDC,0, 0,face_image.getwidth(), face_image.getheight(),SRCCOPY);// 目标 DC // 目标位置的坐标 // 目标位置的长宽 // 源 DC // 源位置的坐标 // 源位置的长宽 // 贴图的光栅模式
      BOX.draw_Text_box();
      data_work(fd_N,fd,msg,fx,fy);//编辑操作
      info(msg,face_image);//输入处理
      face_data_see_box(fd_N,fd,msg,fd_N,C_N);//组件管理器
      UI_DUI(msg);//UI自动对齐 包括点击检测

      draw_UI();//渲染UI
      draw_face_name();//显示表盘名字
      CalculateFrameRate(msg);
      if(IsMouseClickedInWindow(hWnd))   //计时器+
        {
          time_save++;
        }
      line(msg.x, msg.y - 10, msg.x, msg.y + 10);
      line(msg.x - 10, msg.y, msg.x + 10, msg.y);
      FlushBatchDraw();
      if(time_save>7)
        {
          cout<<"撤回保存!\n";
          time_save=0;
          fd_cz.clear();
          for(int i=0; i<fd.size(); i++)
            fd_cz.push_back(fd[i]);
        }
      if((SAVET>save_time)&&save_time>0)//自动保存
        {
          cout<<"自动保存\n",On_save_time(),SAVET=0;
        }
    }


  getch();
  EndBatchDraw();
  closegraph();
  return 0;
}