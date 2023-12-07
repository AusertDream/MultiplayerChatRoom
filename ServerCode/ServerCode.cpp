
#include "framework.h"
#include "SomeTools.h"
using namespace std;
typedef long long ll;
#define MAX_NUMBER  101
#define MAX_BUFFER_SIZE 1024

//初始化网络
 SOCKET InitServer();
 SOCKET Initsocket;
//处理收发的函数
 void SendRecvProc();
//发送函数
 void OnSend(SOCKET, UserParam);
//处理登录函数
 void OnLogIn(SOCKET, UserParam);
//处理登出函数
 void OnLogOut(SOCKET, UserParam);
//处理发送聊天记录函数
void OnSendChatRecord(SOCKET, UserParam,char);
//处理发送用户列表函数
void OnSendUserList(SOCKET, UserParam);
//存放当前在线用户
set<wstring> UserList;
//存放当前所有线程
vector<thread> allThread;
//存放当前所有socket链接
vector<SOCKET> allSocket;
//将线程ID单一映射到用户姓名上

//群聊的聊天记录
vector<UserMessage> ChatRecordGlobal;
vector<UserMessage> ChatRecordCN;

bool isStart = false; //用来表示服务端是否启动
int OnLineUserCnt = 0; //用来表示当前在线用户数

mutex ProtectCoutContorl; //为了格式化输出整齐点而设置的锁
mutex ProtectOnLineUserCnt; //用来保护OnLineUserCnt的互斥锁
mutex ProtectallSocket; //用来保护所有socket数组的互斥锁
mutex ProtectUserList; //用来保护UserList的互斥锁
mutex ProtectChatRecordGlobal; //用来保护ChatRecordGlobal的互斥锁
mutex ProtectChatRecordCN; //用来保护ChatRecordCN的互斥锁
//传输文件格式为  消息类型 发送者 接受者 消息内容 消息根据空格分开

//服务端文件
int main() {
	//初始化
	Initsocket = InitServer();
	UserList.insert(L"GLOBAL");
	UserList.insert(L"CN");
	UserList.insert(L"Server");
	UserList.insert(L"ALL");
	cout<<"启动成功"<<endl;
	//创建多线程处理收发消息
	for (int i = 0; i < MAX_NUMBER; i++) {
		allThread[i] = thread(SendRecvProc);
		allThread[i].detach();
	}
	cout << "输入help查看所有可用指令" << endl;
	string s;
	while (true) {
		cin >> s;
		if (s == "exit") {
			break;
		}
		else if (s == "showuserlist") {
			
			cout << "当前用户总人数：" << UserList.size() << endl;
			cout << "当前在线用户有：" << endl;
			ProtectUserList.lock();
			for (auto e : UserList) {
				USES_CONVERSION;
				cout<<string(W2A(e.c_str()))<<endl;
			}
			ProtectUserList.unlock();
		}
		else if (s == "showonlinenumber") {
			//显示当前在线人数
			ProtectOnLineUserCnt.lock();
			cout<<"当前在线人数："<<OnLineUserCnt<<endl;
			ProtectOnLineUserCnt.unlock();
		}
		else if (s == "showglobalrecord") {
			//显示全球聊天室聊天记录
			
			cout << "全球聊天室聊天记录：" << endl;
			ProtectChatRecordGlobal.lock();
			for (auto e : ChatRecordGlobal) {
				USES_CONVERSION;
				cout << string(W2A(e.UserName.c_str())) << "  " << string(W2A(e.Msg.c_str())) << endl;
			}
			ProtectChatRecordGlobal.unlock();
		}
		else if (s == "showcnrecord") {
			//显示中国聊天室聊天记录
			cout << "中国聊天室聊天记录：" << endl;
			ProtectChatRecordCN.lock();
			for (auto e : ChatRecordCN) {
				USES_CONVERSION;
				cout << string(W2A(e.UserName.c_str())) << "  " << string(W2A(e.Msg.c_str())) << endl;
			}
			ProtectChatRecordCN.unlock();
		
		}
		else if (s == "showsocketleft") {
			//显示当前可用socket数量
			ProtectallSocket.lock();
			int res = allSocket.size();
			ProtectallSocket.unlock();
			cout << "当前可用socket数量：" << MAX_NUMBER-res << endl;
		}
		else if (s == "help") {
			cout << "showuserlist:显示用户列表" << endl;
			cout<<"showonlinenumber:显示当前在线人数"<<endl;
			cout<<"showglobalrecord:显示全球聊天室聊天记录"<<endl;
			cout<<"showcnrecord:显示中国聊天室聊天记录"<<endl;
			cout<<"showsocketleft:显示当前可用socket数量"<<endl;
			cout<<"exit:退出程序"<<endl;
		}
		else {
			cout << "未知指令" << endl;
		}
	}
	WSACleanup();
	return 0;
}


SOCKET InitServer() {
	allThread.resize(MAX_NUMBER);
	//1.确定协议版本
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (LOBYTE(wsaData.wVersion) != 2 || HIBYTE(wsaData.wVersion) != 2) {
		cout<<"协议版本不是2.2"<<endl;
		//清理版本信息
		return NULL;
	}
	//2.创建socket
	//采用流传输模式，TCP协议。AF_INET表示使用的传输协议为TCP,UDP之类的协议。
	SOCKET sSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (SOCKET_ERROR == sSocket) {
		//如果创建失败输出报错信息
		cout<<"创建socket失败"<<endl;
		return NULL;
	}
	//3.确定服务器的协议地址簇
	SOCKADDR_IN addr = { 0 };
	addr.sin_family = AF_INET; //使用的是什么协议与创建SOCKET时候写的须保持一致
	addr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1"); //服务器的ip地址
	addr.sin_port = htons(10086); //服务器提供服务的端口号

	//4. 绑定
	int r = bind(sSocket, (sockaddr*)&addr, sizeof addr);
	if (r == -1) {

		//如果绑定失败输出报错信息
		closesocket(sSocket); //断开链接
		cout<<"绑定失败"<<endl;
		return NULL;
	}
	//5.监听
	r = listen(sSocket, 10);
	if (r == -1) {
		//如果监听失败输出报错信息
		closesocket(sSocket); //断开链接
		cout<<"监听失败"<<endl;
		return NULL;
	}
	return sSocket;
}





void OnSend(SOCKET cSocket, UserParam receivedData) {
	//创建发送的数据，并且填写信息
	UserParam SendData;
	SendData.Type = NORMAL_MSG;
	SendData.Msg = receivedData.Msg;
	SendData.Sender = receivedData.Sender;
	SendData.Receiver = receivedData.Receiver;
	//将结构体化为字符数组，然后使用send发送出去
	wstring buffer;
	buffer.push_back(SendData.Type);
	buffer.push_back(L' ');
	buffer.append(SendData.Sender);
	buffer.push_back(L' ');
	buffer.append(SendData.Receiver);
	buffer.push_back(L' ');
	buffer.append(SendData.Msg);
	buffer.push_back(L'\0');
	USES_CONVERSION;
	string s(W2A(buffer.c_str()));
	s.push_back('\0');
	//发送给所有客户端
	ProtectallSocket.lock();
	for (int i = 0; i < allSocket.size(); i++) {
		send(allSocket[i], s.c_str(), s.size(), NULL);
	}
	ProtectallSocket.unlock();
	//将聊天记录存入到聊天记录中
	if (SendData.Receiver == L"GLOBAL") {
		//读写互斥
		
		UserMessage temp;
		temp.Msg = SendData.Msg;
		temp.UserName = SendData.Sender;
		temp.timeInfo = CTime::GetCurrentTime();
		ProtectChatRecordGlobal.lock();
		ChatRecordGlobal.push_back(temp);
		ProtectChatRecordGlobal.unlock();
	}
	else if (SendData.Receiver == L"CN") {
		//读写互斥
		
		UserMessage temp;
		temp.Msg = SendData.Msg;
		temp.UserName = SendData.Sender;
		temp.timeInfo = CTime::GetCurrentTime();
		ProtectChatRecordCN.lock();
		ChatRecordCN.push_back(temp);
		ProtectChatRecordCN.unlock();
	}
}

void OnLogIn(SOCKET cSocket, UserParam receivedData) {
	USES_CONVERSION;
	//如果当前聊天室人已经满了
	ProtectOnLineUserCnt.lock();
	if (OnLineUserCnt == MAX_NUMBER - 1) {
		//对OnLineUserCnt进行解锁
		ProtectOnLineUserCnt.unlock();
		wstring buffer;
		buffer.push_back(LOGIN_RESULT);
		buffer.push_back(L' ');
		buffer.append(L"Server");
		buffer.push_back(L' ');
		buffer.append(receivedData.Sender);
		buffer.push_back(L' ');
		buffer.append(L"false");
		string s(W2A(buffer.c_str()));
		s.push_back('\0');
		send(cSocket, s.c_str(), s.size(), NULL);
		//在allsocket中删除当前链接，然后重新创建一个线程，用于处理
		int idx_Socket = -1;
		//在allsocket中暴搜寻找
		ProtectallSocket.lock();
		for (int i = 0; i < allSocket.size(); i++) {
			if (allSocket[i] == cSocket) {
				idx_Socket = i;
				break;
			}
		}
		ProtectallSocket.unlock();
		if (idx_Socket != -1) {
			//将原本的这个socket在allSocket中删除
			ProtectallSocket.lock();
			allSocket.erase(allSocket.begin() + idx_Socket);
			ProtectallSocket.unlock();
			//断开链接
			closesocket(cSocket);
			thread th(SendRecvProc);
			th.detach();
		}
		else {
			closesocket(cSocket);
		}
		return;
	}
	else {
		//在线人数加1
		OnLineUserCnt++;
		//对OnLineUserCnt进行解锁
		ProtectOnLineUserCnt.unlock();
	}
	//在对UserList进行操作的时候需要加锁
	ProtectUserList.lock();
	//如果已经存在该用户则返回false
	if (UserList.find(receivedData.Sender) != UserList.end()) {
		ProtectUserList.unlock();
		wstring buffer;
		buffer.push_back(LOGIN_RESULT);
		buffer.push_back(L' ');
		buffer.append(L"Server");
		buffer.push_back(L' ');
		buffer.append(receivedData.Sender);
		buffer.push_back(L' ');
		buffer.append(L"false");
		string s(W2A(buffer.c_str()));
		s.push_back('\0');
		send(cSocket, s.c_str(), s.size(), NULL);
		return;
	}
	//反之，则将用户名字加入到在线用户列表中
	else {
		UserList.insert(receivedData.Sender);
		ProtectUserList.unlock();
		wstring buffer;
		buffer.push_back(LOGIN_RESULT);
		buffer.push_back(L' ');
		buffer.append(L"Server");
		buffer.push_back(L' ');
		buffer.append(receivedData.Sender);
		buffer.push_back(L' ');
		buffer.append(L"true");
		string s(W2A(buffer.c_str()));
		s.push_back('\0');
		send(cSocket, s.c_str(), s.size(), NULL);
		//通知全部用户，有新用户登录
		s.clear();
		s.push_back(NEWUSERLOGIN);
		s.push_back(' ');
		s.append("Server");
		s.push_back(' ');
		s.append("ALL");
		s.push_back(' ');
		s.append(W2A(receivedData.Sender.c_str()));
		s.push_back('\0');
		ProtectallSocket.lock();
		for (int i = 0; i < allSocket.size(); i++) {
			send(allSocket[i], s.c_str(), s.size(), NULL);
		}
		ProtectallSocket.unlock();
		return;
	}
}

void OnLogOut(SOCKET cSocket, UserParam receivedData) {
	//将用户从在线用户列表中删除
	//在对UserList进行操作的时候需要加锁
	ProtectUserList.lock();
	UserList.erase(receivedData.Sender);
	ProtectUserList.unlock();
	string buffer;
	buffer.push_back(LOGOUT_RESULT);
	buffer.push_back(' ');
	buffer.append("Server");
	buffer.push_back(' ');
	buffer.append("ALL");
	buffer.push_back(' ');
	USES_CONVERSION;
	buffer.append(W2A(receivedData.Sender.c_str()));
	buffer.push_back('\0');
	ProtectallSocket.lock();
	for (int i = 0; i < allSocket.size(); i++) {
		send(allSocket[i], buffer.c_str(), buffer.size(), NULL);
	}
	ProtectallSocket.unlock();
	//在线人数减1
	ProtectOnLineUserCnt.lock();
	OnLineUserCnt--;
	ProtectOnLineUserCnt.unlock();
	int idx_Socket = -1;
	//在allsocket中暴搜寻找
	ProtectallSocket.lock();
	for (int i = 0; i < allSocket.size(); i++) {
		if (allSocket[i] == cSocket) {
			idx_Socket = i;
			break;
		}
	}
	ProtectallSocket.unlock();
	if (idx_Socket != -1) {
		//将原本的这个socket在allSocket中删除
		ProtectallSocket.lock();
		allSocket.erase(allSocket.begin() + idx_Socket);
		ProtectallSocket.unlock();
		//断开链接
		closesocket(cSocket);
		cSocket=INVALID_SOCKET;
		thread th(SendRecvProc);
		th.detach();
	}
	else {
		closesocket(cSocket);
	}
}


//处理发送聊天记录函数
void OnSendChatRecord(SOCKET cSocket, UserParam receivedData,char Type) {
	USES_CONVERSION;
	if (Type == GLOBALRECORD_REQUEST) {
		//将聊天记录发送给客户端
		//发送聊天记录的时候需要读写互斥
		ProtectChatRecordGlobal.lock();
		for (auto e : ChatRecordGlobal) {
			string sendbuff;
			sendbuff.push_back(GLOBALRECORD_RESULT);
			sendbuff.push_back(' ');
			sendbuff.append(W2A(e.UserName.c_str()));
			sendbuff.push_back(' ');
			sendbuff.append(W2A(receivedData.Sender.c_str()));
			sendbuff.push_back(' ');
			sendbuff.append(W2A(e.Msg.c_str()));
			sendbuff.push_back('\0');
			send(cSocket, sendbuff.c_str(), sendbuff.size(), NULL);
			Sleep(100);
		}
		ProtectChatRecordGlobal.unlock();
	}
	else if (Type == CNRECORD_REQUEST) {
		//将聊天记录发送给客户端
		//发送聊天记录的时候需要读写互斥
		ProtectChatRecordCN.lock();
		for (auto e : ChatRecordCN) {
			string sendbuff;
			sendbuff.push_back(CNRECORD_RESULT);
			sendbuff.push_back(' ');
			sendbuff.append(W2A(e.UserName.c_str()));
			sendbuff.push_back(' ');
			sendbuff.append(W2A(receivedData.Sender.c_str()));
			sendbuff.push_back(' ');
			sendbuff.append(W2A(e.Msg.c_str()));
			sendbuff.push_back('\0');
			send(cSocket, sendbuff.c_str(), sendbuff.size(), NULL);
			Sleep(100);
		}
		ProtectChatRecordCN.unlock();
	}
}
//处理发送用户列表函数
void OnSendUserList(SOCKET cSocket, UserParam receivedData) {
	//将列表的所有信息发送给客户端。
	string buff;
	buff.push_back(USERLIST_RESULT);
	buff.push_back(' ');
	buff.append("Server");
	buff.push_back(' ');
	USES_CONVERSION;
	buff.append(W2A(receivedData.Sender.c_str()));
	buff.push_back(' ');
	ProtectUserList.lock();
	for (auto e : UserList) {
		string sendbuff = buff;
		string username=W2A(e.c_str());
		string sendername= W2A(receivedData.Sender.c_str());
		if (username != "GLOBAL" && username != "CN" && username != "Server" && username != sendername&&username!="ALL") {
			sendbuff += username;
		}
		else {
			continue;
		}
		sendbuff.push_back('\0');
		send(cSocket, sendbuff.c_str(), sendbuff.size(), NULL);
		Sleep(100);
	}
	ProtectUserList.unlock();
}

void SendRecvProc() {
	bool isQuit = false;
	atomic<bool> shouldExit(false); //用来表示子线程是否应该退出
	mutex ProtectProcQueue; //用来保护ProcQueue的互斥锁
	//消息处理队列
	queue<UserParam> ProcQueue;
	//消息处理函数
	auto MsgProc = [&](SOCKET cSocket, UserParam receivedData) {
		USES_CONVERSION;
		string HintMsg;
		//输出提示信息
		HintMsg.append("接收到用户信息：");
		switch (receivedData.Type)
		{
		case NORMAL_MSG:
			HintMsg.append("普通消息 ");
			break;
		case LOGIN_REQUEST:
			HintMsg.append("登录请求 ");
			break;
		case LOGOUT_REQUEST:
			HintMsg.append("登出请求 ");
			break;
		case USERLIST_REQUEST:
			HintMsg.append("用户列表请求 ");
			break;
		case LOGIN_RESULT:
			HintMsg.append("登录结果 ");
			break;
		case LOGOUT_RESULT:
			HintMsg.append("登出结果 ");
			break;
		case NEWUSERLOGIN:
			HintMsg.append("新用户登录信息 ");
			break;
		case GLOBALRECORD_REQUEST:
			HintMsg.append("全球聊天室聊天记录请求 ");
			break;
		case GLOBALRECORD_RESULT:
			HintMsg.append("全球聊天室聊天记录结果 ");
			break;
		case CNRECORD_REQUEST:
			HintMsg.append("中国聊天室聊天记录请求 ");
			break;
		case CNRECORD_RESULT:
			HintMsg.append("中国聊天室聊天记录结果 ");
			break;
		case USERLIST_RESULT:
			HintMsg.append("用户列表结果 ");
			break;
		default:
			HintMsg.append("未知消息类型 ");
			break;
		}
		HintMsg.append("发送者：");
		HintMsg.append(string(W2A(receivedData.Sender.c_str())));
		HintMsg.push_back(' ');
		HintMsg.append("接收者：");
		HintMsg.append(string(W2A(receivedData.Receiver.c_str())));
		HintMsg.push_back(' ');
		HintMsg.append("消息内容：");
		HintMsg.append(string(W2A(receivedData.Msg.c_str())));
		HintMsg.push_back(' ');
		//根据数据类型来处理消息内容
		switch (receivedData.Type) {
		case NORMAL_MSG:
			OnSend(cSocket, receivedData);
			break;
		case LOGIN_REQUEST:
			OnLogIn(cSocket, receivedData);
			break;
		case LOGOUT_REQUEST:
			OnLogOut(cSocket, receivedData);
			break;
		case USERLIST_REQUEST:
			OnSendUserList(cSocket, receivedData);
			break;
		case GLOBALRECORD_REQUEST:
			OnSendChatRecord(cSocket, receivedData, GLOBALRECORD_REQUEST);
			break;
		case CNRECORD_REQUEST:
			OnSendChatRecord(cSocket, receivedData, CNRECORD_REQUEST);
			break;
		default:
			cout << "未知消息: " << receivedData.Type << " 不处理  " << endl;
			break;
		}
		ProtectCoutContorl.lock();
		cout << HintMsg << endl;
		ProtectCoutContorl.unlock();
	};
	

	//6.等待客户端链接
	SOCKET cSocket = accept(Initsocket, NULL, NULL);
	if (cSocket == SOCKET_ERROR) {
		//如果等待失败说明服务器崩溃输出报错信息
		//断开链接
		closesocket(cSocket);
		//重新创建一个线程
		thread th(SendRecvProc);
		th.detach();
		cout << "服务器崩溃" << endl;
		return;
	}
	//将当前socket加入到allsocket中
	ProtectallSocket.lock();
	allSocket.push_back(cSocket);
	ProtectallSocket.unlock();
	//消息处理线程函数
	auto thProc = [&]() {
		while (true) {
			//如果处理队里中有消息则处理
			//如果socket已经断开则退出
			if (shouldExit == true) {
				break;
			}
			unique_lock<mutex> lock(ProtectProcQueue);
			if (ProcQueue.size()>0) {
				//取出队列中的消息
				UserParam temp = ProcQueue.front();
				ProcQueue.pop();
				lock.unlock();
				//处理消息
				MsgProc(cSocket, temp);
			}
			else {
				Sleep(1);
			}
		}
	};
	thread th(thProc);
	int ret = 0;
	//7.通信
	char recvBuff[MAX_BUFFER_SIZE];
	while (true) {
		//接收消息
		ret = recv(cSocket, recvBuff, MAX_BUFFER_SIZE, NULL);
		if (ret <= 0) {
			//如果消息接收异常
			closesocket(cSocket); //断开链接
			break;
		}
		//将接收到的数据转换为宽字符
		USES_CONVERSION;
		wchar_t* wRecvBuff = A2W(recvBuff);
		wstring buffer(wRecvBuff);
		//将接受到的数据解包，分析。
		UserParam receivedData;
		openTheBuff(buffer, receivedData);
		//将接收到的数据放入到消息队列中
		ProtectProcQueue.lock();
		ProcQueue.push(receivedData);
		ProtectProcQueue.unlock();
	}
	shouldExit = true;
	th.join();
}