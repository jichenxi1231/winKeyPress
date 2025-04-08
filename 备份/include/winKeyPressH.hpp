#ifndef WinKeyPress
#define WinKeyPress
#pragma once
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



class MouseEvent
{
public:
    MouseEvent();

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


class KeyBoardEvent
{
public:
    KeyBoardEvent();

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