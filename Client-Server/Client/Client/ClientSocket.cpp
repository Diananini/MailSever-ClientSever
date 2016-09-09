// ClientSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Client.h"
#include "ClientSocket.h"
#include "ClientDlg.h"

// ClientSocket

ClientSocket::ClientSocket()
{
}

ClientSocket::~ClientSocket()
{
}


// ClientSocket 成员函数


void ClientSocket::OnReceive(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CClientDlg*dlg=(CClientDlg*)AfxGetApp()->GetMainWnd();//取得主窗口指针对象
	TCHAR respond[255];  
	ReceiveFrom(respond,255,dlg->m_IP,dlg->m_port);
	CString t(respond);
	dlg->m_respond=t;
	dlg->UpdateData(false);
	CAsyncSocket::OnReceive(nErrorCode);
}
