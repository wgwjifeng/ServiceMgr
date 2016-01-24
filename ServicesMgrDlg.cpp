
// ServicesMgrDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ServicesMgr.h"
#include "ServicesMgrDlg.h"
#include "afxdialogex.h"

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


// CServicesMgrDlg 对话框



CServicesMgrDlg::CServicesMgrDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CServicesMgrDlg::IDD, pParent)
	, m_RidioGroup(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CServicesMgrDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_SERVICES, m_ServiceList);
	DDX_Radio(pDX, IDC_RADIO_WIN32, m_RidioGroup);
}

BEGIN_MESSAGE_MAP(CServicesMgrDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_RADIO_WIN32, &CServicesMgrDlg::OnBnClickedRadioWin32)
	ON_BN_CLICKED(IDC_RADIO_DRIVERS, &CServicesMgrDlg::OnBnClickedRadioWin32)
	ON_BN_CLICKED(IDC_BUTTON_START, &CServicesMgrDlg::OnBnClickedButtonStart)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CServicesMgrDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_EXIT, &CServicesMgrDlg::OnBnClickedButtonExit)
END_MESSAGE_MAP()


// CServicesMgrDlg 消息处理程序

BOOL CServicesMgrDlg::OnInitDialog()
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

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	DWORD dwExStyle = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	m_ServiceList.SetExtendedStyle(dwExStyle);
	m_ServiceList.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 110, 0);
	m_ServiceList.InsertColumn(1, _T("DisplayName"), LVCFMT_LEFT, 350, 1);
	m_ServiceList.InsertColumn(2, _T("State"), LVCFMT_LEFT, 50, 2);

	ShowServiceList(SERVICE_WIN32);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CServicesMgrDlg::OnSysCommand(UINT nID, LPARAM lParam)
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
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CServicesMgrDlg::OnPaint()
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
HCURSOR CServicesMgrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CServicesMgrDlg::ShowServiceList(DWORD dwServiceType)
{
	m_ServiceList.DeleteAllItems();

	// 打开服务管理器句柄
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == hSCM)
	{
		AfxMessageBox(_T("OpenSCManager Error!"));
		return;
	}

	BOOL bRet = FALSE;
	DWORD dwSize = 0;
	DWORD dwServiceCount = 0;
	LPENUM_SERVICE_STATUS lpInfo = NULL;

	// 首次调用目的是为了获取服务列表需要多大的内存空间
	bRet = EnumServicesStatus(
		hSCM, 
		dwServiceType, 
		SERVICE_STATE_ALL, 
		NULL, 
		0, 
		&dwSize, 
		&dwServiceCount, 
		NULL);

	if (!bRet && GetLastError() == ERROR_MORE_DATA)
	{
		// 根据 dwSize 的大小分配内存
		lpInfo = (LPENUM_SERVICE_STATUS)(new BYTE[dwSize]);

		// 指定足够大的缓存区再次获取服务列表到 lpInfo 指向的内存
		bRet = EnumServicesStatus(
			hSCM,
			dwServiceType,
			SERVICE_STATE_ALL,
			(LPENUM_SERVICE_STATUS)lpInfo,
			dwSize,
			&dwSize,
			&dwServiceCount,
			NULL);
		if (!bRet) {
			// 关闭服务管理器句柄
			CloseServiceHandle(hSCM);
			return;
		}

		// 将数据显示到列表中
		for (DWORD i = 0; i < dwServiceCount; i++)
		{
			m_ServiceList.InsertItem(i, lpInfo[i].lpServiceName);
			m_ServiceList.SetItemText(i, 1, lpInfo[i].lpDisplayName);
			switch (lpInfo[i].ServiceStatus.dwCurrentState)
			{
			case SERVICE_PAUSED:
				m_ServiceList.SetItemText(i, 2, _T("暂停"));
				break;
			case SERVICE_STOPPED:
				m_ServiceList.SetItemText(i, 2, _T("停止"));
				break;
			case SERVICE_RUNNING:
				m_ServiceList.SetItemText(i, 2, _T("运行"));
				break;
			default:
				m_ServiceList.SetItemText(i, 2, _T("未知"));
			}
		}
	}

	delete[] lpInfo;
	// 关闭服务管理器句柄
	CloseServiceHandle(hSCM);
}

void CServicesMgrDlg::OnBnClickedRadioWin32()
{
	// TODO:  在此添加控件通知处理程序代码
	UpdateData(TRUE);
	switch (m_RidioGroup)
	{
	case 0:
		ShowServiceList(SERVICE_WIN32);
		break;
	case 1:
		ShowServiceList(SERVICE_DRIVER);
		break;
	}
}


void CServicesMgrDlg::OnBnClickedButtonStart()
{
	// TODO:  在此添加控件通知处理程序代码
	POSITION Pos = m_ServiceList.GetFirstSelectedItemPosition();
	int nSelect = -1;

	while (Pos)
	{
		nSelect = m_ServiceList.GetNextSelectedItem(Pos);
	}

	if (-1 == nSelect)
	{
		AfxMessageBox(_T("请选择要启动的服务！"));
		return;
	}

	TCHAR szServicename[MAXBYTE];
	m_ServiceList.GetItemText(nSelect, 0, szServicename, MAXBYTE);

	// 打开服务管理器
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == hSCM)
	{
		AfxMessageBox(_T("OpenSCManager Error!"));
		return;
	}

	SC_HANDLE hSCService = OpenService(hSCM, szServicename, SERVICE_ALL_ACCESS);
	if (NULL == hSCService)
	{
		AfxMessageBox(_T("OpenService Error!"));
		CloseServiceHandle(hSCM);
		return;
	}
	BOOL bRet = StartService(hSCService, 0, NULL);
	if (TRUE == bRet)
	{
		m_ServiceList.SetItemText(nSelect, 2, _T("运行"));
	}
	else
	{
		AfxMessageBox(_T("启动失败！"));
	}

	CloseServiceHandle(hSCService);
	CloseServiceHandle(hSCM);
}


void CServicesMgrDlg::OnBnClickedButtonStop()
{
	// TODO:  在此添加控件通知处理程序代码
	POSITION Pos = m_ServiceList.GetFirstSelectedItemPosition();
	int nSelect = -1;

	while (Pos)
	{
		nSelect = m_ServiceList.GetNextSelectedItem(Pos);
	}

	if (-1 == nSelect)
	{
		AfxMessageBox(_T("请选择要启动的服务！"));
		return;
	}

	TCHAR szServicename[MAXBYTE];
	m_ServiceList.GetItemText(nSelect, 0, szServicename, MAXBYTE);

	// 打开服务管理器
	SC_HANDLE hSCM = OpenSCManager(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	if (NULL == hSCM)
	{
		AfxMessageBox(_T("OpenSCManager Error!"));
		return;
	}

	SC_HANDLE hSCService = OpenService(hSCM, szServicename, SERVICE_ALL_ACCESS);
	if (NULL == hSCService)
	{
		AfxMessageBox(_T("OpenService Error!"));
		CloseServiceHandle(hSCM);
		return;
	}
	SERVICE_STATUS ServiceStatus;
	BOOL bRet = ControlService(hSCService, SERVICE_CONTROL_STOP, &ServiceStatus);
	if (TRUE == bRet)
	{
		m_ServiceList.SetItemText(nSelect, 2, _T("停止"));
	}
	else
	{
		AfxMessageBox(_T("停止失败！"));
	}

	CloseServiceHandle(hSCService);
	CloseServiceHandle(hSCM);
}


void CServicesMgrDlg::OnBnClickedButtonExit()
{
	// TODO:  在此添加控件通知处理程序代码
	OnOK();
}
