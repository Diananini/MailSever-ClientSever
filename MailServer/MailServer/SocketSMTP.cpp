// SocketSMTP.cpp : ʵ���ļ�
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


// SocketSMTP ��Ա����


void SocketSMTP::OnReceive(int nErrorCode)//���ղ�������������ݣ�
{
	// TODO: �ڴ����ר�ô����/����û���
	CMailServerDlg*dlg=(CMailServerDlg*)AfxGetApp()->GetMainWnd();//ȡ��������ָ�����
	memset(m_szBuffer,0,sizeof(m_szBuffer)); 
	m_nLength=Receive(m_szBuffer,sizeof(m_szBuffer),0); //��������  
     
	CString buff0(m_szBuffer);
	
	while(data&&!end)//��ֹ�����������󣬿ɶ�ν��մ洢��m_data
	{
		m_data+=buff0;
		if(buff0.Right(5)!="\r\n.\r\n")//δ��������������
			return;
		else
			end=true;//���ݽ������
	}

	
	if(buff0.Left(4)=="EHLO"||buff0.Left(4)=="HELO")
	{
		dlg->m_ctrlRecord.AddString(L"C:"+buff0);
		char buff[100]={"250\n\n"};
		Send(buff,strlen(buff));//��������Ӧ250����ʾ���������ɹ����
		dlg->m_ctrlRecord.AddString(L"S:250 OK 127.0.0.1");
		AsyncSelect(FD_READ); //����ͨ��socket��OnReceive���� 
	}
	if(buff0.Left(10)=="MAIL FROM:")
	{
		dlg->m_ctrlRecord.AddString(L"C:"+buff0);
		char buff[100]={"250\r\n"};
		Send(buff,strlen(buff));//��������Ӧ250����ʾ���������ɹ����
		dlg->m_ctrlRecord.AddString(L"S:250 Sender OK");
		AsyncSelect(FD_READ); //����ͨ��socket��OnReceive���� 
	}
	if(buff0.Left(8)=="RCPT TO:")
	{
		dlg->m_ctrlRecord.AddString(L"C:"+buff0);
		char buff[100]={"250\r\n"};
		Send(buff,strlen(buff));//��������Ӧ250����ʾ���������ɹ����
		dlg->m_ctrlRecord.AddString(L"S:250 Receiver OK");
		AsyncSelect(FD_READ); //����ͨ��socket��OnReceive���� 
	}
	if(buff0.Left(4)=="DATA")
	{
		dlg->m_ctrlRecord.AddString(L"C:"+buff0);
		char buff[100]={"354\r\n"};
		Send(buff,strlen(buff));//��������Ӧ250����ʾ���������ɹ����
		dlg->m_ctrlRecord.AddString(L"S:354  Go ahead. End with <CRLF>.<CRLF>");
		data=true;
		AsyncSelect(FD_READ); //����ͨ��socket��OnReceive���� 
		return;
	}
	if(data&&end)
	{

		char temp[908192];
		dlg->m_ctrlContent.AddString(L"$$$$$$$$$$$$$$$$$$$$$$$");
		bool content=false;
		int tag=0;//��¼����base64֮��ڼ������������س�����
		//��һ��base64����֮�������س�����֮�󵽡�------������ǰ�����س�����֮��Ĳ��������ĵı���
		CString content_all;//�洢�ʼ����ĵĽ���
		content_all.Empty();

		for(int i=0;i<m_data.GetLength()-1;i++)
		{
			int k=0;
			CString t(temp);
			if(t.Right(6)=="base64"&&tag==0)//�õ����ı���
				content=true;
			memset(temp,0,sizeof(temp));
			while(!(m_data[i]=='\r'&&m_data[i+1]=='\n'))//�õ�һ�����ݴ浽temp
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
		int pos_first=0,pos_last;//������ʾ�ʼ�����
		pos_last=decodeContent.Find('\r',pos_first);
		while(pos_last!=-1)
		{
			CString temp=decodeContent.Mid(pos_first,pos_last-pos_first);
			dlg->m_ctrlBody.AddString(temp);
			pos_first=pos_last+4;
			pos_last=decodeContent.Find('\r',pos_first);
		}


		//�����Ĵ���
		m_data.Remove('\r');
		m_data.Remove('\n');
		int index0,index1;
		index0=m_data.Find(L"filename");//filename����֮���һ��˫�������������Ǹ�����
		if(index0!=-1)//�и���
		{
			
			index1=m_data.Find('"',index0);
			index0=m_data.Find('"',index1+1);

			CString filename;
			filename.Empty();

			filename=m_data.Mid(index1+1,index0-index1-1);//�������ı���

			
			index1=m_data.Find(L"------",index0);//������֮��------����֮ǰ�����ݳ�ȥ�س����о��Ǹ����ı���

			CString fileContent=m_data.Mid(index0+1,index1-index0-1);

			std::vector<char> decodeFileContent;
			dlg->base64Decode2(fileContent,decodeFileContent);//�������ݵĽ���

			//��������д�������ļ�
			std::fstream fout(filename, std::ios_base::out | std::ios_base::binary);;
			fout.write(static_cast<const char*>(&decodeFileContent[0]), decodeFileContent.size());
			fout.close();


			//ͼƬ��ʾ
			CImage img;
			img.Load(filename);
			if (!img.IsNull()) 
			{
				SetStretchBltMode(dlg->m_ctrlPicture.GetDC()->GetSafeHdc(), HALFTONE);


				int w=img.GetWidth();
				int h=img.GetHeight();
				 // �ҳ���͸��еĽϴ�ֵ��
				int max=(w>h)?w:h;
				CRect dest;
				dlg->m_ctrlPicture.GetClientRect(&dest);
				int max1=(dest.Width()<dest.Height())?dest.Height():dest.Width();
				// ���㽫ͼƬ���ŵ�TheImage��������ı�������
				float scale = (float) ( (float) max / (float)max1 );
    
				// ���ź�ͼƬ�Ŀ�͸�
				int nw = (int)( w/scale );
				int nh = (int)( h/scale );

				// Ϊ�˽����ź��ͼƬ���� ���в�λ�������ͼƬ�� rectDraw ���Ͻǵ���������ֵ
				int tlx = (nw > nh)? 0: (int)(dest.Width()-nw)/2;
				int tly = (nw > nh)? (int)(dest.Height()-nh)/2: 0;

				// ���� rectDraw,��������ͼƬ img
				CRect rectDraw = dest;
				rectDraw.SetRect(tlx, tly, nw, nh);

				img.Draw(dlg->m_ctrlPicture.GetDC()->GetSafeHdc(), rectDraw);
				
			}
		}

		char buff[100]={"250\r\n"};
		Send(buff,strlen(buff));//��������Ӧ250����ʾ���������ɹ����
		dlg->m_ctrlRecord.AddString(L"S:250  Message accepted for delivery!");
		data=false;
		AsyncSelect(FD_READ); //����ͨ��socket��OnReceive���� 
		return;
	}
		

	if(buff0.Left(4)=="QUIT")
	{
		dlg->m_ctrlRecord.AddString(L"C:"+buff0);
		char buff[100]={"221\r\n"};
		Send(buff,strlen(buff));//��������Ӧ250����ʾ���������ɹ����
		dlg->m_ctrlRecord.AddString(L"S:Quit,Goodbye!");
	}
	CAsyncSocket::OnReceive(nErrorCode);
}


void SocketSMTP::OnSend(int nErrorCode)
{
	// TODO: �ڴ����ר�ô����/����û���
	CMailServerDlg*dlg=(CMailServerDlg*)AfxGetApp()->GetMainWnd();//ȡ��������ָ�����
	char buff[100]={"220\r\n"};
	Send(buff,strlen(buff));//��������Ӧ220����ʾ��������
	dlg->m_ctrlRecord.AddString(L"S:Simple Mail Server Ready For Mail");
	AsyncSelect(FD_READ); //����ͨ��socket��OnReceive���� 
	CAsyncSocket::OnSend(nErrorCode);
}
