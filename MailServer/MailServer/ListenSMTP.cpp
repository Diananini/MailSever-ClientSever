// ListenSMTP.cpp : ʵ���ļ�
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


// ListenSMTP ��Ա����


void ListenSMTP::OnAccept(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	//���׽��ִ��ڼ���״̬�����пͻ����ӵ�ʱ�򣬴˺����ᱻ���ã��ڴ˺����д���ͻ�����������
	
	CMailServerDlg*dlg=(CMailServerDlg*)AfxGetApp()->GetMainWnd();//ȡ��������ָ�����
	dlg->m_ctrlRecord.AddString(L"$$$$$$$$$$$$$$$$$$$$$");
	dlg->m_ctrlRecord.AddString(L"***�յ���������");
	SocketSMTP *pSocket =new SocketSMTP();  
    if(Accept(*pSocket))  
    {  
		dlg->m_ctrlRecord.AddString(L"***��������");
        pSocket->AsyncSelect(FD_WRITE); //����ͨ��socket��OnSend���� 
        //m_pSocket=pSocket;  
    }  
    else  
    {  
        delete pSocket;  
    }  

	//dlg->m_ctrlRecord.AddString(dlg->m_record);
	CAsyncSocket::OnAccept(nErrorCode);
}
