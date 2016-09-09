
// MailServerDlg.cpp : 实现文件
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


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CMailServerDlg 对话框




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


// CMailServerDlg 消息处理程序

BOOL CMailServerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	m_ctrlRecord.ResetContent();//清除原有数据
	m_ctrlRecord.AddString(L"***SMTP服务器准备好");
	m_ctrlContent.SetHorizontalExtent(1000);//水平滚动条
	m_ctrlBody.SetHorizontalExtent(1000);//水平滚动条

	if(listen.m_hSocket ==INVALID_SOCKET)  
    {  
		
        //创建监听套接字,激发FD_ACCEPT事件,默认端口25
        BOOL bFlag =listen.Create(25,SOCK_STREAM,FD_ACCEPT);    //第三个参数表示m_srvrSocket只对FD_ACCEPT事件感兴趣  
		
		if(!bFlag)  
        {  
            AfxMessageBox(_T("Socket创建失败!"));  
            listen.Close();  
            PostQuitMessage(0);  
  
            return TRUE;   
        }   
        //监听成功,显示服务器准备好，并等待连接请求  
        if(!listen.Listen())//如果监听失败  
        {  
            int nErrorCode =listen.GetLastError();    //检测错误信息  
            if(nErrorCode !=WSAEWOULDBLOCK)             //如果不是线程阻塞  
            {  
                AfxMessageBox(_T("Socket错误!"));  
                listen.Close();  
                PostQuitMessage(0);  

                return TRUE;  
            }  
        }  
		
    }  


	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMailServerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CMailServerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMailServerDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	listen.Close();//关闭套接字
	CDialogEx::OnClose();
}

// 解码
void CMailServerDlg::base64Decode2(CString inpt, std::vector<char>& bytes)//图片解码
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

CString CMailServerDlg::base64Decode1(CString inpt, int* len)//正文解码
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
