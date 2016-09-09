// ClientSocket.cpp : 实现文件
//

#include "stdafx.h"
#include "Server.h"
#include "ServerSocket.h"
#include "ServerDlg.h"

// ClientSocket

ServerSocket::ServerSocket()
	
: port(3001)
{
	//获得本机IP地址
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


// ClientSocket 成员函数

//OnReceive函数在数据发送到后自动调用，此函数的主要作用是接收数据
void ServerSocket::OnReceive(int nErrorCode)
{
	
	// TODO: 在此添加专用代码和/或调用基类
	CServerDlg*dlg=(CServerDlg*)AfxGetApp()->GetMainWnd();//取得主窗口指针对象
	ReceiveFrom(request,1000,IP,port);

	/*CString strDate,strTime;
	CTime time=CTime::GetCurrentTime();//构造CTime对象
	strDate.Format(L"%Y-%m-%d",time.GetYear(),time.GetMonth(),time.GetDay());
	strTime.Format(L"%H:%M:%S",time.GetHour(),time.GetMinute(),time.GetSecond());*/

	CString strDate=CTime::GetCurrentTime().Format(L"%Y-%m-%d");
	CString strTime=CTime::GetCurrentTime().Format(L"%H:%M:%S");


	CString respond;//响应

	CString request0(request);
	request0.MakeLower();
	if(request0=="date")//请求日期
		respond=strDate;
	else
		if(request0=="time")//请求时间
			respond=strTime;
		else//其他请求
			respond="Error request!";
	
	CString str,str0;  //向ListBox追加字符串

	SendTo(respond.GetBuffer(),(respond.GetLength()+1)*sizeof(TCHAR),port,IP);

	//工作日志
	str=strDate+L"  "+strTime+L"  收到IP="+IP+L" Port=3001"+L"请求【"+request0+L"】,响应【"+respond+L"】"; 
	dlg->m_ctrlRecord.AddString(str);

	CAsyncSocket::OnReceive(nErrorCode);  
}
