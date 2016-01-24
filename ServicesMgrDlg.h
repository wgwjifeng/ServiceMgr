
// ServicesMgrDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"


// CServicesMgrDlg �Ի���
class CServicesMgrDlg : public CDialogEx
{
// ����
public:
	CServicesMgrDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SERVICESMGR_DIALOG };

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

	// show service list
	void ShowServiceList(DWORD dwServiceType);

public:
	// list control
	CListCtrl m_ServiceList;

	// ridio button
	int m_RidioGroup;
	afx_msg void OnBnClickedRadioWin32();
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonExit();
};
