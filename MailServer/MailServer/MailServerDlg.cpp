
// MailServerDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "MailServer.h"
#include "MailServerDlg.h"
#include "afxdialogex.h"

#include<string>
#include <regex>
#include <sstream>
#include <fstream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMailServerDlg �Ի���




CMailServerDlg::CMailServerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CMailServerDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	
}

void CMailServerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CONTENT, m_ctrlContent);
	DDX_Control(pDX, IDC_RECORD, m_ctrlRecord);
	DDX_Control(pDX, IDC_BODY, m_ctrlBody);
	DDX_Control(pDX, IDC_PICTURE, m_ctrlPicture);
}

BEGIN_MESSAGE_MAP(CMailServerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_CLOSE()
//	ON_LBN_SELCHANGE(IDC_BODY, &CMailServerDlg::OnLbnSelchangeBody)
END_MESSAGE_MAP()


// CMailServerDlg ��Ϣ�������

BOOL CMailServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_ctrlRecord.ResetContent();//���ԭ������
	m_ctrlRecord.AddString(L"***SMTP������׼����");
	m_ctrlContent.SetHorizontalExtent(1000);//ˮƽ������
	m_ctrlBody.SetHorizontalExtent(1000);//ˮƽ������

	if(listen.m_hSocket ==INVALID_SOCKET)  
    {  
		
        //���������׽���,����FD_ACCEPT�¼�,Ĭ�϶˿�25
        BOOL bFlag =listen.Create(25,SOCK_STREAM,FD_ACCEPT);    //������������ʾm_srvrSocketֻ��FD_ACCEPT�¼�����Ȥ  
		
		if(!bFlag)  
        {  
            AfxMessageBox(_T("Socket����ʧ��!"));  
            listen.Close();  
            PostQuitMessage(0);  
  
            return TRUE;   
        }   
        //�����ɹ�,��ʾ������׼���ã����ȴ���������  
        if(!listen.Listen())//�������ʧ��  
        {  
            int nErrorCode =listen.GetLastError();    //��������Ϣ  
            if(nErrorCode !=WSAEWOULDBLOCK)             //��������߳�����  
            {  
                AfxMessageBox(_T("Socket����!"));  
                listen.Close();  
                PostQuitMessage(0);  

                return TRUE;  
            }  
        }  
		
    }  


	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CMailServerDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CMailServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CMailServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMailServerDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	listen.Close();//�ر��׽���
	CDialogEx::OnClose();
}

// ����
void CMailServerDlg::base64Decode2(CString inpt, std::vector<char>& bytes)//ͼƬ����
{
	 unsigned char * base64=(unsigned char *)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

	 int n,i,j,pad;
     unsigned char *p;
     
     pad=0;
     n=inpt.GetLength();

     CString src;
	 src.GetBufferSetLength(n);
     for(i=0;i<n;i++)
         src.SetAt(i,inpt[i]);
 
     while(n>0&&src[n-1]=='=') 
	 {
		 src.SetAt(n-1,0);
         pad++;
         n--;
     }
     for(i=0;i<n;i++)   
	 { /* map base64 ASCII character to 6 bit value */
         p=(unsigned char *)strchr((const char *)base64,(int)src[i]);
         if(!p)
              break;
		 src.SetAt(i,p-(unsigned char *)base64);
     }
 
     for(i=0;i<n;i+=4) 
	 {
		 byte threeByte[3];
         threeByte[0]=(src[i]<<2)+((src[i+1]&0x30)>>4);
         threeByte[1]=((src[i+1]&0x0F)<<4)+((src[i+2]&0x3C)>>2);
         threeByte[2]=((src[i+2]&0x03)<<6)+src[i+3];

		 for(j=0;j<3;j++) 
			bytes.push_back(threeByte[j]);
     }
}

CString CMailServerDlg::base64Decode1(CString inpt, int* len)//���Ľ���
{
	unsigned char * base64=(unsigned char *)"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";

	 int n,i,j,pad;
     unsigned char *p;
     static unsigned char *dst;
     unsigned char * src;
     *len=0;
     pad=0;
     n=inpt.GetLength();
     src=new unsigned char [n];
     for(i=0;i<n;i++)
         src[i]=inpt[i];
 
     while(n>0&&src[n-1]=='=') {
         src[n-1]=0;
         pad++;
         n--;
     }
     for(i=0;i<n;i++)   { /* map base64 ASCII character to 6 bit value */
         p=(unsigned char *)strchr((const char *)base64,(int)src[i]);
         if(!p)
              break;
         src[i]=p-(unsigned char *)base64;
     }
 
     dst=(unsigned char *)malloc(n*3/4+1);
     memset(dst,0,n*3/4+1);
     for(i=0,j=0;i<n;i+=4,j+=3) {
         dst[j]=(src[i]<<2) + ((src[i+1]&0x30)>>4);
         dst[j+1]=((src[i+1]&0x0F)<<4) + ((src[i+2]&0x3C)>>2);
         dst[j+2]=((src[i+2]&0x03)<<6) + src[i+3];
         *len+=3;
     }
     *len-=pad;
     return CString(dst);
}
