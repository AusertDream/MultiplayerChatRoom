#pragma once
#include "framework.h"
#include "SomeTools.h"
using namespace std;
typedef long long ll;



#define NORMAL_MSG  '1'
#define LOGIN_REQUEST  '2'
#define LOGOUT_REQUEST '3'
#define LOGIN_RESULT '4'

//template<typename T> //使用函数模板的MyRandom函数
//T MyRandom(T min, T max) { //采用C++提供的随机数生成引擎来生成随机数
//    random_device seed;//硬件生成随机数种子
//    ranlux48 engine(seed());//利用种子生成随机数引擎
//    uniform_int_distribution<> distrib(min, max);//设置随机数范围，随机数分布方式为均匀分布
//    T res = distrib(engine);//随机数
//    return res;
//}





//序列化函数现已废弃
// 序列化函数：将结构体数据复制到字符数组中 方便使用socket发送数据
void serialize(const UserParam& data, char* buffer) {
    memcpy(buffer, &data, sizeof(UserParam));
}

// 反序列化函数：将字符数组转换为结构体数据 解包接受到的数据
void deserialize(const char* buffer, UserParam& data) {
    memcpy(&data, buffer, sizeof(UserParam));
}


//解包函数
void openTheBuff(wstring buffer, UserParam& data) {
	int cnt = 0;
	for (int l = 0; l < buffer.size(); l++) {
		int r = l + 1;
		while (r < buffer.size() && buffer[r] != L' ') {
			r++;
		}
		if (cnt == 0) {
			data.Type = (char)buffer[l];//将数据类型存入type
		}
		else if (cnt == 1) {
			data.Sender = buffer.substr(l, r - l);//将发送者存入sender
		}
		else if (cnt == 2) {
			data.Receiver = buffer.substr(l, r - l);
		}
		else if (cnt == 3) {
			if (data.Type == LOGIN_RESULT) {
				if (buffer.substr(l, r - l) == L"true") {
					data.res = true;
				}
				else {
					data.res = false;
				}
			}
			else {
				data.Msg = buffer.substr(l, r - l);
			}
		}
		cnt++;
		l = r;
	}
}



// string转wstring
wstring StringToWstring(const string& str)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.from_bytes(str);
}
// wstring转string 
string WstringToString(const wstring& wstr)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
	return converter.to_bytes(wstr);
}



/*
将string类型转化为wchar_t*方法

1.先将string转化为wstring
2.然后将wstring转化为wchar_t*

string s;
wstring ss(s.begin(),s.end());
wchar_t* res=ss.c_str();
*/