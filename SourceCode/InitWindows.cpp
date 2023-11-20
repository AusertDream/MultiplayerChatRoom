#pragma once
#include "framework.h"
#include "InitWindows.h"
#include "SourceCode.h" //导入资源
using namespace std;

HANDLE hStdOutput = 0; //添加DOS控制台窗口用于DEBUG

//#define WM_MYMSG WM_USER+1011 自定义消息类型

//所有窗口的句柄
HWND hMainWindow, hSubMainWindow, hLoginInput, hLoginButton, hTextInput, hUserList, hSendButton, hToolBar, hTextShowWindow;
HWND hTextAlias, hStartServer;

//以下为各窗口的实现过程，以及窗口处理函数的具体处理。

//基于DOS的DEBUG函数
void func() {
	char* text = "dfljaie";
	WriteConsole(hStdOutput, text, strlen(text), NULL, NULL);
}

//处理菜单被点击的操作
void OnCommand(WPARAM wParam) {
	//LOWORD获取点击的菜单的ID
	switch (LOWORD(wParam)) {
	case ID_CN:
		//当点击中国区域的聊天室的时候
		//模式对话框，程序会阻塞在这里
		DialogBox(NULL, (char*)IDD_DIALOG1, NULL, DefWindowProc); 
		//无模式对话框，不会发生阻塞。但是想要显示还需要使用showwindow函数。然而实际上还是阻塞（（（存疑
		//HWND hDialog = CreateDialog(NULL, (char*)IDD_DIALOG1, hMainWindow, DefWindowProc);
		//ShowWindow(hDialog, SW_SHOW);


		break;
	case ID_GLOBAL:
		//当点击全球聊天室的时候


		break;
	}
}

LRESULT CALLBACK SubMainWindowProc(
	HWND hWnd, //窗口句柄
	UINT msgID, //消息ID
	WPARAM wParam, //消息参数1
	LPARAM LParam //消息参数2
) {
	switch (msgID) {
	case WM_PAINT:
		//绘制欢迎文本
		//绘制过程
		PAINTSTRUCT ps;
		//开始绘制
		HDC hdc = BeginPaint(hWnd, &ps);
		// 设置字体
		HFONT hFont = CreateFont(20, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE, "Arial");
		//设置对象
		SelectObject(hdc, hFont);
		// 输出文字
		TextOut(hdc, 155, 20, "欢迎来到多人聊天室", strlen("欢迎来到多人聊天室"));
		// 清除字体
		DeleteObject(hFont);
		//结束绘制
		EndPaint(hWnd, &ps);
		break;
	}
	return DefWindowProc(hWnd, msgID, wParam, LParam);
}

LRESULT CALLBACK StartServerButtonProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM LParam) {
	switch (msgID)
	{
	case WM_COMMAND:
		MessageBox(NULL, "Button is pressed", "y", MB_OK);
		break;
	}
	return DefWindowProc(hWnd, msgID, wParam, LParam);
}


LRESULT CALLBACK MainWindowProc(
	HWND hWnd, //窗口句柄
	UINT msgID, //消息ID
	WPARAM wParam, //消息参数
	LPARAM LParam //消息参数
)
{
	switch (msgID) {
	case WM_COMMAND: //处理菜单被点击的操作
		OnCommand(wParam);
		break;
	case WM_SUCCESSLOGIN:
		//当得到成功登录的消息之后，才将菜单挂上。
		//使用setmenu函数来挂菜单。
		SetMenu(hMainWindow, LoadMenu(NULL, (char*)IDRETRY));
		break;
	case WM_DESTROY: //当关闭总窗口的时候，将进程也结束掉
		PostQuitMessage(0); //当msgid是WM_DESTROY的值的时候，则将进程结束。
		break;
	case WM_SYSCOMMAND:
		//当点击右上角的×的时候，弹出提示框，确认关闭吗
		if (wParam == SC_CLOSE) {
			int mRet = MessageBox(hWnd, "真的要退出吗QAQ", "警告", MB_YESNO);
			if (mRet == IDYES) {
				//如果确认，那么什么都不做，交给默认处理函数处理，关闭窗口
			}
			else {
				//如果否认，那么提前结束函数，阻止默认处理函数关闭窗口
				return 0;
			}
		}
		break;
	}
	return DefWindowProc(hWnd, msgID, wParam, LParam);
}

HWND CreateMainWindow(HINSTANCE hIns)
{
	WNDCLASS MainWindow = { 0 }; //创建一个窗口类 名字叫MainWindow
	MainWindow.cbClsExtra = 0;// 要根据窗口类结构分配的额外字节数，设置需要申请的缓冲区。
	MainWindow.cbWndExtra = 0; //在窗口实例之后分配的额外字节数，设置需要申请的缓冲区。
	MainWindow.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//设置窗口的背景色，COLOR_WINDOW+1为白色。
	MainWindow.hCursor = NULL;//设置窗口光标，设置为NULL为默认光标。
	MainWindow.hIcon = NULL;//设置窗口图标，NULL为默认图标。
	MainWindow.hInstance = hIns; //当前程序实例句柄
	MainWindow.lpfnWndProc = MainWindowProc; //设置当前窗口的处理函数，传入处理函数的函数名即可。
	MainWindow.lpszClassName = "MainWindow";//设置窗口类的名字。
	MainWindow.lpszMenuName = NULL; //设置窗口是否有菜单，NULL为没有菜单。
	MainWindow.style = CS_HREDRAW | CS_VREDRAW; //设置窗口风格，CS_HREDRAW和CS_VREDRAW为当窗口水平或者垂直大小变化时，重画窗口

	//注册窗口，向操作系统中写入窗口信息。
	RegisterClass(&MainWindow);

	//在内存中创建窗口
	HWND hMainWindowRes = CreateWindowEx(
		0,//窗口的额外风格设置
		"MainWindow",//创建的窗口类的名字
		"Ausert的多人聊天室",//窗口的左上角标题文字
		WS_OVERLAPPEDWINDOW,//设置窗口的基本风格。
		200,//窗口的初始x坐标
		200,//窗口的初始y坐标
		500,//窗口的初始宽度
		300,//窗口的初始高度
		NULL,//设置父窗口，没有置空NULL
		NULL,//设置菜单，没有菜单为NULL
		hIns,//当前程序实例句柄
		NULL//没啥用的参数
	);
	//将窗口设置为不可调整大小。
	SetWindowLongPtr(hMainWindowRes, GWL_STYLE, WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX);
	return hMainWindowRes;
}

HWND CreateSubMainWindow(HINSTANCE hIns)
{
	WNDCLASS SubMainWindow = { 0 }; //创建一个窗口类 名字叫SubMainWindow
	SubMainWindow.cbClsExtra = 0;// 要根据窗口类结构分配的额外字节数，设置需要申请的缓冲区。
	SubMainWindow.cbWndExtra = 0; //在窗口实例之后分配的额外字节数，设置需要申请的缓冲区。
	SubMainWindow.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//设置窗口的背景色，COLOR_WINDOW+1为白色。
	SubMainWindow.hCursor = NULL;//设置窗口光标，设置为NULL为默认光标。
	SubMainWindow.hIcon = NULL;//设置窗口图标，NULL为默认图标。
	SubMainWindow.hInstance = hIns; //当前程序实例句柄
	SubMainWindow.lpfnWndProc = SubMainWindowProc; //设置当前窗口的处理函数，传入处理函数的函数名即可。
	SubMainWindow.lpszClassName = "SubMainWindow";//设置窗口类的名字。
	SubMainWindow.lpszMenuName = NULL; //设置窗口是否有菜单，NULL为没有菜单。
	SubMainWindow.style = CS_HREDRAW | CS_VREDRAW; //设置窗口风格，CS_HREDRAW和CS_VREDRAW为当窗口水平或者垂直大小变化时，重画窗口

	//注册窗口，向操作系统中写入窗口信息。
	RegisterClass(&SubMainWindow);


	HWND hSubMainWindowRes = CreateWindowEx(
		0,
		"SubMainWindow",
		NULL,
		WS_CHILD | WS_VISIBLE,
		0,
		0,
		500,
		300,
		hMainWindow,
		NULL,
		hIns,
		NULL
	);

	return hSubMainWindowRes;
}



HWND CreateLoginInput(HINSTANCE hIns)
{

	HWND hLoginInputRes = CreateWindowEx(
		0,
		"EDIT",
		NULL,
		WS_BORDER | WS_CHILD | WS_VISIBLE | ES_LEFT,
		140,
		100,
		260,
		25,
		hSubMainWindow,
		NULL,
		hIns,
		NULL
	);
	return hLoginInputRes;
}

HWND CreateLoginButton(HINSTANCE hIns)
{


	HWND hLoginButtonRes = CreateWindowEx(
		0, //使用该扩展风格
		"BUTTON",
		NULL,
		WS_CHILD | WS_VISIBLE,
		130,
		170,
		200,
		25,
		hSubMainWindow,
		NULL,
		hIns,
		NULL
	);

	return hLoginButtonRes;
}

HWND CreateTextInput(HINSTANCE hIns)
{
	WNDCLASS TextInput = { 0 }; //创建一个窗口类 名字叫TextInput
	TextInput.cbClsExtra = 0;// 要根据窗口类结构分配的额外字节数，设置需要申请的缓冲区。
	TextInput.cbWndExtra = 0; //在窗口实例之后分配的额外字节数，设置需要申请的缓冲区。
	TextInput.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//设置窗口的背景色，COLOR_WINDOW+1为白色。
	TextInput.hCursor = NULL;//设置窗口光标，设置为NULL为默认光标。
	TextInput.hIcon = NULL;//设置窗口图标，NULL为默认图标。
	TextInput.hInstance = hIns; //当前程序实例句柄
	TextInput.lpfnWndProc = DefWindowProc; //设置当前窗口的处理函数，传入处理函数的函数名即可。
	TextInput.lpszClassName = "TextInput";//设置窗口类的名字。
	TextInput.lpszMenuName = NULL; //设置窗口是否有菜单，NULL为没有菜单。
	TextInput.style = CS_HREDRAW | CS_VREDRAW; //设置窗口风格，CS_HREDRAW和CS_VREDRAW为当窗口水平或者垂直大小变化时，重画窗口

	//注册窗口，向操作系统中写入窗口信息。
	RegisterClass(&TextInput);

	HWND hTextInputRes = 0;


	return hTextInputRes;
}

HWND CreateUserList(HINSTANCE hIns)
{
	WNDCLASS UserList = { 0 }; //创建一个窗口类 名字叫UserList
	UserList.cbClsExtra = 0;// 要根据窗口类结构分配的额外字节数，设置需要申请的缓冲区。
	UserList.cbWndExtra = 0; //在窗口实例之后分配的额外字节数，设置需要申请的缓冲区。
	UserList.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//设置窗口的背景色，COLOR_WINDOW+1为白色。
	UserList.hCursor = NULL;//设置窗口光标，设置为NULL为默认光标。
	UserList.hIcon = NULL;//设置窗口图标，NULL为默认图标。
	UserList.hInstance = hIns; //当前程序实例句柄
	UserList.lpfnWndProc = DefWindowProc; //设置当前窗口的处理函数，传入处理函数的函数名即可。
	UserList.lpszClassName = "UserList";//设置窗口类的名字。
	UserList.lpszMenuName = NULL; //设置窗口是否有菜单，NULL为没有菜单。
	UserList.style = CS_HREDRAW | CS_VREDRAW; //设置窗口风格，CS_HREDRAW和CS_VREDRAW为当窗口水平或者垂直大小变化时，重画窗口

	//注册窗口，向操作系统中写入窗口信息。
	RegisterClass(&UserList);

	HWND hUserListRes = 0;


	return hUserListRes;
}

HWND CreateSendButton(HINSTANCE hIns)
{
	WNDCLASS SendButton = { 0 }; //创建一个窗口类 名字叫SendButton
	SendButton.cbClsExtra = 0;// 要根据窗口类结构分配的额外字节数，设置需要申请的缓冲区。
	SendButton.cbWndExtra = 0; //在窗口实例之后分配的额外字节数，设置需要申请的缓冲区。
	SendButton.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//设置窗口的背景色，COLOR_WINDOW+1为白色。
	SendButton.hCursor = NULL;//设置窗口光标，设置为NULL为默认光标。
	SendButton.hIcon = NULL;//设置窗口图标，NULL为默认图标。
	SendButton.hInstance = hIns; //当前程序实例句柄
	SendButton.lpfnWndProc = DefWindowProc; //设置当前窗口的处理函数，传入处理函数的函数名即可。
	SendButton.lpszClassName = "SendButton";//设置窗口类的名字。
	SendButton.lpszMenuName = NULL; //设置窗口是否有菜单，NULL为没有菜单。
	SendButton.style = CS_HREDRAW | CS_VREDRAW; //设置窗口风格，CS_HREDRAW和CS_VREDRAW为当窗口水平或者垂直大小变化时，重画窗口

	//注册窗口，向操作系统中写入窗口信息。
	RegisterClass(&SendButton);

	HWND hSendButtonRes = 0;


	return hSendButtonRes;
}

HWND CreateToolBar(HINSTANCE hIns)
{
	WNDCLASS ToolBar = { 0 }; //创建一个窗口类 名字叫ToolBar
	ToolBar.cbClsExtra = 0;// 要根据窗口类结构分配的额外字节数，设置需要申请的缓冲区。
	ToolBar.cbWndExtra = 0; //在窗口实例之后分配的额外字节数，设置需要申请的缓冲区。
	ToolBar.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//设置窗口的背景色，COLOR_WINDOW+1为白色。
	ToolBar.hCursor = NULL;//设置窗口光标，设置为NULL为默认光标。
	ToolBar.hIcon = NULL;//设置窗口图标，NULL为默认图标。
	ToolBar.hInstance = hIns; //当前程序实例句柄
	ToolBar.lpfnWndProc = DefWindowProc; //设置当前窗口的处理函数，传入处理函数的函数名即可。
	ToolBar.lpszClassName = "ToolBar";//设置窗口类的名字。
	ToolBar.lpszMenuName = NULL; //设置窗口是否有菜单，NULL为没有菜单。
	ToolBar.style = CS_HREDRAW | CS_VREDRAW; //设置窗口风格，CS_HREDRAW和CS_VREDRAW为当窗口水平或者垂直大小变化时，重画窗口

	//注册窗口，向操作系统中写入窗口信息。
	RegisterClass(&ToolBar);

	HWND hToolBarRes = 0;


	return hToolBarRes;
}

HWND CreateTextShowWindow(HINSTANCE hIns)
{
	WNDCLASS TextShowWindow = { 0 }; //创建一个窗口类 名字叫TextShowWindow
	TextShowWindow.cbClsExtra = 0;// 要根据窗口类结构分配的额外字节数，设置需要申请的缓冲区。
	TextShowWindow.cbWndExtra = 0; //在窗口实例之后分配的额外字节数，设置需要申请的缓冲区。
	TextShowWindow.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//设置窗口的背景色，COLOR_WINDOW+1为白色。
	TextShowWindow.hCursor = NULL;//设置窗口光标，设置为NULL为默认光标。
	TextShowWindow.hIcon = NULL;//设置窗口图标，NULL为默认图标。
	TextShowWindow.hInstance = hIns; //当前程序实例句柄
	TextShowWindow.lpfnWndProc = DefWindowProc; //设置当前窗口的处理函数，传入处理函数的函数名即可。
	TextShowWindow.lpszClassName = "TextShowWindow";//设置窗口类的名字。
	TextShowWindow.lpszMenuName = NULL; //设置窗口是否有菜单，NULL为没有菜单。
	TextShowWindow.style = CS_HREDRAW | CS_VREDRAW; //设置窗口风格，CS_HREDRAW和CS_VREDRAW为当窗口水平或者垂直大小变化时，重画窗口

	//注册窗口，向操作系统中写入窗口信息。
	RegisterClass(&TextShowWindow);


	HWND hTextShowWindowRes = 0;



	return hTextShowWindowRes;
}

HWND CreateTextAlias(HINSTANCE hIns)
{

	//在内存中创建窗口
	HWND hTextAliasRes = CreateWindowEx(
		0,//窗口的额外风格设置
		"STATIC",//创建的窗口类的名字
		"昵称：",//文本内容
		WS_VISIBLE | WS_CHILD,//设置窗口的基本风格。
		90,
		105,
		45,
		20,
		hSubMainWindow,//设置父窗口，没有置空NULL
		NULL,//设置菜单，没有菜单为NULL
		hIns,//当前程序实例句柄
		NULL//没啥用的参数
	);

	return hTextAliasRes;
}


HWND CreateStartServer(HINSTANCE hIns) {

	//在内存中创建窗口
	HWND hStartServerRes = CreateWindowEx(
		0,//窗口的额外风格设置
		"BUTTON",//创建的窗口类的名字
		"启动服务器",//文本内容
		WS_VISIBLE | WS_CHILD,//设置窗口的基本风格。
		370,
		20,
		100,
		20,
		hSubMainWindow,//设置父窗口，没有置空NULL
		NULL,//设置菜单，没有菜单为NULL
		hIns,//当前程序实例句柄
		NULL//没啥用的参数
	);
	return hStartServerRes;
}
