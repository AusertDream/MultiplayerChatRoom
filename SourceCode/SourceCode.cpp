// SourceCode.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "SourceCode.h"
using namespace std;

//#define WM_MYMSG WM_USER+1011 自定义消息类型

//所有窗口的句柄
HWND hMainWindow,hSubMainWindow,hLoginInput, hLoginButton,hTextInput,hUserList,hSendButton,hToolBar,hTextShowWindow;

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

//窗口处理函数，处理消息。
LRESULT CALLBACK MyWndProc(
	HWND hWnd, //窗口句柄
	UINT msgID, //消息ID
	WPARAM wParam, //消息参数
	LPARAM LParam //消息参数
	) 
{
	switch (msgID) {
	case WM_DESTROY:
		PostQuitMessage(0); //当msgid是WM_DESTROY的值的时候，则将进程结束。
		break;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE) {
			int mRet = MessageBox(hWnd, L"真的要退出吗QAQ", L"Alert", MB_YESNO);
			if (mRet == IDYES) {
				
			}
			else {
				return 0;
			}
		}
		break;
	case WM_SIZE:

		break;
	}
	return DefWindowProc(hWnd, msgID, wParam, LParam);
}


int WINAPI WinMain(
	HINSTANCE hIns, //当前程序实例句柄
	HINSTANCE hPreIns, //现已废弃不用，但必须标注出来
 	LPSTR LpCmdLine, //命令行参数
	int nCmdShow //窗口显示方式
	)
{
	
	hMainWindow = CreateMainWindow(hIns); //创建主窗口
	hSubMainWindow = CreateSubMainWindow(hIns); //创建子主窗口
	hLoginInput = CreateLoginInput(hIns); //创建用户名输入框 
	hLoginButton = CreateLoginButton(hIns); //创建登录按钮
	hTextInput = CreateTextInput(hIns); //创建输入框
	hUserList = CreateUserList(hIns); //创建用户列表
	hSendButton = CreateSendButton(hIns); //创建发送按钮
	hToolBar = CreateToolBar(hIns); //创建工具栏
	hTextShowWindow = CreateTextShowWindow(hIns); //创建聊天记录显示框

	
	
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

HWND CreateMainWindow(HINSTANCE hIns)
{
	WNDCLASS MainWindow = { 0 }; //创建一个窗口类 名字叫MainWindow
	MainWindow.cbClsExtra = 0;// 要根据窗口类结构分配的额外字节数，设置需要申请的缓冲区。
	MainWindow.cbWndExtra = 0; //在窗口实例之后分配的额外字节数，设置需要申请的缓冲区。
	MainWindow.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//设置窗口的背景色，COLOR_WINDOW+1为白色。
	MainWindow.hCursor = NULL;//设置窗口光标，设置为NULL为默认光标。
	MainWindow.hIcon = NULL;//设置窗口图标，NULL为默认图标。
	MainWindow.hInstance = hIns; //当前程序实例句柄
	MainWindow.lpfnWndProc = MyWndProc; //设置当前窗口的处理函数，传入处理函数的函数名即可。
	MainWindow.lpszClassName = L"MainWindow";//设置窗口类的名字。
	MainWindow.lpszMenuName = NULL; //设置窗口是否有菜单，NULL为没有菜单。
	MainWindow.style = CS_HREDRAW | CS_VREDRAW; //设置窗口风格，CS_HREDRAW和CS_VREDRAW为当窗口水平或者垂直大小变化时，重画窗口

	//注册窗口，向操作系统中写入窗口信息。
	RegisterClass(&MainWindow);

	//在内存中创建窗口
	HWND hMainWindowRes = CreateWindowEx(
		0,//窗口的额外风格设置
		L"MainWindow",//创建的窗口类的名字
		L"Ausert的多人聊天室",//窗口的左上角标题文字
		WS_OVERLAPPEDWINDOW,//设置窗口的基本风格。
		200,//窗口的初始x坐标
		200,//窗口的初始y坐标
		500,//窗口的初始宽度
		500,//窗口的初始高度
		NULL,//设置父窗口，没有置空NULL
		NULL,//设置菜单，没有菜单为NULL
		hIns,//当前程序实例句柄
		NULL//没啥用的参数
	);
	return hMainWindowRes;
}

HWND CreateSubMainWindow(HINSTANCE hIns)
{
	WNDCLASS SubMainWindow = { 0 }; //创建一个窗口类 名字叫MainWindow
	SubMainWindow.cbClsExtra = 0;// 要根据窗口类结构分配的额外字节数，设置需要申请的缓冲区。
	SubMainWindow.cbWndExtra = 0; //在窗口实例之后分配的额外字节数，设置需要申请的缓冲区。
	SubMainWindow.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//设置窗口的背景色，COLOR_WINDOW+1为白色。
	SubMainWindow.hCursor = NULL;//设置窗口光标，设置为NULL为默认光标。
	SubMainWindow.hIcon = NULL;//设置窗口图标，NULL为默认图标。
	SubMainWindow.hInstance = hIns; //当前程序实例句柄
	SubMainWindow.lpfnWndProc = DefWindowProc; //设置当前窗口的处理函数，传入处理函数的函数名即可。
	SubMainWindow.lpszClassName = L"SubMainWindow";//设置窗口类的名字。
	SubMainWindow.lpszMenuName = NULL; //设置窗口是否有菜单，NULL为没有菜单。
	SubMainWindow.style = CS_HREDRAW | CS_VREDRAW; //设置窗口风格，CS_HREDRAW和CS_VREDRAW为当窗口水平或者垂直大小变化时，重画窗口

	//注册窗口，向操作系统中写入窗口信息。
	RegisterClass(&SubMainWindow);


	HWND hSubMainWindowRes = CreateWindowEx(
		0,
		L"SubMainWindow",
		NULL,
		WS_CHILD | WS_VISIBLE,
		0,
		0,
		500,
		500,
		hMainWindow,
		NULL,
		hIns,
		NULL
	);

	return hSubMainWindowRes;
}


//↓还未写好create

HWND CreateLoginInput(HINSTANCE hIns)
{
	WNDCLASS LoginInput = { 0 }; //创建一个窗口类 名字叫MainWindow
	LoginInput.cbClsExtra = 0;// 要根据窗口类结构分配的额外字节数，设置需要申请的缓冲区。
	LoginInput.cbWndExtra = 0; //在窗口实例之后分配的额外字节数，设置需要申请的缓冲区。
	LoginInput.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//设置窗口的背景色，COLOR_WINDOW+1为白色。
	LoginInput.hCursor = NULL;//设置窗口光标，设置为NULL为默认光标。
	LoginInput.hIcon = NULL;//设置窗口图标，NULL为默认图标。
	LoginInput.hInstance = hIns; //当前程序实例句柄
	LoginInput.lpfnWndProc = DefWindowProc; //设置当前窗口的处理函数，传入处理函数的函数名即可。
	LoginInput.lpszClassName = L"LoginInput";//设置窗口类的名字。
	LoginInput.lpszMenuName = NULL; //设置窗口是否有菜单，NULL为没有菜单。
	LoginInput.style = CS_HREDRAW | CS_VREDRAW; //设置窗口风格，CS_HREDRAW和CS_VREDRAW为当窗口水平或者垂直大小变化时，重画窗口

	//注册窗口，向操作系统中写入窗口信息。
	RegisterClass(&LoginInput);

	HWND hLoginInputRes = CreateWindowEx(
		0,
		L"EDIT",
		NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOVSCROLL,
		100,
		130,
		260,
		50,
		hSubMainWindow,
		NULL,
		hIns,
		NULL
	);
	return hLoginInputRes;
}

HWND CreateLoginButton(HINSTANCE hIns)
{
	WNDCLASS LoginButton = { 0 }; //创建一个窗口类 名字叫MainWindow
	LoginButton.cbClsExtra = 0;// 要根据窗口类结构分配的额外字节数，设置需要申请的缓冲区。
	LoginButton.cbWndExtra = 0; //在窗口实例之后分配的额外字节数，设置需要申请的缓冲区。
	LoginButton.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//设置窗口的背景色，COLOR_WINDOW+1为白色。
	LoginButton.hCursor = NULL;//设置窗口光标，设置为NULL为默认光标。
	LoginButton.hIcon = NULL;//设置窗口图标，NULL为默认图标。
	LoginButton.hInstance = hIns; //当前程序实例句柄
	LoginButton.lpfnWndProc = DefWindowProc; //设置当前窗口的处理函数，传入处理函数的函数名即可。
	LoginButton.lpszClassName = L"LoginButton";//设置窗口类的名字。
	LoginButton.lpszMenuName = NULL; //设置窗口是否有菜单，NULL为没有菜单。
	LoginButton.style = CS_HREDRAW | CS_VREDRAW; //设置窗口风格，CS_HREDRAW和CS_VREDRAW为当窗口水平或者垂直大小变化时，重画窗口

	//注册窗口，向操作系统中写入窗口信息。
	RegisterClass(&LoginButton);

	HWND hLoginButtonRes = CreateWindowEx(
		0,
		L"BUTTON",
		NULL,
		WS_CHILD | WS_VISIBLE,
		200,
		200,
		100,
		30,
		hSubMainWindow,
		NULL,
		hIns,
		NULL
	);

	return hLoginButtonRes;
}

HWND CreateTextInput(HINSTANCE hIns)
{
	WNDCLASS TextInput = { 0 }; //创建一个窗口类 名字叫MainWindow
	TextInput.cbClsExtra = 0;// 要根据窗口类结构分配的额外字节数，设置需要申请的缓冲区。
	TextInput.cbWndExtra = 0; //在窗口实例之后分配的额外字节数，设置需要申请的缓冲区。
	TextInput.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//设置窗口的背景色，COLOR_WINDOW+1为白色。
	TextInput.hCursor = NULL;//设置窗口光标，设置为NULL为默认光标。
	TextInput.hIcon = NULL;//设置窗口图标，NULL为默认图标。
	TextInput.hInstance = hIns; //当前程序实例句柄
	TextInput.lpfnWndProc = DefWindowProc; //设置当前窗口的处理函数，传入处理函数的函数名即可。
	TextInput.lpszClassName = L"TextInput";//设置窗口类的名字。
	TextInput.lpszMenuName = NULL; //设置窗口是否有菜单，NULL为没有菜单。
	TextInput.style = CS_HREDRAW | CS_VREDRAW; //设置窗口风格，CS_HREDRAW和CS_VREDRAW为当窗口水平或者垂直大小变化时，重画窗口

	//注册窗口，向操作系统中写入窗口信息。
	RegisterClass(&TextInput);

	HWND hTextInputRes = 0;


	return hTextInputRes;
}

HWND CreateUserList(HINSTANCE hIns)
{
	WNDCLASS UserList = { 0 }; //创建一个窗口类 名字叫MainWindow
	UserList.cbClsExtra = 0;// 要根据窗口类结构分配的额外字节数，设置需要申请的缓冲区。
	UserList.cbWndExtra = 0; //在窗口实例之后分配的额外字节数，设置需要申请的缓冲区。
	UserList.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//设置窗口的背景色，COLOR_WINDOW+1为白色。
	UserList.hCursor = NULL;//设置窗口光标，设置为NULL为默认光标。
	UserList.hIcon = NULL;//设置窗口图标，NULL为默认图标。
	UserList.hInstance = hIns; //当前程序实例句柄
	UserList.lpfnWndProc = DefWindowProc; //设置当前窗口的处理函数，传入处理函数的函数名即可。
	UserList.lpszClassName = L"UserList";//设置窗口类的名字。
	UserList.lpszMenuName = NULL; //设置窗口是否有菜单，NULL为没有菜单。
	UserList.style = CS_HREDRAW | CS_VREDRAW; //设置窗口风格，CS_HREDRAW和CS_VREDRAW为当窗口水平或者垂直大小变化时，重画窗口

	//注册窗口，向操作系统中写入窗口信息。
	RegisterClass(&UserList);

	HWND hUserListRes = 0;


	return hUserListRes;
}

HWND CreateSendButton(HINSTANCE hIns)
{
	WNDCLASS SendButton = { 0 }; //创建一个窗口类 名字叫MainWindow
	SendButton.cbClsExtra = 0;// 要根据窗口类结构分配的额外字节数，设置需要申请的缓冲区。
	SendButton.cbWndExtra = 0; //在窗口实例之后分配的额外字节数，设置需要申请的缓冲区。
	SendButton.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//设置窗口的背景色，COLOR_WINDOW+1为白色。
	SendButton.hCursor = NULL;//设置窗口光标，设置为NULL为默认光标。
	SendButton.hIcon = NULL;//设置窗口图标，NULL为默认图标。
	SendButton.hInstance = hIns; //当前程序实例句柄
	SendButton.lpfnWndProc = DefWindowProc; //设置当前窗口的处理函数，传入处理函数的函数名即可。
	SendButton.lpszClassName = L"SendButton";//设置窗口类的名字。
	SendButton.lpszMenuName = NULL; //设置窗口是否有菜单，NULL为没有菜单。
	SendButton.style = CS_HREDRAW | CS_VREDRAW; //设置窗口风格，CS_HREDRAW和CS_VREDRAW为当窗口水平或者垂直大小变化时，重画窗口

	//注册窗口，向操作系统中写入窗口信息。
	RegisterClass(&SendButton);

	HWND hSendButtonRes = 0;


	return hSendButtonRes;
}

HWND CreateToolBar(HINSTANCE hIns)
{
	WNDCLASS ToolBar = { 0 }; //创建一个窗口类 名字叫MainWindow
	ToolBar.cbClsExtra = 0;// 要根据窗口类结构分配的额外字节数，设置需要申请的缓冲区。
	ToolBar.cbWndExtra = 0; //在窗口实例之后分配的额外字节数，设置需要申请的缓冲区。
	ToolBar.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//设置窗口的背景色，COLOR_WINDOW+1为白色。
	ToolBar.hCursor = NULL;//设置窗口光标，设置为NULL为默认光标。
	ToolBar.hIcon = NULL;//设置窗口图标，NULL为默认图标。
	ToolBar.hInstance = hIns; //当前程序实例句柄
	ToolBar.lpfnWndProc = DefWindowProc; //设置当前窗口的处理函数，传入处理函数的函数名即可。
	ToolBar.lpszClassName = L"ToolBar";//设置窗口类的名字。
	ToolBar.lpszMenuName = NULL; //设置窗口是否有菜单，NULL为没有菜单。
	ToolBar.style = CS_HREDRAW | CS_VREDRAW; //设置窗口风格，CS_HREDRAW和CS_VREDRAW为当窗口水平或者垂直大小变化时，重画窗口

	//注册窗口，向操作系统中写入窗口信息。
	RegisterClass(&ToolBar);

	HWND hToolBarRes = 0;
	

	return hToolBarRes;
}

HWND CreateTextShowWindow(HINSTANCE hIns)
{
	WNDCLASS TextShowWindow = { 0 }; //创建一个窗口类 名字叫MainWindow
	TextShowWindow.cbClsExtra = 0;// 要根据窗口类结构分配的额外字节数，设置需要申请的缓冲区。
	TextShowWindow.cbWndExtra = 0; //在窗口实例之后分配的额外字节数，设置需要申请的缓冲区。
	TextShowWindow.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//设置窗口的背景色，COLOR_WINDOW+1为白色。
	TextShowWindow.hCursor = NULL;//设置窗口光标，设置为NULL为默认光标。
	TextShowWindow.hIcon = NULL;//设置窗口图标，NULL为默认图标。
	TextShowWindow.hInstance = hIns; //当前程序实例句柄
	TextShowWindow.lpfnWndProc = DefWindowProc; //设置当前窗口的处理函数，传入处理函数的函数名即可。
	TextShowWindow.lpszClassName = L"TextShowWindow";//设置窗口类的名字。
	TextShowWindow.lpszMenuName = NULL; //设置窗口是否有菜单，NULL为没有菜单。
	TextShowWindow.style = CS_HREDRAW | CS_VREDRAW; //设置窗口风格，CS_HREDRAW和CS_VREDRAW为当窗口水平或者垂直大小变化时，重画窗口

	//注册窗口，向操作系统中写入窗口信息。
	RegisterClass(&TextShowWindow);


	HWND hTextShowWindowRes = 0;
	


	return hTextShowWindowRes;
}
