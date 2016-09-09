
// ClientDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "Client.h"
#include "ClientDlg.h"
#include "afxdialogex.h"
#include "ClientSocket.h"
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


// CClientDlg �Ի���




CClientDlg::CClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CClientDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	//��ñ���IP��ַ
	WORD wVersionRequested;
    WSADATA wsaData;
    char name[255];
    CString ip;
    PHOSTENT hostinfo;
    wVersionRequested=MAKEWORD(2,0);
    if(WSAStartup( wVersionRequested,&wsaData )==0 )
    {
        if(gethostname(name, sizeof(name))==0)
        {
            if((hostinfo = gethostbyname(name)) != NULL)
            {
                m_IP = inet_ntoa (*(struct in_addr *)*hostinfo->h_addr_list);
            }
        }
        WSACleanup( );
    } 
	m_port=3000;
	m_respond = _T("");
}

void CClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_IP, m_IP);
	//DDX_Text(pDX, IDC_PORT, m_port);
	DDX_Text(pDX, IDC_REQUEST, m_request);

	DDX_Text(pDX, IDC_PORT, m_port);
	DDX_Text(pDX, IDC_RESPOND, m_respond);
}

BEGIN_MESSAGE_MAP(CClientDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND, &CClientDlg::OnClickedSend)
	ON_EN_CHANGE(IDC_IP, &CClientDlg::OnChangeIp)
	ON_EN_CHANGE(IDC_PORT, &CClientDlg::OnChangePort)
	ON_EN_CHANGE(IDC_REQUEST, &CClientDlg::OnChangeRequest)
	ON_EN_CHANGE(IDC_RESPOND, &CClientDlg::OnChangeRespond)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CClientDlg ��Ϣ�������

BOOL CClientDlg::OnInitDialog()
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
	if(m_port!=3000)
	{
		MessageBoxW(_T("�������˿ںŴ���!"));	 		//�����׽ӿڴ�����
		exit(0);
	}
	BOOL bFlag=client.Create(3001,SOCK_DGRAM,FD_READ);//�����ݱ���ʽ����socket
	if(!bFlag)
	{
		MessageBoxW(_T("Socket����ʧ��!"));	 		//�����׽ӿڴ�����
		exit(0);
	}
	
	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CClientDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CClientDlg::OnPaint()
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
HCURSOR CClientDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CClientDlg::OnClickedSend()
{
	
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	
	client.SendTo(m_request,(m_request.GetLength()+1)*sizeof(TCHAR),m_port,m_IP);
	
	
}


void CClientDlg::OnChangeIp()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	   UpdateData(true);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CClientDlg::OnChangePort()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	   UpdateData(true);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CClientDlg::OnChangeRequest()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
	   UpdateData(true);

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CClientDlg::OnChangeRespond()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�
       UpdateData(true);
	//Invalidate(true);
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CClientDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	client.Close();
	CDialogEx::OnClose();
}
