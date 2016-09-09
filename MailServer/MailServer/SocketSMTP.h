#pragma once



// SocketSMTP 命令目标
//用于处理命令和显示附件
class SocketSMTP : public CAsyncSocket
{
public:
	SocketSMTP();
	virtual ~SocketSMTP();
	virtual void OnReceive(int nErrorCode);
	// 消息长度
	UINT m_nLength;
	// 消息缓冲区
	char m_szBuffer[908192];
	virtual void OnSend(int nErrorCode);
	// 标记接收的是数据
	bool data;
	// 附件
	bool end;
	// 存放邮件内容
	CString m_data;
};


