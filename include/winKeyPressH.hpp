#ifndef WinKeyPress
#define WinKeyPress
#include <iostream>
#include <windows.h>
#include <cstdio>
#include <ctime>
#include <unistd.h> // For sleep()
#include <csignal> // For signal()
#include <regex>  // 正则表达头文件
#include <vector>  // 自动管理内存
#include <thread>
#include <unistd.h>
#include <functional>
using namespace std;
enum BOOLS {True=1, False=0};

// 定义一个检查是否注册末尾清理的变量 //
static bool autoClear = False;

// 定义一个程序结束清理的函数 //
void cleanup_check();

/************* 屏幕事件 ****************/
typedef struct  // 声明结构体获取屏幕大小
{
    int x;
    int y;
} ScreenSize;  // 定义标签

typedef struct MousePosition  // 获取光标所在当前位置
{
    LONG x;
    LONG y;
}MousePosition;

ScreenSize GetScreenSIze(bool out_put);  // 获取分辨率函数，布偶值选择是否输出坐标

MousePosition GetMousePosition(bool out_put);  // 获取光标目前所在的坐标，鼠标坐标，布偶值选择是否输出坐标


/************* press up flag **************/

typedef struct MouseFlag  // 键盘按下的标志，后期程序结束如果程序员忘记是覅则自动释放按键
{
    bool mouse_left_down;  // 左键标志
    bool mouse_right_down;  // 右键标准
}MouseFlag;

static MouseFlag mouse_flag;  // 实例化结构体


// 鼠标事件监听的结构体标志 //
typedef struct
{
    bool MouseLeftDown;  // 左键标志
    bool MouseRightDown;  // 右键标准

    bool MouseLeftUp;  // 左键标志
    bool MouseRightUp;  // 右键标准

    bool MouseRollDown;  // 滚轮按下
    bool MouseRollUp;  // 滚轮抬起
}MouseState;

static MouseState mouseEventFlag;


class MouseEvent
{
public:
    MouseEvent()
    {
        // 检查是否注册末尾清理函数，False为没有注册 //
        if (!autoClear)
        {
            std::atexit(cleanup_check);
            autoClear = True;  // 标志设置为True表示注册
        }

        UINT systemCode = GetACP();
        if (systemCode == 65001)
        {
            // 当系统编码为65001时，将其改为GBK，以GBK模式输出 //
            SetConsoleOutputCP(936);  // 设置编码65001控制台输出为UTF-8
            SetConsoleCP(65001);  // 设置控制台输入为UTF-8
        }
    }

    void MouseMoveTo(int x, int y);  // x,y坐标;

    void MouseDown(const string& button);  // 鼠标按下键位

    void MouseUp(const string& button);  // 鼠标释放

    void MouseClick(int x, int y, int clicks, const string& button);  // 鼠标点击函数,默认左键

    void MouseRoll(int move);  // 鼠标滚轮,正数向上负数向下

    void ListenMouseEvent();  // 监听鼠标事件 //

    static void ExitMouseEvent();  // 退出鼠标监听事件 //
};


/************************** 键盘事件 *****************************/
// 字符串复制到剪切板 //
typedef struct copy_str_structs  // 字符窜剪切的结构体(防止后续重复利用相同字符窜)
{
    const char* textToCopy;  // 一个常量指针字符窜
    void (*copy_str_in)(struct copy_str_structs);  // 复制到剪切板的函数，后续包裹到CopyStr中
}copy_str_structs;

// 检查键盘是否松开结构体 //
typedef struct PressHotKeyNameUp
{
    int key_num;  // 按下的数量
    int key_code[300];
}PressHotKeyNameUp;  // 释放按下的按键
static PressHotKeyNameUp free_keys = {0, 0}; // 释放按键


// 功能实现类 //
// 存放快捷键虚键码的结构体与容器 //
typedef struct
{
    int key1;
    int key2;
    int key3;
    int key4;
    int FuncIndex;
    int FuncID;  // 函数的索引顺序，ID号
}KeyCode;

// 存放快捷键虚键码的容器 //
static vector<KeyCode>key_code;


// 快捷键的虚键码 //
static vector<int>fourKeyCode;  // 四个键位的键值
static vector<int> threeKeyCode;  // 三个键位的键值
static vector<int> twoKeyCode;  // 两个键位的键值
static vector<int> oneKeyCode;  // 一个键位的键值

// 存放快捷键函数的容器 //
//vector<void (*)()>fourKeyFunc;
//vector<void (*)()>threeKeyFunc;
//vector<void (*)()>twoKeyFunc;
//vector<void (*)()>oneKeyFunc;

// 存放快捷键函数的容器，可以存放对象函数 //
static vector<std::function<void()>>fourKeyFunc;
static vector<std::function<void()>>threeKeyFunc;
static vector<std::function<void()>>twoKeyFunc;
static vector<std::function<void()>>oneKeyFunc;

// 快捷键计数器 //
static int fourKeyIndex = 0;  // 四个键位的索引
static int threeKeyIndex = 0;  // 三个键位的索引
static int twoKeyIndex = 0;  // 两个键位的索引
static int oneKeyIndex = 0;  // 一个键位的索引

class KeyBoardEvent
{
public:
    KeyBoardEvent()
    {
        // 检查是否注册末尾清理函数，False为没有注册 //
        if (!autoClear)
        {
            // 注册清理函数 //
            std::atexit(cleanup_check);
            autoClear = True;  // 标志设置为True表示注册
        }
        UINT systemCode = GetACP();
        if (systemCode == 65001)
        {
            // 当系统编码为65001时，将其改为GBK，以GBK模式输出 //
            SetConsoleOutputCP(936);  // 设置编码65001控制台输出为UTF-8
            SetConsoleCP(65001);  // 设置控制台输入为UTF-8
        }
    }
    // 功能部分
    void KeyDown(const string& key);  // 键盘按下

    void KeyUp(const string& key);  // 键盘释放

    void PressKey(const string& key); // 按键

    void PressHotKey(const string& key1, const string& key2, const string& key3);  // 按下快捷键

    void WriteStr(string output); // 输入字符窜函数但仅可用英文

    void CopyStr(const char *str);  // 复制字符串函数

//    string PasteStr();  // 返回字符串的一个函数

//    ~KeyBoardEvent()

    // 注册快捷键,普通函数与类函数 //
//    void AddHotKey(const string &key, void (*targetVoidFunc)());
    void AddHotKey(const string &key, std::function<void()> targetVoidFunc);

    void WaitHotKey();  // 监听快捷键函数

    // 清理快捷键 //
    static void ClearHotKey();  // 清除快捷键,设置static静态函数可以被成员函数实力化后回调 //
};

// 回调清理函数 //
void ExitCheckWork();

void winKeyPressHelp();



#endif