#pragma once
#include "socketsmtp.h"

// ListenSMTP ����Ŀ��
//������ʾͨѶ����
class ListenSMTP : public CAsyncSocket
{
public:
	ListenSMTP();
	virtual ~ListenSMTP();
	virtual void OnAccept(int nErrorCode);
	// ָ��һ�����ӵ�socket����
	SocketSMTP*m_pSocket;
};


