#include <iostream>
#include "winKeyPressH.hpp"
#include <thread>
#include <unistd.h>
using namespace std;
bool run = true;
int a = 0;

// 获取鼠标位置 //
void mousePositon()
{
    // 默认输出到控制台 //
    MousePosition mousePosition = GetMousePosition(true);

    // 将坐标存储到变量 //
    int x, y;
    x = mousePosition.x;
    y = mousePosition.y;
}

// 获取屏幕大小 //
void screenSize()
{
    // 默认输出到控制台 //
    ScreenSize screenSize = GetScreenSIze(true);

    // 将坐标存储到变量 //
    int x, y;
    x = screenSize.x;
    y = screenSize.y;
}

/****** 关于MouseEvent类的使用方法 ******/
// 鼠标移动，关于鼠标操作都在类MouseEvent中 //
void mouseMove()
{
    // 移动到指定坐标
    MouseEvent m;
    m.MouseMoveTo(100, 100);
}

// 鼠标按下，鼠标抬起一样，使用MouseUp()函数
void mouseDown()
{
    MouseEvent m;
    m.MouseDown("left");
}

// 鼠标滚轮移动，向上正数向下负数 //
void mouseRoll()
{
    MouseEvent m;
    m.MouseRoll(-1000);  // 向下滚动
}


// 鼠标监听(目前属于半成品未开发完成) //
void ListenMouseEvent()
{
    MouseEvent m;
    m.ListenMouseEvent();

}

/****** 关于KeyBoardEvent类的使用方法 ******/
// 按下某个按键，抬起则是KeyUp用法相同，注意按下需要手动调用KeyUp，鼠标操作也是否则不会抬起 //
void keyDown()
{
    KeyBoardEvent k;
    k.KeyDown("a");
}

// 按键点击，按下雨抬起，不需要担心忘记调用抬起函数所导致按键一直按下 //
void pressKey()
{
    KeyBoardEvent k;
    k.PressKey("a");
}


// 快捷键按下，最大支持三个快捷键 //
void pressHotKey()
{
    KeyBoardEvent k;
    k.PressHotKey("ctrl", "alt", "c");
    k.PressHotKey("ctrl", "v", "NULL");
}

// 模拟键盘输入，仅支持英文，如果需要中文可以使用CopyStr函数代替 //
void writeStr()
{
    KeyBoardEvent k;
    k.WriteStr("Hello World");
}

// 复制字符串 //
void copyStr()
{
    KeyBoardEvent k;
    k.CopyStr("Hello World");
}

class Test
{
public:
    void print()
    {
        cout<<"your press ctrl+d";
    }
};

void print()
{
    cout<<"your press ctrl+d";
    KeyBoardEvent::ClearHotKey();
}

// 注册快捷键,单按键监听不支持特殊按键:ctrl,alt,shift //
void addHotKey()
{
    KeyBoardEvent k;
    Test t;
    // 调用普通函数 //
    k.AddHotKey("esc", print);

    // 调用类成员函数 //
    k.AddHotKey("ctrl+d", [&t](){t.print(); KeyBoardEvent::ClearHotKey();});
    k.WaitHotKey();
}

int main()
{
    return 0;
}