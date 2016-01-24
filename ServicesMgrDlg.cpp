
// ServicesMgrDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ServicesMgr.h"
#include "ServicesMgrDlg.h"
#include "afxdialogex.h"

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


// CServicesMgrDlg �Ի���



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


// CServicesMgrDlg ��Ϣ�������

BOOL CServicesMgrDlg::OnInitDialog()
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

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO:  �ڴ���Ӷ���ĳ�ʼ������
	DWORD dwExStyle = LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES;
	m_ServiceList.SetExtendedStyle(dwExStyle);
	m_ServiceList.InsertColumn(0, _T("Name"), LVCFMT_LEFT, 110, 0);
	m_ServiceList.InsertColumn(1, _T("DisplayName"), LVCFMT_LEFT, 350, 1);
	m_ServiceList.InsertColumn(2, _T("State"), LVCFMT_LEFT, 50, 2);

	ShowServiceList(SERVICE_WIN32);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CServicesMgrDlg::OnPaint()
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
HCURSOR CServicesMgrDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CServicesMgrDlg::ShowServiceList(DWORD dwServiceType)
{
	m_ServiceList.DeleteAllItems();

	// �򿪷�����������
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

	// �״ε���Ŀ����Ϊ�˻�ȡ�����б���Ҫ�����ڴ�ռ�
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
		// ���� dwSize �Ĵ�С�����ڴ�
		lpInfo = (LPENUM_SERVICE_STATUS)(new BYTE[dwSize]);

		// ָ���㹻��Ļ������ٴλ�ȡ�����б� lpInfo ָ����ڴ�
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
			// �رշ�����������
			CloseServiceHandle(hSCM);
			return;
		}

		// ��������ʾ���б���
		for (DWORD i = 0; i < dwServiceCount; i++)
		{
			m_ServiceList.InsertItem(i, lpInfo[i].lpServiceName);
			m_ServiceList.SetItemText(i, 1, lpInfo[i].lpDisplayName);
			switch (lpInfo[i].ServiceStatus.dwCurrentState)
			{
			case SERVICE_PAUSED:
				m_ServiceList.SetItemText(i, 2, _T("��ͣ"));
				break;
			case SERVICE_STOPPED:
				m_ServiceList.SetItemText(i, 2, _T("ֹͣ"));
				break;
			case SERVICE_RUNNING:
				m_ServiceList.SetItemText(i, 2, _T("����"));
				break;
			default:
				m_ServiceList.SetItemText(i, 2, _T("δ֪"));
			}
		}
	}

	delete[] lpInfo;
	// �رշ�����������
	CloseServiceHandle(hSCM);
}

void CServicesMgrDlg::OnBnClickedRadioWin32()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
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
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos = m_ServiceList.GetFirstSelectedItemPosition();
	int nSelect = -1;

	while (Pos)
	{
		nSelect = m_ServiceList.GetNextSelectedItem(Pos);
	}

	if (-1 == nSelect)
	{
		AfxMessageBox(_T("��ѡ��Ҫ�����ķ���"));
		return;
	}

	TCHAR szServicename[MAXBYTE];
	m_ServiceList.GetItemText(nSelect, 0, szServicename, MAXBYTE);

	// �򿪷��������
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
		m_ServiceList.SetItemText(nSelect, 2, _T("����"));
	}
	else
	{
		AfxMessageBox(_T("����ʧ�ܣ�"));
	}

	CloseServiceHandle(hSCService);
	CloseServiceHandle(hSCM);
}


void CServicesMgrDlg::OnBnClickedButtonStop()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	POSITION Pos = m_ServiceList.GetFirstSelectedItemPosition();
	int nSelect = -1;

	while (Pos)
	{
		nSelect = m_ServiceList.GetNextSelectedItem(Pos);
	}

	if (-1 == nSelect)
	{
		AfxMessageBox(_T("��ѡ��Ҫ�����ķ���"));
		return;
	}

	TCHAR szServicename[MAXBYTE];
	m_ServiceList.GetItemText(nSelect, 0, szServicename, MAXBYTE);

	// �򿪷��������
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
		m_ServiceList.SetItemText(nSelect, 2, _T("ֹͣ"));
	}
	else
	{
		AfxMessageBox(_T("ֹͣʧ�ܣ�"));
	}

	CloseServiceHandle(hSCService);
	CloseServiceHandle(hSCM);
}


void CServicesMgrDlg::OnBnClickedButtonExit()
{
	// TODO:  �ڴ���ӿؼ�֪ͨ����������
	OnOK();
}
