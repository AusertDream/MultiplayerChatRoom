#pragma once

#include "framework.h"
#include "SourceCode.h"
using namespace std;

extern HANDLE hStdOutput; //添加DOS控制台窗口用于DEBUG
#define WM_SUCCESSLOGIN WM_USER+1011  //成功登录的消息
#define WM_UPDATETEXTSHOWWINDOW WM_USER+1012 //更新聊天记录的消息
#define BUTTONLOGIN   1 
#define BUTTONTEST   2 
#define BUTTONSENDMESSAGE 3
#define USERLIST 4
#define TOOLBAR 5
#define KAOMOJI 6
#define MAX_NUMBER  101
#define MAX_BUFFER_SIZE 1024
#define MAX_TEST_NUMBER 10
//所有的窗口句柄
extern HWND hMainWindow, hSubMainWindow, hLoginInput, hLoginButton, hTextInput, hUserList, hSendButton, hToolBar, hTextShowWindow;
extern HWND hTextAlias, hStartServer;

//注册创建所有的窗口






