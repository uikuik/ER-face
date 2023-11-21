#include <vector>
#include <iostream>
#include "jsoncpp.cpp" // JSON库
#include <fstream>
#include <time.h>
#include <string>
#include "Files.h"
bool fd_ch=0;//0正常表盘 1AOD表盘
int font_b=25;//字体大小
bool R_C=1;//启动跳动预览
bool biger_ALL=0;//全屏是否覆盖任务栏
string face_result="";//表盘地址
bool biger_windows=0;//是否全屏
int examine_face_T=0;//是否启动检查
bool reload=0;//重新加载标志
string face_name="???";

ExMessage msg;
class face_data
{
  public:
    string draw_data;
    string type="?";
    int x=0;
    int y=0;
    int w=0;//宽
    int h=0;//高
    vector<string> image;
    int showCount=0;
    int align=1;
    int spacing=0;
    int maxValue=0;//指针-最大取值
    int allAngle=0;//指针-推测是最大旋转角度*10  allangle/10/maxvalue=每进1旋转的角度
    int imageRotateX=0;//指针-指针围绕坐标
    int imageRotateY=0;//指针-指针围绕坐标
    int animInterval=0;//动画间隔 单位为ms
    int animRepeat=0;//指定动画重复次数 0为无限
    string jumpName="?";//跳转名字
    string jumpCode="?";//跳转代码
    string showZero="false";//false
    string dataSrc="?";
    int R=0;//随机数 用于显示！！！
    int R_time=0;//计时数据 用于显示！！！
    double Xuan_Angle=-1;//指针的旋转角度 用于显示！！！
    int anim_index=0;//动画下标 用于显示！！！
    bool dataSrc_choose=0;//是否正在选择数据源
    bool lock=0;//是否锁定
    int error_warn=0;//是否报错 0 为无 1为error 2为warn
    vector<string> imageList;
    vector<int> imageIndexList;//百分比
    vector<IMAGE> image_data;//图片数据
    bool attach=0;//是否存在附加图片

    face_data(const face_data& rhs)
    {
      type = rhs.type;
      x = rhs.x;
      y = rhs.y;
      w = rhs.w;
      h = rhs.h;
      image = rhs.image;
      showCount = rhs.showCount;
      align = rhs.align;
      spacing = rhs.spacing;
      maxValue = rhs.maxValue;
      allAngle = rhs.allAngle;
      imageRotateX = rhs.imageRotateX;
      imageRotateY = rhs.imageRotateY;
      animInterval = rhs.animInterval;
      animRepeat = rhs.animRepeat;
      jumpName = rhs.jumpName;
      jumpCode = rhs.jumpCode;
      showZero = rhs.showZero;
      dataSrc = rhs.dataSrc;
      R = rhs.R;
      R_time = rhs.R_time;
      Xuan_Angle = rhs.Xuan_Angle;
      anim_index = rhs.anim_index;
      dataSrc_choose = rhs.dataSrc_choose;
      imageList = rhs.imageList;
      imageIndexList = rhs.imageIndexList;
      error_warn = rhs.error_warn;
      lock = rhs.lock;
      attach = rhs.attach;
    }

    face_data()
    {
      //image_data = vector<IMAGE>(); // 初始化为空vector
    }
};

string dataSRC[41] = {"0811","0911","0A11","1011","1111","1211","1811","1911","1A11","0812","1012","1812","1912","1A12","2012","0821","0822","1022","0823","0851","1051","0824","0826","0828","0841","2031","3031","5031","2831","1013","3041","2041","1841","1021","1023","3012","3812","1832","2032","7831","4831"};
string SRC_str[41] = {"小时","小时(个)","小时(十)","分钟","分钟(个)","分钟(十)","秒","秒(个)","秒(十)","日期-年","日期-月","日期-日","日期-日(个)","日期-日(十)","星期","步数","心率","心率值区间","卡路里","距离-米","距离-千米","活动","压力","睡眠","电量","天气-温度","天气-类型","天气-空气","天气-湿度","上午下午","锁定","蓝牙","勿扰","步数-百分比","卡路里-百分比","农历 月","农历 日","当日最高气温","当日最低气温","紫外线","风力"};

string _jumpNameSTR[] = {"天气状况","步数","血氧","卡路里","站立","运动强度","睡眠","气压","日出时间","日落时间","湿度","紫外线指数","空气质量","闹钟","倒计时","秒表","音乐控制","拍照","微信支付","卡包","心率","电量","小爱同学"};
string _jumpName[] = {"weatherCondition","step","oxygen","calorie","stand","intensity","sleep","pressure","sunRise","sunSet","humidity","UVI","AQI","alarm","countdown","stopwatch","musicController","photograph","wechatPayment","cardBag","heartRate","batteryLevel","AIVS"};
string _jumpCode[] = {"030B1F02","03023F01","0307EF00","03033F01","03043F01","03053F01","0300CF00","03083F00","050B1F02","050B1F02","030B1F02","030B1F02","030B1F02","010C8F02","010C6F02","010C0F02","010C5F01","010CCF01","010D5F02","010ABF02","03065F00","03090C00","010CDF02"};


string dataSRC_out(string data,string find)//数据源对照表 find:查找类型  str返回中文  src返回数据源id  ,find_i 在表上调整指针位置对应的序号
{
  if(find=="str")
    {
      for(int i=0; i<42; i++)
        {
          if(dataSRC[i]==data)
            return SRC_str[i];
        }
    }
  if(find=="scr")
    {
      for(int i=0; i<42; i++)
        {
          if(SRC_str[i]==data)
            return dataSRC[i];
        }
    }
  return "?";
}

string dataSRC_out_2(string data,int n)//返回指定数据源 +多少位
{
  for(int i=0; i<42; i++)
    {
      if(dataSRC[i]==data)
        {
          if(i+n>41)
            i=0;
          if(i+n<0)
            i=41;
          return dataSRC[i+n];
        }
    }
  return "";
}

void parseFile(const string& filepath, vector<face_data>& faceDataList)//读取
{

  Json::Value root;
  Json::Reader reader;

  ifstream infile(filepath);
  if (!infile)
    {
      cerr << "Error opening file: " << filepath << endl;
      return;
    }

  if (!reader.parse(infile, root))
    {
      cerr << "Error parsing json" << endl;
      return;
    }
  face_name=root["name"].asString();

  auto elements = root["elementsNormal"];
  for(auto element : elements)
    {

      face_data data;

      data.type = element["type"].asString();
      if (element.isMember("x"))
        {
          data.x = element["x"].asInt();
        }
      if (element.isMember("y"))
        {
          data.y = element["y"].asInt();
        }
      if (element.isMember("showCount"))
        {
          data.showCount = element["showCount"].asInt();
        }
      if (element.isMember("spacing"))
        {
          data.spacing = element["spacing"].asInt();
        }
      if (element.isMember("dataSrc"))
        {
          data.dataSrc = element["dataSrc"].asString();
        }
      if (element.isMember("align"))
        {
          data.align = element["align"].asInt();
        }
      if (element.isMember("showZero"))
        {
          data.showZero = element["showZero"].asString();
        }
      if (element.isMember("maxValue"))
        {
          data.maxValue = element["maxValue"].asInt();
        }
      if (element.isMember("allAngle"))
        {
          data.allAngle = element["allAngle"].asInt();
        }
      if (element.isMember("imageRotateX"))
        {
          data.imageRotateX = element["imageRotateX"].asInt();
        }
      if (element.isMember("imageRotateY"))
        {
          data.imageRotateY = element["imageRotateY"].asInt();
        }
      if (element.isMember("animInterval"))
        {
          data.animInterval = element["animInterval"].asInt();
        }
      if (element.isMember("animRepeat"))
        {
          data.animRepeat = element["animRepeat"].asInt();
        }
      if (element.isMember("jumpName"))
        {
          data.jumpName = element["jumpName"].asString();
        }
      if (element.isMember("jumpCode"))
        {
          data.jumpCode = element["jumpCode"].asString();
        }

      //...解析其他字段

      // 解析image字段
      if (element.isMember("image"))
        {
          data.image.push_back(element["image"].asString());
          if(data.type=="widge_dignum")
            {
              cout<<"附加图片\n";
              data.attach=1;
            }
        }

      // 解析imageList字段
      if (element.isMember("imageList"))
        {
          auto imageList = element["imageList"];
          for(auto img : imageList)
            {
              data.imageList.push_back(img.asString());
            }
        }
      if (element.isMember("imageIndexList"))
        {
          auto imageIndexList = element["imageIndexList"];
          for(auto img : imageIndexList)
            {
              data.imageIndexList.push_back(img.asInt());
            }
        }

      faceDataList.push_back(data);
    }

}

void parseFile_AOD(const string& filepath, vector<face_data>& faceDataList)//读取！！！AOD
{

  Json::Value root;
  Json::Reader reader;

  ifstream infile(filepath);
  if (!infile)
    {
      cerr << "Error opening file: " << filepath << endl;
      return;
    }

  if (!reader.parse(infile, root))
    {
      cerr << "Error parsing json" << endl;
      return;
    }

  auto elements = root["elementsAod"];
  for(auto element : elements)
    {

      face_data data;

      data.type = element["type"].asString();
      if (element.isMember("x"))
        {
          data.x = element["x"].asInt();
        }
      if (element.isMember("y"))
        {
          data.y = element["y"].asInt();
        }
      if (element.isMember("showCount"))
        {
          data.showCount = element["showCount"].asInt();
        }
      if (element.isMember("spacing"))
        {
          data.spacing = element["spacing"].asInt();
        }
      if (element.isMember("dataSrc"))
        {
          data.dataSrc = element["dataSrc"].asString();
        }
      if (element.isMember("align"))
        {
          data.align = element["align"].asInt();
        }
      if (element.isMember("showZero"))
        {
          data.showZero = element["showZero"].asString();
        }
      if (element.isMember("maxValue"))
        {
          data.maxValue = element["maxValue"].asInt();
        }
      if (element.isMember("allAngle"))
        {
          data.allAngle = element["allAngle"].asInt();
        }
      if (element.isMember("imageRotateX"))
        {
          data.imageRotateX = element["imageRotateX"].asInt();
        }
      if (element.isMember("imageRotateY"))
        {
          data.imageRotateY = element["imageRotateY"].asInt();
        }
      if (element.isMember("animInterval"))
        {
          data.animInterval = element["animInterval"].asInt();
        }
      if (element.isMember("animRepeat"))
        {
          data.animRepeat = element["animRepeat"].asInt();
        }

      //...解析其他字段

      // 解析image字段
      if (element.isMember("image"))
        {
          data.image.push_back(element["image"].asString());
          if(data.type=="widge_dignum")
            {
              cout<<"附加图片\n";
              data.attach=1;
            }
        }

      // 解析imageList字段
      if (element.isMember("imageList"))
        {
          auto imageList = element["imageList"];
          for(auto img : imageList)
            {
              data.imageList.push_back(img.asString());
            }
        }
      if (element.isMember("imageIndexList"))
        {
          auto imageIndexList = element["imageIndexList"];
          for(auto img : imageIndexList)
            {
              data.imageIndexList.push_back(img.asInt());
            }
        }

      faceDataList.push_back(data);
    }

}
void serializeToJson(const vector<face_data>& faceDataList,const vector<face_data>& faceDataList_aod, const string& filepath)//保存
{
  Json::Value root;
  Json::Value elementsNormal(Json::arrayValue);
  Json::Value elementsAod(Json::arrayValue);

  root["name"] = face_name;
  root["id"] = "267210032";
  root["previewImg"] = "preview";

  for (const auto& data : faceDataList)
    {
      Json::Value element;

      element["type"] = data.type;

      if (data.type == "element")
        {
          element["x"] = data.x;
          element["y"] = data.y;
          if(data.jumpCode!="?"&&data.jumpName!="?")
            {
              element["jumpName"] = data.jumpName;
              element["jumpCode"] = data.jumpCode;
            }
          if (!data.image.empty())
            {
              cout<<"element data.image:"<<data.image[0]<<endl;
              element["image"] = data.image[0];
            }
        }
      if(data.type =="element_anim")
        {
          element["x"] = data.x;
          element["y"] = data.y;
          element["animInterval"] = data.animInterval;
          element["animRepeat"] = data.animRepeat;
          if (!data.imageList.empty())
            {
              Json::Value imageList(Json::arrayValue);
              for (const auto& img : data.imageList)
                {
                  imageList.append(img);
                }
              element["imageList"] = imageList;
            }
        }
      if(data.type =="widge_imagelist")
        {
          element["x"] = data.x;
          element["y"] = data.y;
          element["dataSrc"] = data.dataSrc;
          if (!data.imageList.empty())
            {
              Json::Value imageList(Json::arrayValue);
              for (const auto& img : data.imageList)
                {
                  imageList.append(img);
                }
              element["imageList"] = imageList;
            }
          if (!data.imageIndexList.empty())
            {
              Json::Value imageIndexList(Json::arrayValue);
              for (const auto& img : data.imageIndexList)
                {
                  imageIndexList.append(img);
                }
              element["imageIndexList"] = imageIndexList;
            }
        }
      if(data.type =="widge_dignum")
        {
          element["x"] = data.x;
          element["y"] = data.y;
          element["showCount"] = data.showCount;
          element["spacing"] = data.spacing;
          element["dataSrc"] = data.dataSrc;
          element["align"] = data.align;
          if(data.showZero=="true")
            element["showZero"] = true;
          if(data.showZero=="false")
            element["showZero"] = false;
          // Serialize imageList field
          if (!data.imageList.empty())
            {
              Json::Value imageList(Json::arrayValue);
              for (const auto& img : data.imageList)
                {
                  imageList.append(img);
                }
              element["imageList"] = imageList;
            }
          if (!data.image.empty())
            {
              cout<<"element data.image:"<<data.image[0]<<endl;
              element["image"] = data.image[0];
            }
        }
      if(data.type =="widge_pointer")//指针
        {
          element["x"] = data.x;
          element["y"] = data.y;
          element["dataSrc"] = data.dataSrc;
          element["maxValue"] = data.maxValue;
          element["allAngle"] = data.allAngle;
          element["imageRotateX"] = data.imageRotateX;
          element["imageRotateY"] = data.imageRotateY;
          if (!data.image.empty())
            {
              element["image"] = data.image[0];
            }
        }

      elementsNormal.append(element);
    }

  root["elementsNormal"] = elementsNormal;

  for (const auto& data : faceDataList_aod)
    {
      Json::Value element;

      element["type"] = data.type;

      if (data.type == "element")
        {
          element["x"] = data.x;
          element["y"] = data.y;
          if (!data.image.empty())
            {
              cout<<"element data.image:"<<data.image[0]<<endl;
              element["image"] = data.image[0];
            }
        }
      if(data.type =="element_anim")
        {
          element["x"] = data.x;
          element["y"] = data.y;
          element["animInterval"] = data.animInterval;
          element["animRepeat"] = data.animRepeat;
          if (!data.imageList.empty())
            {
              Json::Value imageList(Json::arrayValue);
              for (const auto& img : data.imageList)
                {
                  imageList.append(img);
                }
              element["imageList"] = imageList;
            }
        }
      if(data.type =="widge_imagelist")
        {
          element["x"] = data.x;
          element["y"] = data.y;
          element["dataSrc"] = data.dataSrc;
          if (!data.imageList.empty())
            {
              Json::Value imageList(Json::arrayValue);
              for (const auto& img : data.imageList)
                {
                  imageList.append(img);
                }
              element["imageList"] = imageList;
            }
          if (!data.imageIndexList.empty())
            {
              Json::Value imageIndexList(Json::arrayValue);
              for (const auto& img : data.imageIndexList)
                {
                  imageIndexList.append(img);
                }
              element["imageIndexList"] = imageIndexList;
            }
        }
      if(data.type =="widge_dignum")
        {
          element["x"] = data.x;
          element["y"] = data.y;
          element["showCount"] = data.showCount;
          element["spacing"] = data.spacing;
          element["dataSrc"] = data.dataSrc;
          element["align"] = data.align;
          if(data.showZero=="true")
            element["showZero"] = true;
          if(data.showZero=="false")
            element["showZero"] = false;
          // Serialize imageList field
          if (!data.imageList.empty())
            {
              Json::Value imageList(Json::arrayValue);
              for (const auto& img : data.imageList)
                {
                  imageList.append(img);
                }
              element["imageList"] = imageList;
            }
          if (!data.image.empty())
            {
              cout<<"element data.image:"<<data.image[0]<<endl;
              element["image"] = data.image[0];
            }
        }
      if(data.type =="widge_pointer")//指针
        {
          element["x"] = data.x;
          element["y"] = data.y;
          element["dataSrc"] = data.dataSrc;
          element["maxValue"] = data.maxValue;
          element["allAngle"] = data.allAngle;
          element["imageRotateX"] = data.imageRotateX;
          element["imageRotateY"] = data.imageRotateY;
          if (!data.image.empty())
            {
              element["image"] = data.image[0];
            }
        }

      elementsAod.append(element);
    }

  root["elementsAod"] = elementsAod;


  Json::StyledWriter writer;
  std::ofstream outfile(filepath);
  outfile << writer.write(root);
}
bool moveElement(std::vector<face_data>& vec, int index, int n)//移动控件图层
{
  int size = vec.size();
  if((index+n)<-1)
    return 1;
  if((index+n)>size)
    return 1;

  if (n > 0)
    {
      if (index - n < 0)
        {
          // 向前移动会越界,不做任何操作
          return 1;
        }
      // 向前移动
      face_data temp = vec[index];
      for (int i = index; i > index - n; i--)
        {
          vec[i] = vec[i-1];
        }
      vec[index - n] = temp;

    }
  else if (n < 0)
    {
      if (index - n >= size)
        {
          // 向后移动会越界,不做任何操作
          return 1;
        }
      // 向后移动
      face_data temp = vec[index];
      for (int i = index; i < index - n; i++)
        {
          vec[i] = vec[i+1];
        }
      vec[index - n] = temp;
    }
  return 0;
}
string replaceSlash(const string& str)//替换  \\->//
{
  string result = str;

  for(size_t i = 0; i < result.length(); i++)
    {
      if (result[i] == '\\')
        {
          result.replace(i, 1, "//");
          //i++; // 加2是因为增加了两个字符
        }
    }
  string s2="//wfDef.json";
  while (result.find(s2) < result.length())
    {
      result.erase(result.find(s2), s2.size());
    }
  return result;
}

void transparentimage3(IMAGE* dstimg, int x, int y, IMAGE* srcimg) //新版png
{
  HDC dstDC = GetImageHDC(dstimg);
  HDC srcDC = GetImageHDC(srcimg);
  int w = srcimg->getwidth();
  int h = srcimg->getheight();
  BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
  AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

int getRand(int min, int max)//生成随机数
{
  return ( rand() % (max - min + 1) ) + min ;
}

void image_wh(const string& filepath,vector<face_data>& faceDataList,bool ch)//计算控件长宽
{
  for(int i=0; i<faceDataList.size(); i++)
    {
      if(faceDataList[i].type=="element")
        {
          string data;
          if(ch==0)
            data=filepath+"\\"+"images"+"\\"+faceDataList[i].image[0]+".png";
          else
            data=filepath+"\\"+"images_aod"+"\\"+faceDataList[i].image[0]+".png";
          data=replaceSlash(data);
          IMAGE img;
          loadimage(&img,data.c_str());
          faceDataList[i].w = img.getwidth();
          faceDataList[i].h = img.getheight();
        }
      if(faceDataList[i].type=="widge_imagelist"||faceDataList[i].type=="element_anim")
        {
          string data;
          if(ch==0)
            data=filepath+"\\"+"images"+"\\"+faceDataList[i].imageList[0]+".png";
          else
            data=filepath+"\\"+"images_aod"+"\\"+faceDataList[i].imageList[0]+".png";
          data=replaceSlash(data);
          IMAGE img;
          loadimage(&img,data.c_str());
          faceDataList[i].w = img.getwidth();
          faceDataList[i].h = img.getheight();
        }
      if(faceDataList[i].type=="widge_dignum")
        {
          string data;
          if(ch==0)
            data=filepath+"\\"+"images"+"\\"+faceDataList[i].imageList[faceDataList[i].anim_index]+".png";
          else
            data=filepath+"\\"+"images_aod"+"\\"+faceDataList[i].imageList[faceDataList[i].anim_index]+".png";
          data=replaceSlash(data);
          IMAGE img;
          loadimage(&img,data.c_str());
          faceDataList[i].w = (img.getwidth()*faceDataList[i].showCount)+(faceDataList[i].showCount*faceDataList[i].spacing);
          faceDataList[i].h = img.getheight();
        }
      if(faceDataList[i].type=="widge_pointer")
        {
          string data;
          if(ch==0)
            data=filepath+"\\"+"images"+"\\"+faceDataList[i].image[0]+".png";
          else
            data=filepath+"\\"+"images_aod"+"\\"+faceDataList[i].image[0]+".png";
          data=replaceSlash(data);
          IMAGE img;
          loadimage(&img,data.c_str());
          faceDataList[i].w = img.getwidth();
          faceDataList[i].h = img.getheight();
        }
    }
}

void reloadImageData(vector<face_data>& faceDataList, const string& filepath)
{

  for (face_data& fdata : faceDataList)
    {

      fdata.image_data.clear();

      if (fdata.type == "element")
        {
          // 加载单张图片
          IMAGE img;
          string data;
          if(fd_ch==0)
            data=filepath+"\\"+"images"+"\\"+fdata.image[0]+".png";
          else
            data=filepath+"\\"+"images_aod"+"\\"+fdata.image[0]+".png";
          data=replaceSlash(data);

          loadimage(&img, data.c_str());
          fdata.image_data.push_back(img);

        }
      else if (fdata.type == "widge_imagelist")
        {

          // 加载多张图片
          for (auto imgName : fdata.imageList)
            {
              IMAGE img;
              string data;
              if(fd_ch==0)
                data=filepath+"\\"+"images"+"\\"+imgName+".png";
              else
                data=filepath+"\\"+"images_aod"+"\\"+imgName+".png";
              data=replaceSlash(data);
              loadimage(&img, data.c_str());
              fdata.image_data.push_back(img);
            }

        }
      else if (fdata.type == "widge_dignum")
        {
          // 加载多张图片
          for (auto imgName : fdata.imageList)
            {
              IMAGE img;
              string data;
              if(fd_ch==0)
                data=filepath+"\\"+"images"+"\\"+imgName+".png";
              else
                data=filepath+"\\"+"images_aod"+"\\"+imgName+".png";
              data=replaceSlash(data);
              loadimage(&img, data.c_str());
              fdata.image_data.push_back(img);
            }
          if(fdata.attach)//附加图片加载
            {
              IMAGE img2;
              string data2;
              if(fd_ch==0)
                data2=filepath+"\\"+"images"+"\\"+fdata.image[0]+".png";
              else
                data2=filepath+"\\"+"images_aod"+"\\"+fdata.image[0]+".png";
              data2=replaceSlash(data2);

              loadimage(&img2, data2.c_str());
              fdata.image_data.push_back(img2);
            }
        }
      else if (fdata.type == "widge_pointer")
        {

          // 加载单张图片
          IMAGE img;
          string imgPath;
          if(fd_ch==0)
            imgPath=filepath+"\\"+"images"+"\\"+fdata.image[0]+".png";
          else
            imgPath=filepath+"\\"+"images_aod"+"\\"+fdata.image[0]+".png";
          imgPath=replaceSlash(imgPath);
          loadimage(&img, imgPath.c_str());
          fdata.image_data.push_back(img);

        }
      else if (fdata.type == "element_anim")
        {

          // 加载多张动画图片
          for (auto imgName : fdata.imageList)
            {
              IMAGE img;
              string data;
              if(fd_ch==0)
                data=filepath+"\\"+"images"+"\\"+imgName+".png";
              else
                data=filepath+"\\"+"images_aod"+"\\"+imgName+".png";
              data=replaceSlash(data);
              loadimage(&img, data.c_str());
              fdata.image_data.push_back(img);
            }

        }

    }

}



IMAGE draw_face(const string& filepath, vector<face_data>& faceDataList)
{

  // 先重新加载所有图片


  IMAGE result(336, 480);

  for (auto& data : faceDataList)
    {
      if (!data.image_data.empty())
        {
          if(R_C==0)
            data.R=0;
          // 访问data.image_data

          if (data.type == "element")
            {
              transparentimage3(&result, data.x, data.y, &data.image_data[0]);
            }
          else if (data.type == "widge_imagelist")
            {
              if(data.R_time > 500)
                {
                  if(data.R<data.image_data.size()-1)
                    data.R++;
                  else
                    data.R=0;
                  data.R_time = 0;
                }
              else
                {
                  // data.R_time还没大于5,直接增加
                  data.R_time++;
                }
              if(R_C==0)
                data.R=0;
              transparentimage3(&result, data.x, data.y, &data.image_data[data.R]);
            }
          else if (data.type == "widge_dignum")
            {
              if(data.type=="widge_dignum")//数字
                {
                  if(data.R_time > 100)
                    {
                      if(!data.attach)
                        {
                          if(data.R<data.image_data.size()-1)
                            data.R++;
                          else
                            data.R=0;
                          data.R_time = 0;
                        }
                      if(data.attach)
                        {
                          if(data.R<data.image_data.size()-2)
                            data.R++;
                          else
                            data.R=0;
                          data.R_time = 0;
                        }
                    }
                  else
                    {
                      // data.R_time还没大于5,直接增加
                      data.R_time++;
                    }
                  if(R_C==0)
                    data.R=0;
                  bool DEBOU=0;//小数点Rain.Cl专属部分
                  int showCount=data.showCount;
                  if(data.showCount==20)
                    {
                      DEBOU=1;
                      showCount=3;//睡眠小数点Rain.Cl专属部分
                    }
                    int XI=0;
                  for(int j=0; j<showCount; j++)
                    {
                      int mun=(data.imageList.size());
                      int R=data.R;
                      if(getRand(0,40)==5)
                        {
                          if(data.attach)
                            R=getRand(0,mun-2);
                          if(!data.attach)
                            R=getRand(0,mun-1);
                        }
                      if(j==0)
                        {
                          if(data.draw_data!="")
                            data.R=data.draw_data[0]- '0';
                          if(data.align==1)
                            {
                              transparentimage3(&result,data.x, data.y,&data.image_data[data.R]);
                            }
                          if(data.align==0&&DEBOU==0)
                            {
                              transparentimage3(&result,data.x-data.w, data.y,&data.image_data[data.R]);
                            }
                          if(data.align==2)
                            {
                              transparentimage3(&result,data.x-(data.w/2), data.y,&data.image_data[data.R]);
                            }
                          if(data.align==0&&DEBOU==1)//睡眠小数点Rain.Cl专属部分
                            {
                              transparentimage3(&result,data.x-(data.w/20), data.y,&data.image_data[data.R]);
                            }
                        }
                      else
                        {
                          if(data.draw_data!="")
                            {
                              if(j>=data.draw_data.length())
                                break;
                              data.R=data.draw_data[j]- '0';
                              if(data.draw_data[j]=='|'&&data.image_data.size()==11)//小数点
                                XI=showCount,showCount=3,data.R=10;
                            }
                          if(data.align==1)
                            {
                              transparentimage3(&result,data.x+data.image_data[data.R].getwidth()*j+data.spacing*j, data.y,&data.image_data[data.R]);
                            }
                          if(data.align==0&&DEBOU==0)
                            {
                              transparentimage3(&result,data.x+data.image_data[data.R].getwidth()*j+(data.spacing*j-data.w), data.y,&data.image_data[data.R]);
                            }
                          if(data.align==2)
                            {
                              transparentimage3(&result,data.x+data.image_data[data.R].getwidth()*j+(data.spacing*j-(data.w/2)), data.y,&data.image_data[data.R]);
                            }
                          if(data.align==0&&DEBOU==1)//睡眠小数点Rain.Cl专属部分
                            {
                              transparentimage3(&result,data.x-data.image_data[data.R].getwidth()*j+data.spacing*j-(data.w/20), data.y,&data.image_data[data.R]);
                            }
                        }
                      if((j+1)==showCount)//附加图片
                        {
                          if(data.attach)
                            {
                              if(data.align==1)
                                transparentimage3(&result,data.x + data.image_data[data.R].getwidth()*j+data.spacing*j + data.image_data[data.R].getwidth(), data.y,&data.image_data.back());
                              if(data.align==0&&DEBOU==0)
                                transparentimage3(&result,data.x + data.image_data[data.R].getwidth()*j+(data.spacing*j-data.w) + data.image_data[data.R].getwidth(), data.y,&data.image_data.back());
                              if(data.align==2)
                                transparentimage3(&result,data.x + data.image_data[data.R].getwidth()*j+(data.spacing*j-(data.w/2)) + data.image_data[data.R].getwidth(), data.y,&data.image_data.back());
                              if(data.align==0&&DEBOU==1)//睡眠小数点Rain.Cl专属部分
                                transparentimage3(&result,data.x - data.image_data[data.R].getwidth()*j+data.spacing*j-(data.w/20) + data.image_data[data.R].getwidth(), data.y,&data.image_data.back());

                            }
                        }
                    }
                    if(XI!=0)
                    {
                    	data.showCount=XI;
					}
                }
            }
          else if (data.type == "widge_pointer")
            {
              transparentimage3(&result, data.x, data.y, &data.image_data[0]);
              int oldlinecolor = getlinecolor();
              SetWorkingImage(&result);//渲染光标
              setlinecolor(RGB(243,102,56));
              line(data.x+data.imageRotateX-6, data.y+data.imageRotateY-6,data.x+data.imageRotateX+6, data.y+data.imageRotateY+6);
              line(data.x+data.imageRotateX+6, data.y+data.imageRotateY-6,data.x+data.imageRotateX-6, data.y+data.imageRotateY+6);
              SetWorkingImage(NULL);//渲染光标
              setlinecolor(oldlinecolor);
            }
          else if (data.type == "element_anim")
            {
              if(data.R_time > 50)
                {
                  if(data.R<data.image_data.size()-1)
                    data.R++;
                  else
                    data.R=0;
                  data.R_time = 0;
                }
              else
                {
                  // data.R_time还没大于5,直接增加
                  data.R_time++;
                }
              if(R_C==0)
                data.R=0;
              transparentimage3(&result, data.x, data.y, &data.image_data[data.anim_index]);
            }
        }
    }

  return result;

}

void cl_msg()
{
  flushmessage();
  msg.x=300;
  msg.y=100;
  msg.message=0;
}

string choose_image_data;
void choose_image(int N,vector<face_data>& data)//修改控件 加载图片
{
  if(biger_windows==1&&biger_ALL==0)
    {
      ShowWindow(hWnd, SW_RESTORE);
      SetWindowNormalWithBorder(hWnd);
    }
  choose_image_data=chooseFiles();
  if(choose_image_data=="")
    {
      return ;
    }
  string s2=".png";
  while (choose_image_data.find(s2) < choose_image_data.length())
    {
      choose_image_data.erase(choose_image_data.find(s2), s2.size());
    }
  vector<string> result = splitPath(choose_image_data);

  cout<<endl<<choose_image_data<<endl;

  choose_image_data="";
  cout<<endl<<"拆分:"<<endl;
  for(int i=0; i<result.size(); i++)
    {
      cout<<result[i]<<endl;
    }

  if(data[N].type=="element")
    {
      /*face_data M;
      M.type="element";
      M.x=0;
      M.y=0;
      M.image.push_back(result[0]);
      data.push_back(M);
      cout<<"\n创建 element";*/
      data[N].image[0]=result[0];
      string result_data;
      if(fd_ch==0)
        result_data=face_result+"\\"+"images"+"\\"+data[N].image[0]+".png";
      else
        result_data=face_result+"\\"+"images_aod"+"\\"+data[N].image[0]+".png";
      //string result_data=face_result+"\\"+"images"+"\\"+data[N].image[0]+".png";//重新计算大小
      result_data=replaceSlash(result_data);
      IMAGE img;
      loadimage(&img,result_data.c_str());
      data[N].w = img.getwidth();
      data[N].h = img.getheight();
    }
  if(data[N].type=="widge_pointer")
    {
      /*face_data M;
      M.type="element";
      M.x=0;
      M.y=0;
      M.image.push_back(result[0]);
      data.push_back(M);
      cout<<"\n创建 element";*/
      data[N].image[0]=result[0];

      string result_data;
      if(fd_ch==0)
        result_data=face_result+"\\"+"images"+"\\"+data[N].image[0]+".png";
      else
        result_data=face_result+"\\"+"images_aod"+"\\"+data[N].image[0]+".png";

      // string result_data=face_result+"\\"+"images"+"\\"+data[N].image[0]+".png";//重新计算大小
      result_data=replaceSlash(result_data);
      IMAGE img;
      loadimage(&img,result_data.c_str());
      data[N].w = img.getwidth();
      data[N].h = img.getheight();
    }
  if(data[N].type=="widge_imagelist"||data[N].type=="widge_dignum"||data[N].type=="element_anim")
    {
      /*face_data M;
      M.type="element";
      M.x=0;
      M.y=0;
      M.image.push_back(result[0]);
      data.push_back(M);
      cout<<"\n创建 element";*/
      if(data[N].imageList.size()==result.size())
        {
          cout<<"\n数量一样";
          for(int i=0; i<data[N].imageList.size(); i++)
            {

              data[N].imageList[i]=result[i];
            }
          if(data[N].type=="widge_imagelist"||data[N].type=="element_anim")
            {
              string result_data;
              if(fd_ch==0)
                result_data=face_result+"\\"+"images"+"\\"+data[N].imageList[0]+".png";
              else
                result_data=face_result+"\\"+"images_aod"+"\\"+data[N].imageList[0]+".png";
              // string result_data=face_result+"\\"+"images"+"\\"+data[N].imageList[0]+".png";//重新计算大小
              result_data=replaceSlash(result_data);
              IMAGE img;
              loadimage(&img,result_data.c_str());
              data[N].w = img.getwidth();
              data[N].h = img.getheight();
            }
          if(data[N].type=="widge_dignum")//指针
            {
              string result_data;
              if(fd_ch==0)
                result_data=face_result+"\\"+"images"+"\\"+data[N].imageList[0]+".png";
              else
                result_data=face_result+"\\"+"images_aod"+"\\"+data[N].imageList[0]+".png";
//             / string result_data=face_result+"\\"+"images"+"\\"+data[N].imageList[0]+".png";//重新计算大小
              result_data=replaceSlash(result_data);
              IMAGE img;
              loadimage(&img,result_data.c_str());
              data[N].w = (img.getwidth()*data[N].showCount)+(data[N].showCount*data[N].spacing);
              data[N].h = img.getheight();
            }
        }
      if(data[N].imageList.size()!=result.size())
        {
          cout<<"\n数量不一样";
          data[N].imageList.clear();

          for(int i=0; i<result.size(); i++)
            {
              data[N].imageList.push_back(result[i]);
            }
        }
      if(data[N].type=="widge_imagelist"||data[N].type=="element_anim")
        {
          string result_data;
          if(fd_ch==0)
            result_data=face_result+"\\"+"images"+"\\"+data[N].imageList[0]+".png";
          else
            result_data=face_result+"\\"+"images_aod"+"\\"+data[N].imageList[0]+".png";
          //string result_data=face_result+"\\"+"images"+"\\"+data[N].imageList[0]+".png";//重新计算大小
          result_data=replaceSlash(result_data);
          IMAGE img;
          loadimage(&img,result_data.c_str());
          data[N].w = img.getwidth();
          data[N].h = img.getheight();
        }
      if(data[N].type=="widge_dignum")
        {
          string result_data;
          if(fd_ch==0)
            result_data=face_result+"\\"+"images"+"\\"+data[N].imageList[0]+".png";
          else
            result_data=face_result+"\\"+"images_aod"+"\\"+data[N].imageList[0]+".png";
          //string result_data=face_result+"\\"+"images"+"\\"+data[N].imageList[0]+".png";//重新计算大小
          result_data=replaceSlash(result_data);
          IMAGE img;
          loadimage(&img,result_data.c_str());
          data[N].w = (img.getwidth()*data[N].showCount)+(data[N].showCount*data[N].spacing);
          data[N].h = img.getheight();

        }

    }
  flushmessage();
  reloadImageData(data, face_result);
  cl_msg();
}

void data_work(int N,vector<face_data>& data,ExMessage msg,int fx,int fy)//控件编辑操作
{
  if(N==0)
    {
      return ;
    }
  N=N-1;

  if(data[N].type=="element"&&data[N].lock==0)
    {
      if(FIND_Button_no_dui("widge_imagelist")==1||FIND_Button_no_dui("widge_dignum")==1||FIND_Button_no_dui("widge_pointer")==1)
        {
          DEL_Button_no_dui("widge_imagelist");
          DEL_Button_no_dui("widge_pointer");
          DEL_Button_no_dui("widge_dignum");
          BOX.draw_Text_box();
          cout<<"删除\n";
        }
      settextstyle(font_b+7, 0, _T("Segoe UI"));
      int oldlinecolor = getlinecolor();
      setlinecolor(RGB(243,102,56));
      RECT r = {X-300,0,X,40};
      drawtext(_T("图片控件"), &r,DT_CENTER);
      line(X-300,40,X,40);

      r = {X-290,68,X, 108};
      string _o="X:";
      _o+=to_string(data[N].x);
      _o+="  Y:";
      _o+=to_string(data[N].y);
      drawtext(_T(_o.c_str()), &r,DT_LEFT);

      r = {X-290,108,X, 148};
      _o="图层:";
      _o+=to_string(N);
      drawtext(_T(_o.c_str()), &r,DT_LEFT);

      if(FIND_Button_no_dui("element")==0)
        {
          NEW_Button_no_dui(X-240,43,X-57, 70, _T("选择图片(按住)"), "element",NULL);
          cout<<"创建\n";
        }
      if(X-230 <= msg.x && msg.x <= X-87 && 43 <= msg.y && msg.y <= 68&&IsMouseClickedInWindow(hWnd))
        {
          choose_image(N,data);
        }
      settextstyle(font_b, 0, _T("Segoe UI"));
      EasyInBox a;
      r = {X-290,148,X, 188};
      _o="jumpName:";
      EasyInBox OnchSRC;
      //_o+=to_string(data[N].showCount);
      drawtext(_T(_o.c_str()), &r,DT_LEFT);
      a.Create(X-200,148,X-20,178,data[N].jumpName);
      a.draw();
      if (GetAsyncKeyState(VK_LBUTTON) && 0x8000) // 左键按下
        {
          string M=a.OnClick(msg.x,msg.y);
          if(M!=data[N].jumpName)
            {
              data[N].jumpName= M;
            }
        }
      r = {X-290,188,X, 228};
      _o="jumpCode:";
      //_o+=to_string(data[N].showCount);
      drawtext(_T(_o.c_str()), &r,DT_LEFT);
      a.Create(X-200,188,X-20,218,data[N].jumpCode);
      a.draw();
      if (GetAsyncKeyState(VK_LBUTTON) && 0x8000) // 左键按下
        {
          string M=a.OnClick(msg.x,msg.y);
          if(M!=data[N].jumpCode)
            {
              data[N].jumpCode= M;
            }
        }
      r = {X-290,228,X, 268};
      _o="按下Ctrl键快速选择";
      drawtext(_T(_o.c_str()), &r,DT_LEFT);
      setlinecolor(oldlinecolor);
      settextstyle(font_b, 0, _T("Segoe UI"));
    }
  if(data[N].type=="widge_pointer"&&data[N].lock==0)
    {
      if(FIND_Button_no_dui("widge_imagelist")==1||FIND_Button_no_dui("widge_dignum")==1||FIND_Button_no_dui("element")==1)
        {
          DEL_Button_no_dui("widge_imagelist");
          DEL_Button_no_dui("widge_dignum");
          DEL_Button_no_dui("element");
          BOX.draw_Text_box();
          cout<<"删除\n";
        }
      settextstyle(font_b+7, 0, _T("Segoe UI"));
      int oldlinecolor = getlinecolor();
      setlinecolor(RGB(243,102,56));
      RECT r = {X-300,0,X,40};
      drawtext(_T("指针控件"), &r,DT_CENTER);
      line(X-300,40,X,40);

      r = {X-290,68,X, 108};
      string _o="X:";
      _o+=to_string(data[N].x);
      _o+="  Y:";
      _o+=to_string(data[N].y);
      drawtext(_T(_o.c_str()), &r,DT_LEFT);

      r = {X-290,108,X, 148};
      _o="图层:";
      _o+=to_string(N);
      drawtext(_T(_o.c_str()), &r,DT_LEFT);

      if(FIND_Button_no_dui("widge_pointer")==0)
        {
          NEW_Button_no_dui(X-240,43,X-57, 70, _T("选择图片(按住)"), "widge_pointer",NULL);
          cout<<"创建\n";
        }
      if(X-230 <= msg.x && msg.x <= X-87 && 43 <= msg.y && msg.y <= 68&&msg.message == WM_LBUTTONDOWN)
        {
          choose_image(N,data);
        }
      EasyInBox a;
      r = {X-290,148,X, 188};
      _o="最大取值:";
      EasyInBox OnchSRC;
      //_o+=to_string(data[N].showCount);
      drawtext(_T(_o.c_str()), &r,DT_LEFT);
      a.Create(X-160,148,X-50,178,to_string(data[N].maxValue));
      a.draw();
      if (GetAsyncKeyState(VK_LBUTTON) && 0x8000) // 左键按下
        {
          string M=a.OnClick(msg.x,msg.y);
          if(M!=to_string(data[N].maxValue))
            {
              int num = 0;
              std::istringstream ss(M);
              ss >> num,data[N].maxValue= num;
            }
        }
      r = {X-290,188,X, 228};
      _o="allangle";
      //_o+=to_string(data[N].showCount);
      drawtext(_T(_o.c_str()), &r,DT_LEFT);
      a.Create(X-160,188,X-50,218,to_string(data[N].allAngle));
      a.draw();
      if (GetAsyncKeyState(VK_LBUTTON) && 0x8000) // 左键按下
        {
          string M=a.OnClick(msg.x,msg.y);
          if(M!=to_string(data[N].allAngle))
            {
              int num = 0;
              std::istringstream ss(M);
              ss >> num,data[N].allAngle= num;
            }
        }
      r = {X-260,268,X,308};
      _o="UP,Dn键调整数据源";
      drawtext(_T(_o.c_str()), &r,DT_LEFT);
      drawtext(_T(_o.c_str()), &r,DT_LEFT);
      EasyCheckBox OnchSRC2;//选择数据源

      OnchSRC2.Create(X-295,268,X-275, 288);

      OnchSRC2.checked=data[N].dataSrc_choose;
      if (IsMouseClickedInWindow(hWnd))
        OnchSRC2.OnClick(msg.x,msg.y);

      OnchSRC2.Draw();
      data[N].dataSrc_choose=OnchSRC2.checked;

      r = {X-290,228,X, 268};
      _o="数据源:";
      _o+=dataSRC_out(data[N].dataSrc,"str");
      drawtext(_T(_o.c_str()), &r,DT_LEFT);

      r = {X-290,348,X, 388};
      _o="旋转中心:";
      _o+=to_string(data[N].imageRotateX);
      _o+="|";
      _o+=to_string(data[N].imageRotateY);
      drawtext(_T(_o.c_str()), &r,DT_LEFT);

      r = {X-290,308,X, 348};
      _o="调整指针中心";
      drawtext(_T(_o.c_str()), &r,DT_LEFT);
      OnchSRC2.checked=0;
      OnchSRC2.Create(X-290,308,X-110, 348);
      OnchSRC2.Draw();
      if (GetAsyncKeyState(VK_LBUTTON) && 0x8000)
        {
          int mx = 0;
          int my = 0;
          OnchSRC2.OnClick(msg.x,msg.y);
          if( OnchSRC2.checked==1)
            {
              Sleep(200);
              cout<<"\n选择中心点\n";
              fx=fx+data[N].x;
              fy=fy+data[N].y;
              bool wasd_t=0;//使用方向键
              while(1)
                {
                  cout<<"旋转中心:"<<mx<<"|"<<my<<endl;
                  msg = getmessage(EM_MOUSE|EM_KEY);			// 获取消息输入
                  int fw=data[N].w;
                  int fh=data[N].h;
                  int dx = msg.x - fx;
                  int dy = msg.y - fy;
                  // 将偏移量映射到原图片大小
                  float sx = (float)dx / fw * data[N].w;
                  float sy = (float)dy / fh * data[N].h;
                  if(wasd_t==0)
                    {
                      mx = (int)sx;
                      my = (int)sy;
                    }
                  string data_M=face_result+"\\"+"images"+"\\"+data[N].image[0]+".png";
                  data_M=replaceSlash(data_M);
                  IMAGE img;
                  loadimage(&img,data_M.c_str());
                  SetWorkingImage(&img);//渲染光标
                  int len = 15; // 十字线段长度
                  line(mx, my - len, mx, my + len);
                  line(mx - len, my, mx + len, my);
                  SetWorkingImage(NULL);
                  putimage(fx,fy, &img);
                  if (GetAsyncKeyState(2) && 0x8000)   // 右键键按
                    {
                      mx=5,
                      my=5;
                      wasd_t=!wasd_t;
                      Sleep(100);
                    }
                  if (GetAsyncKeyState(87) && 0x8000)   // 上方向键按
                    {
                      my--;
                      Sleep(10);
                    }
                  else if (GetAsyncKeyState(83) && 0x8000)     // 下方向键按
                    {
                      my++;
                      Sleep(10);
                    }
                  else if (GetAsyncKeyState(65) && 0x8000 )     // 左方向键被按下
                    {
                      mx--;
                      Sleep(10);
                    }
                  else if (GetAsyncKeyState(68) && 0x8000 )     // 右方向键被按下
                    {
                      mx++;
                      Sleep(10);
                    }
                  if(GetAsyncKeyState(VK_LBUTTON) && 0x8000)
                    {
                      Sleep(200);
                      break;
                    }
                  FlushBatchDraw();
                }
              data[N].imageRotateX=mx;
              data[N].imageRotateY=my;
            }
        }
      OnchSRC2.Draw();

      setlinecolor(oldlinecolor);
      settextstyle(font_b, 0, _T("Segoe UI"));
    }


  if((data[N].type=="widge_imagelist"||data[N].type=="element_anim")&&data[N].lock==0)
    {
      if(FIND_Button_no_dui("element")==1||FIND_Button_no_dui("widge_dignum")==1||FIND_Button_no_dui("widge_pointer")==1)
        {
          DEL_Button_no_dui("element");
          DEL_Button_no_dui("widge_dignum");
          DEL_Button_no_dui("widge_pointer");
          BOX.draw_Text_box();
          cout<<"删除\n";
        }
      settextstyle(font_b+7, 0, _T("Segoe UI"));
      int oldlinecolor = getlinecolor();
      setlinecolor(RGB(243,102,56));
      RECT r = {X-300,0,X,40};
      if(data[N].type=="widge_imagelist")
        drawtext(_T("图片列表控件"), &r,DT_CENTER);
      if(data[N].type=="element_anim")
        drawtext(_T("动态图片控件"), &r,DT_CENTER);
      line(X-300,40,X,40);

      r = {X-290,68,X, 108};
      string _o="X:";
      _o+=to_string(data[N].x);
      _o+="  Y:";
      _o+=to_string(data[N].y);
      drawtext(_T(_o.c_str()), &r,DT_LEFT);

      r = {X-290,108,X, 148};
      _o="图层:";
      _o+=to_string(N);
      drawtext(_T(_o.c_str()), &r,DT_LEFT);

      if(FIND_Button_no_dui("widge_imagelist")==0)
        {
          NEW_Button_no_dui(X-240,43,X-57, 70, _T("选择图片(按住)"), "widge_imagelist",NULL);
        }
      if(X-230 <= msg.x && msg.x <= X-87 && 43 <= msg.y && msg.y <= 68&&msg.message == WM_LBUTTONDOWN)
        {
          choose_image(N,data);
        }
      if(data[N].type=="element_anim")//动态图片-----------------------------------------
        {
          EasyInBox a;
          r = {X-290,148,X, 188};
          _o="动画间隔:";
          EasyInBox OnchSRC;
          //_o+=to_string(data[N].showCount);
          drawtext(_T(_o.c_str()), &r,DT_LEFT);
          a.Create(X-160,148,X-50,178,to_string(data[N].animInterval));
          a.draw();
          if (GetAsyncKeyState(VK_LBUTTON) && 0x8000) // 左键按下
            {
              string M=a.OnClick(msg.x,msg.y);
              if(M!=to_string(data[N].animInterval))
                {
                  int num = 0;
                  std::istringstream ss(M);
                  ss >> num,data[N].animInterval= num;
                }
            }
          r = {X-290,188,X, 228};
          _o="重复次数";
          //_o+=to_string(data[N].showCount);
          drawtext(_T(_o.c_str()), &r,DT_LEFT);
          a.Create(X-160,188,X-50,218,to_string(data[N].animRepeat));
          a.draw();
          if (GetAsyncKeyState(VK_LBUTTON) && 0x8000) // 左键按下
            {
              string M=a.OnClick(msg.x,msg.y);
              if(M!=to_string(data[N].animRepeat))
                {
                  int num = 0;
                  std::istringstream ss(M);
                  ss >> num,data[N].animRepeat= num;
                }
            }
        }
      //动态图片-----------------------------------------
      if(data[N].type=="widge_imagelist")//图片列表-----------------------------------------
        {

          r = {X-290,148,X, 188};
          _o="数据源:";
          _o+=dataSRC_out(data[N].dataSrc,"str");
          drawtext(_T(_o.c_str()), &r,DT_LEFT);

          r = {X-260,188,X, 228};
          _o="UP,Dn键调整数据源";
          drawtext(_T(_o.c_str()), &r,DT_LEFT);
          EasyCheckBox OnchSRC;//选择数据源

          OnchSRC.Create(X-295,193,X-275, 213);

          OnchSRC.checked=data[N].dataSrc_choose;
          if (IsMouseClickedInWindow(hWnd))
            OnchSRC.OnClick(msg.x,msg.y);

          OnchSRC.Draw();
          data[N].dataSrc_choose=OnchSRC.checked;

          r = {X-290,228,X, 268};
          _o="数据索引";
          drawtext(_T(_o.c_str()), &r,DT_LEFT);
          EasyInBox a;
          a.Create(X-290,268,X-50,520,to_string(data[N].showCount));
          a.data=joinIntsToString(data[N].imageIndexList);
          a.draw();
          if (GetAsyncKeyState(VK_LBUTTON) && 0x8000) // 左键按下
            {
              a.OnClick(msg.x,msg.y);
              if(a.data!="")
                {
                  data[N].imageIndexList=parseString(a.data);
                }
            }
        }//图片列表-----------------------------------------

      setlinecolor(oldlinecolor);
      settextstyle(font_b, 0, _T("Segoe UI"));
    }


  if(data[N].type=="widge_dignum"&&data[N].lock==0)
    {
      if(FIND_Button_no_dui("element")==1||FIND_Button_no_dui("widge_imagelist")==1||FIND_Button_no_dui("widge_pointer")==1)
        {
          DEL_Button_no_dui("element");
          DEL_Button_no_dui("widge_imagelist");
          DEL_Button_no_dui("widge_pointer");
          BOX.draw_Text_box();
          cout<<"删除\n";
        }
      settextstyle(font_b+7, 0, _T("Segoe UI"));
      int oldlinecolor = getlinecolor();
      setlinecolor(RGB(243,102,56));
      RECT r = {X-300,0,X,40};
      drawtext(_T("数字显示控件"), &r,DT_CENTER);
      line(X-300,40,X,40);

      r = {X-290,68,X, 108};
      string _o="X:";
      _o+=to_string(data[N].x);
      _o+="  Y:";
      _o+=to_string(data[N].y);
      drawtext(_T(_o.c_str()), &r,DT_LEFT);

      r = {X-290,108,X, 148};
      _o="图层:";
      _o+=to_string(N);
      drawtext(_T(_o.c_str()), &r,DT_LEFT);

      if(FIND_Button_no_dui("widge_dignum")==0)
        {
          NEW_Button_no_dui(X-240,43,X-57, 70, _T("选择图片(按住)"), "widge_dignum",NULL);
          cout<<"创建\n";
        }
      if(X-230 <= msg.x && msg.x <= X-87 && 43 <= msg.y && msg.y <= 68&&msg.message == WM_LBUTTONDOWN)
        {
          choose_image(N,data);
        }

      r = {X-290,148,X, 188};
      _o="数据源:";
      _o+=dataSRC_out(data[N].dataSrc,"str");
      drawtext(_T(_o.c_str()), &r,DT_LEFT);

      r = {X-260,188,X, 228};
      _o="UP,Dn键调整数据源";
      drawtext(_T(_o.c_str()), &r,DT_LEFT);
      EasyCheckBox OnchSRC;//选择数据源

      OnchSRC.Create(X-295,193,X-275, 213);

      OnchSRC.checked=data[N].dataSrc_choose;
      if (IsMouseClickedInWindow(hWnd))
        OnchSRC.OnClick(msg.x,msg.y);

      OnchSRC.Draw();
      data[N].dataSrc_choose=OnchSRC.checked;

      r = {X-290,228,X, 268};
      _o="显示的数量:";
      OnchSRC.checked=0;
      //_o+=to_string(data[N].showCount);
      drawtext(_T(_o.c_str()), &r,DT_LEFT);

      EasyInBox a;
      a.Create(X-160,228,X-50,258,to_string(data[N].showCount));
      a.draw();
      if (GetAsyncKeyState(VK_LBUTTON) && 0x8000) // 左键按下
        {
          //cout<<"按下左键";
          a.OnClick(msg.x,msg.y);
        }
      if(data[N].showCount!=stoi(a.data))
        {
          data[N].showCount=stoi(a.data);
          string result_data=face_result+"\\"+"images"+"\\"+data[N].imageList[0]+".png";//重新计算大小
          result_data=replaceSlash(result_data);
          IMAGE img;
          loadimage(&img,result_data.c_str());
          data[N].w = (img.getwidth()*data[N].showCount)+(data[N].showCount*data[N].spacing);
          data[N].h = img.getheight();
        }

      r = {X-290,268,X, 308};
      _o="对齐方式:";
      if(data[N].align==0)
        _o+="右对齐";
      if(data[N].align==1)
        _o+="左对齐";
      if(data[N].align==2)
        _o+="居中";
      drawtext(_T(_o.c_str()), &r,DT_LEFT);
      OnchSRC.Create(X-70,278,X-50, 298);
      if (IsMouseClickedInWindow(hWnd))
        OnchSRC.OnClick(msg.x,msg.y);

      OnchSRC.Draw();
      if(OnchSRC.checked==1)
        {
          Sleep(50);
          switch(data[N].align)
            {
              case 0:
                data[N].align=1;
                break;
              case 1:
                data[N].align=2;
                break;
              case 2:
                data[N].align=0;
                break;
            }
        }

      r = {X-290,308,X, 348};
      _o="是否前面显示0:";
      if(data[N].showZero=="false")
        _o+="否";
      if(data[N].showZero=="true")
        _o+="是";
      drawtext(_T(_o.c_str()), &r,DT_LEFT);
      OnchSRC.Create(X-70,318,X-50, 338);
      OnchSRC.checked=0;
      if (IsMouseClickedInWindow(hWnd))
        OnchSRC.OnClick(msg.x,msg.y);
      OnchSRC.Draw();
      if(OnchSRC.checked==1)
        {
          int n=0;
          if(data[N].showZero=="true")
            n=1;
          if(data[N].showZero=="false")
            n=0;
          switch(n)
            {
              case 0:
                data[N].showZero="true";
                break;
              case 1:
                data[N].showZero="false";
                break;
            }
        }

      r = {X-290,348,X, 388};
      _o="数字间隔:";
      _o+=to_string(data[N].spacing);
      drawtext(_T(_o.c_str()), &r,DT_LEFT);
      OnchSRC.Create(X-70,358,X-50, 378);
      OnchSRC.checked=0;
      if (IsMouseClickedInWindow(hWnd))
        OnchSRC.OnClick(msg.x,msg.y);
      OnchSRC.Draw();
      if(OnchSRC.checked==1)
        {
          data[N].spacing++;
        }
      OnchSRC.Create(X-70,378,X-50, 398);
      OnchSRC.checked=0;
      if (IsMouseClickedInWindow(hWnd))
        OnchSRC.OnClick(msg.x,msg.y);
      OnchSRC.Draw();
      if(OnchSRC.checked==1)
        {
          data[N].spacing--;
        }

      r = {X-290,388,X, 428};
      _o="附加图片";
      if(data[N].attach==1)
        _o+="(启动)";
      if(data[N].attach==0)
        _o+="(未启动)";
      drawtext(_T(_o.c_str()), &r,DT_LEFT);
      OnchSRC.Create(X-290,428,X-220, 458);
      OnchSRC.checked=0;
      if (IsMouseClickedInWindow(hWnd))
        OnchSRC.OnClick(msg.x,msg.y);
      OnchSRC.Draw();
      if(OnchSRC.checked==1)
        {
          string choose_data=chooseFiles();
          if(!(choose_data==""))
            {
              data[N].attach=1;

              string s2=".png";
              while (choose_data.find(s2) < choose_data.length())
                {
                  choose_data.erase(choose_data.find(s2), s2.size());
                }
              vector<string> result = splitPath(choose_data);

              cout<<endl<<choose_data<<endl;
              choose_image_data="";
              cout<<endl<<"拆分:"<<endl;
              for(int i=0; i<result.size(); i++)
                {
                  cout<<result[i]<<endl;
                }
              if(data[N].image.empty())
                data[N].image.push_back(result[0]);
              if(!data[N].image.empty())
                data[N].image[0]=(result[0]);

              reload=1;
            }
        }
      outtextxy(X-285,423, _T("选择"));

      setlinecolor(oldlinecolor);
      settextstyle(font_b, 0, _T("Segoe UI"));
    }
}
int see_box_X=0,see_box_Y=0;//坐标
int box_C=0;//页数
int old_H=0;//旧高
float CO_J=0;//拖动动画进度
int CO_S=0;//拖动动画延迟启动

void face_data_see_box(int N,vector<face_data>& data,ExMessage msg,int &fd_N,int C_N)//组件管理器 C_N但前选择页面:1快速选择跳转
{
  if(N==0)
    {
      return ;
    }
  N=N-1;
  int oldlinecolor = getlinecolor();
  setlinecolor(RGB(221,231,240));//RGB(221,231,240)

  rectangle(0,0,200,Y);
  settextstyle(font_b-5, 0, _T("Segoe UI"));

  if(C_N==1)//选择跳转
    {
      for(int i=0; sizeof(_jumpName) / sizeof(_jumpName[0])>i; i++)
        {
          string _o=_jumpNameSTR[i];
          if(data[N].jumpCode==_jumpCode[i])
            {
              rectangle(0,20*i+26,200,20*i+26+20);
            }
          outtextxy(1,20*i+26,_T(_o.c_str()));
          _o+=":";
          _o+=_jumpName[i];
          outtextxy(1,20*i+26,_T(_o.c_str()));
        }
      if (GetAsyncKeyState(4) & 0x8000) //重新选中
        {
          if(msg.x >= 0 && msg.x <= 200 && msg.y >= 26 && msg.y <= Y)
            {
              int m=(msg.y-26)/20;
              data[N].jumpName=_jumpName[m];
              data[N].jumpCode=_jumpCode[m];
            }
          flushmessage();
        }
      settextstyle(font_b, 0, _T("Segoe UI"));
      return ;
    }

  int H=(Y-60)/data.size();//自适应间隔
  if(H>font_b)
    H=font_b;
  settextstyle(H, 0, _T("Segoe UI"));

  if(data[N].dataSrc_choose==1)
    {
      H=(Y-60)/40;//自适应间隔
      if(H>font_b)
        H=font_b;
      settextstyle(H, 0, _T("Segoe UI"));
      for(int i=0; sizeof(dataSRC) / sizeof(dataSRC[0])>i; i++)
        {
          string _o=SRC_str[i];

          if(dataSRC_out(data[N].dataSrc,"str")==SRC_str[i])
            {
              rectangle(0,H*i+26,200,H*i+26+H);
            }
          //_o+=dataSRC_out(data[i].dataSrc,"str");
          outtextxy(1,H*i+26,_T(_o.c_str()));
        }

      if (GetAsyncKeyState(4) & 0x8000) //重新选中
        {
          if(msg.x >= 0 && msg.x <= 200 && msg.y >= 26 && msg.y <= Y)
            {
              int m=(msg.y-26)/H;
              data[N].dataSrc=dataSRC[m];
            }
          flushmessage();
        }
      settextstyle(font_b, 0, _T("Segoe UI"));
      setlinecolor(oldlinecolor);
      return ;
    }


  for(int i=box_C; i<data.size(); i++)
    {
      string _o;
      if(data[i].type=="element")
        {
          _o="图片控件:";
        }
      if(data[i].type=="widge_dignum")
        {
          _o="数字控件:";
        }
      if(data[i].type=="widge_imagelist")
        {
          _o="图片列表控件:";
        }
      if(data[i].type=="widge_pointer")
        {
          _o="指针控件:";
        }
      if(data[i].type=="element_anim")
        {
          _o="动图控件:";
        }
      if(N==i)
        {
          if((GetAsyncKeyState(4) & 0x8000))//动画
            {
              if(CO_S>40)
                {
                  if(CO_J==0) CO_J=1;
                  string co=_o+dataSRC_out(data[i].dataSrc,"str");
                  rectangle(CO_J,msg.y-(H/2),CO_J+210,msg.y+H-(H/2));
                  outtextxy(CO_J,msg.y-(H/2),_T(co.c_str()));
                  if(CO_J<200)CO_J=CO_J+CO_J*1.06;
                  if(CO_J>201)CO_J=201;
                  setlinecolor(RGB(255,201,14));
                  rectangle(0,((msg.y-26)/H)*H+26,400,((msg.y-26)/H)*H+26+H);
                  setlinecolor(RGB(221,231,240));
                }
              else
                CO_S++;
            }
          else
            {
              if(CO_S>40&&CO_J>10&&((msg.y-26)/H)<data.size())
                {
                  cout<<"拖动\n";
                  auto value = *(data.begin() + (fd_N-1)); // 保存元素的值
                  data.erase(data.begin() + (fd_N-1));
                  fd_N=(msg.y-26)/H+1;
                  data.insert(data.begin() + (fd_N-1), value);
                  cout<<"fd_N o:"<<fd_N-1;
                  reloadImageData(data, face_result);
                }
              CO_J=0;
              CO_S=0;
            }
          setlinecolor(RGB(243,102,56));
          rectangle(0,H*i+26,CO_S*4,H*i+26+H);
          rectangle(0,H*i+26,200,H*i+26+H);
          setlinecolor(RGB(221,231,240));
        }
      _o+=dataSRC_out(data[i].dataSrc,"str");
      outtextxy(1,H*i+26,_T(_o.c_str()));
      if(old_H!=H)
        {
          cout<<"重新加载PNG,H不一样\n";
          double ZOOM=getScale(H,H,28,28);
          ZOOM=ZOOM*1.1;
          cout<<"缩放比:"<<ZOOM;
          ZoomImage(&NO_LOCK_draw,&NO_LOCK_M,ZOOM,0);
          ZoomImage(&LOCK_draw,&LOCK_M,ZOOM,0);

          ZoomImage(&Error_draw,&Error_M,ZOOM,0);
          ZoomImage(&Warn_draw,&Warn_M,ZOOM,0);
          old_H=H;
          // loadimage(&NO_LOCK_M, _T("data//PNG//解锁.png")),H,H);
        }
      if(data[i].lock==0)
        putimage(200-H,H*i+26,&NO_LOCK_draw);
      if(data[i].lock==1)
        putimage(200-H,H*i+26,&LOCK_draw);
      if(data[i].error_warn==2)
        putimage(200-H+H+3,H*i+26,&Warn_draw);
      if(data[i].error_warn==1)
        putimage(200-H+H+3,H*i+26,&Error_draw);
    }
  if (GetAsyncKeyState(4) & 0x8000) //重新选中 + 锁定模式
    {
      if(msg.x >= 0 && msg.x <= 200-H && msg.y >= 26 && msg.y <= Y&&CO_S<40)
        {
          int m=(msg.y-26)/H;
          if((m+1)<=data.size())
            {
              fd_N=m+1;
            }
        }
      if(msg.x >= 200-H && msg.x <= 200 && msg.y >= 26 && msg.y <= Y&&CO_S<40)
        {
          int m=(msg.y-26)/H;
          data[m].lock=!data[m].lock;
          if( data[m].lock==0)
            putimage(200,H*m+26,&NO_LOCK_draw);
          if( data[m].lock==1)
            putimage(200,H*m+26,&LOCK_draw);
          FlushBatchDraw();
          while((GetAsyncKeyState(4) & 0x8000))
            {
            }
        }
      flushmessage();
    }
  settextstyle(font_b, 0, _T("Segoe UI"));
  setlinecolor(oldlinecolor);
}

void SET_UI()//设置
{
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
  string _o="设置 (ESC退出)";
  outtextxy(left+5,top,_T(_o.c_str()));
  setlinecolor(RGB(243,102,56));
  line(left, top+35, left+500, top+35);
  _o="选项:";
  outtextxy(left+5,top+40,_T(_o.c_str()));
  ExMessage msg;

  EasyCheckBox _R_C;//表盘预览跳动
  EasyCheckBox _biger_ALL;//最大化是否覆盖任务栏
  EasyCheckBox _face_name;//表盘名称
  _R_C.Create(left+5,top+75,left+25,top+75+25); //+35
  _biger_ALL.Create(left+5,top+110,left+25,top+110+25); //+35
  _face_name.Create(left+5,top+145,left+25,top+145+25); //+35

  settextstyle(font_b, 0, _T("Segoe UI"));
  _o="表盘预览跳动(切换)";
  outtextxy(left+5+30,top+75,_T(_o.c_str()));
  _o="最大化是否覆盖任务栏(切换)";
  outtextxy(left+5+30,top+110,_T(_o.c_str()));
  _o="表盘名称: ";
  _o+=face_name;
  outtextxy(left+5+30,top+145,_T(_o.c_str()));

  _R_C.checked_show=R_C;//状态显示
  _biger_ALL.checked_show=biger_ALL;//状态显示

  while(1)
    {
      msg = getmessage(EM_MOUSE|EM_KEY);			// 获取消息输入
      if (GetAsyncKeyState(VK_ESCAPE) && 0x8000)
        {
          break;
        }
      if (msg.message == WM_LBUTTONDOWN)
        {
          _R_C.OnClick(msg.x,msg.y);
          _biger_ALL.OnClick(msg.x,msg.y);
          _face_name.OnClick(msg.x,msg.y);
        }
      _R_C.Draw();
      _biger_ALL.Draw();
      _face_name.Draw();
      if(_R_C.checked)
        {
          R_C=!R_C;
          break;
        }
      if(_biger_ALL.checked)
        {
          biger_ALL=! biger_ALL;
          break;
        }
      if(_face_name.checked)
        {
          face_name=ShowInputBox();
          cout<<"face_name:"<<face_name<<endl;
          break;
        }
      FlushBatchDraw();
    }
  Sleep(100);
}

void draw_face_name()//显示表盘名字
{
  settextstyle(font_b-6, 0, _T("Segoe UI"));//字体
  RECT r = {X-290,Y-24,X,Y};
  string _o;
  _o+="项目名:";
  _o+=face_name;
  _o+=" FPS:";
  _o+=to_string(FPS);
  _o+=" V:";
  _o+="DEV 0.9.2 10/28";
  outtextxy(X-298,Y-70,_T(_o.c_str()));
}

int IncludeChinese(const std::string& str)
{

  for(char c : str)
    {
      if((c & 0x80) && (c & 0x80))
        {
          return 1;
        }
    }

  return 0;
}
void SetColor(int color)
{
  HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
  SetConsoleTextAttribute(hConsole, color);
}

int examine_face(vector<face_data>& data)//检查表盘
{
  if(examine_face_T==0)
    return 0;

  int warn=0;
  int error=0;
  cerr<<"\n>=== examine_face 模块已启动====================< \n\n";
  cerr<<"face_name:"<<face_name<<endl;
  if(IncludeChinese(face_name)) SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY),cerr<<"Error:表盘名不能为中文"<<endl,SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED),error++;
  ifstream fin(replaceSlash(face_result+"\\"+"images"+"\\"+"preview.png"));
  if(! fin)cout<<"报错:"<<replaceSlash(face_result+"\\"+"images"+"\\"+"preview.png"),SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY),cerr<<"Error:没有表盘预览图"<<endl,SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED),error++;
//replaceSlash
  for(int i=0; i<data.size(); i++)
    {
      SetColor(FOREGROUND_GREEN);
      cerr<<"=====================================<";
      cerr<<" ["<<i<<"]\n";
      SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
      cerr<<"类型:"<<data[i].type<<endl;
      cerr<<"X:"<<data[i].x<<" Y:"<<data[i].y<<endl;
      cerr<<"W:"<<data[i].w<<" H:"<<data[i].h<<endl;
      if(data[i].w==0&&data[i].h==0)SetColor( FOREGROUND_RED | FOREGROUND_GREEN ),cerr<<"Warn:控件长宽为0"<<endl,SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED),warn++,data[i].error_warn=2;
      if((data[i].w+data[i].x)>336&&(data[i].h+data[i].y)>480)SetColor( FOREGROUND_RED | FOREGROUND_GREEN ),cerr<<"Warn:控件体积超出表盘"<<endl,SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED),warn++,data[i].error_warn=2;
      //检查图片
      if(data[i].type=="element"||data[i].type=="widge_pointer")
        {
          cerr<<"图片:"<<data[i].image[0]<<" | ";
          if(IncludeChinese(data[i].image[0]))SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY),cerr<<"=>Error:图片名不能为中文",SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED),error++,data[i].error_warn=1;
          cerr<<endl;
        }
      if(data[i].type=="element_anim"&&data[i].animInterval==0)
        {
          cerr<<"动态图片:"<<data[i].imageList[0]<<" | ";
          SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY),cerr<<"=>Error:动画间隔不能为0",SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED),error++,data[i].error_warn=1;
        }

      bool Consistent_size=0;//大小是否一样
      int pw=0,ph=0;//图片小
      if(data[i].type=="widge_dignum"||data[i].type=="widge_imagelist"||data[i].type=="element_anim")
        {
          if(data[i].imageList.size()!=data[i].image_data.size())
            {
              SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY),cerr<<"=>Error:内存中的图片和图片列表不一致\n",SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED),error++,data[i].error_warn=1;
              SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
              cerr<<"<<<严重错误 编译终止\n\n";
              SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
              MessageBox(NULL,"打包失败 >严重错误< 请检查表盘!!!","确定",MB_OK|MB_ICONEXCLAMATION | MB_SETFOREGROUND);
              return 1;
            }
          cerr<<"图片: ";
          for(int j=0; j<data[i].imageList.size(); j++)
            {
              SetColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_INTENSITY);
              cerr<<data[i].imageList[j];
              if(j==0)//记录第一张图片大小
                {
                  pw=data[i].image_data[0].getwidth();
                  ph=data[i].image_data[0].getheight();
                }
              else
                {
                  if(pw!=data[i].image_data[j].getwidth()||ph!=data[i].image_data[j].getheight())
                    {
                      SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY),cerr<<"=>Error:图片大小不一致",SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED),error++,data[i].error_warn=1;
                    }
                }
              if(IncludeChinese(data[i].imageList[j]))SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY),cerr<<"=>Error:图片名不能为中文",SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED),error++,data[i].error_warn=1;
              SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
              cerr<<"|";
            }
          cerr<<endl;
          SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
        }
    }
  SetColor( FOREGROUND_RED | FOREGROUND_GREEN );
  cerr<<"\n一共"<<warn<<"个警告";
  SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
  cerr<<" | ";
  SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
  cerr<<error<<"个错误\n\n";
  SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
  if(error!=0)
    {
      SetColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
      cerr<<"严重错误 编译终止\n\n";
      SetColor(FOREGROUND_BLUE | FOREGROUND_GREEN | FOREGROUND_RED);
      MessageBox(NULL,"打包失败 >严重错误< 请检查表盘!!!","确定",MB_OK|MB_ICONEXCLAMATION | MB_SETFOREGROUND);
      return 1;
    }
  if(warn!=0)
    {
      return 2;//仍然有错误 ->警告
    }
  return 0;
}
class auxiliary_data_class
{
  public:
    int x1=0;
    int y1=0;
    int x2=0;
    int y2=0;
    int N;
};

vector<auxiliary_data_class>auxiliary_data;//用于记录辅助显示数据
vector<int>auxiliary_n;//用于记录辅助显示的控件
void auxiliary_make(int fx,int fy,int fw,int fh,int W,int H,string Model,vector<face_data>& data,int fd_N,IMAGE *I)//显示辅助线 336, 480 渲染位置 渲染大小 表盘世界大小 手环类型 选中的控件数据
{
  if(fd_N==0)
    return ;

  int i=fd_N-1;
  int oldlinecolor = getlinecolor();
  setlinecolor(LIGHTGRAY);			// 设置画线颜色
  line(fx+fw+7,fy,fx+fw+17,fy);
  line(fx+fw+7,fy+fh/2,fx+fw+17,fy+fh/2);
  line(fx+fw+17,fy,fx+fw+17,fy+fh);
  line(fx+fw+7,fy+fh,fx+fw+17,fy+fh);

  string _o=to_string(H);
  outtextxy(fx+fw+22,(fy+fh/2)-12,_T(_o.c_str()));

  line(fx,fy+fh+7,fx,fy+fh+17);
  line(fx+fw,fy+fh+7,fx+fw,fy+fh+17);
  line(fx,fy+fh+17,fx+fw,fy+fh+17);
  line(fx+fw/2,fy+fh+7,fx+fw/2,fy+fh+17);

  _o=to_string(W);
  outtextxy((fx+fw/2)-14,fy+fh+18,_T(_o.c_str()));

  outtextxy(fx,fy-20,_T(Model.c_str()));

  SetWorkingImage(I);
  for(int j=0; j<auxiliary_n.size(); j++)
    {
      i=auxiliary_n[j];
      setlinecolor(RGB(255,201,14));			// 设置画线颜色
      line(0,data[i].y-1,W,data[i].y-1);
      line(data[i].x-1,0,data[i].x-1,H);
      line(0,data[i].y+data[i].h+1,W,data[i].y+data[i].h+1);
      line(data[i].x+data[i].w+1,0,data[i].x+data[i].w+1,H);
    }
  setlinecolor(WHITE);
  for(int j=0; j<auxiliary_data.size(); j++)
    {
      int sx=auxiliary_data[j].x1;
      int sy=auxiliary_data[j].y1;
      if(abs(sx-auxiliary_data[j].x2)>abs(sy-auxiliary_data[j].y2))
        {
          line(sx,sy,auxiliary_data[j].x2,sy);
          line(sx,sy-5,sx,sy+5);
          line(auxiliary_data[j].x2,sy-5,auxiliary_data[j].x2,sy+5);
          string _o=to_string(abs(sx-auxiliary_data[j].x2));
          if((auxiliary_data[j].x2-sx)>0)
            outtextxy(sx+(auxiliary_data[j].x2-sx)/2,sy-15,_T(_o.c_str()));
          if((sx-auxiliary_data[j].x2)>0)
            outtextxy(sx-(sx-auxiliary_data[j].x2)/2,sy-15,_T(_o.c_str()));
        }
      if(abs(sx-auxiliary_data[j].x2)<abs(sy-auxiliary_data[j].y2))
        {
          line(sx,sy,sx,auxiliary_data[j].y2);
          line(sx-5,sy,sx+5,sy);
          line(sx-5,auxiliary_data[j].y2,sx+5,auxiliary_data[j].y2);
          string _o=to_string(abs(sy-auxiliary_data[j].y2));
          if((auxiliary_data[j].y2-sy)>0)
            outtextxy(sx,sy-15+(auxiliary_data[j].y2-sy)/2,_T(_o.c_str()));
          if((sy-auxiliary_data[j].y2)>0)
            outtextxy(sx,sy-15-(sy-auxiliary_data[j].y2)/2,_T(_o.c_str()));
        }
    }
  SetWorkingImage(NULL);

  setlinecolor(oldlinecolor);
}


