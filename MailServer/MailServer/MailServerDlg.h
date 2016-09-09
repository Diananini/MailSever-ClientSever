
// MailServerDlg.h : 头文件
//

#pragma once
#include "listensmtp.h"
#include <vector>

// CMailServerDlg 对话框
class CMailServerDlg : public CDialogEx
{
// 构造
public:
	CMailServerDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MAILSERVER_DIALOG };

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
	
	CListBox m_ctrlContent;
	CListBox m_ctrlRecord;
	// 监听套接字
	ListenSMTP listen;
	afx_msg void OnClose();

	
	CListBox m_ctrlBody;

	// 解码
	void base64Decode2(CString inpt, std::vector<char>& bytes);
	CStatic m_ctrlPicture;

	CString base64Decode1(CString inpt, int* len);
};
