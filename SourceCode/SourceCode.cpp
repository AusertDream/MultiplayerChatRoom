// SourceCode.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "SourceCode.h" //导入资源
#include "InitWindows.h"
using namespace std;



int WINAPI WinMain(
	HINSTANCE hIns, //当前程序实例句柄
	HINSTANCE hPreIns, //现已废弃不用，但必须标注出来
 	LPSTR LpCmdLine, //命令行参数
	int nCmdShow //窗口显示方式
	)
{

	/*
	AllocConsole();//启用DOS窗口
	hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);//获取标准输出句柄。
	char* text = "放假了";
	WriteConsole(hStdOutput, text, strlen(text), NULL, NULL); //在控制台上输出内容
	*/

	hMainWindow = CreateMainWindow(hIns); //创建主窗口
	hSubMainWindow = CreateSubMainWindow(hIns); //创建子主窗口
	hLoginInput = CreateLoginInput(hIns); //创建用户名输入框 
	hLoginButton = CreateLoginButton(hIns); //创建登录按钮
	hTextInput = CreateTextInput(hIns); //创建输入框
	hUserList = CreateUserList(hIns); //创建用户列表
	hSendButton = CreateSendButton(hIns); //创建发送按钮
	hToolBar = CreateToolBar(hIns); //创建工具栏
	hTextShowWindow = CreateTextShowWindow(hIns); //创建聊天记录显示框
	hTextAlias = CreateTextAlias(hIns);//创建昵称提示文本
	hStartServer = CreateStartServer(hIns);//创建启动服务器的按钮

	//设置按钮的文字内容
	SetWindowText(hLoginButton, "登录");
	SetWindowText(hStartServer, "启动服务器");

	//TextOut(hdc, 100, 100, "hello", 5);

	//PostMessage(hMainWindow, WM_SUCCESSLOGIN, NULL, NULL); //向mainWindow发送成功登录的消息
	
	//显示窗口。
	ShowWindow(hMainWindow,SW_SHOW); //第一个参数传入创建窗口返回的句柄，第二个参数传入显示的方式。SW_SHOW为默认显示。
	UpdateWindow(hMainWindow);//刷新创建的窗口。

	//消息循环
	MSG nMsg = { 0 }; //消息结构体
	/*
	while (GetMessage(&nMsg, NULL, 0, 0)) { //获取消息
		//getmessage只有抓到WM_QUIT消息才会返回0
		//GetMessage第一个参数为存放消息的地方
		//第二个为设置窗口句柄，如果设置了某个句柄，那么GetMessage只抓这个句柄的窗口的消息，设置为NULL则所有的窗口消息都抓。
		//第三个为获取消息的最小ID
		//第四个为获取消息的最大ID。即只抓消息ID在min~max的消息，如果全部置0，则抓所有的。
		TranslateMessage(&nMsg); //翻译消息
		DispatchMessage(&nMsg); //把消息发给消息处理函数来处理。
	}
	*/

	
	while (true) {
		if (PeekMessage(&nMsg, NULL, 0, 0, PM_NOREMOVE)) {
			//忙处理
			if (GetMessage(&nMsg, NULL, 0, 0)) { //如果消息非QUIT消息
				TranslateMessage(&nMsg); //翻译消息
				DispatchMessage(&nMsg); //把消息发给消息处理函数来处理。
			}
			else {//如果是QUIT消息，说明需要结束程序。
				break;
			}

		}
		else {
			//空闲处理

		}

	}
	return 0;
}

