#pragma once
#include "afxtempl.h"

// ClientSocket ����Ŀ��

class ServerSocket : public CAsyncSocket
{
public:
	ServerSocket();
	virtual ~ServerSocket();
	virtual void OnReceive(int nErrorCode);
	// �û�����
	TCHAR request[1000];
	CString IP;
	// �˿ں�
	UINT port;
private:
//	POSITION m_pos;
//	CTypedPtrList <CObList,datetime*> m_pDataList;
};


