
// ClientDlg.h : 头文件
//

#pragma once
#include "clientsocket.h"


// CClientDlg 对话框
class CClientDlg : public CDialogEx
{
// 构造
public:
	CClientDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CLIENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
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
