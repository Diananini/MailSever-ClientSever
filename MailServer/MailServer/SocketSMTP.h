#pragma once



// SocketSMTP ����Ŀ��
//���ڴ����������ʾ����
class SocketSMTP : public CAsyncSocket
{
public:
	SocketSMTP();
	virtual ~SocketSMTP();
	virtual void OnReceive(int nErrorCode);
	// ��Ϣ����
	UINT m_nLength;
	// ��Ϣ������
	char m_szBuffer[908192];
	virtual void OnSend(int nErrorCode);
	// ��ǽ��յ�������
	bool data;
	// ����
	bool end;
	// ����ʼ�����
	CString m_data;
};


