#pragma once
#include "socketsmtp.h"

// ListenSMTP 命令目标
//用于显示通讯过程
class ListenSMTP : public CAsyncSocket
{
public:
	ListenSMTP();
	virtual ~ListenSMTP();
	virtual void OnAccept(int nErrorCode);
	// 指向一个连接的socket对象
	SocketSMTP*m_pSocket;
};


