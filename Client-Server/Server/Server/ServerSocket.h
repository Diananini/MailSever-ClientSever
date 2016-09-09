#pragma once
#include "afxtempl.h"

// ClientSocket 命令目标

class ServerSocket : public CAsyncSocket
{
public:
	ServerSocket();
	virtual ~ServerSocket();
	virtual void OnReceive(int nErrorCode);
	// 用户请求
	TCHAR request[1000];
	CString IP;
	// 端口号
	UINT port;
private:
//	POSITION m_pos;
//	CTypedPtrList <CObList,datetime*> m_pDataList;
};


