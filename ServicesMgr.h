
// ServicesMgr.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CServicesMgrApp: 
// �йش����ʵ�֣������ ServicesMgr.cpp
//

class CServicesMgrApp : public CWinApp
{
public:
	CServicesMgrApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CServicesMgrApp theApp;