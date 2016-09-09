// SocketSMTP.cpp : 实现文件
//

#include "stdafx.h"
#include "MailServer.h"
#include "SocketSMTP.h"
#include "MailServerDlg.h"
#include <atlimage.h>
#include <regex>
#include <sstream>
#include <fstream>
using namespace std;
// SocketSMTP

SocketSMTP::SocketSMTP()
	: m_nLength(0)
	, data(false)
	, end(false)
	, m_data(_T(""))
{
	memset(m_szBuffer,0,sizeof(m_szBuffer));
	m_data.Empty();
}

SocketSMTP::~SocketSMTP()
{
	if(m_hSocket !=INVALID_SOCKET)  
    {  
        Close();  
    }  

}


// SocketSMTP 成员函数


void SocketSMTP::OnReceive(int nErrorCode)//接收并处理命令和数据，
{
	// TODO: 在此添加专用代码和/或调用基类
	CMailServerDlg*dlg=(CMailServerDlg*)AfxGetApp()->GetMainWnd();//取得主窗口指针对象
	memset(m_szBuffer,0,sizeof(m_szBuffer)); 
	m_nLength=Receive(m_szBuffer,sizeof(m_szBuffer),0); //接收数据  
     
	CString buff0(m_szBuffer);
	
	while(data&&!end)//防止缓冲区不够大，可多次接收存储到m_data
	{
		m_data+=buff0;
		if(buff0.Right(5)!="\r\n.\r\n")//未接收完所有数据
			return;
		else
			end=true;//数据接收完毕
	}

	
	if(buff0.Left(4)=="EHLO"||buff0.Left(4)=="HELO")
	{
		dlg->m_ctrlRecord.AddString(L"C:"+buff0);
		char buff[100]={"250\n\n"};
		Send(buff,strlen(buff));//服务器响应250，表示请求的命令成功完成
		dlg->m_ctrlRecord.AddString(L"S:250 OK 127.0.0.1");
		AsyncSelect(FD_READ); //触发通信socket的OnReceive函数 
	}
	if(buff0.Left(10)=="MAIL FROM:")
	{
		dlg->m_ctrlRecord.AddString(L"C:"+buff0);
		char buff[100]={"250\r\n"};
		Send(buff,strlen(buff));//服务器响应250，表示请求的命令成功完成
		dlg->m_ctrlRecord.AddString(L"S:250 Sender OK");
		AsyncSelect(FD_READ); //触发通信socket的OnReceive函数 
	}
	if(buff0.Left(8)=="RCPT TO:")
	{
		dlg->m_ctrlRecord.AddString(L"C:"+buff0);
		char buff[100]={"250\r\n"};
		Send(buff,strlen(buff));//服务器响应250，表示请求的命令成功完成
		dlg->m_ctrlRecord.AddString(L"S:250 Receiver OK");
		AsyncSelect(FD_READ); //触发通信socket的OnReceive函数 
	}
	if(buff0.Left(4)=="DATA")
	{
		dlg->m_ctrlRecord.AddString(L"C:"+buff0);
		char buff[100]={"354\r\n"};
		Send(buff,strlen(buff));//服务器响应250，表示请求的命令成功完成
		dlg->m_ctrlRecord.AddString(L"S:354  Go ahead. End with <CRLF>.<CRLF>");
		data=true;
		AsyncSelect(FD_READ); //触发通信socket的OnReceive函数 
		return;
	}
	if(data&&end)
	{

		char temp[908192];
		dlg->m_ctrlContent.AddString(L"$$$$$$$$$$$$$$$$$$$$$$$");
		bool content=false;
		int tag=0;//记录遇到base64之后第几次遇到两个回车换行
		//第一个base64出现之后两个回车换行之后到“------”出现前两个回车换行之间的部分是正文的编码
		CString content_all;//存储邮件正文的解码
		content_all.Empty();

		for(int i=0;i<m_data.GetLength()-1;i++)
		{
			int k=0;
			CString t(temp);
			if(t.Right(6)=="base64"&&tag==0)//得到正文编码
				content=true;
			memset(temp,0,sizeof(temp));
			while(!(m_data[i]=='\r'&&m_data[i+1]=='\n'))//得到一行数据存到temp
				temp[k++]=m_data[i++];
			
			CString buff(temp);
			dlg->m_ctrlContent.AddString(buff);
			if(content)
				tag++;

			buff.Remove('\r');
			buff.Remove('\n');

			if(buff.Left(6)==L"------")
				content=false;
			if(content&&tag!=1)
				content_all=content_all+buff;
		}
		
		int len;
		CString decodeContent=dlg->base64Decode1(content_all,&len);
		int pos_first=0,pos_last;//分行显示邮件正文
		pos_last=decodeContent.Find('\r',pos_first);
		while(pos_last!=-1)
		{
			CString temp=decodeContent.Mid(pos_first,pos_last-pos_first);
			dlg->m_ctrlBody.AddString(temp);
			pos_first=pos_last+4;
			pos_last=decodeContent.Find('\r',pos_first);
		}


		//附件的处理
		m_data.Remove('\r');
		m_data.Remove('\n');
		int index0,index1;
		index0=m_data.Find(L"filename");//filename出现之后第一个双引号括起来的是附件名
		if(index0!=-1)//有附件
		{
			
			index1=m_data.Find('"',index0);
			index0=m_data.Find('"',index1+1);

			CString filename;
			filename.Empty();

			filename=m_data.Mid(index1+1,index0-index1-1);//附件名的编码

			
			index1=m_data.Find(L"------",index0);//”出现之后------出现之前的内容除去回车换行就是附件的编码

			CString fileContent=m_data.Mid(index0+1,index1-index0-1);

			std::vector<char> decodeFileContent;
			dlg->base64Decode2(fileContent,decodeFileContent);//附件内容的解码

			//附件解码写到磁盘文件
			std::fstream fout(filename, std::ios_base::out | std::ios_base::binary);;
			fout.write(static_cast<const char*>(&decodeFileContent[0]), decodeFileContent.size());
			fout.close();


			//图片显示
			CImage img;
			img.Load(filename);
			if (!img.IsNull()) 
			{
				SetStretchBltMode(dlg->m_ctrlPicture.GetDC()->GetSafeHdc(), HALFTONE);


				int w=img.GetWidth();
				int h=img.GetHeight();
				 // 找出宽和高中的较大值者
				int max=(w>h)?w:h;
				CRect dest;
				dlg->m_ctrlPicture.GetClientRect(&dest);
				int max1=(dest.Width()<dest.Height())?dest.Height():dest.Width();
				// 计算将图片缩放到TheImage区域所需的比例因子
				float scale = (float) ( (float) max / (float)max1 );
    
				// 缩放后图片的宽和高
				int nw = (int)( w/scale );
				int nh = (int)( h/scale );

				// 为了将缩放后的图片存入 正中部位，需计算图片在 rectDraw 左上角的期望坐标值
				int tlx = (nw > nh)? 0: (int)(dest.Width()-nw)/2;
				int tly = (nw > nh)? (int)(dest.Height()-nh)/2: 0;

				// 设置 rectDraw,用来存入图片 img
				CRect rectDraw = dest;
				rectDraw.SetRect(tlx, tly, nw, nh);

				img.Draw(dlg->m_ctrlPicture.GetDC()->GetSafeHdc(), rectDraw);
				
			}
		}

		char buff[100]={"250\r\n"};
		Send(buff,strlen(buff));//服务器响应250，表示请求的命令成功完成
		dlg->m_ctrlRecord.AddString(L"S:250  Message accepted for delivery!");
		data=false;
		AsyncSelect(FD_READ); //触发通信socket的OnReceive函数 
		return;
	}
		

	if(buff0.Left(4)=="QUIT")
	{
		dlg->m_ctrlRecord.AddString(L"C:"+buff0);
		char buff[100]={"221\r\n"};
		Send(buff,strlen(buff));//服务器响应250，表示请求的命令成功完成
		dlg->m_ctrlRecord.AddString(L"S:Quit,Goodbye!");
	}
	CAsyncSocket::OnReceive(nErrorCode);
}


void SocketSMTP::OnSend(int nErrorCode)
{
	// TODO: 在此添加专用代码和/或调用基类
	CMailServerDlg*dlg=(CMailServerDlg*)AfxGetApp()->GetMainWnd();//取得主窗口指针对象
	char buff[100]={"220\r\n"};
	Send(buff,strlen(buff));//服务器响应220，表示建立连接
	dlg->m_ctrlRecord.AddString(L"S:Simple Mail Server Ready For Mail");
	AsyncSelect(FD_READ); //触发通信socket的OnReceive函数 
	CAsyncSocket::OnSend(nErrorCode);
}
