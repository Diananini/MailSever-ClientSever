
// ClientDlg.h : ͷ�ļ�
//

#pragma once
#include "clientsocket.h"


// CClientDlg �Ի���
class CClientDlg : public CDialogEx
{
// ����
public:
	CClientDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnClickedSend();
	afx_msg void OnChangeIp();
	afx_msg void OnChangePort();
	afx_msg void OnChangeRequest();
	afx_msg void OnChangeRespond();
	CString m_IP;
	CString m_request;
	UINT m_port;
	CString m_respond;
	afx_msg void OnClose();
	ClientSocket client;
};
