#pragma once

#include "framework.h"
#include "SourceCode.h"
using namespace std;

extern HANDLE hStdOutput; //添加DOS控制台窗口用于DEBUG
#define WM_SUCCESSLOGIN WM_USER+1011  //成功登录的消息
#define BUTTONLOGIN   1 
#define BUTTONSERVER   2 
#define BUTTONSENDMESSAGE 3
//所有的窗口句柄
extern HWND hMainWindow, hSubMainWindow, hLoginInput, hLoginButton, hTextInput, hUserList, hSendButton, hToolBar, hTextShowWindow;
extern HWND hTextAlias, hStartServer;

//注册创建所有的窗口






