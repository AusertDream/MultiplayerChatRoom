// SourceCode.cpp : 定义应用程序的入口点。
//

#include "framework.h"
#include "SourceCode.h"
using namespace std;




int WINAPI WinMain(HINSTANCE hIns,
					HINSTANCE hPreIns,
					LPSTR LpCmdLine,
					int nCmdShow){
	int t;
	t=MessageBox(NULL, L"hello world", L"InFo", MB_YESNOCANCEL|MB_ICONERROR);
	string s;
	
	if (t == IDYES) {
		s = "fjielajf";
		wstring ss(s.begin(),s.end());
		MessageBox(NULL, L"fda", ss.c_str(), MB_YESNOCANCEL | MB_ICONERROR);
	}
	
	return 0;
}