// ClientSocket.cpp : ʵ���ļ�
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


// ClientSocket ��Ա����


void ClientSocket::OnReceive(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	CClientDlg*dlg=(CClientDlg*)AfxGetApp()->GetMainWnd();//ȡ��������ָ�����
	TCHAR respond[255];  
	ReceiveFrom(respond,255,dlg->m_IP,dlg->m_port);
	CString t(respond);
	dlg->m_respond=t;
	dlg->UpdateData(false);
	CAsyncSocket::OnReceive(nErrorCode);
}
