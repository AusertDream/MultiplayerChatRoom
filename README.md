# 基于win32和C++的多人聊天室

一款基于C++的多人聊天室，UI采用WIN32设计，通信采用winsock。<br>
只能实现局域网内通信，不能跨网络通信。<br>

## 技术栈
`C/C++`<br>
`win32`<br>
`STL`<br>
`winsock`<br>


## 使用说明
多人聊天室采用C/S服务模式，需要先启动服务端，客户端才能正常使用。<br>
聊天室默认容量100，想要修改可以更改Server源码中的MAX_NUMBER宏定义。<br>



## 未解决的问题
1.win32实现表情的浮窗
2.当前双端程序运行起来cpu占用率和电源使用率过高
3.在cpu运行速度较慢的时候，仍然会发生线程同步问题
4.对于消息队列的处理有极大的问题，由于消息处理和接收进行了线程分开，由于消息处理频繁的while循环，给保护消息队列的mutex频繁上锁解锁，导致消息接收时不时阻塞，会漏接消息。
5.写的依托的线程复用问题



