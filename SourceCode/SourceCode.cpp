﻿// SourceCode.cpp : 定义应用程序的入口点。
//
#pragma once
#include "framework.h"
#include "SourceCode.h" //导入资源
#include "InitWindows.h"
#include "SomeTools.h"
#include "TestProgram.h"
using namespace std;

HANDLE hStdOutput = 0; //添加DOS控制台窗口用于DEBUG
wstring username; //当前用户的用户名
//#define WM_MYMSG WM_USER+1011 自定义消息类型
HINSTANCE hIns ; //当前程序实例句柄

//所有窗口的句柄
HWND hMainWindow, hSubMainWindow, hLoginInput, hLoginButton, hTextInput, hUserList, hSendButton, hToolBar, hTextShowWindow;
HWND hTextAlias, hStartServer,hSubMainWindow2;

SOCKET SendRecvSocket; //用户发送和接收的socket


//主窗口
HWND CreateMainWindow(HINSTANCE hIns); //创建主窗口
//登录界面
HWND CreateSubMainWindow(HINSTANCE hIns); //创建子主窗口
HWND CreateLoginInput(HINSTANCE hIns); //创建用户名输入框
HWND CreateLoginButton(HINSTANCE hIns); //创建登录按钮
HWND CreateTextAlias(HINSTANCE hIns);//创建昵称提示文字
HWND CreateStartServer(HINSTANCE hIns);//创建启动服务器的按钮
//聊天室界面
HWND CreateSubMainWindow2(HINSTANCE hIns); //创建子主窗口2
HWND CreateTextInput(HINSTANCE hIns); //创建输入框
HWND CreateUserList(HINSTANCE hIns); //创建用户列表
HWND CreateSendButton(HINSTANCE hIns); //创建发送按钮
HWND CreateToolBar(HINSTANCE hIns); //创建发送表情的按钮
HWND CreateTextShowWindow(HINSTANCE hIns); //创建聊天记录显示框

LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM LParam);//主窗口处理函数，处理消息。
LRESULT CALLBACK SubMainWindowProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM LParam);//子窗口的处理函数
LRESULT CALLBACK SubMainWindow2Proc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM LParam);//子窗口2的处理函数
LRESULT CALLBACK DlgProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM LParam);//表情窗口的处理函数
SOCKET StartClient(bool&); //启动socket服务的函数
void thRecv(SOCKET); //接受消息的线程函数
void func(); //用于测试的函数
void OnCommand(WPARAM wParam); //处理菜单被点击的操作
string CurrentTarget = "GLOBAL"; //当前聊天对象


mutex ProtectProcQueue; //保护ProcQueue的互斥锁
bool isSocketStart = false; //socket是否启动
queue<UserParam> ProcQueue; //消息处理队列
bool isStart = false; //是否启动了测试程序


int WINAPI WinMain(
	HINSTANCE hIns_, //当前程序实例句柄
	HINSTANCE hPreIns, //现已废弃不用，但必须标注出来
 	LPSTR LpCmdLine, //命令行参数
	int nCmdShow //窗口显示方式
	)
{
	hIns = hIns_;

	
	
	//AllocConsole();//启用DOS窗口
	hStdOutput = GetStdHandle(STD_OUTPUT_HANDLE);//获取标准输出句柄。
	//char* text = "放假了";
	//WriteConsole(hStdOutput, text, strlen(text), NULL, NULL); //在控制台上输出内容
	
	hMainWindow = CreateMainWindow(hIns); //创建主窗口
	hSubMainWindow = CreateSubMainWindow(hIns); //创建子主窗口
	hLoginInput = CreateLoginInput(hIns); //创建用户名输入框 
	hLoginButton = CreateLoginButton(hIns); //创建登录按钮
	hTextAlias = CreateTextAlias(hIns);//创建昵称提示文本
	hStartServer = CreateStartServer(hIns);//创建启动服务器的按钮
	hSubMainWindow2 = CreateSubMainWindow2(hIns); //创建子主窗口2
	hTextInput = CreateTextInput(hIns); //创建输入框
	hUserList = CreateUserList(hIns); //创建用户列表
	hSendButton = CreateSendButton(hIns); //创建发送按钮
	//hToolBar = CreateToolBar(hIns); //创建工具栏
	hTextShowWindow = CreateTextShowWindow(hIns); //创建聊天记录显示框
	
	//设置按钮的文字内容
	SetWindowText(hLoginButton, "登录");
	SetWindowText(hStartServer, "启动测试程序");
	
	//显示窗口。
	ShowWindow(hMainWindow,SW_SHOW); //第一个参数传入创建窗口返回的句柄，第二个参数传入显示的方式。SW_SHOW为默认显示。
	UpdateWindow(hMainWindow);//刷新创建的窗口。
	//在用户列表里面添加提示语
	SendMessage(hUserList,LB_ADDSTRING, 0, (LPARAM)"当前用户列表：");
	Sleep(100);

	
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
			Sleep(10);
		}

	}
	WSACleanup();
	return 0;
}


SOCKET StartClient(bool& res) {
	//1.确定协议版本
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		MessageBox(hMainWindow, "通信版本非2.2", "Error", MB_OK);
	}
	//2.创建socket
	//采用流传输模式，TCP协议。AF_INET表示使用的传输协议为TCP,UDP之类的协议。
	SOCKET sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == sSocket) {
		char* ErrorMsg = " ";
		//如果创建失败输出报错信息
		sprintf(ErrorMsg, "创建socket失败：%d\n", GetLastError());
		MessageBox(hMainWindow, ErrorMsg, "Error", MB_OK);
	}
	//3.确定服务器的协议地址簇
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET; //使用的是什么协议与创建SOCKET时候写的须保持一致
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //服务器的ip地址
	addr.sin_port = htons(10086); //服务器提供服务的端口号

	//4. 连接服务器
	int r = connect(sSocket, (sockaddr*)&addr, sizeof addr);
	if (r < 0) {
		MessageBox(hMainWindow, "连接服务器失败", "Error", MB_OK);
		//如果连接失败输出报错信息
		closesocket(sSocket); //断开链接
		return NULL;
	}
	res = true;
	isSocketStart = true;
	return sSocket;
}

void thRecv(SOCKET sSocket)
{
	//消息处理函数
	auto MsgProc = [&](UserParam recvBuffer) {
		USES_CONVERSION;
		if (recvBuffer.Type == NORMAL_MSG) {
			//如果是普通消息
			//将消息显示在聊天记录框中
			//检查自己是否是接收方
			wstring wstringCurrentTarget(A2W(CurrentTarget.c_str()));
			if (recvBuffer.Receiver == username || recvBuffer.Receiver == wstringCurrentTarget || recvBuffer.Receiver == L"ALL") {
				//如果这是一条私聊消息，就提醒他，然后进入对应的显示框内
				if (recvBuffer.Receiver == username) {
					//如果是私聊消息
					//如果当前聊天对象不是发送方，那么就将当前聊天对象设置为发送方
					if (wstringCurrentTarget != recvBuffer.Sender) {
						CurrentTarget = W2A(recvBuffer.Sender.c_str());
						MessageBox(hSubMainWindow2, "您收到了一条私聊消息，已自动切换到对应的聊天窗口", "通知", MB_OK);
						//发送更新窗口的消息
						PostMessage(hSubMainWindow2, WM_UPDATETEXTSHOWWINDOW, NULL, NULL);
					}
				}
				//格式化消息内容显示方式
				wstring text = recvBuffer.Sender + L" " + L"> " + recvBuffer.Msg;
				text.push_back('\n');
				char* showmsg = W2A(text.c_str());
				SendMessage(hTextShowWindow, EM_SETSEL, -1, -1); // 将光标移动到文本末尾
				SendMessage(hTextShowWindow, EM_REPLACESEL, 0, (LPARAM)showmsg); //将消息内容发送过去
			}
		}
		else if (recvBuffer.Type == LOGIN_RESULT) {
			//如果是登录结果
			if (recvBuffer.res == false) {
				//如果登录失败
				MessageBox(hMainWindow, "登录失败,可能是昵称重名或者聊天室人满了", "Oops", MB_OK);
				closesocket(SendRecvSocket); //断开链接
				isSocketStart = false;
			}
			else {
				//如果登录成功
				SetWindowText(hLoginInput, "");
				//DestroyWindow(hSubMainWindow);
				PostMessage(hSubMainWindow, WM_CLOSE, NULL, NULL); //向子窗口发送关闭消息
				SetWindowPos(hMainWindow, NULL, 350, 250, 1000, 700, SWP_NOZORDER); //设置主窗口的大小
				ShowWindow(hSubMainWindow2, SW_SHOW);
				PostMessage(hMainWindow, WM_SUCCESSLOGIN, NULL, NULL); //向mainWindow发送成功登录的消息
				//获取用户列表
				string buff;
				buff.push_back(USERLIST_REQUEST);
				buff.push_back(' ');
				buff.append(W2A(username.c_str()));
				buff.push_back(' ');
				buff.append("Server");
				buff.push_back(' ');
				buff.push_back('\0');
				send(SendRecvSocket, buff.c_str(), buff.size(), NULL);
				//获取全球聊天室的聊天记录
				MessageBox(hMainWindow, "登录成功", "通知", MB_OK);
				PostMessage(hSubMainWindow2, WM_UPDATETEXTSHOWWINDOW, NULL, NULL);
			}
		}
		else if (recvBuffer.Type == LOGOUT_RESULT) {
			//需要让用户列表中的该用户的用户名消失
			string DeletedName(W2A(recvBuffer.Msg.c_str()));
			//去列表中寻找该名字的下标
			int index = SendMessage(hUserList, LB_FINDSTRINGEXACT, (WPARAM)-1, (LPARAM)DeletedName.c_str());
			//然后删除该下标的名字
			SendMessage(hUserList, LB_DELETESTRING, index, NULL);
		}
		else if (recvBuffer.Type == NEWUSERLOGIN) {
			string NewUserName(W2A(recvBuffer.Msg.c_str()));
			NewUserName.push_back('\0');
			SendMessage(hUserList, LB_ADDSTRING, 0, (LPARAM)NewUserName.c_str());
		}
		else if (recvBuffer.Type == GLOBALRECORD_RESULT) {
			//如果是全球聊天室的聊天记录
			//将消息显示在聊天记录框中
			//格式化消息内容显示方式
			wstring text = recvBuffer.Sender + L" " + L"> " + recvBuffer.Msg;
			text.push_back('\n');
			char* showmsg = W2A(text.c_str());
			SendMessage(hTextShowWindow, EM_SETSEL, -1, -1); // 将光标移动到文本末尾
			SendMessage(hTextShowWindow, EM_REPLACESEL, 0, (LPARAM)showmsg); //将消息内容发送过去
		}
		else if (recvBuffer.Type == CNRECORD_RESULT) {
			//如果是中国聊天室的聊天记录
			//将消息显示在聊天记录框中
			//格式化消息内容显示方式
			wstring text = recvBuffer.Sender + L" " + L"> " + recvBuffer.Msg;
			text.push_back('\n');
			char* showmsg = W2A(text.c_str());
			SendMessage(hTextShowWindow, EM_SETSEL, -1, -1); // 将光标移动到文本末尾
			SendMessage(hTextShowWindow, EM_REPLACESEL, 0, (LPARAM)showmsg); //将消息内容发送过去
		}

		else if (recvBuffer.Type == USERLIST_RESULT) {
			//将收到的用户名字添加到用户列表当中去
			string NewUserName(W2A(recvBuffer.Msg.c_str()));
			NewUserName.push_back('\0');
			SendMessage(hUserList, LB_ADDSTRING, 0, (LPARAM)NewUserName.c_str());
		}
	};
	//处理函数
	auto thProc = [&]() {
		while (true) {
			//如果队列不为空，那么就处理队列中的消息
			/*unique_lock<mutex> lock(ProtectProcQueue);*/
			if (ProcQueue.size() != 0) {
				UserParam recvBuffer = ProcQueue.front();
				ProcQueue.pop();
				MsgProc(recvBuffer);
			}
			else {
				/*lock.unlock();*/
				Sleep(100);
			}
			
		}
	};
	//为消息处理函数单独开启一个线程
	thread th(thProc);
	th.detach();
	int r;
	//5.接受消息
	while (true) {
		UserParam recvBuffer;
		char buff[MAX_BUFFER_SIZE];
		r = recv(sSocket, buff, MAX_BUFFER_SIZE, NULL);
		if (r < 0) {
			//如果接受失败输出报错信息
		
			closesocket(SendRecvSocket); //断开链接
			
			MessageBox(hMainWindow, "本地socket已经被关闭", "通知", MB_OK);
			isSocketStart = false;
			break;
		}
		//将char转化为wstring类型
		USES_CONVERSION;
		wchar_t* wbuff = A2W(buff);
		wstring wstr(wbuff);
		//解包wstr
		openTheBuff(wstr, recvBuffer);
		//将解包后的数据放入队列中
		/*unique_lock<mutex> lock(ProtectProcQueue);*/
		ProcQueue.push(recvBuffer);
		/*lock.unlock();*/
	}
}



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
	case KAOMOJI:
	{
		HWND hDial = CreateDialog(hIns, MAKEINTRESOURCE(IDD_DIALOG1), hSubMainWindow2, (DLGPROC)DlgProc);
		ShowWindow(hDial, SW_SHOW);
		break;
	}
	case BUTTONSENDMESSAGE:
	{
		if (isSocketStart == false) {
			MessageBox(hMainWindow, "socket异常关闭，当前处于离线状态，可能是您已掉线或者服务器异常关闭", "Oops", MB_OK);
			break;
		}
		//当点击发送按钮的时候
		//获取输入框的内容
		WCHAR buffer[MAX_BUFFER_SIZE];
		GetWindowTextW(hTextInput, buffer, MAX_BUFFER_SIZE);

		//将内容转换为string
		USES_CONVERSION;
		string msg;
		msg.assign(W2A(buffer));
		if (msg.size() == 0) {
			MessageBox(hMainWindow, "请不要发送空消息", "Oops", MB_OK);
			break;
		}
		else if (msg.size() >= 100) {
			MessageBox(hMainWindow, "消息过长，请不要超过100个字符", "警告", MB_OK);
			break;
		}
		msg.push_back('\0');
		//将内容发送出去
		string sendbuff;
		sendbuff.push_back(NORMAL_MSG);
		sendbuff.push_back(' ');
		string str(W2A(username.c_str())); //将宽字节的用户名转化为char
		sendbuff+=str;
		sendbuff.push_back(' ');
		sendbuff+=CurrentTarget;
		sendbuff.push_back(' ');
		sendbuff += msg;
		sendbuff.push_back('\0');
		send(SendRecvSocket,sendbuff.c_str(), sendbuff.size(), NULL);
		SetWindowText(hTextInput, "");
		break;
	}
	case ID_CN:
		//当点击中国区域的聊天室的时候
		CurrentTarget = "CN";
		PostMessage(hSubMainWindow2,WM_UPDATETEXTSHOWWINDOW,NULL,NULL);
		break;
	case ID_GLOBAL:
		//当点击全球聊天室的时候
		CurrentTarget="GLOBAL";
		PostMessage(hSubMainWindow2, WM_UPDATETEXTSHOWWINDOW, NULL, NULL);
		break;
	case BUTTONLOGIN:
	{
		if (isSocketStart == false) {
			//开启socket服务
			bool res=false;
			SendRecvSocket = StartClient(res);
			if (res == false) {
				MessageBox(hMainWindow, "socket链接失败，可能是服务器未开启", "Oops", MB_OK);
				break;
			}
			thread threcv(thRecv, SendRecvSocket);
			threcv.detach();
		}
		WCHAR buffer[MAX_BUFFER_SIZE];
		GetWindowTextW(hLoginInput, buffer, MAX_BUFFER_SIZE);
		//检查用户名是否有空格
		bool isSpace = false;
		for (auto e : buffer) {
			if (e == '\0') {
				break;
			}
			if (e == ' ') {
				isSpace = true;
			}
		}

		//将用户名保存
		username.assign(buffer);

		if (username.size() == 0) {
			MessageBox(hMainWindow, "请不要输入空昵称", "Oops", MB_OK);
			break;
		}
		
		//如果有空格则弹出提示框
		if (isSpace) {
			MessageBox(hMainWindow, "昵称中不要带有空格", "Oops", MB_OK);
		}
		else {
			string buffer;
			buffer.push_back(LOGIN_REQUEST);
			buffer.push_back(' ');
			USES_CONVERSION;
			string str(W2A(username.c_str()));
			buffer+=str;
			buffer.push_back(' ');
			buffer.append("Server");
			buffer.push_back('\0');
			send(SendRecvSocket, buffer.c_str(),buffer.size(), NULL);
		}
	break;
	}
	case BUTTONTEST:
		if (isStart == true) {
			MessageBox(hMainWindow, "测试程序已经启动，请勿重复启动", "Oops", MB_OK);
			break;
		}
		thread thTestProgram(StartTestProgram,MAX_TEST_NUMBER);
		thTestProgram.detach();
		MessageBox(hMainWindow, "测试程序启动成功。\n如果此时服务端没有启动，测试程序大概率会寄", "通知", MB_OK);
		isStart = true;
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
	case WM_COMMAND: //处理菜单被点击的操作
		OnCommand(wParam);
		break;
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

LRESULT SubMainWindow2Proc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM LParam)
{
	switch (msgID) {
	case WM_KEYDOWN:
		//如果按下的是回车键，相当于按了发送按钮。
		if (wParam == VK_RETURN) {
			SetFocus(hTextInput);
			PostMessage(hTextInput, WM_COMMAND, MAKEWPARAM(BUTTONSENDMESSAGE, NULL), NULL);
		}
		break;
	case WM_UPDATETEXTSHOWWINDOW:
		//将消息显示窗口清空
		SetWindowText(hTextShowWindow, "");
		//根据不同的聊天对象，显示不同的欢迎信息
		if (CurrentTarget == "GLOBAL") {
			SendMessage(hTextShowWindow, EM_REPLACESEL, 0, (LPARAM)"欢迎来到全球聊天室\n");
			//获取全球聊天室的聊天记录
			string buff;
			buff.push_back(GLOBALRECORD_REQUEST);
			buff.push_back(' ');
			USES_CONVERSION;
			buff.append(W2A(username.c_str()));
			buff.push_back(' ');
			buff.append("Server");
			buff.push_back(' ');
			buff.push_back('\0');
			send(SendRecvSocket, buff.c_str(), buff.size(), NULL);
		}
		else if (CurrentTarget == "CN") {
			SendMessage(hTextShowWindow, EM_REPLACESEL, 0, (LPARAM)"欢迎来到中国聊天室\n");
			//获取中国聊天室的聊天记录
			string buff;
			buff.push_back(CNRECORD_REQUEST);
			buff.push_back(' ');
			USES_CONVERSION;
			buff.append(W2A(username.c_str()));
			buff.push_back(' ');
			buff.append("Server");
			buff.push_back(' ');
			buff.push_back('\0');
			send(SendRecvSocket, buff.c_str(), buff.size(), NULL);
		}
		else {
			string s = "当前聊天用户为："+CurrentTarget+'\n';
			SendMessage(hTextShowWindow, EM_REPLACESEL, 0, (LPARAM)s.c_str());
		}
		//将光标设置为下一行的第一个位置。
		SendMessage(hTextShowWindow, EM_SETSEL, -1, -1);
		break;
	case WM_COMMAND: //处理菜单被点击的操作
		if (HIWORD(wParam) == LBN_SELCHANGE) {
			//当点击用户列表的时候,发送的消息就是LBN_SELCHANGE
			// 获取当前选择的项的索引
			int selectedIndex = SendMessage(hUserList, LB_GETCURSEL, 0, 0);
			//如果没有获取异常
			if (selectedIndex != LB_ERR) {
				// 获取选中项的文本
				char selectedText[MAX_BUFFER_SIZE];
				SendMessage(hUserList, LB_GETTEXT, selectedIndex, (LPARAM)selectedText);

				string recverName(selectedText);
				// 获取到点击的用户的昵称的时候，然后打开对应的对话窗。
				//如果点击的是提示语或者是自己的名字，不操作。
				USES_CONVERSION;
				if (recverName == "当前用户列表："||recverName==string(W2A(username.c_str()))) {
					break;
				}
				else {
					//如果点击的是其他用户的名字，那么打开对应的对话窗口。
					//如果对话窗口已经打开，那么不操作。
					if (CurrentTarget == recverName) {
						break;
					}
					//反之，对话消息显示窗口更新到当前目标
					else {
						CurrentTarget = recverName;
						//发送窗口更新的消息
						PostMessage(hSubMainWindow2, WM_UPDATETEXTSHOWWINDOW, NULL, NULL);
					}
				}
			}
		}
		else {
			OnCommand(wParam);
		}
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
	case WM_CREATE: //当窗口创建的时候，会发送WM_CREATE消息
	{
		
		break;
	}
	case WM_COMMAND: //处理菜单被点击的操作
		OnCommand(wParam);
		break;
	case WM_SUCCESSLOGIN:
		//当得到成功登录的消息之后，才将菜单挂上。
		//使用setmenu函数来挂菜单。
		SetMenu(hMainWindow, LoadMenu(NULL, (LPCSTR)IDRETRY));
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
				//检查socket是否被关闭了，如果已经关闭，不执行以下操作
				if (isSocketStart == false) {

				}
				else {
					wstring buffer;
					buffer.push_back(LOGOUT_REQUEST);
					buffer.push_back(' ');
					buffer.append(username);
					buffer.push_back(' ');
					buffer.append(L"Server");
					buffer.push_back(' ');
					buffer.append(L"确认退出");
					USES_CONVERSION;
					string s(W2A(buffer.c_str()));
					s.push_back('\0');
					send(SendRecvSocket, s.c_str(), s.size(), NULL);
					//关闭连接
					
					closesocket(SendRecvSocket);
					
				}
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
		WS_CHILD | WS_VISIBLE | WS_TABSTOP,
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

HWND CreateSubMainWindow2(HINSTANCE hIns)
{
	WNDCLASS SubMainWindow = { 0 }; //创建一个窗口类 名字叫SubMainWindow
	SubMainWindow.cbClsExtra = 0;// 要根据窗口类结构分配的额外字节数，设置需要申请的缓冲区。
	SubMainWindow.cbWndExtra = 0; //在窗口实例之后分配的额外字节数，设置需要申请的缓冲区。
	SubMainWindow.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);//设置窗口的背景色，COLOR_WINDOW+1为白色。
	SubMainWindow.hCursor = NULL;//设置窗口光标，设置为NULL为默认光标。
	SubMainWindow.hIcon = NULL;//设置窗口图标，NULL为默认图标。
	SubMainWindow.hInstance = hIns; //当前程序实例句柄
	SubMainWindow.lpfnWndProc = SubMainWindow2Proc; //设置当前窗口的处理函数，传入处理函数的函数名即可。
	SubMainWindow.lpszClassName = "SubMainWindow2";//设置窗口类的名字。
	SubMainWindow.lpszMenuName = NULL; //设置窗口是否有菜单，NULL为没有菜单。
	SubMainWindow.style = CS_HREDRAW | CS_VREDRAW; //设置窗口风格，CS_HREDRAW和CS_VREDRAW为当窗口水平或者垂直大小变化时，重画窗口

	//注册窗口，向操作系统中写入窗口信息。
	RegisterClass(&SubMainWindow);
	HWND hSubMainWindowRes = CreateWindowEx(
		0,
		"SubMainWindow2",
		NULL,
		WS_CHILD | WS_TABSTOP,
		0,
		0,
		1000,
		700,
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
		WS_EX_WINDOWEDGE,
		"EDIT",
		NULL,
		WS_BORDER | WS_CHILD | WS_VISIBLE | ES_LEFT | WS_TABSTOP,
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
		(HMENU)BUTTONLOGIN,
		hIns,
		NULL
	);

	return hLoginButtonRes;
}

HWND CreateTextInput(HINSTANCE hIns)
{
	HWND hTextInputRes = CreateWindowEx(
		WS_EX_CLIENTEDGE,
		"EDIT",
		"开始聊天吧~",
		WS_VISIBLE | WS_CHILD | WS_BORDER | ES_AUTOHSCROLL | ES_LEFT,
		0,
		500,
		700,
		195,
		hSubMainWindow2,
		NULL,
		hIns,
		NULL
	);
	return hTextInputRes;
}

HWND CreateUserList(HINSTANCE hIns)
{

	HWND hUserListRes = CreateWindowEx(
		NULL,
		"LISTBOX",
		NULL,
		WS_CHILD | WS_VISIBLE | WS_BORDER | WS_VSCROLL | LBS_NOTIFY, 
		//LBS_NOTIFY属性表示通知应用程序用户已双击列表框中的项
		700,
		0,
		300,
		500,
		hSubMainWindow2,
		NULL,
		hIns,
		NULL
	);
	return hUserListRes;
}

HWND CreateSendButton(HINSTANCE hIns)
{

	HWND hSendButtonRes = CreateWindowEx(
		0,//窗口的额外风格设置
		"BUTTON",//创建的窗口类的名字
		"发送！",//文本内容
		WS_VISIBLE | WS_CHILD,//设置窗口的基本风格。
		700,
		575,
		300,
		65,
		hSubMainWindow2,//设置父窗口，没有置空NULL
		(HMENU)BUTTONSENDMESSAGE,//设置菜单，没有菜单为NULL
		hIns,//当前程序实例句柄
		NULL//没啥用的参数
	);
	return hSendButtonRes;
}

HWND CreateToolBar(HINSTANCE hIns)
{	
	//工具栏位于用户列表和发送按钮之间
	HWND hToolBarRes = CreateWindowEx(
		0,//窗口的额外风格设置
		"BUTTON",//创建的窗口类的名字
		"颜文字",//文本内容为空
		WS_CHILD | WS_VISIBLE ,//设置窗口的基本风格。
		700,
		500,
		300,
		75,
		hSubMainWindow2,//父窗口为主窗口
		(HMENU)KAOMOJI,//设置菜单，没有菜单为NULL
		hIns,//当前程序实例句柄
		NULL//没啥用的参数
		);
	return hToolBarRes;
}

HWND CreateTextShowWindow(HINSTANCE hIns)
{
	//创建消息显示框
	HWND hTextShowWindowRes = CreateWindowEx(
		WS_EX_CLIENTEDGE,  //额外的窗口扩展风格
		"EDIT",  //窗口类名
		NULL,  //窗口名字
		//设置为只读模式，多行模式，自动换行模式，可见模式，有垂直滚动条 
		WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_MULTILINE | ES_AUTOVSCROLL | ES_READONLY,
		0,
		0, 
		700, 
		500, 
		hSubMainWindow2, //父窗口是子窗口2
		(HMENU)USERLIST, 
		hIns,
		NULL);
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
		(HMENU)BUTTONTEST,//设置菜单，没有菜单为NULL
		hIns,//当前程序实例句柄
		NULL//没啥用的参数
	);
	return hStartServerRes;
}


LRESULT CALLBACK DlgProc(HWND hWnd, UINT msgID, WPARAM wParam, LPARAM LParam) {
	//当点击dialog框中的颜文字的时候，将颜文字发送到输入框中去。
	switch (msgID) {
	case IDC_KAOMOJI1:
		SendMessage(hTextInput, EM_REPLACESEL, NULL, (LPARAM)L"(இдஇ; )");
		break;
	case IDC_KAOMOJI2:
		SendMessage(hTextInput, EM_REPLACESEL, NULL, (LPARAM)L"Σ(っ °Д °;)っ");
		break;
	case IDC_KAOMOJI3:
		SendMessage(hTextInput, EM_REPLACESEL, NULL, (LPARAM)L"( ﾉД`)");
		break;
	case IDC_KAOMOJI4:
		SendMessage(hTextInput, EM_REPLACESEL, NULL, (LPARAM)L"(´-ωก`)");
		break;
	case IDC_KAOMOJI5:
		SendMessage(hTextInput, EM_REPLACESEL, NULL, (LPARAM)L"๑•́₃•̀๑");
		break;
	}
	return DefWindowProc(hWnd, msgID, wParam, LParam);
}


