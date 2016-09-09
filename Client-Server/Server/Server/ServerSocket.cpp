// ClientSocket.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Server.h"
#include "ServerSocket.h"
#include "ServerDlg.h"

// ClientSocket

ServerSocket::ServerSocket()
	
: port(3001)
{
	//��ñ���IP��ַ
	WORD wVersionRequested;
    WSADATA wsaData;
    char name[255];
    PHOSTENT hostinfo;
    wVersionRequested=MAKEWORD(2,0);
    if(WSAStartup( wVersionRequested,&wsaData )==0 )
    {
        if(gethostname(name, sizeof(name))==0)
        {
            if((hostinfo = gethostbyname(name)) != NULL)
            {
                 IP=inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
            }
        }
        WSACleanup( );
    } 
}

ServerSocket::~ServerSocket()
{
}


// ClientSocket ��Ա����

//OnReceive���������ݷ��͵����Զ����ã��˺�������Ҫ�����ǽ�������
void ServerSocket::OnReceive(int nErrorCode)
{
	
	// TODO: �ڴ����ר�ô����/����û���
	CServerDlg*dlg=(CServerDlg*)AfxGetApp()->GetMainWnd();//ȡ��������ָ�����
	ReceiveFrom(request,1000,IP,port);

	/*CString strDate,strTime;
	CTime time=CTime::GetCurrentTime();//����CTime����
	strDate.Format(L"%Y-%m-%d",time.GetYear(),time.GetMonth(),time.GetDay());
	strTime.Format(L"%H:%M:%S",time.GetHour(),time.GetMinute(),time.GetSecond());*/

	CString strDate=CTime::GetCurrentTime().Format(L"%Y-%m-%d");
	CString strTime=CTime::GetCurrentTime().Format(L"%H:%M:%S");


	CString respond;//��Ӧ

	CString request0(request);
	request0.MakeLower();
	if(request0=="date")//��������
		respond=strDate;
	else
		if(request0=="time")//����ʱ��
			respond=strTime;
		else//��������
			respond="Error request!";
	
	CString str,str0;  //��ListBox׷���ַ���

	SendTo(respond.GetBuffer(),(respond.GetLength()+1)*sizeof(TCHAR),port,IP);

	//������־
	str=strDate+L"  "+strTime+L"  �յ�IP="+IP+L" Port=3001"+L"����"+request0+L"��,��Ӧ��"+respond+L"��"; 
	dlg->m_ctrlRecord.AddString(str);

	CAsyncSocket::OnReceive(nErrorCode);  
}
