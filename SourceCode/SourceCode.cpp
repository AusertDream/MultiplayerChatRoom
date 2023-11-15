// SourceCode.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "SourceCode.h"
using namespace std;

//窗口处理函数，处理消息。
LRESULT CALLBACK MyWndProc(
	HWND hWnd, //窗口句柄
	UINT msgID, //消息ID
	WPARAM wParam,
	LPARAM IParam
	) 
{
	
	return DefWindowProc(hWnd, msgID, wParam, IParam);
}


int WINAPI WinMain(
	HINSTANCE hIns, //当前程序实例句柄
	HINSTANCE hPreIns, //现已废弃不用，但必须标注出来
 	LPSTR LpCmdLine, //命令行参数
	int nCmdShow //窗口显示方式
	)
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
	HWND hMainWindow = CreateWindowEx(
		0,//窗口的额外风格设置
		L"MainWindow",//创建的窗口类的名字
		L"Ausert的多人聊天室",//窗口的左上角标题文字
		WS_OVERLAPPEDWINDOW,//设置窗口的基本风格。
		100,//窗口的初始x坐标
		100,//窗口的初始y坐标
		500,//窗口的初始宽度
		500,//窗口的初始高度
		NULL,//设置父窗口，没有置空NULL
		NULL,//设置菜单，没有菜单为NULL
		hIns,//当前程序实例句柄
		NULL//没啥用的参数
	);

	//显示窗口。
	ShowWindow(hMainWindow,SW_SHOW); //第一个参数传入创建窗口返回的句柄，第二个参数传入显示的方式。SW_SHOW为默认显示。
	UpdateWindow(hMainWindow);//刷新创建的窗口。

	MSG nMsg = { 0 };

	//消息循环
	return 0;
}