
// MailServerDlg.h : ͷ�ļ�
//

#pragma once
#include "listensmtp.h"
#include <vector>

// CMailServerDlg �Ի���
class CMailServerDlg : public CDialogEx
{
// ����
public:
	CMailServerDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MAILSERVER_DIALOG };

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
	
	CListBox m_ctrlContent;
	CListBox m_ctrlRecord;
	// �����׽���
	ListenSMTP listen;
	afx_msg void OnClose();

	
	CListBox m_ctrlBody;

	// ����
	void base64Decode2(CString inpt, std::vector<char>& bytes);
	CStatic m_ctrlPicture;

	CString base64Decode1(CString inpt, int* len);
};
