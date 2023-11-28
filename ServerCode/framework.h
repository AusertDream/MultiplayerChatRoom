// header.h: 标准系统包含文件的包含文件，
// 或特定于项目的包含文件
//

#pragma once

#define WIN32_LEAN_AND_MEAN             // 从 Windows 头文件中排除极少使用的内容
#define _WINSOCK_DEPRECATED_NO_WARNINGS //取消掉使用inet_addr函数的警告
#define _CRT_SECURE_NO_WARNINGS //取消掉不建议使用sprintf而是使用sprintf_s的警告
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS //取消掉使用wstring_convert的警告
#define WM_SUCCESSLOGIN WM_USER+1011  //成功登录的消息
#define BUTTONLOGIN   1 
#define BUTTONSERVER   2 
#define BUTTONSENDMESSAGE 3
// Windows 头文件
#include <windows.h>
// C++ 运行时头文件
#include <stdlib.h>
#include <stdio.h>
#include <atltime.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <thread>
#include <semaphore>
#include <winSock2.h>
#pragma comment(lib,"ws2_32.lib")
#include <queue>
#include <stack>
#include <map>
#include <set>
#include<random>
#include<mutex>
#include <locale>
#include <codecvt>
#include <richedit.h> // 需要包含 richedit.h 头文件





