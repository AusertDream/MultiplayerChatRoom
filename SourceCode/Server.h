#pragma once
#include "framework.h"
#include "InitWindows.h"
#include "SomeTools.h"
using namespace std;
typedef long long ll;

#define MAX_NUMBER  101

//存放当前在线用户
extern set<string> UserList;
//存放当前所有线程
extern vector<thread> allThread;
//存放当前所有socket链接
extern vector<SOCKET> allSocket;
//将线程ID单一映射到用户姓名上
extern map<thread::id, string> ID_UserName;

//服务端文件
// 
//初始化网络
extern SOCKET InitServer();
//处理收发的函数
extern void SendRecvProc(SOCKET);
 //发送函数
extern void OnSend(SOCKET, UserParam);
 //处理登录函数
extern void OnLogIn(SOCKET, UserParam);
 //处理登出函数
extern void OnLogOut(SOCKET, UserParam);

//启动服务函数
extern void StartServer();
