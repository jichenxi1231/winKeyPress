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
#include <cctype>
#include "winKeyPressH.hpp"
using namespace std;

/*********** 虚键码对照表 Virtual key code mapping table **************/
string alphabet_list[] = {"a", "b", "c", "d", "e", "f", "g",
                          "h", "i", "j", "k", "l", "m",
                          "n", "o", "p", "q", "r", "s",
                          "t", "u", "v", "w", "x", "y", "z"  // 26字母
        ,"0", "1", "2", "3", "4", "5", "6"
        , "7", "8", "9"};  // 按键列  // 字母列表


string function_keys_list [] = {"ctrl", "alt", "shift", "F1", "F2", "F3", "F4", "F5",
                                "F6", "F7", "F8", "F9", "F10", "F11", "F12", "esc",
                                "space", "delete", "tab", "enter", "caps", "clear",
                                "backspace", "win", "Pause", "page up", "page down", "left arrow",
                                "right arrow", "down arrow", "up arrow", "insert"};

/****************************虚键码列表********************************/
int alphabet_code[] = {65, 66, 67, 68, 69, 70,
                       71, 72, 73, 74, 75, 76,
                       77, 78, 79, 80, 81, 82,
                       83, 84, 85, 86, 87, 88,
                       89, 90, 48, 49, 50, 51,  // 48 以后是数组
                       52, 53, 54, 55, 56, 57};  // 虚拟键码

int function_code[] = {VK_CONTROL, VK_MENU, VK_SHIFT, VK_F1, VK_F2, VK_F3,
                       VK_F4, VK_F5, VK_F6, VK_F7, VK_F8, VK_F9, VK_F10,
                       VK_F11, VK_F12, VK_ESCAPE, VK_SPACE, VK_DELETE, VK_TAB,
                       VK_RETURN, VK_CAPITAL, VK_CLEAR, VK_BACK, VK_LWIN, VK_PAUSE,
                       VK_PRIOR, VK_NEXT, VK_LEFT, VK_RIGHT, VK_DOWN, VK_UP, VK_INSERT
};


/**************************** 符合虚键码列表 ********************************/
string symbol_list_not_shift_press[] = {"`", "[", "]", "\\", ";", "''", ",", ".", "/",
                                        "-", "=", "，", "。", "’", "‘"};

string symbol_list_need_shift_press[] = {"~", "{", "}", "|", ":", "\"\"", "<", ">", "?",
                                         "_", "+","!", "@", "#", "$", "%", "^",
                                         "&", "*", "(",")"};

string chinese_symbol_shift_press[] = {"：", "“”", "《", "》", "？",
                                       "——", "！" , "￥", "……","（","）"};

//虚键码列表//
int symbol_not_shift_code[] = {VK_OEM_3, VK_OEM_4, VK_OEM_6, VK_OEM_5, VK_OEM_1, VK_OEM_7, 188,
                               190, VK_OEM_2, VK_OEM_MINUS, VK_OEM_PLUS, 188, 190, VK_OEM_7,
                               VK_OEM_7};

int symbol_need_shift_code[] = {VK_OEM_3, VK_OEM_4, VK_OEM_6, VK_OEM_5, VK_OEM_1, VK_OEM_7, 188,
                                190, VK_OEM_2, VK_OEM_MINUS, VK_OEM_PLUS, 49,
                                50, 51,52, 53, 54, 55, 56, 57, 48 };

int chinese_symbol_shift_code[] = {VK_OEM_1, VK_OEM_7, 188,190, VK_OEM_2,
                                   VK_OEM_MINUS,  49,52, 53, 57, 48 };


/************** 屏幕事件 **************/
ScreenSize GetScreenSIze(bool out_put=True)
{
    ScreenSize screen_size;
    int x_len, y_len;
    x_len=GetSystemMetrics(SM_CXSCREEN);  // 获取x长度
    y_len=GetSystemMetrics(SM_CYSCREEN);  // 获取y长度
    screen_size.x = x_len;  // 结构体储存大小(屏幕)
    screen_size.y = y_len;  // 结构体储存大小(屏幕)
    if (out_put)
    {
        printf("Display size x:%d, y:%d\n", screen_size.x, screen_size.y);
    }
    return screen_size;
//    If you want to get coordinates you need to instantiate the struct function:
//    ScreenSizeGet screen_size = GetScreenSIze; screen_size.x, screen_size.y
}


/************************获取光标坐标*******************************/
MousePosition GetMousePosition(bool out_put=True)  // 获取光标目前所在的坐标
{
    POINT cursorPos;  // 实例化光标结构体，获取位置
    MousePosition mouse_position;  // 实力结构体取其中变量
    if (GetCursorPos(&cursorPos))
    {
        if (out_put)
        {
            printf("The current position of the cursor is x:%ld, y:%ld\n", cursorPos.x, cursorPos.y);
        }
        mouse_position.x = cursorPos.x;
        mouse_position.y = cursorPos.y;
        return mouse_position;  // 返回这个实例化的坐标，如果想要拿到坐标需要实例化
        // 获取坐标需要实力化结构体如和将函数的值复制在结构体实例的变量中当做参数传给结构体再获取
//        If you want to get coordinates you need to instantiate the struct function:
//        MousePositionGet mouse_position = GetMousePosition(); mouse_position.x,mouse_position.y
    }
    else
    {
        printf("Sorry, cursor coordinates not found\n");
        return mouse_position;
    }

}

/***************** 鼠标事件的类 *******************/

void MouseEvent::MouseMoveTo(int x, int y)  // MouseMoveTo是void类型
{
    SetCursorPos(x, y);  // 移动鼠标到某处(设置光标位置)
}

void MouseEvent::MouseClick(int x, int y, int clicks=1, const string& button="left")
{
    /* 当click为1时并且button不为None时执行单击一次的指令 */
    SetCursorPos(x, y);  // 移动鼠标到某处(设置光标位置)
    if (clicks!=0)
    {
        int click_time = 0;  // 初始化点击次数
        while (click_time <clicks && button== "left")  // 当初始点击次数小于目标点击次数增
        {
            // 如果没传参数默认左键
            INPUT input = {0};  // 定义INPUT结构体变量表示输入事件信息
            input.type = INPUT_MOUSE;  // 指定输入事件类型为鼠标事件
            input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;  // 设定鼠标事件为左键按下
            SendInput(1, &input, sizeof(INPUT));  // 使用SendInput函数发送鼠标事件

            input.mi.dwFlags = MOUSEEVENTF_LEFTUP;  // 设定鼠标事件为左键释放
            SendInput(1, &input, sizeof(INPUT));  // 发送释放鼠标左键事件
            click_time++;
        }

        while (click_time <clicks && button == "right")  // 当初始点击次数小于目标点击次数增加
        {
            INPUT input = {0};  // 定义INPUT结构体变量表示输入事件信息
            input.type = INPUT_MOUSE;  // 指定输入事件类型为鼠标事件
            input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;  // 设定鼠标事件为左键按下
            SendInput(1, &input, sizeof(INPUT));  // 使用SendInput函数发送鼠标事件


            input.mi.dwFlags = MOUSEEVENTF_RIGHTUP;  // 设定鼠标事件为右键释放
            SendInput(1, &input, sizeof(INPUT));  // 发送释放鼠标右键事件
            click_time++;  // 初始值增加
        }
    }
}

void MouseEvent::MouseDown(const string& button="left")
{
    if (button == "left")
    {
        // 如果没传参数默认左键
        INPUT input = {0};  // 定义INPUT结构体变量表示输入事件信息
        input.type = INPUT_MOUSE;  // 指定输入事件类型为鼠标事件
        input.mi.dwFlags = MOUSEEVENTF_LEFTDOWN;  // 设定鼠标事件为左键按下
        SendInput(1, &input, sizeof(INPUT));  // 使用SendInput函数发送鼠标事件
        // 设置鼠标被按下
        mouse_flag.mouse_left_down = True;  // 标志为true
    }
    else if (button == "right")
    {
        // 如果没传参数默认左键
        INPUT input = {0};  // 定义INPUT结构体变量表示输入事件信息
        input.type = INPUT_MOUSE;  // 指定输入事件类型为鼠标事件
        input.mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;  // 设定鼠标事件为左键按下
        SendInput(1, &input, sizeof(INPUT));  // 使用SendInput函数发送鼠标事件
        // 设置鼠标被按下
        mouse_flag.mouse_right_down = True;
    }
}

void MouseEvent::MouseUp(const string& button="left")
{
    if  (button == "left")
    {
        INPUT input = {0};
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_LEFTUP; // 假设是左键，根据需要调整
        SendInput(1, &input, sizeof(INPUT));
        // 松开标志为False
        mouse_flag.mouse_left_down = False;
    }
    else if (button == "right")
    {
        INPUT input = {0};
        input.type = INPUT_MOUSE;
        input.mi.dwFlags = MOUSEEVENTF_RIGHTUP; // 假设是左键，根据需要调整
        SendInput(1, &input, sizeof(INPUT));
        // 松开标志为False
        mouse_flag.mouse_right_down = False;
    }
}

void MouseEvent::MouseRoll(int move)
{
    INPUT input = {0};
    input.type = INPUT_MOUSE;
    input.mi.dx = 0;       // x坐标
    input.mi.dy = 0;       // y坐标
    input.mi.mouseData = move; // 滚动方向和距离
    input.mi.dwFlags = MOUSEEVENTF_WHEEL; // 发送滚轮事件
    input.mi.time = 0;     // 当前系统时间
    input.mi.dwExtraInfo = 0; // 额外信息

    SendInput(1, &input, sizeof(INPUT));
}

//              鼠标事件监听             //

// 定义全局变量，用于存储鼠标钩子句柄
HHOOK hMouseHook = NULL;

// 钩子回调函数，用于处理鼠标事件 //
LRESULT CALLBACK MouseProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    // 获取鼠标事件的详细信息
    MSLLHOOKSTRUCT* pMouseStruct = (MSLLHOOKSTRUCT*)lParam;

    // 如果钩子代码有效（nCode >= 0），处理鼠标事件
    if (nCode >= 0)
    {
        // 根据wParam判断鼠标事件类型
        switch (wParam)
        {
            case WM_LBUTTONDOWN: // 左键按下
                std::cout << "Mouse left button down at (" << pMouseStruct->pt.x << ", " << pMouseStruct->pt.y << ")" << std::endl;

                // 左键按下时，将左键按下设置 True，抬起设为 False //
                mouseEventFlag.MouseLeftDown = True;
                mouseEventFlag.MouseLeftUp = False;
                cout<<"Press!\n";
                break;
            case WM_LBUTTONUP: // 左键释放
                std::cout << "Mouse left button up at (" << pMouseStruct->pt.x << ", " << pMouseStruct->pt.y << ")" << std::endl;

                // 左键释放时，将左键按下设为 False，抬起设为 True //
                mouseEventFlag.MouseLeftDown = False;
                mouseEventFlag.MouseLeftUp = True;
                break;
            case WM_RBUTTONDOWN: // 右键按下
//                std::cout << "Mouse right button down at (" << pMouseStruct->pt.x << ", " << pMouseStruct->pt.y << ")" << std::endl;

                // 右键按下时将，右键按下设置 True，抬起设为 False //
                mouseEventFlag.MouseRightDown = True;
                mouseEventFlag.MouseLeftUp = False;
                break;
            case WM_RBUTTONUP: // 右键释放
//                std::cout << "Mouse right button up at (" << pMouseStruct->pt.x << ", " << pMouseStruct->pt.y << ")" << std::endl;

                // 右键释放时将，右键按下设置 False，抬起设为 True //
                mouseEventFlag.MouseRightDown = False;
                mouseEventFlag.MouseLeftUp = True;

            break;
            case WM_MBUTTONDOWN: // 中键按下
//                std::cout << "Mouse middle button down at (" << pMouseStruct->pt.x << ", " << pMouseStruct->pt.y << ")" << std::endl;

                // 中键按下时将，中键按下设置 True，抬起设为 False //
                mouseEventFlag.MouseRollDown = True;
                mouseEventFlag.MouseLeftUp = False;
                break;
            case WM_MBUTTONUP: // 中键释放
//                std::cout << "Mouse middle button up at (" << pMouseStruct->pt.x << ", " << pMouseStruct->pt.y << ")" << std::endl;

                // 中键释放时将，中键按下设置 False，抬起设为 True //
                mouseEventFlag.MouseRollDown = False;
                mouseEventFlag.MouseLeftUp = True;
                break;
        }
    }

    // 调用下一个钩子，确保其他程序也能正常处理鼠标事件
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

// 退出函数，用于清理资源并退出程序
void MouseEvent:: ExitMouseEvent()
{
    // 检查全局变量hMouseHook是否有效
    if (hMouseHook != NULL)
    {
        // 移除鼠标钩子
        UnhookWindowsHookEx(hMouseHook);
        hMouseHook = NULL; // 将全局变量置为空
    }

    // 发送程序结束消息 //
    PostQuitMessage(0);

//    exit(0); // 退出程序
}

// 监听鼠标函数 //
void MouseEvent::ListenMouseEvent()
{
    // 安装鼠标钩子，并将句柄存储到全局变量中
    hMouseHook = SetWindowsHookEx(WH_MOUSE_LL, MouseProc, GetModuleHandle(NULL), 0);

    // 初始化鼠标事件标志 //
    mouseEventFlag.MouseLeftDown = False;
    mouseEventFlag.MouseRightDown = False;

    mouseEventFlag.MouseLeftUp = False;
    mouseEventFlag.MouseRightUp = False;

    mouseEventFlag.MouseRollDown = False;
    mouseEventFlag.MouseRollUp = False;

    // 检查安装钩子是否成功 //
    if (hMouseHook == NULL)
    {
        std::cerr << "Failed to install mouse hook." << std::endl;
        return ; // 如果安装钩子失败，退出程序
    }

    // 当 PostQuitMessage(0) 被调用时，GetMessage会返回 0, 从而退出循环。 //
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0) > 0) // 使用GetMessage返回值判断是否收到退出消息
    {
        TranslateMessage(&msg); // 翻译消息
        DispatchMessage(&msg); // 派发消息
    }
}


/************************* 键盘事件 ****************************/
// 键盘按下记录方便后期查询是否释放 //
void key_down_record(int key_code)  // 记录键盘按下 (避免重复)
{
    int len = sizeof(free_keys.key_code) / sizeof(free_keys.key_code[0]);  // 检查按键数组大小
    bool found = false;  // 标记是否找到按键码

    // 遍历数组，检查按键码是否已经存在
    for (int i = 0; i < len; i++)
    {
        if (free_keys.key_code[i] == key_code)
        {
            found = true;  // 找到按键码，设置标记为true
            break;
        }
    }

    // 如果没有找到按键码，添加到数组的第一个空位置
    if (!found)  // 如果没找到 就重新遍历数组 寻找值为0的位置 将虚键码赋值在上面
    {
        for (int i = 0; i < len; i++)
        {
            if (free_keys.key_code[i] == 0)  // 找到数组的第一个空位置 (找到数组为0的位置，将虚键码赋值到上面)
            {
                free_keys.key_code[i] = key_code;
                break;
            }
        }
    }
}

// 记录键盘松开，清空按键 //
void key_up_record(int key_code)
{
    int len = sizeof(free_keys.key_code) / sizeof(free_keys.key_code[0]);  // 检查释放键盘的数组大小
    for (int i = 0; i < len; i++)
    {
        if (free_keys.key_code[i] == key_code)
        {
            free_keys.key_code[i] = 0;  // 清空按键码
            break;
        }
    }
}

// 键盘按下 //
void KeyBoardEvent::KeyDown(const string& key)
{
    /****************列表长度************************/
    int alphabet_list_len = sizeof (alphabet_list)/sizeof (alphabet_list[0]);  // 字母数字长度
    int function_keys_list_len = sizeof (function_keys_list)/sizeof (function_keys_list[0]);  // 功能键数组长度


    for (int i = 0; i<alphabet_list_len; i++)
    {
        // 检查数组的字符是否与传入的参数一样(判断字母) 如果都没进入下一个循环
        if (key == alphabet_list[i])
        {
            INPUT input; // 声明一个 INPUT 结构体变量 input，用于描述按键事件

            input.type = INPUT_KEYBOARD; // 指定 input 的类型为键盘输入
            input.ki.wScan = 0; // 扫描码置为 0，通常不需要使用
            input.ki.time = 0; // 时间戳置为 0
            input.ki.dwExtraInfo = 0; // 额外信息置为 0

            // 模拟按下目标键
            input.ki.wVk = alphabet_code[i]; // 指定模拟按下的键为 目标 键
            input.ki.dwFlags = 0; // 指定键盘按下事件，dwFlags 为 0
            SendInput(1, &input, sizeof(INPUT)); // 发送按键事件给系统

            key_down_record(alphabet_code[i]);
            break;
        }
    }

    for (int i=0; i<function_keys_list_len; i++)
    {
        // 是否与传入的参数一样 (判断功能键)
        if (key == function_keys_list[i])
        {
            INPUT input; // 声明一个 INPUT 结构体变量 input，用于描述按键事件

            input.type = INPUT_KEYBOARD; // 指定 input 的类型为键盘输入
            input.ki.wScan = 0; // 扫描码置为 0，通常不需要使用
            input.ki.time = 0; // 时间戳置为 0
            input.ki.dwExtraInfo = 0; // 额外信息置为 0

            // 模拟按下目标键
            input.ki.wVk = function_code[i]; // 指定模拟按下的键为 目标 键
            input.ki.dwFlags = 0; // 指定键盘按下事件，dwFlags 为 0
            SendInput(1, &input, sizeof(INPUT)); // 发送按键事件给系统

            key_down_record(function_code[i]);
            break;
        }
    }
}

// 键盘释放 //
void KeyBoardEvent::KeyUp(const string& key)
{
    int len = sizeof (free_keys.key_code)/sizeof (free_keys.key_code[0]);
/****************列表长度************************/
    int alphabet_list_len = sizeof (alphabet_list)/sizeof (alphabet_list[0]);  // 字母数字长度
    int function_keys_list_len = sizeof (function_keys_list)/sizeof (function_keys_list[0]);  // 功能键数组长度

    for (int i = 0; i<alphabet_list_len; i++) {
        // 检查数组的字符是否与传入的参数一样(判断字母) 如果都没进入下一个循环
        if (key == alphabet_list[i]) {
            INPUT input = {0};
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = alphabet_code[i];  // 键位吗码
            // 如果之前按键被按下了，现在需要释放
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &input, sizeof(INPUT));

            key_up_record(alphabet_code[i]);
            break;
        }
    }


    for (int i=0; i<function_keys_list_len; i++)
    {
        // 是否与传入的参数一样 (判断功能键)
        if (key == function_keys_list[i])
        {
            INPUT input = {0};
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = function_code[i];  // 键位吗码
            // 如果之前按键被按下了，现在需要释放
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &input, sizeof(INPUT));

            key_up_record(function_code[i]);
            break;
        }
    }

//    INPUT input = {0};
//    input.type = INPUT_KEYBOARD;
//    input.ki.wVk = vkCode;  // 键位吗码
//    // 如果之前按键被按下了，现在需要释放
//    input.ki.dwFlags = KEYEVENTF_KEYUP;
//    SendInput(1, &input, sizeof(INPUT));
}

// 键盘点击 //
void KeyBoardEvent::PressKey(const string& key)
{
/****************列表长度************************/
    int alphabet_list_len = sizeof (alphabet_list)/sizeof (alphabet_list[0]);  // 字母数字长度
    int symbol_not_shift_len = sizeof (symbol_list_not_shift_press)/sizeof(symbol_list_not_shift_press[0]); // 字符长度
    int symbol_need_shift_len = sizeof (symbol_list_need_shift_press)/sizeof (symbol_list_need_shift_press[0]); // 字符数组长度
    int function_keys_list_len = sizeof (function_keys_list)/sizeof (function_keys_list[0]);  // 功能键数组长度
    int chinese_symbol_shift_press_len = sizeof (chinese_symbol_shift_press)/sizeof (chinese_symbol_shift_press[0]);  // 中文字符数组列表长度


    for (int i = 0; i<alphabet_list_len; i++)
    {
        // 检查数组的字符是否与传入的参数一样(判断字母) 如果都没进入下一个循环
        if (key == alphabet_list[i])
        {
            INPUT input; // 声明一个 INPUT 结构体变量 input，用于描述按键事件

            input.type = INPUT_KEYBOARD; // 指定 input 的类型为键盘输入
            input.ki.wScan = 0; // 扫描码置为 0，通常不需要使用
            input.ki.time = 0; // 时间戳置为 0
            input.ki.dwExtraInfo = 0; // 额外信息置为 0

            // 模拟按下目标键
            input.ki.wVk = alphabet_code[i]; // 指定模拟按下的键为 目标 键
            input.ki.dwFlags = 0; // 指定键盘按下事件，dwFlags 为 0
            SendInput(1, &input, sizeof(INPUT)); // 发送按键事件给系统

            // 模拟释放目标键
            input.ki.wVk = alphabet_code[i]; // 指定模拟释放的键仍然是 目标 键
            input.ki.dwFlags = KEYEVENTF_KEYUP; // 指定键盘释放事件，dwFlags 设置为 KEYEVENTF_KEYUP
            SendInput(1, &input, sizeof(INPUT)); // 再次发送按键事件给系统
            break;
        }
    }

    for (int i=0; i<function_keys_list_len; i++)
    {
        // 是否与传入的参数一样 (判断功能键)
        if (key == function_keys_list[i])
        {
            INPUT input; // 声明一个 INPUT 结构体变量 input，用于描述按键事件

            input.type = INPUT_KEYBOARD; // 指定 input 的类型为键盘输入
            input.ki.wScan = 0; // 扫描码置为 0，通常不需要使用
            input.ki.time = 0; // 时间戳置为 0
            input.ki.dwExtraInfo = 0; // 额外信息置为 0

            // 模拟按下目标键
            input.ki.wVk = function_code[i]; // 指定模拟按下的键为 目标 键
            input.ki.dwFlags = 0; // 指定键盘按下事件，dwFlags 为 0
            SendInput(1, &input, sizeof(INPUT)); // 发送按键事件给系统

            input.ki.wVk = function_code[i]; // 指定模拟释放的键仍然是 目标 键
            input.ki.dwFlags = KEYEVENTF_KEYUP; // 指定键盘释放事件，dwFlags 设置为 KEYEVENTF_KEYUP
            SendInput(1, &input, sizeof(INPUT)); // 再次发送按键事件给系统
            break;
        }
    }

    for (int i=0; i< symbol_not_shift_len; i++)
    {
        // 是否与传入的参数一样 (判断无需按shift按键的符号)
        if (key == symbol_list_not_shift_press[i])
        {
            INPUT input; // 声明一个 INPUT 结构体变量 input，用于描述按键事件

            input.type = INPUT_KEYBOARD; // 指定 input 的类型为键盘输入
            input.ki.wScan = 0; // 扫描码置为 0，通常不需要使用
            input.ki.time = 0; // 时间戳置为 0
            input.ki.dwExtraInfo = 0; // 额外信息置为 0

            // 模拟按下目标键
            input.ki.wVk = symbol_not_shift_code[i]; // 指定模拟按下的键为 目标 键
            input.ki.dwFlags = 0; // 指定键盘按下事件，dwFlags 为 0
            SendInput(1, &input, sizeof(INPUT)); // 发送按键事件给系统

            input.ki.wVk = symbol_not_shift_code[i]; // 指定模拟释放的键仍然是 目标 键
            input.ki.dwFlags = KEYEVENTF_KEYUP; // 指定键盘释放事件，dwFlags 设置为 KEYEVENTF_KEYUP
            SendInput(1, &input, sizeof(INPUT)); // 再次发送按键事件给系统
            break;
        }
    }

    for (int i=0; i<symbol_need_shift_len; i++)
    {
        // 需要按shift的符合
        if (key == symbol_list_need_shift_press[i])
        {
            // 创建两个输入结构体并初始化
            INPUT inputs[2] = {0};

            // 第一个按键 'shift' 的输入结构体
            inputs[0].type = INPUT_KEYBOARD;
            inputs[0].ki.wVk = VK_SHIFT; // 'shift'键的虚拟键码
            inputs[0].ki.dwFlags = 0; // 表示按下按键

            // 第二个按键 '目标参数' 的输入结构体
            inputs[1].type = INPUT_KEYBOARD;
            inputs[1].ki.wVk = symbol_need_shift_code[i]; // '目标参数'键的虚拟键码
            inputs[1].ki.dwFlags = 0; // 表示按下按键
            SendInput(2, inputs, sizeof (INPUT));  // 发生指令按下shift与参数按键

            inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
            inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
            SendInput(2, inputs, sizeof(INPUT));  // 同时释放
            break;
        }
    }

    for (int i=0; i<chinese_symbol_shift_press_len; i++)
    {
        // 中文字符判断
        if (key == chinese_symbol_shift_press[i])
        {
            // 创建两个输入结构体并初始化
            INPUT inputs[2] = {0};

            // 第一个按键 'shift' 的输入结构体
            inputs[0].type = INPUT_KEYBOARD;
            inputs[0].ki.wVk = VK_SHIFT; // 'shift'键的虚拟键码
            inputs[0].ki.dwFlags = 0; // 表示按下按键

            // 第二个按键 '目标参数' 的输入结构体
            inputs[1].type = INPUT_KEYBOARD;
            inputs[1].ki.wVk = chinese_symbol_shift_code[i]; // '目标参数'键的虚拟键码
            inputs[1].ki.dwFlags = 0; // 表示按下按键
            SendInput(2, inputs, sizeof (INPUT));  // 发生指令按下shift与参数按键

            inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
            inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
            SendInput(2, inputs, sizeof(INPUT));  // 同时释放
            break;
        }
    }
}

void KeyBoardEvent::PressHotKey(const string& key1="NULL", const string& key2="MULL", const string& key3="NULL")
{
    // 如果不使用其他快捷键输入None
    // 这里是查看两个数组，如果参数传入的与数组匹配就索引数组获得按键名称以及索引码


    string button_list[] = {"a", "b", "c", "d", "e", "f", "g",
                            "h", "i", "j", "k", "l", "m",
                            "n", "o", "p", "q", "r", "s",
                            "t", "u", "v", "w", "x", "y", "z"  // 26字母
            ,"0", "1", "2", "3", "4", "5", "6"
            , "7", "8", "9"};  // 按键列


    int button_code_list[] = {65, 66, 67, 68, 69, 70,
                              71, 72, 73, 74, 75, 76,
                              77, 78, 79, 80, 81, 82,
                              83, 84, 85, 86, 87, 88,
                              89, 90, 48, 49, 50, 51,
                              52, 53, 54, 55, 56, 57};  // 虚拟键码

    int len = sizeof (button_list)/sizeof (button_list[0]);  // 数组大小


    if (key1 != "null" && key2 != "null")  // 检查两个字符窜是否为None
    {
        if (key1 == "ctrl") // 如果快捷键以ctrl开头
        {
            for (int i=0; i <len; i++)  // 匹配按键列表中与传入参数相同的按键并获取按键虚拟码
            {
                // 如果第二个键与参数相符
                if (button_list[i] == key2)
                {
                    // 创建一个输入结构体并初始化
                    INPUT inputs[2] = {0};  // [2]表示两个元素执行两个按键点击，{0}初始化数组
                    // 第一个按键 'ctrl' 的输入结构体
                    inputs[0].type = INPUT_KEYBOARD;
                    inputs[0].ki.wVk = VK_CONTROL; // 'ctrl'键的虚拟键码
                    inputs[0].ki.dwFlags = 0; // 表示按下按键

                    // 第二个按键 '传入的字符' 的输入结构体
                    inputs[1].type = INPUT_KEYBOARD;
                    inputs[1].ki.wVk = button_code_list[i]; // '函数参数'键的虚拟键码
                    inputs[1].ki.dwFlags = 0; // 表示按下按键
                    // 发送输入，同时按下'ctrl'和'指定参数'键
                    SendInput(2, inputs, sizeof(INPUT));


                    // 模拟按键释放
                    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                    SendInput(2, inputs, sizeof(INPUT));
                    break;  // break防止循环中多次点击快捷键
                }

                    // 如果第二个键是alt，， 以ctrl开头
                else if (key2 == "alt")
                {
                    if (key3!="NULL" && button_list[i] == key3)
                    {
                        // 创建一个输入结构体并初始化
                        INPUT inputs[3] = {0};  // [2]表示两个元素执行两个按键点击，{0}初始化数组
                        inputs[0].type = INPUT_KEYBOARD;
                        inputs[0].ki.wVk = VK_CONTROL; // 'ctrl'键的虚拟键码
                        inputs[0].ki.dwFlags = 0; // 表示按下按键

                        inputs[1].type = INPUT_KEYBOARD;
                        inputs[1].ki.wVk = VK_MENU; // 'alt'键的虚拟键码
                        inputs[1].ki.dwFlags = 0; // 表示按下按键

                        // 第二个按键 '传入的字符' 的输入结构体
                        inputs[2].type = INPUT_KEYBOARD;
                        inputs[2].ki.wVk = button_code_list[i]; // '函数参数'键的虚拟键码
                        inputs[2].ki.dwFlags = 0; // 表示按下按键
                        // 发送输入，同时按下'ctrl，"alt',和'指定参数'键
                        SendInput(3, inputs, sizeof(INPUT));  // 按下

                        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                        SendInput(3, inputs, sizeof(INPUT));
                        break;
                    }

                }

                    // 如果第二个参数为shift， ，以ctrl开头
                else if (key2 == "shift")
                {
                    if (key3 !="NULL" && button_list[i] == key3)
                    {
                        // 创建一个输入结构体并初始化
                        INPUT inputs[3] = {0};  // [2]表示两个元素执行两个按键点击，{0}初始化数组
                        inputs[0].type = INPUT_KEYBOARD;
                        inputs[0].ki.wVk = VK_CONTROL; // 'ctrl'键的虚拟键码
                        inputs[0].ki.dwFlags = 0; // 表示按下按键

                        inputs[1].type = INPUT_KEYBOARD;
                        inputs[1].ki.wVk = VK_SHIFT; // 'shift'键的虚拟键码
                        inputs[1].ki.dwFlags = 0; // 表示按下按键

                        // 第二个按键 '传入的字符' 的输入结构体
                        inputs[2].type = INPUT_KEYBOARD;
                        inputs[2].ki.wVk = button_code_list[i]; // '函数参数'键的虚拟键码
                        inputs[2].ki.dwFlags = 0; // 表示按下按键
                        // 发送输入，同时按下'ctrl，"shift',和'指定参数'键
                        SendInput(3, inputs, sizeof(INPUT));  // 按下

                        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                        SendInput(3, inputs, sizeof(INPUT));
                        break;
                    }

                }
            }
        }


            // 判断alt开头
        else if (key1 == "alt") // 如果快捷键以alt开头
        {
            for (int i=0; i <len; i++)  // 匹配按键列表中与传入参数相同的按键并获取按键虚拟码
            {
                if (button_list[i] == key2)
                {
                    // 创建一个输入结构体并初始化
                    INPUT inputs[2] = {0};  // [2]表示两个元素执行两个按键点击，{0}初始化数组
                    // 第一个按键 'alt' 的输入结构体
                    inputs[0].type = INPUT_KEYBOARD;
                    inputs[0].ki.wVk = VK_MENU; // 'alt'键的虚拟键码
                    inputs[0].ki.dwFlags = 0; // 表示按下按键

                    // 第二个按键 '传入的字符' 的输入结构体
                    inputs[1].type = INPUT_KEYBOARD;
                    inputs[1].ki.wVk = button_code_list[i]; // '传入参数'键的虚拟键码
                    inputs[1].ki.dwFlags = 0; // 表示按下按键
                    // 发送输入，同时按下'ctrl'和'指定参数'键
                    SendInput(2, inputs, sizeof(INPUT));


                    // 模拟按键释放
                    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                    SendInput(2, inputs, sizeof(INPUT));

                    break;
                }

                    // 如果第二个参数为ctrl，， 以alt开头
                else if(key2 == "ctrl")
                {
                    if (key3 !="NULL" && button_list[i] == key3)
                    {
                        // 创建一个输入结构体并初始化
                        INPUT inputs[3] = {0};  // [2]表示两个元素执行两个按键点击，{0}初始化数组
                        inputs[0].type = INPUT_KEYBOARD;
                        inputs[0].ki.wVk = VK_CONTROL; // 'ctrl'键的虚拟键码
                        inputs[0].ki.dwFlags = 0; // 表示按下按键

                        inputs[1].type = INPUT_KEYBOARD;
                        inputs[1].ki.wVk = VK_MENU; // 'alt'键的虚拟键码
                        inputs[1].ki.dwFlags = 0; // 表示按下按键

                        // 第二个按键 '传入的字符' 的输入结构体
                        inputs[2].type = INPUT_KEYBOARD;
                        inputs[2].ki.wVk = button_code_list[i]; // '函数参数'键的虚拟键码
                        inputs[2].ki.dwFlags = 0; // 表示按下按键
                        // 发送输入，同时按下'ctrl，"alt',和'指定参数'键
                        SendInput(3, inputs, sizeof(INPUT));  // 按下

                        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                        SendInput(3, inputs, sizeof(INPUT));
                        break;
                    }

                }

                    // 如果第二个参数是shift,,以alt开头
                else if (key2 == "shift")
                {
                    if (key3 !="NULL" && button_list[i] == key3)
                    {
                        // 创建一个输入结构体并初始化
                        INPUT inputs[3] = {0};  // [2]表示两个元素执行两个按键点击，{0}初始化数组
                        inputs[0].type = INPUT_KEYBOARD;
                        inputs[0].ki.wVk = VK_MENU; // 'alt'键的虚拟键码
                        inputs[0].ki.dwFlags = 0; // 表示按下按键

                        inputs[1].type = INPUT_KEYBOARD;
                        inputs[1].ki.wVk = VK_SHIFT; // 'shift'键的虚拟键码
                        inputs[1].ki.dwFlags = 0; // 表示按下按键

                        // 第二个按键 '传入的字符' 的输入结构体
                        inputs[2].type = INPUT_KEYBOARD;
                        inputs[2].ki.wVk = button_code_list[i]; // '函数参数'键的虚拟键码
                        inputs[2].ki.dwFlags = 0; // 表示按下按键
                        // 发送输入，同时按下'alt'，"shift',和'指定参数'键
                        SendInput(3, inputs, sizeof(INPUT));  // 按下

                        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                        SendInput(3, inputs, sizeof(INPUT));
                        break;
                    }
                }
            }
        }


            // 判断shift开头
        else if (key1 == "shift") // 如果快捷键以shift开头
        {
            for (int i=0; i <len; i++)  // 匹配按键列表中与传入参数相同的按键并获取按键虚拟码
            {
                if (button_list[i] == key2)
                {
                    // 创建一个输入结构体并初始化
                    INPUT inputs[2] = {0};  // [2]表示两个元素执行两个按键点击，{0}初始化数组
                    // 第一个按键 'shift' 的输入结构体
                    inputs[0].type = INPUT_KEYBOARD;
                    inputs[0].ki.wVk = VK_SHIFT; // 'shift'键的虚拟键码
                    inputs[0].ki.dwFlags = 0; // 表示按下按键

                    // 第二个按键 '传入的字符' 的输入结构体
                    inputs[1].type = INPUT_KEYBOARD;
                    inputs[1].ki.wVk = button_code_list[i]; // 'B'键的虚拟键码
                    inputs[1].ki.dwFlags = 0; // 表示按下按键

                    // 发送输入，同时按下'shift'和'指定参数'键
                    SendInput(2, inputs, sizeof(INPUT));

                    // 模拟按键释放
                    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                    SendInput(2, inputs, sizeof(INPUT));
                    break;
                }

                    // 如果第二个参数为ctrl.，，，shift开头
                else if (key2 == "ctrl")
                {
                    if (key3 !="NULL" && button_list[i] == key3)
                    {
                        // 创建一个输入结构体并初始化
                        INPUT inputs[3] = {0};  // [2]表示两个元素执行两个按键点击，{0}初始化数组
                        inputs[0].type = INPUT_KEYBOARD;
                        inputs[0].ki.wVk = VK_CONTROL; // 'ctrl'键的虚拟键码
                        inputs[0].ki.dwFlags = 0; // 表示按下按键

                        inputs[1].type = INPUT_KEYBOARD;
                        inputs[1].ki.wVk = VK_SHIFT; // 'shift'键的虚拟键码
                        inputs[1].ki.dwFlags = 0; // 表示按下按键

                        // 第二个按键 '传入的字符' 的输入结构体
                        inputs[2].type = INPUT_KEYBOARD;
                        inputs[2].ki.wVk = button_code_list[i]; // '函数参数'键的虚拟键码
                        inputs[2].ki.dwFlags = 0; // 表示按下按键
                        // 发送输入，同时按下'ctrl，"alt',和'指定参数'键
                        SendInput(3, inputs, sizeof(INPUT));  // 按下
                        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                        SendInput(3, inputs, sizeof(INPUT));
                        break;
                    }
                }

                    // 判断第二个参数是否是alt,,,以shift开头
                else if(key2 == "alt")
                {
                    if (key3 !="NULL" && button_list[i] == key3)
                    {
                        // 创建一个输入结构体并初始化
                        INPUT inputs[3] = {0};  // [2]表示两个元素执行两个按键点击，{0}初始化数组
                        inputs[0].type = INPUT_KEYBOARD;
                        inputs[0].ki.wVk = VK_MENU; // 'alt'键的虚拟键码
                        inputs[0].ki.dwFlags = 0; // 表示按下按键

                        inputs[1].type = INPUT_KEYBOARD;
                        inputs[1].ki.wVk = VK_SHIFT; // 'shift'键的虚拟键码
                        inputs[1].ki.dwFlags = 0; // 表示按下按键

                        // 第二个按键 '传入的字符' 的输入结构体
                        inputs[2].type = INPUT_KEYBOARD;
                        inputs[2].ki.wVk = button_code_list[i]; // '函数参数'键的虚拟键码
                        inputs[2].ki.dwFlags = 0; // 表示按下按键
                        // 发送输入，同时按下'alt'，"shift',和'指定参数'键
                        SendInput(3, inputs, sizeof(INPUT));  // 按下

                        inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                        inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                        inputs[2].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                        SendInput(3, inputs, sizeof(INPUT));
                        break;
                    }
                }
            }
        }

            // 如果是win键
        else if(key1 == "win")
        {
            for (int i=0; i<len; i++)
            {
                // 如果第二个键与参数相符
                if (button_list[i] == key2)
                {
                    // 创建一个输入结构体并初始化
                    INPUT inputs[2] = {0};  // [2]表示两个元素执行两个按键点击，{0}初始化数组
                    // 第一个按键 'win' 的输入结构体
                    inputs[0].type = INPUT_KEYBOARD;
                    inputs[0].ki.wVk = VK_LWIN; // 'win'键的虚拟键码
                    inputs[0].ki.dwFlags = 0; // 表示按下按键

                    // 第二个按键 '传入的字符' 的输入结构体
                    inputs[1].type = INPUT_KEYBOARD;
                    inputs[1].ki.wVk = button_code_list[i]; // '函数参数'键的虚拟键码
                    inputs[1].ki.dwFlags = 0; // 表示按下按键

                    // 发送输入，同时按下'win'和'指定参数'键
                    SendInput(2, inputs, sizeof(INPUT));
                    // 模拟按键释放
                    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                    SendInput(2, inputs, sizeof(INPUT));
                    break;
                }
            }
        }
    }
}


/**
 *                    _ooOoo_
 *                   o8888888o
 *                   88" . "88
 *                   (| -_- |)
 *                    O\ = /O
 *                ____/`---'\____
 *              .   ' \\| |// `.
 *               / \\||| : |||// \
 *             / _||||| -:- |||||- \
 *               | | \\\ - /// | |
 *             | \_| ''\---/'' | |
 *              \ .-\__ `-` ___/-. /
 *           ___`. .' /--.--\ `. . __
 *        ."" '< `.___\_<|>_/___.' >'"".
 *       | | : `- \`.;`\ _ /`;.`/ - ` : | |
 *         \ \ `-. \_ __\ /__ _/ .-` / /
 * ======`-.____`-.___\_____/___.-`____.-'======
 *                    `=---='
 *
 * .............................................
 *          佛祖保佑             永无BUG
 **/

// 模拟输入文字 //
void KeyBoardEvent::WriteStr(string output)
{
    // 检查是否没有按下shift键 //
    bool shift_up = false;
    /****************列表长度************************/
    int alphabet_list_len = sizeof (alphabet_list)/sizeof (alphabet_list[0]);  // 字母数字长度
    int symbol_not_shift_len = sizeof (symbol_list_not_shift_press)/sizeof(symbol_list_not_shift_press[0]); // 字符长度
    int symbol_need_shift_len = sizeof (symbol_list_need_shift_press)/sizeof (symbol_list_need_shift_press[0]); // 字符数组长度

    /****************************** 执行部分 ************************************/
    // 循环遍历输入字符串中的每一个字符
    for (auto i:output)
    {
        char current_char = i;  // 将字符存储到字符变量

        // 处理字母
        for (int alp = 0; alp < alphabet_list_len; alp++)
        {
            // 检查是否为小写字母
            if (tolower(current_char) == alphabet_list[alp][0])
            {
                // 检查是否为大写字母
                bool is_uppercase = isupper(current_char); // 如果是大写

                if (is_uppercase)  // 如果有大写字母
                {
                    INPUT inputs[2] = {0};  // 设置两个按键
                    // 如果是大写字母，则首先模拟按下 Shift 键
                    inputs[0].type = INPUT_KEYBOARD;
                    inputs[0].ki.wVk = VK_SHIFT; // 'shift'键的虚拟键码
                    inputs[0].ki.dwFlags = 0; // 表示按下按键

                    // 模拟按下目标键
                    inputs[1].type = INPUT_KEYBOARD;
                    inputs[1].ki.wVk = alphabet_code[alp]; // 目标键的虚拟键码
                    inputs[1].ki.dwFlags = 0; // 表示按下按键
                    SendInput(2, inputs, sizeof(INPUT));  // 发送按键事件给系统
                    shift_up = true;  // shift被按下

                    // 释放 Shift 键和目标键
                    inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                    inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                    SendInput(2, inputs, sizeof(INPUT));  // 再次发送按键事件给系统
                    shift_up = false;  // shift被释放
                }
                else
                {
                    // 如果是小写字母，则直接模拟按下目标键
                    INPUT input; // 声明一个 INPUT 结构体变量 input，用于描述按键事件

                    input.type = INPUT_KEYBOARD;
                    input.ki.wScan = 0; // 扫描码置为 0，通常不需要使用
                    input.ki.time = 0; // 时间戳置为 0
                    input.ki.dwExtraInfo = 0; // 额外信息置为 0

                    // 模拟按下目标键
                    input.ki.wVk = alphabet_code[alp]; // 指定模拟按下的键为 目标 键
                    input.ki.dwFlags = 0; // 指定键盘按下事件，dwFlags 为 0
                    SendInput(1, &input, sizeof(INPUT)); // 发送按键事件给系统
                    shift_up = true;  // shift被按下

                    // 模拟释放目标键
                    input.ki.dwFlags = KEYEVENTF_KEYUP; // 指定键盘释放事件，dwFlags 设置为 KEYEVENTF_KEYUP
                    SendInput(1, &input, sizeof(INPUT)); // 再次发送按键事件给系统
                    shift_up = false;  // shift被释放
                }
                break;
            }
        }


        for (int syb_no_shift=0; syb_no_shift< symbol_not_shift_len; syb_no_shift++)
        {
            // 是否与传入的参数一样 (判断无需按shift按键的符号)
            if (current_char == symbol_list_not_shift_press[syb_no_shift][0])  // 取出当前字符与中的字符与参数传入的字符进行比较
            {
                INPUT input; // 声明一个 INPUT 结构体变量 input，用于描述按键事件

                input.type = INPUT_KEYBOARD; // 指定 input 的类型为键盘输入
                input.ki.wScan = 0; // 扫描码置为 0，通常不需要使用
                input.ki.time = 0; // 时间戳置为 0
                input.ki.dwExtraInfo = 0; // 额外信息置为 0

                // 模拟按下目标键
                input.ki.wVk = symbol_not_shift_code[syb_no_shift]; // 指定模拟按下的键为 目标 键
                input.ki.dwFlags = 0; // 指定键盘按下事件，dwFlags 为 0
                SendInput(1, &input, sizeof(INPUT)); // 发送按键事件给系统
                shift_up = true;  // shift被按下

                input.ki.wVk = symbol_not_shift_code[syb_no_shift]; // 指定模拟释放的键仍然是 目标 键
                input.ki.dwFlags = KEYEVENTF_KEYUP; // 指定键盘释放事件，dwFlags 设置为 KEYEVENTF_KEYUP
                SendInput(1, &input, sizeof(INPUT)); // 再次发送按键事件给系统
                shift_up = false;  // shift被释放
                break;
            }
            // 处理空格
            if (current_char == ' ')
            {
                // 模拟按下空格键
                INPUT input; // 声明一个 INPUT 结构体变量 input，用于描述按键事件

                input.type = INPUT_KEYBOARD;
                input.ki.wScan = 0; // 扫描码置为 0，通常不需要使用
                input.ki.time = 0; // 时间戳置为 0
                input.ki.dwExtraInfo = 0; // 额外信息置为 0


                input.ki.wVk = VK_SPACE; // 指定模拟按下的键为 空格 键
                input.ki.dwFlags = 0; // 指定键盘按下事件，dwFlags 为 0
                SendInput(1, &input, sizeof(INPUT)); // 发送按键事件给系统
                shift_up = true;  // shift被按下

                // 模拟释放空格键
                input.ki.dwFlags = KEYEVENTF_KEYUP; // 指定键盘释放事件，dwFlags 设置为 KEYEVENTF_KEYUP
                SendInput(1, &input, sizeof(INPUT)); // 再次发送按键事件给系统
                shift_up = false;  // shift被释放
                break;
            }
        }

        for (int syb_need_shift=0; syb_need_shift< symbol_need_shift_len; syb_need_shift++)
        {
            // 是否与传入的参数一样 (判断需按shift按键的符号功能)
            if (current_char == symbol_list_need_shift_press[syb_need_shift][0])  // 取出当前字符与中的字符与参数传入的字符进行比较
            {
                // 创建两个输入结构体并初始化
                INPUT inputs[2] = {0};

                // 第一个按键 'shift' 的输入结构体
                inputs[0].type = INPUT_KEYBOARD;
                inputs[0].ki.wVk = VK_SHIFT; // 'shift'键的虚拟键码
                inputs[0].ki.dwFlags = 0; // 表示按下按键

                // 第二个按键 '目标参数' 的输入结构体
                inputs[1].type = INPUT_KEYBOARD;
                inputs[1].ki.wVk = symbol_need_shift_code[syb_need_shift]; // '目标参数'键的虚拟键码
                inputs[1].ki.dwFlags = 0; // 表示按下按键
                SendInput(2, inputs, sizeof (INPUT));  // 发生指令按下shift与参数按键
                shift_up = true;  // shift被按下

                inputs[0].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                inputs[1].ki.dwFlags = KEYEVENTF_KEYUP; // 表示按键释放
                SendInput(2, inputs, sizeof(INPUT));  // 同时释放
                shift_up = false;  // shift被释放
                break;
            }
        }
    }
    if (!shift_up)
    {
        // 定义INPUT结构体
        INPUT input_shift = {0};
        input_shift.type = INPUT_KEYBOARD;  // 指定输入类型为键盘

        // 模拟释放Shift键
        input_shift.ki.wVk = VK_SHIFT;       // 设置虚拟键码为Shift
        input_shift.ki.dwFlags = KEYEVENTF_KEYUP;  // 指定键盘释放事件

        // 发送键盘释放事件
        SendInput(1, &input_shift, sizeof(INPUT));
    }
}
/*********** 剪切板******************/
// 字符串拷贝辅助哈纳斯 //
void copy_str_in(copy_str_structs self)  // 复制到剪切板的函数，后续包裹在CopyStr函数中
{
    if (OpenClipboard(NULL))
    {
        // 清空剪切板内容
        EmptyClipboard();

        // 分配内存并将文本内容复制到全局内存块
        HGLOBAL hglbCopy = GlobalAlloc(GMEM_MOVEABLE, strlen(self.textToCopy) + 1); // +1是为了包含字符串的结尾 null 字符
        LPVOID lpCopy = NULL;
        if (hglbCopy!= NULL)
        {
            lpCopy = GlobalLock(hglbCopy);
            strcpy((char*)lpCopy, self.textToCopy);
            GlobalUnlock(hglbCopy);

            // 将全局内存块设置为剪切板内容
            SetClipboardData(CF_TEXT, hglbCopy);
        }
        else
        {
            // 如果hglbCopy分配失败，检查lpCopy是否已经被获取并释放
            if (lpCopy!= NULL)
            {
                GlobalFree(lpCopy);
            }
        }

        // 关闭剪切板
        CloseClipboard();
    }
}

// 赋值到剪切板 //
void KeyBoardEvent::CopyStr(const char *str)
{
    // 复制字符窜到剪切板
    copy_str_structs self = {str, copy_str_in};
    self.copy_str_in(self);
}

/*************** 注册快捷键部分 ******************/
// 定义全局变量来保存钩子句柄 //
HHOOK hHook = nullptr;  // HHOOK 类型用于保存钩子句柄;

// 定义一个监听键盘按下与抬起钩子句柄 //
HHOOK hHookDownOrUp = nullptr;  // 钩子句柄

// 获取单个按键虚键码并返回 //
int getOneKeyCode(string key_name)
{
    // 初始化索引值为第一个 //
    int index = 0;

    // 变量字母的列表，查找虚键码 //
    for (auto i:alphabet_list)
    {
        // 如果与传入的字符串相等，截取索引值获得虚键码并返回，反之索引增加 //
        if (key_name == i)
        {
//            cout<<"index in :alphabet_list"<<index<<endl;
            return alphabet_code[index];
        }
        index++;
    }

    // 如果第一轮没有找到，归零索引重新在功能名称找对应虚键码 //
    index = 0;

    // 变量字母的列表，查找虚键码 //
    for (auto i:function_keys_list)
    {
        // 如果与传入的字符串相等，截取索引值获得虚键码并返回，反之索引增加 //
        if (key_name == i)
        {
//            cout<<"index int function_keys_list:"<<index<<endl;
            return function_code[index];
        }
        index++;
    }

    // 如果都找不到说明快捷键注册不合法，返回-1 //
    cout<<"The shortcut name does not exist. Make sure to register a valid shortcut\n";
    return  -1;
}

// 键盘钩子的回调函数
LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam)
{
    for (auto hotKeys:key_code)
    {
        // 检查钩子操作是否成功 //
        if (nCode >= 0)
        {
            // 获取键盘钩子结构体 //
            PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;

//            // 先检查是否有单个快捷键是特殊按键注册 //
//            if (hotKeys.FuncID == 1 && hotKeys.key1 != 0)
//            {
////                printf("键码为: %d\n ctrl虚键码: %d\n", hotKeys.key1, VK_CONTROL);
//                // 检查是否为键盘按下消息，且同时按下注册的快捷键
//                if ((GetAsyncKeyState(hotKeys.key1) & 0x8000))               // 检查第二个键是否按下
//                {
//                    // 回调注册的函数
//                    oneKeyFunc[hotKeys.FuncIndex]();
//                    return CallNextHookEx(nullptr, nCode, wParam, lParam);
//                }
//            }

            // 先注册单个快捷键.并且确保快捷键虚键码有效，ID确保快捷键数量一样，比如单快捷键[a]，双快捷键等待[ctrl+a] //
            if (hotKeys.FuncID==1 &&hotKeys.key1 !=0)
            {
                // 监听键盘按下事件，监听热键结构体列表的key1
                if (wParam == WM_KEYDOWN &&pKey->vkCode == hotKeys.key1)
                {
                    // 回调注册的函数  //
                    oneKeyFunc[hotKeys.FuncIndex]();
//                    cout<<"注册单个快捷键:"<<hotKeys.key1<<" 函数索引为:"<<hotKeys.FuncIndex<<"ID:"<<hotKeys.FuncID<<endl;
                    return CallNextHookEx(nullptr, nCode, wParam, lParam);
                }
            }

                // 双快捷键注册,查询双快捷键结构体的虚键码是否都不为0 //
            else if (hotKeys.FuncID == 2 && hotKeys.key1 != 0 && hotKeys.key2 != 0)
            {
                // 检查是否为键盘按下消息，且同时按下注册的快捷键
                if ((wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) &&  // 监听按键按下,(wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) 优先级判断
                    (GetAsyncKeyState(hotKeys.key1) & 0x8000) &&  // 检查修饰键（如Ctrl或Alt）是否按下
                    (pKey->vkCode == hotKeys.key2))               // 检查第二个键是否按下
                {
                    // 回调注册的函数
                    twoKeyFunc[hotKeys.FuncIndex]();
                    return CallNextHookEx(nullptr, nCode, wParam, lParam);
                }
            }

                // 三快捷键注册,查询三快捷键结构体的虚键码是否都不为0 //
            else if (hotKeys.FuncID == 3 && hotKeys.key1 != 0 && hotKeys.key2 != 0 && hotKeys.key3 != 0)
            {
                // 获取键盘钩子结构体
                PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;

                // 检查是否为键盘按下消息，且同时按下注册的快捷键
                if ((wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) &&  // 监听按键按下
                    (GetAsyncKeyState(hotKeys.key1) & 0x8000) &&  // 检查修饰键1（如Ctrl）是否按下
                    (GetAsyncKeyState(hotKeys.key2) & 0x8000) &&  // 检查修饰键2（如Alt）是否按下
                    (pKey->vkCode == hotKeys.key3))               // 检查第三个键是否按下
                {
                    // 回调注册的函数
                    threeKeyFunc[hotKeys.FuncIndex]();
                    return CallNextHookEx(nullptr, nCode, wParam, lParam);
                }
            }

                // 四快捷键注册,查询四快捷键结构体的虚键码是否都不为0 //
            else if (hotKeys.FuncID == 4 && hotKeys.key1 != 0 && hotKeys.key2 != 0 && hotKeys.key3 != 0 && hotKeys.key4 != 0)
            {
//                printf("k1:%d key2:%d key3:%d key4:%d\n", hotKeys.key1, hotKeys.key2, hotKeys.key3, hotKeys.key4);
                // 获取键盘钩子结构体
                PKBDLLHOOKSTRUCT pKey = (PKBDLLHOOKSTRUCT)lParam;

                // 检查是否为键盘按下消息，且同时按下注册的快捷键
                if ((wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) &&  // 监听按键按下
                    (GetAsyncKeyState(hotKeys.key1) & 0x8000) &&  // 检查修饰键1（如Ctrl）是否按下
                    (GetAsyncKeyState(hotKeys.key2) & 0x8000) &&  // 检查修饰键2（如Alt）是否按下
                    (GetAsyncKeyState(hotKeys.key3) & 0x8000) &&  // 检查修饰键3（如Shift）是否按下
                    (pKey->vkCode == hotKeys.key4))               // 检查第四个键是否按下
                {
                    // 回调注册的函数
                    fourKeyFunc[hotKeys.FuncIndex]();
                    return CallNextHookEx(nullptr, nCode, wParam, lParam);
                }
            }
        }
    }
    return CallNextHookEx(hHook, nCode, wParam, lParam);
}


// 注册一个键盘低级钩子,调用注册函数代替循环 //
//void KeyBoardEvent::AddHotKey(const string &key, void (*targetVoidFunc)())
//{
//    // 获取注册的快捷键名称 //
//    std::regex wordRegex(R"((\w+)+)");
//    // 创建一个正则表达式迭代器，用于在text字符串中搜索匹配wordRegex模式的子串。
//    // text.begin()和text.end()定义了搜索的范围寻找快捷键名称。
//    auto wordBegin = std::sregex_iterator(key.begin(), key.end(), wordRegex);
//
//    // 创建一个结束迭代器，用于在搜索结束时作为迭代器的比较目标。
//    auto wordEnd = std::sregex_iterator();
//
//    // 临时存放虚键码的数组，每次循环开始就重新定义 //
//    vector<int> keyCodeTemp;
//
//    // 使用for循环遍历所有匹配的子串。
//    for (std::sregex_iterator j = wordBegin; j != wordEnd; ++j)
//    {
//        // 获取当前迭代器指向的匹配结果。
//        std::smatch match = *j;
//
//        // 输出匹配到的字母序列，使用match.str()获取匹配的字符串。
////        std::cout << "匹配到的字母序列：" << match.str() << std::endl;
//
//        // 获取虚键码函数，将其推送到临时数组中 //
//        keyCodeTemp.push_back(getOneKeyCode(match.str()));
//    }
//    // 获取数组长度，进行快捷键数组长度判断，并赋值虚键码 //
//    int arrSize = keyCodeTemp.size();
//
//    // 临时虚键码存放//
//    int Ctrl = 0;
//    int Alt = 0;
//    int Shift = 0;
//    int Other = 0;
//
//    //  根据长度进行赋值虚键码，长度为1：case1：等等 //
//    switch (arrSize)
//    {
//        // 给有虚键码赋值虚键码，无的赋值0 //
//        case 1:KeyCode oneKey;
//            oneKey.key1 = keyCodeTemp[0];
//            oneKey.key2 = 0;
//            oneKey.key3 = 0;
//            oneKey.key4 = 0;
//
//            oneKey.FuncIndex = oneKeyIndex;  // 赋值索引后索引值+1，调用注册函数采用先注册的顺序
//            oneKey.FuncID = 1;  // 代表只有一个按键注册
//            key_code.push_back(oneKey);  // 添加快捷键结构体到数组
//            oneKeyFunc.push_back(targetVoidFunc);  // 单快捷键数组增加快捷键函数
//            oneKeyIndex++;  // ID自增
//            break;
//
//            // 双快捷键 //
//        case 2:KeyCode  twoKey;
//            // 如果第二个快捷键为ctrl,alt,shift就纠正顺序 //
//            if (keyCodeTemp[1] ==VK_CONTROL || keyCodeTemp[1] ==VK_SHIFT || keyCodeTemp[1] ==VK_MENU)
//            {
//                // 如果第一个不是ctrl等按键那么将后面的赋值给前面 //
//                twoKey.key1 = keyCodeTemp[1];  // 确保第一个键是ctrl等按键
//                twoKey.key2 = keyCodeTemp[0];
//                twoKey.key3 = 0;
//                twoKey.key4 = 0;
//
//                twoKey.FuncIndex = twoKeyIndex;  // 赋值索引后索引值+1，调用注册函数采用先注册的顺序
//                twoKey.FuncID = 2;  // 代表有两个按键注册
//                key_code.push_back(twoKey);  // 添加快捷键结构体到数组
//                twoKeyFunc.push_back(targetVoidFunc);  // 双快捷键数组增加快捷键函数
//                twoKeyIndex++;  // ID自增
//                break;
//            }
//            twoKey.key1 = keyCodeTemp[0];
//            twoKey.key2 = keyCodeTemp[1];
//            twoKey.key3 = 0;
//            twoKey.key4 = 0;
//
//            twoKey.FuncIndex = twoKeyIndex;  // 赋值索引后索引值+1，调用注册函数采用先注册的顺序
//            twoKey.FuncID = 2;  // 代表有两个按键注册
//            key_code.push_back(twoKey);  // 添加快捷键结构体到数组
//            twoKeyFunc.push_back(targetVoidFunc);  // 双快捷键数组增加快捷键函数
//            twoKeyIndex++;  // ID自增
//            break;
//
//            // 三快捷键 //
//        case 3:KeyCode threeKey;
//            // 遍历虚键码列表调整顺序 //
//            for (const auto &i:keyCodeTemp)
//            {
//                switch (i)
//                {
//                    case VK_CONTROL:
//                        Ctrl = i;
//                        break;
//                    case VK_SHIFT:
//                        Shift = i;
//                        break;
//                    case VK_MENU:
//                        Alt = i;
//                        break;
//                    default:
//                        Other = i;
//                        break;
//                }
//            }
//            if (Ctrl !=0 && Alt !=0 && Shift ==0)
//            {
//                // 如果Ctrl，Alt被注册，Alt没有被注册，按照顺序来 //
//                threeKey.key1 = Ctrl;
//                threeKey.key2 = Alt;
//                threeKey.key3 = Other;
//                threeKey.key4 = 0;
//            }
//            else if (Ctrl !=0 && Shift !=0 && Alt ==0)
//            {
//                // 如果Ctrl，Shift被注册，Alt没有被注册，按照顺序来 //
//                threeKey.key1 = Ctrl;
//                threeKey.key2 = Shift;
//                threeKey.key3 = Other;
//                threeKey.key4 = 0;
//            }
//            else if (Alt !=0 && Shift !=0 && Ctrl ==0)
//            {
//                // 如果Alt，Shift被注册,Ctrl没有被注册，按照顺序来 //
//                threeKey.key1 = Alt;
//                threeKey.key2 = Shift;
//                threeKey.key3 = Other;
//                threeKey.key4 = 0;
//            }
//            threeKey.FuncIndex = threeKeyIndex;  // 赋值索引后索引值+1，调用注册函数采用先注册的顺序
//            threeKey.FuncID = 3;  // 代表有三个按键注册
//            key_code.push_back(threeKey);  // 添加快捷键结构体到数组
//            threeKeyFunc.push_back(targetVoidFunc);  // 三快捷键数组增加快捷键函数
//            threeKeyIndex++;  // ID自增
//            break;
//
//            // 四快捷键 //
//        case 4:KeyCode fourKey;
//            // 遍历虚键码列表调整顺序 //
//            for (const auto &i:keyCodeTemp)
//            {
//                switch (i)
//                {
//                    case VK_CONTROL:
//                        Ctrl = i;
//                        cout<<"ctrl:"<<Ctrl<<endl;
//                        break;
//                    case VK_SHIFT:
//                        Shift = i;
//                        cout<<"shift:"<<Shift<<endl;
//                        break;
//                    case VK_MENU:
//                        Alt = i;
//                        cout<<"alt:"<<Alt<<endl;
//                        break;
//                    default:
//                        Other = i;
//                        cout<<"other:"<<Other<<endl;
//                        break;
//                }
//            }
//            printf("\n");
//
//            if (Ctrl !=0 && Alt !=0 && Shift !=0)
//            {
//                // 如果Ctrl,Alt,Shift都被注册 //
//                fourKey.key1 = Ctrl;
//                fourKey.key2 = Alt;
//                fourKey.key3 = Shift;
//                fourKey.key4 = Other;
//            }
//
//            fourKey.FuncIndex = fourKeyIndex;  // 赋值索引后索引值+1，调用注册函数采用先注册的顺序
//            fourKey.FuncID = 4; // 代表有四个按键注册
//            key_code.push_back(fourKey);  // 添加快捷键结构体到数组
//            fourKeyFunc.push_back(targetVoidFunc);  // 四快捷键数组增加快捷键函数
//            fourKeyIndex++;  // ID自增
//            break;
//
//    }
//}


// 监听快捷键使用wait hot key 回调KeyboardProc函数 //
void KeyBoardEvent::AddHotKey(const string &key, std::function<void()> targetVoidFunc)
{
    // 获取注册的快捷键名称 //
    std::regex wordRegex(R"((\w+)+)");
    // 创建一个正则表达式迭代器，用于在text字符串中搜索匹配wordRegex模式的子串。
    // text.begin()和text.end()定义了搜索的范围寻找快捷键名称。
    auto wordBegin = std::sregex_iterator(key.begin(), key.end(), wordRegex);

    // 创建一个结束迭代器，用于在搜索结束时作为迭代器的比较目标。
    auto wordEnd = std::sregex_iterator();

    // 临时存放虚键码的数组，每次循环开始就重新定义 //
    vector<int> keyCodeTemp;

    // 使用for循环遍历所有匹配的子串。
    for (std::sregex_iterator j = wordBegin; j != wordEnd; ++j)
    {
        // 获取当前迭代器指向的匹配结果。
        std::smatch match = *j;

        // 输出匹配到的字母序列，使用match.str()获取匹配的字符串。
//        std::cout << "匹配到的字母序列：" << match.str() << std::endl;

        // 获取虚键码函数，将其推送到临时数组中 //
        keyCodeTemp.push_back(getOneKeyCode(match.str()));
    }
    // 获取数组长度，进行快捷键数组长度判断，并赋值虚键码 //
    int arrSize = keyCodeTemp.size();

    // 临时虚键码存放//
    int Ctrl = 0;
    int Alt = 0;
    int Shift = 0;
    int Other = 0;

    //  根据长度进行赋值虚键码，长度为1：case1：等等 //
    switch (arrSize)
    {
        // 给有虚键码赋值虚键码，无的赋值0 //
        case 1:KeyCode oneKey;
            oneKey.key1 = keyCodeTemp[0];
            oneKey.key2 = 0;
            oneKey.key3 = 0;
            oneKey.key4 = 0;

            oneKey.FuncIndex = oneKeyIndex;  // 赋值索引后索引值+1，调用注册函数采用先注册的顺序
            oneKey.FuncID = 1;  // 代表只有一个按键注册
            key_code.push_back(oneKey);  // 添加快捷键结构体到数组
            oneKeyFunc.push_back(targetVoidFunc);  // 单快捷键数组增加快捷键函数
            oneKeyIndex++;  // ID自增
            if (keyCodeTemp[0] == VK_CONTROL)
            {
                printf("ctrl %d\n", keyCodeTemp[0]);
            }
            if (keyCodeTemp[0] == VK_SHIFT)
            {
                printf("shift %d\n", keyCodeTemp[0]);
            }
            if (keyCodeTemp[0] == VK_MENU)
            {
                printf("alt %d\n", keyCodeTemp[0]);
            }
            break;

            // 双快捷键 //
        case 2:KeyCode  twoKey;
            // 如果第二个快捷键为ctrl,alt,shift就纠正顺序 //
            if (keyCodeTemp[1] ==VK_CONTROL || keyCodeTemp[1] ==VK_SHIFT || keyCodeTemp[1] ==VK_MENU)
            {
                // 如果第一个不是ctrl等按键那么将后面的赋值给前面 //
                twoKey.key1 = keyCodeTemp[1];  // 确保第一个键是ctrl等按键
                twoKey.key2 = keyCodeTemp[0];
                twoKey.key3 = 0;
                twoKey.key4 = 0;

                twoKey.FuncIndex = twoKeyIndex;  // 赋值索引后索引值+1，调用注册函数采用先注册的顺序
                twoKey.FuncID = 2;  // 代表有两个按键注册
                key_code.push_back(twoKey);  // 添加快捷键结构体到数组
                twoKeyFunc.push_back(targetVoidFunc);  // 双快捷键数组增加快捷键函数
                twoKeyIndex++;  // ID自增
                break;
            }
            twoKey.key1 = keyCodeTemp[0];
            twoKey.key2 = keyCodeTemp[1];
            twoKey.key3 = 0;
            twoKey.key4 = 0;

            twoKey.FuncIndex = twoKeyIndex;  // 赋值索引后索引值+1，调用注册函数采用先注册的顺序
            twoKey.FuncID = 2;  // 代表有两个按键注册
            key_code.push_back(twoKey);  // 添加快捷键结构体到数组
            twoKeyFunc.push_back(targetVoidFunc);  // 双快捷键数组增加快捷键函数
            twoKeyIndex++;  // ID自增
            break;

            // 三快捷键 //
        case 3:KeyCode threeKey;
            // 遍历虚键码列表调整顺序 //
            for (const auto &i:keyCodeTemp)
            {
                switch (i)
                {
                    case VK_CONTROL:
                        Ctrl = i;
                        break;
                    case VK_SHIFT:
                        Shift = i;
                        break;
                    case VK_MENU:
                        Alt = i;
                        break;
                    default:
                        Other = i;
                        break;
                }
            }
            if (Ctrl !=0 && Alt !=0 && Shift ==0)
            {
                // 如果Ctrl，Alt被注册，Shift没有被注册，按照顺序来 //
                threeKey.key1 = Ctrl;
                threeKey.key2 = Alt;
                threeKey.key3 = Other;
                threeKey.key4 = 0;
            }
            else if (Ctrl !=0 && Shift !=0 && Alt ==0)
            {
                // 如果Ctrl，Shift被注册，Alt没有被注册，按照顺序来 //
                threeKey.key1 = Ctrl;
                threeKey.key2 = Shift;
                threeKey.key3 = Other;
                threeKey.key4 = 0;
            }
            else if (Alt !=0 && Shift !=0 && Ctrl ==0)
            {
                // 如果Alt，Shift被注册,Ctrl没有被注册，按照顺序来 //
                threeKey.key1 = Alt;
                threeKey.key2 = Shift;
                threeKey.key3 = Other;
                threeKey.key4 = 0;
            }
            threeKey.FuncIndex = threeKeyIndex;  // 赋值索引后索引值+1，调用注册函数采用先注册的顺序
            threeKey.FuncID = 3;  // 代表有三个按键注册
            key_code.push_back(threeKey);  // 添加快捷键结构体到数组
            threeKeyFunc.push_back(targetVoidFunc);  // 三快捷键数组增加快捷键函数
            threeKeyIndex++;  // ID自增
            break;

            // 四快捷键 //
        case 4:KeyCode fourKey;
            // 遍历虚键码列表调整顺序 //
            for (const auto &i:keyCodeTemp)
            {
                switch (i)
                {
                    case VK_CONTROL:
                        Ctrl = i;
                        cout<<"ctrl:"<<Ctrl<<endl;
                        break;
                    case VK_SHIFT:
                        Shift = i;
                        cout<<"shift:"<<Shift<<endl;
                        break;
                    case VK_MENU:
                        Alt = i;
                        cout<<"alt:"<<Alt<<endl;
                        break;
                    default:
                        Other = i;
                        cout<<"other:"<<Other<<endl;
                        break;
                }
            }
            printf("\n");

            if (Ctrl !=0 && Alt !=0 && Shift !=0)
            {
                // 如果Ctrl,Alt,Shift都被注册 //
                fourKey.key1 = Ctrl;
                fourKey.key2 = Alt;
                fourKey.key3 = Shift;
                fourKey.key4 = Other;
            }

            fourKey.FuncIndex = fourKeyIndex;  // 赋值索引后索引值+1，调用注册函数采用先注册的顺序
            fourKey.FuncID = 4; // 代表有四个按键注册
            key_code.push_back(fourKey);  // 添加快捷键结构体到数组
            fourKeyFunc.push_back(targetVoidFunc);  // 四快捷键数组增加快捷键函数
            fourKeyIndex++;  // ID自增
            break;

    }
}



// 等待快捷键 //
void KeyBoardEvent::WaitHotKey()
{
    // 监听快捷键循环函数
    hHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, GetModuleHandle(NULL), 0);  // 安装低级键盘钩子

    MSG msg;          // MSG 结构体用于存储从消息队列中获取的消息


    // 进入消息循环
    while (GetMessage(&msg, NULL, 0, 0))  // 从消息队列中获取消息
    {
        TranslateMessage(&msg);           // 将虚拟键码转换成字符
        DispatchMessage(&msg);            // 分发消息到窗口过程
    }
}


// 函数为 static ClearHotKey 类成员成员函数调用清理快捷键函数 //
void KeyBoardEvent::ClearHotKey()
{
    if (hHook != NULL || hHook!= nullptr)
    {
        // 确保钩子被卸载
        UnhookWindowsHookEx(hHook);       // 卸载钩子
        // 发送程序结束消息
        PostQuitMessage(0);
        hHook = nullptr;  // 重置钩子句柄
//        cout<<"钩子已卸载\n";
    }
}



/**************** 清理工作 ******************/
void cleanup_check()
{
//    cout<<"开始清理工作\n";
    // 清理未释放的鼠标 //
    MouseEvent mouse;
    // 释放鼠标，如果鼠标按下 //
    if (mouse_flag.mouse_left_down)
    {
        // 释放左键
        mouse.MouseUp("left");
        mouse_flag.mouse_left_down = False;  // 更新标志
//        cout<<"左键已释放\n";
    }
    else if(mouse_flag.mouse_right_down)
    {
        // 释放右键
        mouse.MouseUp("right");
        mouse_flag.mouse_right_down = False;  // 更新标志
//        cout<<"右键已释放\n";
    }

    // 清理未释放的键盘 //
    // 释放 Ctrl、Alt、Shift 和 Win 键
    ////// 释放键盘 //////
    int len = sizeof (free_keys.key_code)/sizeof (free_keys.key_code[0]);
    for (int f=0; f<len; f++)
    {
        if (free_keys.key_code[f] !=0)
        {
            INPUT input = {0};
            input.type = INPUT_KEYBOARD;
            input.ki.wVk = free_keys.key_code[f];  // 键位吗码
            input.ki.dwFlags = KEYEVENTF_KEYUP;
            SendInput(1, &input, sizeof(INPUT));
//            printf("释放忘记释放的按键: %d \n", free_keys.key_code[f]);
        }
    }

    // 检查鼠标事件钩子是否被注销 //
    if (hMouseHook != NULL || hMouseHook!= nullptr)
    {
        // 检查全局变量hMouseHook是否有效
        if (hMouseHook != NULL)
        {
            // 移除鼠标钩子
            UnhookWindowsHookEx(hMouseHook);
            hMouseHook = NULL; // 将全局变量置为空
            // 发送程序结束消息
        }
//    exit(0); // 退出程序
    }

    // 重新检查键盘快捷键hHook是否被注销 //
    if (hHook != NULL || hHook!= nullptr)
    {
        // 确保钩子被卸载
        UnhookWindowsHookEx(hHook);       // 卸载钩子
        // 发送程序结束消息
        PostQuitMessage(0);
        hHook = nullptr;  // 重置钩子句柄
    }

}

// 清理函数调用 //
void ExitCheckWork()
{
    atexit(cleanup_check);  // 退出所执行的任务
}

void winKeyPressHelp()
{
    string user_help = R"(
# WinKeyPress 库文档

## 概述
该库提供对Windows系统鼠标和键盘事件的模拟操作，支持屏幕信息获取、光标控制、快捷键注册等功能。适用于自动化脚本、热键管理及输入模拟场景。

---

## 头文件引入
```cpp
#include "winKeyPressH.hpp"
using namespace std;
```

---

## 鼠标操作 (MouseEvent)

### 初始化
```cpp
MouseEvent mouse;
```

### 方法列表

#### 1. 获取屏幕分辨率
```cpp
ScreenSize GetScreenSIze(bool output = true);
```
- **参数**：`output` 是否输出分辨率信息到控制台
- **返回**：`ScreenSize`结构体 (包含 `x`, `y` 属性)
```cpp
ScreenSize screen = GetScreenSIze();
cout << "Width: " << screen.x << ", Height: " << screen.y;
```

#### 2. 获取鼠标位置
```cpp
MousePosition GetMousePosition(bool output = true);
```
- **返回**：`MousePosition`结构体 (包含 `x`, `y` 属性)
```cpp
MousePosition pos = GetMousePosition();
mouse.MouseMoveTo(pos.x + 100, pos.y); // 右移100像素
```

#### 3. 移动鼠标
```cpp
void MouseMoveTo(int x, int y);
```
```cpp
mouse.MouseMoveTo(500, 300); // 移动到坐标(500,300)
```

#### 4. 鼠标点击
```cpp
void MouseClick(int x, int y, int clicks = 1, const string& button = "left");
```
- **参数**：
  - `x`, `y`: 目标坐标
  - `clicks`: 点击次数
  - `button`: "left"(默认)/"right"
```cpp
mouse.MouseClick(100, 200, 2); // 左键双击
mouse.MouseClick(300, 400, 1, "right"); // 右键单击
```

#### 5. 按下/释放鼠标键
```cpp
void MouseDown(const string& button = "left");
void MouseUp(const string& button = "left");
```
```cpp
mouse.MouseDown(); // 按下左键
sleep(1);
mouse.MouseUp();   // 释放左键
```

#### 6. 滚轮滚动
```cpp
void MouseRoll(int move);
```
- **参数**：正数向上滚动，负数向下
```cpp
mouse.MouseRoll(120);  // 向上滚动
mouse.MouseRoll(-120); // 向下滚动
```

#### 7. 监听鼠标事件
```cpp
void ListenMouseEvent();
```
- **阻塞式监听**，需在独立线程运行
```cpp
thread t([&](){ mouse.ListenMouseEvent(); });
t.detach();
```

---

## 键盘操作 (KeyBoardEvent)

### 初始化
```cpp
KeyBoardEvent keyboard;
```

### 方法列表

#### 1. 按键按下/释放
```cpp
void KeyDown(const string& key);
void KeyUp(const string& key);
```
- **支持按键名**：字母、数字、"ctrl", "alt", "shift", "F1"-"F12", "esc", "space"等
```cpp
keyboard.KeyDown("ctrl");
keyboard.KeyDown("c");
keyboard.KeyUp("c");
keyboard.KeyUp("ctrl"); // 模拟Ctrl+C
```

#### 2. 单次按键
```cpp
void PressKey(const string& key);
```
```cpp
keyboard.PressKey("enter"); // 按下并释放回车
```

#### 3. 组合键
```cpp
void PressHotKey(const string& key1, const string& key2, const string& key3 = "NULL");
```
```cpp
keyboard.PressHotKey("ctrl", "shift", "esc"); // Ctrl+Shift+Esc
```

#### 4. 输入字符串
```cpp
void WriteStr(string output);
```
```cpp
keyboard.WriteStr("Hello World!"); // 模拟输入
```

#### 5. 剪贴板操作
```cpp
void CopyStr(const char *str);
```
```cpp
keyboard.CopyStr("Copied text"); // 复制到剪贴板
```

#### 6. 注册快捷键
```cpp
void AddHotKey(const string &key, std::function<void()> targetVoidFunc);
void WaitHotKey();
```
- **快捷键格式**：用空格分隔修饰键和主键，如 `"ctrl alt s"`
```cpp
keyboard.AddHotKey("ctrl k", [](){
    cout << "快捷键触发！" << endl;
});
keyboard.WaitHotKey(); // 阻塞监听，需在独立线程运行
```

---

## 清理与注意事项

### 自动清理
- 构造函数自动注册退出清理函数，释放未松开的按键。
- 手动调用清理：
  ```cpp
  KeyBoardEvent::ClearHotKey();
  MouseEvent::ExitMouseEvent();
  ```

### 多线程建议
- 监听函数 (`ListenMouseEvent`, `WaitHotKey`) 需在独立线程运行以避免阻塞主程序。

---

## 示例代码

```cpp
#include <thread>
#include "winKeyPressH.hpp"

int main() {
    // 鼠标示例
    MouseEvent mouse;
    mouse.MouseClick(500, 300); // 点击屏幕中心

    // 键盘示例
    KeyBoardEvent keyboard;
    keyboard.WriteStr("自动输入测试");

    // 快捷键注册
    keyboard.AddHotKey("ctrl shift q", [](){
        cout << "退出程序" << endl;
        exit(0);
    });

    thread hotkeyThread([&](){ keyboard.WaitHotKey(); });
    hotkeyThread.join();

    return 0;
}
```
)";
    printf("%s\n", user_help.c_str());
}