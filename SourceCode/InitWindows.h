#pragma once

#include "framework.h"
#include "SourceCode.h"
using namespace std;


extern HANDLE hStdOutput; //添加DOS控制台窗口用于DEBUG
#define WM_SUCCESSLOGIN WM_USER+1011  //成功登录的消息
//所有的窗口句柄
extern HWND hMainWindow, hSubMainWindow, hLoginInput, hLoginButton, hTextInput, hUserList, hSendButton, hToolBar, hTextShowWindow;
extern HWND hTextAlias, hStartServer;

//注册创建所有的窗口


HWND CreateMainWindow(HINSTANCE hIns); //创建主窗口
HWND CreateSubMainWindow(HINSTANCE hIns); //创建子主窗口
HWND CreateLoginInput(HINSTANCE hIns); //创建用户名输入框
HWND CreateLoginButton(HINSTANCE hIns); //创建登录按钮
HWND CreateTextInput(HINSTANCE hIns); //创建输入框
HWND CreateUserList(HINSTANCE hIns); //创建用户列表
HWND CreateSendButton(HINSTANCE hIns); //创建发送按钮
HWND CreateToolBar(HINSTANCE hIns); //创建工具栏
HWND CreateTextShowWindow(HINSTANCE hIns); //创建聊天记录显示框
HWND CreateTextAlias(HINSTANCE hIns);//创建昵称提示文字
HWND CreateStartServer(HINSTANCE hIns);//创建启动服务器的按钮
LRESULT CALLBACK MyWndProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM LParam);//窗口处理函数，处理消息。
void func();
void OnCommand(WPARAM wParam); //处理菜单被点击的操作



