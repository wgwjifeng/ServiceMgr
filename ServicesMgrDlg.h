
// ServicesMgrDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"


// CServicesMgrDlg 对话框
class CServicesMgrDlg : public CDialogEx
{
// 构造
public:
	CServicesMgrDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_SERVICESMGR_DIALOG };

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
