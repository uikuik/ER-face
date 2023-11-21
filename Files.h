/*
TCHAR*转char*
*/
#include <codecvt>
#include <locale>
#include <shlobj.h>


char* chooseFiles()
{

  OPENFILENAME ofn;
  TCHAR szOpenFileNames[80 * MAX_PATH] = { 0 };
  TCHAR szPath[MAX_PATH];
  TCHAR szFileName[80 * MAX_PATH];

  int nLen = 0;
  TCHAR* p = NULL;
  ZeroMemory(&ofn, sizeof(ofn));

  // 结构体大小
  ofn.lStructSize = sizeof(ofn);

  // 拥有着窗口句柄
  ofn.hwndOwner = NULL;

  // 接收返回的文件名，注意第一个字符需要为NULL
  ofn.lpstrFile = szOpenFileNames;

  // 缓冲区长度
  ofn.nMaxFile = sizeof(szOpenFileNames);

  // _T可替换为TEXT，使用_T需要引tchar.h
  ofn.lpstrFile[0] = _T('\0');

  // 设置过滤
  ofn.lpstrFilter = _T("All\0*.*\0.json\0");

  // 过滤器索引
  ofn.nFilterIndex = 1;

  // 窗口标题
  ofn.lpstrTitle = _T("请选择");

  // 文件必须存在、允许多选、隐藏只读选项、对话框使用资源管理器风格的用户界面
  // 官方文档：https://docs.microsoft.com/en-us/windows/win32/api/commdlg/ns-commdlg-openfilenamea
  ofn.Flags = OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_HIDEREADONLY | OFN_EXPLORER;

  // 如果打开文件失败，则不操作
  if (!::GetOpenFileName(&ofn))
    {
      return new char[0];
    }

  // 把第一个文件名前的复制到szPath,即:
  // 如果只选了一个文件,就复制到最后一个'/'
  // 如果选了多个文件,就复制到第一个NULL字符
  lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset);

  // 当只选了一个文件时,下面这个NULL字符是必需的.
  // 这里不区别对待选了一个和多个文件的情况
  szPath[ofn.nFileOffset] = '\0';
  nLen = lstrlen(szPath);

  // 如果选了多个文件,则必须加上'//'
  if (szPath[nLen - 1] != '\\')
    {
      lstrcat(szPath, _T("\\"));
    }

  // 把指针移到第一个文件
  p = szOpenFileNames + ofn.nFileOffset;

  // 对szFileName进行清零
  ZeroMemory(szFileName, sizeof(szFileName));

  // 定义字符串，用于拼接所选的所有文件的完整路径
  string str = "";

  while (*p)
    {
      // 读取文件名
      char* str2 = _TEXT(p);
      string fileName = str2;

      // 读取文件所在文件夹路径

      string filePath = _TEXT(szPath);

      // 拼接文件完整路径
      string completePath = filePath + fileName;

      // 拼接字符串
      str += completePath + "|";

      //移至下一个文件
      p += lstrlen(p) + 1;
    }

  // 将string转为char*
  char* strc = new char[strlen(str.c_str()) + 1];
  const char* cc = str.c_str();
  strcpy_s(strc, str.size() + 1, cc);

  cout<<"strc:"<<strc<<" "<<endl;

  return strc;
}

char* SelectFolder()
{

  TCHAR szFolder[MAX_PATH];
  BROWSEINFO bi = {0};

  bi.hwndOwner = GetDesktopWindow(); // 设置拥有者为桌面窗口
  bi.lpszTitle = _T("请选择文件夹");
  LPITEMIDLIST pidl = SHBrowseForFolder(&bi);

  if(pidl)
    {
      // 获取选择文件夹路径
      SHGetPathFromIDList(pidl, szFolder);

      // 释放内存
      IMalloc* imalloc = 0;
      if (SUCCEEDED(SHGetMalloc(&imalloc)))
        {
          imalloc->Free(pidl);
          imalloc->Release();
        }

      // 将对话框窗口置顶
      HWND hWnd = GetParent(bi.hwndOwner);
      SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
    }

  return _strdup(szFolder);
}
vector<string> splitPath(string path)
{
  vector<string> paths;

  // 先用|拆分整个路径
  stringstream ss(path);
  string temp;
  while(getline(ss, temp, '|'))
    {
      paths.push_back(temp);
    }

  // 再对每个路径使用\进行拆分
  for(string& p : paths)
    {
      size_t pos = p.find_last_of("\\");
      p = p.substr(pos+1);
    }

  return paths;
}

void LoadConfig(const char* configPath, map<string, int*>* configVars)//读取配置文件
{

  // 打开配置文件
  ifstream fin(configPath);
  if (!fin) return;

  // 读取每行
  string line;
  while (getline(fin, line))
    {

      // 分割键值对
      int pos = line.find(":");
      string key = line.substr(0, pos);
      int value = atoi(line.substr(pos+1).c_str());

      // 查找变量并赋值
      if (configVars->count(key) > 0)
        {
          *(configVars->at(key)) = value;
        }
    }

  // 关闭文件
  fin.close();

}
wstring UTF8ToUnicode(const string& str)
{
  int len = 0;
  len = str.length();
  int unicodeLen = ::MultiByteToWideChar(CP_UTF8,
                                         0,
                                         str.c_str(),
                                         -1,
                                         NULL,
                                         0);
  wchar_t * pUnicode;
  pUnicode = new wchar_t[unicodeLen + 1];
  memset(pUnicode, 0, (unicodeLen + 1) * sizeof(wchar_t));
  ::MultiByteToWideChar(CP_UTF8,
                        0,
                        str.c_str(),
                        -1,
                        (LPWSTR)pUnicode,
                        unicodeLen);
  wstring rt;
  rt = (wchar_t*)pUnicode;
  delete pUnicode;
  return rt;
}

void CopyFile(std::string source, std::string dest)//拷贝
{

  std::string cmd = "copy ";
  cmd += "\"" + source + "\" ";
  cmd += "\"" + dest + "\"";

  system(cmd.c_str());
}

bool PathExists(const std::string& path)//检测路径是否存在
{

  DWORD attr = GetFileAttributes(path.c_str());

  if (attr == INVALID_FILE_ATTRIBUTES)
    {
      return false;
    }

  return (attr & FILE_ATTRIBUTE_DIRECTORY) != 0 ||
         (attr & FILE_ATTRIBUTE_NORMAL) != 0;
}

void ReplaceBin(const std::string& path, const std::string& newValue)
{

  // 打开二进制文件进行读写
  std::fstream fs(path, std::ios::in | std::ios::out | std::ios::binary);

  // 查找第40字节位置
  fs.seekg(40, std::ios::beg);

  // 读取9字节原数据
  char buf[9];
  fs.read(buf, sizeof(buf));
  cout<<"原本ID:"<<buf<<endl;
  // 将新值转换为ASCII码写入
  for(int i = 0; i < 9; i++)
    {
      buf[i] = newValue[i];
    }

  // 寻址回40字节位置
  fs.seekp(40, std::ios::beg);
  cout<<"现在ID:"<<buf<<endl;

  // 覆盖写入9字节数据
  fs.write(buf, sizeof(buf));

  fs.close();
}
bool CopyAndRename(const std::string& srcPath, const std::string& destPath)
{

  // 打开源文件
  HANDLE src = CreateFile(srcPath.c_str(), GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, NULL);
  if (src == INVALID_HANDLE_VALUE)
    {
      std::cout << "打开源文件失败:" << srcPath << std::endl;
      return false;
    }

  // 打开目标文件
  HANDLE dest = CreateFile(destPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, 0, NULL);
  if (dest == INVALID_HANDLE_VALUE)
    {
      std::cout << "打开目标文件失败:" << destPath << std::endl;
      CloseHandle(src);
      return false;
    }

  // 拷贝数据
  DWORD bytesRead = 0;
  DWORD bytesWritten = 0;
  BYTE buffer[4096];

  bool success = true;
  while (ReadFile(src, buffer, 4096, &bytesRead, NULL))
    {
      if (!WriteFile(dest, buffer, bytesRead, &bytesWritten, NULL))
        {
          success = false;
          std::cout << "拷贝文件失败!" << std::endl;
          break;
        }
    }

  // 关闭文件
  CloseHandle(src);
  CloseHandle(dest);

  return success;
}

