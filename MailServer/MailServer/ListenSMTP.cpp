// ListenSMTP.cpp : 实现文件
//

#include "stdafx.h"
#include "MailServer.h"
#include "ListenSMTP.h"
#include "MailServerDlg.h"

// ListenSMTP

ListenSMTP::ListenSMTP()
{
}

ListenSMTP::~ListenSMTP()
{
}


// ListenSMTP 成员函数


void ListenSMTP::OnAccept(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	//若套接字处于监听状态，当有客户连接的时候，此函数会被调用，在此函数中处理客户的连接请求
	
	CMailServerDlg*dlg=(CMailServerDlg*)AfxGetApp()->GetMainWnd();//取得主窗口指针对象
	dlg->m_ctrlRecord.AddString(L"$$$$$$$$$$$$$$$$$$$$$");
	dlg->m_ctrlRecord.AddString(L"***收到连接请求");
	SocketSMTP *pSocket =new SocketSMTP();  
    if(Accept(*pSocket))  
    {  
		dlg->m_ctrlRecord.AddString(L"***建立连接");
        pSocket->AsyncSelect(FD_WRITE); //触发通信socket的OnSend函数 
        //m_pSocket=pSocket;  
    }  
    else  
    {  
        delete pSocket;  
    }  

	//dlg->m_ctrlRecord.AddString(dlg->m_record);
	CAsyncSocket::OnAccept(nErrorCode);
}
